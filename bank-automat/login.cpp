#include "cardinfo.h"
#include "environment.h"
#include "login.h"
#include "ui_login.h"
#include "debitorcredit.h"

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


    // if reply == 'multi' -> kysy tiliä -> joku mysql proseduuri / backend logiikka jolla valitaan oikea tili.

    return lockstatus;
}

int login::checkCardStatus()
{
    int cardStatus = 0;

    qDebug()<<"checkCardStatus";
    QString url = Environment::base_url()+"/cards/check-lock-status/"+ui->textUsername->text();
    QNetworkRequest request(url);
    postManager = new QNetworkAccessManager(this);
    connect(postManager,SIGNAL(finished(QNetworkReply*)), this, SLOT(loginSlot(QNetworkReply*)));
    reply = postManager->get(request);
    
    qDebug()<<"reply";
    qDebug()<<reply->readAll();
    
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


void delay() // erittäin tyhmä implementaatio kahden ikkunan yhtäaikaa avaamisen estämiseen (multi kortilla)
{
    QTime dieTime= QTime::currentTime().addSecs(10); // 10 sekunnin viive tapahtumien välillä, ei jäädytä UI:ta
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
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



                int cardType = 0; // koodattu = 1 arvoksi testausta varten, 1 tulisi requestistä
                checkCardType();
                // ennen kun avataan ikkuna, tarkistetaan kortin tilat ja tyyppi
                int cardLockStatus = checkCardStatus();


                if (cardLockStatus == 1) // tarkistetaan kortin lukko ennen kun avataan debit/credit liittymää (jos olisi multi kortti)
                {
                    // error viestin käsittely
                }


                if (cardType == 1)
                {
                    DebitOrCredit *DOC = new DebitOrCredit(this);
                    DOC->open();
                    delay(); // 10 sekunnin viive, eli ei avaa objCardInfo->Open ikkunaa
                }

                else // jos ei ole käytössä multi kortti ja kortti ei ole lukossa, avataan liittymä
                {
                    objCardInfo->open();
                }

            }
            else {
                ui->labelInfo->setText("Väärä tunnus/salasana");
            }

        }

    }

    reply->deleteLater();
    postManager->deleteLater();
}
