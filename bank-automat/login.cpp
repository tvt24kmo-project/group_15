#include "cardinfo.h"
#include "environment.h"
#include "login.h"
#include "ui_login.h"

login::login(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::login)
{
    ui->setupUi(this);
}

login::~login()
{
    delete ui;
}

void login::on_btnLogin_clicked()
{

    QJsonObject jsonObj;
    jsonObj.insert("cardnumber",ui->textUsername->text());
    jsonObj.insert("pinhash",ui->textPassword->text());

    QString site_url=Environment::base_url()+"/login";
    QNetworkRequest request(site_url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    postManager = new QNetworkAccessManager(this);
    connect(postManager,SIGNAL(finished(QNetworkReply*)), this, SLOT(loginSlot(QNetworkReply*)));
    reply = postManager->post(request, QJsonDocument(jsonObj).toJson());
}

void login::loginSlot(QNetworkReply *reply)
{

    response_data=reply->readAll();
    if(response_data.length()<2){
        qDebug()<<"Palvelin ei vastaa";
        ui->labelInfo->setText("Palvelin ei vastaa!");
    }
    else {
        if(response_data=="-11"){
            ui->labelInfo->setText("Tietokanta virhe!");
        }
        else {
            if(response_data!="false" && response_data.length()>20) {
                ui->labelInfo->setText("Kirjautuminen OK");
                QByteArray myToken="Bearer "+response_data;
                cardInfo *objCardInfo= new cardInfo(this);
                objCardInfo->setUsername(ui->textUsername->text());
                objCardInfo->setMyToken(myToken);
                objCardInfo->open();
            }
            else {
                ui->labelInfo->setText("Väärä tunnus/salasana");
            }

        }

    }

    reply->deleteLater();
    postManager->deleteLater();
}
