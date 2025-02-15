#include "cardinfo.h"
#include "environment.h"
#include "login.h"
#include "ui_login.h"
#include "debitorcredit.h"
#include "accountdata.h"

login::login(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::login)
    , accountDataPtr(new accountData(this))
    , objCardInfo(nullptr)
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
    delete accountDataPtr;
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
    // qDebug()<<"\n\nfetchAttempts";
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
    // qDebug()<<"sendAttemptToServer";
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
            //qDebug() << "Väärä yritys lähetetty palvelimelle: " << reply->readAll();
        }
        else
        {
            //qDebug() << "Väärän yrityksen lähetys epäonnistui" << reply->errorString();
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
    if (objCardInfo != nullptr)
    {
        objCardInfo->deleteLater();
        objCardInfo = nullptr;
    }
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
                qDebug()<<"login::loginSlot: token tallennettu private muuttujaan: " << myToken;


                int cardLockStatus = checkCardStatus();
                qDebug() << "login::loginSlot: Kortin lukitus status: " << cardLockStatus;
                if (cardLockStatus == 1)
                {
                    // tee joku järkevämpi pop up ikkuna tms, tämä ajaa asiansa testauksessa
                    ui->labelInfo->setText("Kortti on lukittu");
                    return;
                }

                int cardType = checkCardType();
                qDebug() << "login::loginSlot: Kortin tyyppi: " << cardType;

                accountDataPtr->setUsername(ui->textUsername->text());
                accountDataPtr->setMyToken(myToken);

                sendAttemptToServer(0); // nollataan väärät yritykset, koska tässä kirjautuminen on onnistunut ja tarkistettu kortin lukitusstatus ylempänä


                if (cardType == 1)
                {
                    DebitOrCredit *DOC = new DebitOrCredit(this, accountDataPtr);
                    DOC->findChild<QLabel*>("label")->setText(ui->textUsername->text()); //mitvit
                    connect(DOC, &DebitOrCredit::debitChosen, this, &login::handleDebitChosen);
                    connect(DOC, &DebitOrCredit::creditChosen, this, &login::handleCreditChosen);
                    DOC->open();
                    // delay(); // 10 sekunnin viive, eli ei avaa objCardInfo->Open ikkunaa heti perään
                }

                else if (cardType == 0 && cardLockStatus == 0)
                {
                    QString account_url = Environment::base_url() + "/accounts/" + ui->textUsername->text();
                    QNetworkRequest account_request(account_url);
                    account_request.setRawHeader("Authorization", myToken);

                    QNetworkAccessManager *accountManager = new QNetworkAccessManager(this);

                    QEventLoop loop;
                    connect(accountManager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);

                    QNetworkReply *account_reply = accountManager->get(account_request);
                    loop.exec();

                    QByteArray account_data = account_reply->readAll();
                    QJsonDocument account_doc = QJsonDocument::fromJson(account_data);

                    if (account_doc.isArray()) {
                        QJsonArray accountArray = account_doc.array();
                        if (!accountArray.isEmpty()) {
                            QJsonObject accountObject = accountArray.first().toObject(); // haetaan yksi ja ainoa tili

                            int accountId = accountObject["idaccount"].toInt();
                            accountDataPtr->setAccountId(accountId); // NYT VASTA asetetaan accountID, koska se on haettu vasta tässä vaiheessa

                            // ** GET CUSTOMER ID **
                            QString customer_url = Environment::base_url() + "/cards/customer/" + ui->textUsername->text();
                            qDebug() << "login::loginSlot: Customer ID URL: " << customer_url;
                            QNetworkRequest customer_request(customer_url);
                            customer_request.setRawHeader("Authorization", myToken);
                            QNetworkAccessManager *customerManager = new QNetworkAccessManager(this);

                            QEventLoop customerLoop;
                            connect(customerManager, &QNetworkAccessManager::finished, &customerLoop, &QEventLoop::quit);
                            QNetworkReply* customer_reply = customerManager->get(customer_request);
                            customerLoop.exec();

                            QByteArray customer_data = customer_reply->readAll();
                            qDebug() << "login::loginSlot: Customer data: " << customer_data;
                            QJsonDocument customer_doc = QJsonDocument::fromJson(customer_data);
                            qDebug() << "login::loginSlot: Customer doc: " << customer_doc;
                            int customerID = -1; // Initialize with an invalid value
                            if (customer_doc.isArray()) {
                                QJsonArray customerArray = customer_doc.array();
                                if (!customerArray.isEmpty()) {
                                    QJsonObject customerObject = customerArray.first().toObject();
                                    customerID = customerObject["idcustomer"].toInt();
                                    qDebug() << "login::loginSlot: Customer ID: " << customerID;
                                }
                            }
                            customer_reply->deleteLater();
                            customerManager->deleteLater();

                            // ** NOW we can create cardInfo and set all necessary data **
                            objCardInfo = new cardInfo(this, accountDataPtr);
                            objCardInfo->setUsername(ui->textUsername->text());
                            objCardInfo->setMyToken(myToken);
                            objCardInfo->setCustomerID(customerID); // ** SET CUSTOMER ID **
                            connect(objCardInfo, &QDialog::finished, this, &login::onWindowFinished);
                            QRect geometry = this->geometry();  // Tallenna sijainti ja koko
                            objCardInfo->setGeometry(geometry);
                            objCardInfo->open();

                        } else {
                            qDebug() << "Account array is empty!"; // ei pitäisi ikinä tapahtua
                        }
                    }
                    account_reply->deleteLater();
                    accountManager->deleteLater();
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
                else // jos kortti ei ole lukittu, kasvatetaan yritysten määrää
                {
                    //qDebug() << "väärien yritysten määrä (palautuksen jälkeen):" << wrongAttemptsCounter;
                    wrongAttemptsCounter++; // kasvatetaan väärien yritysten määrää
                    //qDebug() << "väärien yritysten määrä (palautuksen ja +1 jälkeen):" << wrongAttemptsCounter;
                    sendAttemptToServer(wrongAttemptsCounter); // lähetetään väärän yrityksen numero serverille
                }
            }

        }

    }

    reply->deleteLater();
    postManager->deleteLater();
}




void login::handleDebitChosen(int accountId) {
    // qDebug() << "handleDebitChosen called, Account ID: " << accountId;

    accountDataPtr->setUsername(ui->textUsername->text());
    accountDataPtr->setMyToken(myToken);
    accountDataPtr->setAccountId(accountId);

    accountDataPtr->fetchData();

    // Pass accountDataPtr to cardInfo
    objCardInfo = new cardInfo(this, accountDataPtr);
    objCardInfo->setUsername(ui->textUsername->text());
    objCardInfo->setMyToken(accountDataPtr->getMyToken());
    connect(objCardInfo, &QDialog::finished, this, &login::onWindowFinished);
    objCardInfo->open();
}

void login::handleCreditChosen(int accountId) {
    // qDebug() << "handleCreditChosen called, Account ID: " << accountId;

    accountDataPtr->setUsername(ui->textUsername->text());
    accountDataPtr->setMyToken(myToken);
    accountDataPtr->setAccountId(accountId);

    accountDataPtr->fetchData();

    // Pass accountDataPtr to cardInfo
    objCardInfo = new cardInfo(this, accountDataPtr);
    objCardInfo->setUsername(ui->textUsername->text());
    objCardInfo->setMyToken(accountDataPtr->getMyToken());
    connect(objCardInfo, &QDialog::finished, this, &login::onWindowFinished);
    objCardInfo->open();
}

void login::on_btnCancel_clicked()
{
    this->close();
}



