#include "debitorcredit.h"
#include "ui_debitorcredit.h"
#include "environment.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>


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

        if (json_doc.isArray()) {
            QJsonArray jsonArray = json_doc.array();
            qDebug() << "JsonArray size: " << jsonArray.size();
            if (jsonArray.size() >= 2) {
                QJsonObject firstAccount = jsonArray[0].toObject();
                QJsonObject secondAccount = jsonArray[1].toObject();

                qDebug() << "First account credit limit: " << firstAccount["credit_limit"];
                qDebug() << "Second account credit limit: " << secondAccount["credit_limit"];

                if (firstAccount["credit_limit"].toDouble() > 0) {
                    creditAccountId = firstAccount["idaccount"].toInt();
                    debitAccountId = secondAccount["idaccount"].toInt();
                } else {
                    creditAccountId = secondAccount["idaccount"].toInt();
                    debitAccountId = firstAccount["idaccount"].toInt();
                }
            }
            //add else if size = 1 debug messages
        } else if (json_doc.isObject()) {
            //add debug messages here
            QJsonObject json_obj = json_doc.object();
            debitAccountId = json_obj["idaccount"].toInt();
        }
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
