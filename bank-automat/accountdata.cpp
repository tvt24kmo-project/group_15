#include "accountdata.h"
#include "ui_accountdata.h"
#include "environment.h"
#include <QJsonArray>

accountData::accountData(QWidget *parent) : QDialog(parent), ui(new Ui::accountData)
{
    ui->setupUi(this);
    // Luo ajastin ikkunalle
    timeoutTimer = new QTimer(this);
    timeoutTimer->setInterval(10000);
    timeoutTimer->start();
    // Kun aikakatkaisu tapahtuu, tämä ikkuna sulkeutuu
    connect(timeoutTimer, &QTimer::timeout, this, &accountData::close);
}

accountData::~accountData()
{
    delete ui;
}

void accountData::setUsername(const QString &newUsername)
{
    username = newUsername;

}

void accountData::setMyToken(const QByteArray &newMyToken)
{
    myToken = newMyToken;
}

void accountData::fetchData()
{
    QString site_url = Environment::base_url() + "/accounts/" + username;
    qDebug() << "Site URL:" << site_url;

    QNetworkRequest request(site_url);
    request.setRawHeader(QByteArray("Authorization"), myToken);

    dataManager = new QNetworkAccessManager(this);
    connect(dataManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(showDataSlot(QNetworkReply*)));

    reply = dataManager->get(request);
}

QString accountData::getUsername() const
{
    return username;
}

QJsonObject accountData::findAccountObject(const QJsonArray &jsonArray, int targetAccountId) {
    for (const QJsonValue &value : jsonArray) {
        QJsonObject obj = value.toObject();
        if (obj["idaccount"].toInt() == targetAccountId) {
            return obj;
        }
    }
    return QJsonObject();
}

void accountData::showDataSlot(QNetworkReply *reply)
{
    timeoutTimer->start();// Käynnistetään ajastin uudelleen

    qDebug()<<"show data SLOT";
    response_data=reply->readAll();
    qDebug()<<response_data;
    QJsonDocument json_doc = QJsonDocument::fromJson(response_data);

    if (json_doc.isArray()) {
        QJsonArray jsonArray = json_doc.array();
        QJsonObject jsonObj = findAccountObject(jsonArray, accountId);

        if (!jsonObj.isEmpty())
        {
            // accountId = jsonObj["idaccount"].toInt();
            ui->labelIdaccount->setText(QString::number(accountId));
            ui->labelCustomer_id->setText(QString::number(jsonObj["customer_id"].toInt()));
            ui->labelBalance->setText(jsonObj["balance"].toString());
            ui->labelAccountiban->setText(jsonObj["accountiban"].toString());
            ui->labelCredit_limit->setText(jsonObj["credit_limit"].toString());
        } else {
            qDebug() << "Account not found in JSON array";
        }
    } else {
        qDebug() << "response_data is not json array";
    }
    reply->deleteLater();
    dataManager->deleteLater();
}


int accountData::getAccountId() 
{
    return accountId;
}

QByteArray accountData::getMyToken()
{
    return myToken;
}

void accountData::setAccountId(int id)
{
    accountId = id;
}

QString accountData::getAccountIban() const
{
    return ui->labelAccountiban->text();
}

void accountData::on_btnClose_clicked()
{
    accountData::close();
}

