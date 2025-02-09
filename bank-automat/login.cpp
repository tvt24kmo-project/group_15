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
    int cardType=0; // kortti on oletuksena normaali ellei tunnisteta multi kortiksi
    // qDebug()<<"checkCardType";
    QString url =  Environment::base_url()+"/procedures/getAccountType/";
    QNetworkRequest request(url);

    request.setRawHeader("Authorization", myToken);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    // qDebug()<< "Tokeni checkCardTypessä: " << myToken;

    QEventLoop loop;
    postManager = new QNetworkAccessManager(this);

    connect(postManager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    connect(postManager, &QNetworkAccessManager::finished, this,  [&cardType](QNetworkReply *reply) {
        QByteArray response_data = reply->readAll();
        // qDebug()<<"response_data" << response_data;


        // puretaan vastaus palasiksi: {"account_type":"multi"}
        QJsonDocument json_doc = QJsonDocument::fromJson(response_data); // luodaan QJsonDocument vastauksesta
        QJsonObject json_obj = json_doc.object(); // viedään vastaus QJsonObjectiin

        if(json_obj["account_type"].toString() == "Multi")
        {
            cardType = 1;
            // qDebug()<<"multi kortti tunnistettu";
        }
    });


    QJsonObject jsonObj;
    jsonObj.insert("account_id",ui->textUsername->text());

    // lähetetään POST requesti JSON bodyllä
    reply = postManager->post(request, QJsonDocument(jsonObj).toJson());
    loop.exec();

    return cardType;
}

int login::checkCardStatus()
{
    int cardStatus = 0;
    // qDebug()<<"checkCardStatus";
    QString url = Environment::base_url()+"/cards/check-lock-status/"+ui->textUsername->text();
    QNetworkRequest request(url);

    request.setRawHeader("Authorization", myToken);
    // qDebug()<< "Tokeni checkCardStatusissa: " << myToken;

    QEventLoop loop;
    postManager = new QNetworkAccessManager(this);
    
    connect(postManager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    connect(postManager, &QNetworkAccessManager::finished, this,  [&cardStatus](QNetworkReply *reply) {
        QByteArray response_data = reply->readAll();
        // qDebug()<<"response_data" << response_data;


        QJsonDocument json_doc = QJsonDocument::fromJson(response_data); // luodaan QJsonDocument vastauksesta
        QJsonArray jsonArray = json_doc.array(); // viedään vastaus QJsonObjectiin

        if (!jsonArray.isEmpty())
        {
            QJsonObject json_obj = jsonArray[0].toObject();
            cardStatus = json_obj["islocked"].toInt();
            // qDebug()<<"Kortin lukitus status: " << cardStatus;
        }
    });

    reply = postManager->get(request);
    loop.exec();

    return cardStatus;
}

int login::fetchAttempts()
{
    int attempts = 0;
    qDebug()<<"\n\nfetchAttempts";
    QString url = Environment::base_url()+"/cards/check-card-attempts/"+ui->textUsername->text();
    // qDebug()<<url;

    QNetworkRequest request(url);

    QEventLoop loop;
    postManager = new QNetworkAccessManager(this);

    connect(postManager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    connect(postManager, &QNetworkAccessManager::finished, this,  [&attempts](QNetworkReply *reply) {
        QByteArray response_data = reply->readAll();
        // qDebug()<<"response_data" << response_data;
        // response_data "[{\"wrong_attempts\":0}]"

        QJsonDocument json_doc = QJsonDocument::fromJson(response_data); // luodaan QJsonDocument vastauksesta
        QJsonArray jsonArray = json_doc.array(); // viedään vastaus QJsonObjectiin

        if (!jsonArray.isEmpty())
        {
            QJsonObject json_obj = jsonArray[0].toObject();
            attempts = json_obj["wrong_attempts"].toInt();
            // qDebug()<<"Väärät yritykset (jsonarray): " << attempts;
        }
    });

    reply = postManager->get(request);
    loop.exec();


    // lähetä pyyntö serverille joka palauttaa väärien yritysten määrän, aseta muuttujaan attempts
    // qDebug()<<"FetchAttempts palauttaa arvon: " << attempts;
    return attempts;
}

void login::sendAttemptToServer(int wrongAttempt)
{
    qDebug()<<"sendAttemptToServer";
    QString url = Environment::base_url()+"/cards/update-card-attempts/"+ui->textUsername->text();
    //qDebug()<<url;
    //qDebug()<<"Väärät yritykset (sendAttemptToServer): " << wrongAttempt;
    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject jsonObj;
    jsonObj.insert("wrong_attempts",wrongAttempt);

    postManager = new QNetworkAccessManager(this);


    QEventLoop loop;
    connect(postManager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);

    connect(postManager, &QNetworkAccessManager::finished, this,  [](QNetworkReply *reply) {
        if (reply->error() == QNetworkReply::NoError)
        {
            qDebug() << "Väärä yritys lähetetty palvelimelle: " << reply->readAll();
        }
        else
        {
            qDebug() << "Väärän yrityksen lähetys epäonnistui" << reply->errorString();
        }
        reply->deleteLater();
    });


    reply = postManager->put(request, QJsonDocument(jsonObj).toJson());
    loop.exec();

    postManager->deleteLater();
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
    // qDebug()<<site_url;
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

                // tokeni talteen luokan sisälle
                myToken="Bearer "+response_data; // ei luoda erillistä QByteArrayta vaan käytetään privatessa olevaa
                // qDebug()<<"token tallennettu private muuttujaan: " << myToken;


                int cardLockStatus = checkCardStatus();
                qDebug() << "Kortin lukitus status: " << cardLockStatus;
                if (cardLockStatus == 1)
                {
                    // tee joku järkevämpi pop up ikkuna tms, tämä ajaa asiansa testauksessa
                    ui->labelInfo->setText("Kortti on lukittu");
                    return;
                }
                
                int cardType = checkCardType();

                cardInfo *objCardInfo= new cardInfo(this);
                objCardInfo->setUsername(ui->textUsername->text());
                objCardInfo->setMyToken(myToken);
                connect(objCardInfo, &QDialog::finished, this, &login::onWindowFinished);


                if (cardType == 1)
                {
                    DebitOrCredit *DOC = new DebitOrCredit(this);
                    DOC->open();
                    delay(); // 10 sekunnin viive, eli ei avaa objCardInfo->Open ikkunaa heti perään
                }

                else if (cardType == 0 && cardLockStatus == 0)// jos ei ole käytössä multi kortti ja kortti ei ole lukossa, avataan liittymä
                {
                    objCardInfo->open();
                }

            }
            else {
                ui->labelInfo->setText("Väärä tunnus/salasana");
                qDebug()<<"Väärä tunnus/salasana";

                //wrongattemptscounter alustetaan vasta tässä
                wrongAttemptsCounter = fetchAttempts(); // haetaan jo olemassa olevien väärin syötettyjen yritysten määrä (jos käynnistetään uudelleen tai yritetään joskus myöhemmin)
                if (wrongAttemptsCounter == 3) // jos vääriä yrityksiä on 3, ei kasvateta laskureita enempää
                {
                    ui->labelInfo->setText("Kortti lukittu");
                }
                else
                {
                    qDebug() << "väärien yritysten määrä (palautuksen jälkeen):" << wrongAttemptsCounter;
                    wrongAttemptsCounter++; // kasvatetaan väärien yritysten määrää
                    qDebug() << "väärien yritysten määrä (palautuksen ja +1 jälkeen):" << wrongAttemptsCounter;
                    sendAttemptToServer(wrongAttemptsCounter); // lähetetään väärän yrityksen numero serverille
                }
            }

        }

    }

    reply->deleteLater();
    postManager->deleteLater();
}
