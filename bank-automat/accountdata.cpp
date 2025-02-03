#include "accountdata.h"
#include "ui_accountdata.h"
#include "environment.h"


accountData::accountData(QWidget *parent) : QDialog(parent), ui(new Ui::accountData)
{
    ui->setupUi(this);

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
    fetchData();
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

void accountData::showDataSlot(QNetworkReply *reply)
{
      qDebug()<<"show data SLOT";
    response_data=reply->readAll();
    qDebug()<<response_data;
    QJsonDocument json_doc = QJsonDocument::fromJson(response_data);
    QJsonObject jsonObj = json_doc.object();

    //accounts: idaccount,customer_id,balance,accountiban,credit_limit

    accountId = jsonObj["idaccount"].toInt();
    ui->labelIdaccount->setText(QString::number(accountId));

    ui->labelCustomer_id->setText(QString::number(jsonObj["customer_id"].toInt()));
    ui->labelBalance->setText(jsonObj["balance"].toString());
    ui->labelAccountiban->setText(jsonObj["accountiban"].toString());
    ui->labelCredit_limit->setText(jsonObj["credit_limit"].toString());






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

QString accountData::getAccountIban() const
{
    return ui->labelAccountiban->text();
}