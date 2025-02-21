#include "debitorcredit.h"
#include "ui_debitorcredit.h"
#include "environment.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMainWindow>
#include "login.h"


DebitOrCredit::DebitOrCredit(QWidget *parent, accountData *data)
    : QDialog(parent)
    , ui(new Ui::DebitOrCredit)
    , accountDataPtr(data)
    , debitAccountId(-1)
    , creditAccountId(-1)
{
    ui->setupUi(this);
    if (accountDataPtr)
    {
        fetchAccountIds();
    }
}

DebitOrCredit::~DebitOrCredit()
{
    delete ui;
}

void DebitOrCredit::fetchAccountIds()
{
    qDebug() << "fetchAccountIds()";
    QString url = Environment::base_url() + "/accounts/" + accountDataPtr->getUsername();
    qDebug() << "FetchAccountIds URL: " << url;
    QNetworkRequest request(url);
    request.setRawHeader("Authorization", accountDataPtr->getMyToken());

    getManager = new QNetworkAccessManager(this);
    QEventLoop loop;

    connect(getManager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    connect(getManager, &QNetworkAccessManager::finished, this, [this](QNetworkReply *reply) {
        QByteArray response_data = reply->readAll();
        qDebug() << "Response Data: " << response_data;

        QJsonDocument json_doc = QJsonDocument::fromJson(response_data);

        if (json_doc.isArray()) {  // ALWAYS expect an array
            QJsonArray jsonArray = json_doc.array();
            if (jsonArray.size() >= 2) {
                QJsonObject firstAccount = jsonArray[0].toObject();
                QJsonObject secondAccount = jsonArray[1].toObject();

                if (firstAccount["credit_limit"].toDouble() > 0) {
                    creditAccountId = firstAccount["idaccount"].toInt();
                    debitAccountId = secondAccount["idaccount"].toInt();
                } else {
                    creditAccountId = secondAccount["idaccount"].toInt();
                    debitAccountId = firstAccount["idaccount"].toInt();
                }
            } else if (jsonArray.size() == 1) { // Handle single-account case
                QJsonObject singleAccount = jsonArray[0].toObject();
                debitAccountId = singleAccount["idaccount"].toInt(); // Assume debit if only one
                creditAccountId = -1; // No credit account
            }
        } // No need for an else if (json_doc.isObject()) anymore

        qDebug() << "Debit Account ID (in fetch): " << debitAccountId;
        qDebug() << "Credit Account ID (in fetch): " << creditAccountId;

        reply->deleteLater();
    });

    reply = getManager->get(request);
    loop.exec();
    getManager->deleteLater();
}




void DebitOrCredit::on_buttonCredit_clicked()
{
    qDebug() << "creditbutton clicked - inside slot";
    if (creditAccountId != -1) {
        emit creditChosen(creditAccountId);
        qDebug() << "Credit Account ID: " << creditAccountId;
        accept();
    }
}

void DebitOrCredit::on_buttonDebit_clicked()
{
    qDebug() << "debitbutton clicked - inside slot";
    if (debitAccountId != -1) {
        emit debitChosen(debitAccountId);
        qDebug() << "Debit Account ID: " << debitAccountId;
        accept();
    }
}


void DebitOrCredit::closeEvent(QCloseEvent *event)
{
    if (QWidget *parentWidget = this->parentWidget()) {
        if (login *loginWindow = qobject_cast<login *>(parentWidget)) {
            loginWindow->clearLoginFields();
            loginWindow->close();
        }
    }

    foreach(QWidget *widget, QApplication::topLevelWidgets()) {
        if (QMainWindow *mainWindow = qobject_cast<QMainWindow *>(widget)) {
            mainWindow->show();
            break;
        }
    }

    event->accept();
}