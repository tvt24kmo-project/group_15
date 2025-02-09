#include "cardinfo.h"
#include "environment.h"
#include "login.h"
#include "ui_login.h"

login::login(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::login)
{
    ui->setupUi(this);

    // Luo ajastin ikkunalle
    timeoutTimer = new QTimer(this);
    timeoutTimer->setInterval(30000);
    timeoutTimer->start();
    // Kun aikakatkaisu tapahtuu, tämä ikkuna sulkeutuu
    connect(timeoutTimer, &QTimer::timeout, this, &login::close);
}

login::~login()
{
    delete ui;
}


int login::checkCardType()
{
    int lockstatus=0;
    qDebug()<<"checkCardType";

    QString url =  Environment::base_url()+"/procedures/getAccountType/"+ui->textUsername->text();

    qDebug()<<url;


    // if reply == 'multi' -> kysy tiliä -> joku mysql proseduuri / backend logiikka jolla valitaan oikea tili.

    return lockstatus;
}

int login::checkCardStatus()
{
    int cardStatus = 0;

    qDebug()<<"checkCardStatus";
    QString url = Environment::base_url()+"/cards/check-lock-status/"+ui->textUsername->text();
    qDebug()<<url;
    QNetworkRequest request(url);
    postManager = new QNetworkAccessManager(this);
    connect(postManager,SIGNAL(finished(QNetworkReply*)), this, SLOT(loginSlot(QNetworkReply*)));
    reply = postManager->get(request);

    return cardStatus;

}




void login::onWindowFinished()
{
    timeoutTimer->start();
}

void login::on_btnLogin_clicked()
{
    timeoutTimer->start();

    QJsonObject jsonObj;
    jsonObj.insert("cardnumber",ui->textUsername->text());
    jsonObj.insert("pinhash",ui->textPassword->text());

    QString site_url=Environment::base_url()+"/login";
    qDebug()<<site_url;
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
                timeoutTimer->stop(); // lopeta ajastin

                ui->labelInfo->setText("Kirjautuminen OK");
                QByteArray myToken="Bearer "+response_data;
                cardInfo *objCardInfo= new cardInfo(this);
                objCardInfo->setUsername(ui->textUsername->text());
                objCardInfo->setMyToken(myToken);
                connect(objCardInfo, &QDialog::finished, this, &login::onWindowFinished);



                int statusCheck = 0;
                // ennen kun avataan ikkuna, tarkistetaan kortin tilat ja tyyppi
                int cardstatus =
                checkCardStatus();
                checkCardType();




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
