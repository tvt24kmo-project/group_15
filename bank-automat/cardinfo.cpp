#include "cardinfo.h"
#include "ui_cardinfo.h"
#include "accountdata.h"
#include "withdrawcash.h"
#include "transfer.h"
#include "historywindow.h"
#include "login.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QApplication>

cardInfo::cardInfo(QWidget *parent, accountData *data) : 
    QDialog(parent)
    , ui(new Ui::cardInfo)
    , accountDataPtr(data)
    , imageFetcher(new ImageFetcher(this))
{
    ui->setupUi(this);

    connect(imageFetcher, &ImageFetcher::imageFetched, this, &cardInfo::onImageFetched);
    connect(imageFetcher, &ImageFetcher::fetchFailed, this, &cardInfo::onFetchFailed);


    // Luo ajastin ikkunalle
    timeoutTimer = new QTimer(this);
    timeoutTimer->setInterval(30000);
    timeoutTimer->start();
    // Kun aikakatkaisu tapahtuu, tämä ikkuna sulkeutuu
    connect(timeoutTimer, &QTimer::timeout, this, &cardInfo::close);


    QString filename = "landingpage.png";
    QString url = Environment::base_url() + "/images/" + filename;
    imageFetcher->fetchImage(QUrl(url));
}

void cardInfo::onImageFetched(const QPixmap &pixmap) // kun kuva on haettu niin asetetaan se labeliin
{
    ui->labelBG->setPixmap(pixmap); // asetetaan pixmap labeliin
    ui->labelBG->setScaledContents(true); // skaalataan kuva labeliin sopivaksi
}

void cardInfo::onFetchFailed(const QString &error) // jos haku epäonnistuu niin asetetaan labeliin virheilmoitus
{
    ui->labelBG->setText("Kuvan haku epäonnistui: " + error); // jos kuva ei lataudu niin asetetaan labeliin virheilmoitus (siiretäänkö QDebugiin mieluummin??)
}




cardInfo::~cardInfo()
{
    delete ui;
}

void cardInfo::setUsername(const QString &newUsername)
{
    username = newUsername;
    //ui->labelUsername->setText(username);
}

void cardInfo::setCustomerID(const int &newCustomerID)
{
    //qDebug() << "cardInfo::setCustomerID: customerID received: " << newCustomerID; // More specific debug
    customerID = newCustomerID;
    fetchFullName(customerID);
}

void cardInfo::fetchFullName(int customerId) {
    //qDebug() << "cardInfo::fetchFullName: Customer ID: " << customerId;
    QString url = Environment::base_url() + "/customers/fullname/" + QString::number(customerId);
    //qDebug() << "cardInfo::fetchFullName: URL: " << url;
    QNetworkRequest request((url));

    //qDebug() << "cardInfo::fetchFullName: Token before setting header: " << myToken;
    request.setRawHeader("Authorization", myToken);
    //qDebug() << "cardInfo::fetchFullName: Token after setting header: " << request.rawHeader("Authorization");

    QNetworkAccessManager *nam = new QNetworkAccessManager(this);
    connect(nam, &QNetworkAccessManager::finished, this, &cardInfo::onFullNameFetched);
    nam->get(request);
}


void cardInfo::onFullNameFetched(QNetworkReply *reply) {
    //qDebug() << "cardInfo::onFullNameFetched: Entered";
    if (reply->error()) {
        //qDebug() << "cardInfo::onFullNameFetched: Network error: " << reply->errorString();
        ui->labelUsername->setText("virhe");
        reply->deleteLater();
        return;
    }

    QByteArray response_data = reply->readAll();
    //qDebug() << "cardInfo::onFullNameFetched: Raw response data: " << response_data;
    QJsonDocument json_doc = QJsonDocument::fromJson(response_data);
    //qDebug() << "cardInfo::onFullNameFetched: Parsed JSON doc: " << json_doc;

    if (json_doc.isArray()) {
        QJsonArray json_array = json_doc.array();
        //qDebug() << "cardInfo::onFullNameFetched: JSON array: " << json_array;
        if (!json_array.isEmpty()) {
            QJsonObject json_obj = json_array.first().toObject();
            //qDebug() << "cardInfo::onFullNameFetched: JSON object: " << json_obj;
            QString fullName = json_obj["fname"].toString() + " " + json_obj["lname"].toString();
            //qDebug() << "cardInfo::onFullNameFetched: Extracted full name: " << fullName;
            ui->labelUsername->setText(fullName);
        } else {
            //qDebug() << "cardInfo::onFullNameFetched: JSON array is empty";
            ui->labelUsername->setText("nimeä ei löydy");
        }
    } else {
        //qDebug() << "cardInfo::onFullNameFetched: JSON doc is not an array";
        ui->labelUsername->setText("väärä vastaus");
    }

    reply->deleteLater();
}

void cardInfo::setMyToken(const QByteArray &newMyToken)
{
    //qDebug() << "cardInfo::setMyToken: Token received: " << newMyToken; // Debug the token
    myToken = newMyToken;
}

void cardInfo::onWindowFinished(){
    timeoutTimer->start();  // Käynnistetään ajastin uudelleen
}

void cardInfo::on_btnData_clicked()
{
    timeoutTimer->stop(); // pysäytä ajastin
    /* käytetään accountDataPtr:ää joten ei tarvitse tehdä uutta objektia
    accountData *objAccountData=new accountData(this);
    objAccountData->setUsername(username);
    objAccountData->setMyToken(myToken);
    */
    accountDataPtr->fetchData();
    connect(accountDataPtr, &QDialog::finished, this, &cardInfo::onWindowFinished);
    QRect geometry = this->geometry();  // Tallenna sijainti ja koko
    accountDataPtr->setGeometry(geometry);
    accountDataPtr->open();

}

void cardInfo::on_buttonWithdrawCash_clicked()
{
    timeoutTimer->stop(); // pysäytä ajastin
    WithdrawCash *objWithdrawCash = new WithdrawCash(this);
    /*
    accountData *objAccountData = new accountData(this);
    objAccountData->setUsername(username);
    objAccountData->setMyToken(myToken);
    objAccountData->fetchData();
    objWithdrawCash->setAccountDataObject(objAccountData); // asetetaan accountData objekti withdrawCash objektiin
    */

    objWithdrawCash->setAccountDataObject(accountDataPtr); // asetetaan accountData objekti withdrawCash objektiin
    connect(objWithdrawCash, &QDialog::finished, this, &cardInfo::onWindowFinished);
    QRect geometry = this->geometry();  // Tallenna sijainti ja koko
    objWithdrawCash->setGeometry(geometry);
    objWithdrawCash->open();
}


void cardInfo::on_btnTransfer_clicked()
{
    timeoutTimer->stop(); // pysäytä ajastin
    // Tilisiirrot-nappia painettu
    Transfer *objTransfer = new Transfer(this);
    connect(objTransfer, &QDialog::finished, this, &cardInfo::onWindowFinished);
    QRect geometry = this->geometry();  // Tallenna sijainti ja koko
    objTransfer->setGeometry(geometry);
    objTransfer->open();
}


void cardInfo::on_btnHistory_clicked()
{
    timeoutTimer->stop(); // pysäytä ajastin
    /*
    accountData *objAccountData=new accountData(this);
    objAccountData->setUsername(username);
    objAccountData->setMyToken(myToken);
    objAccountData->fetchData();
    */

    accountDataPtr->setUsername(username); // ladataan "hiljaa" käyttäjän tiedot koska historiaa ei näy jos ei avaa accountdataa ensin.
    accountDataPtr->setMyToken(myToken);
    accountDataPtr->fetchData();

    HistoryWindow *objHistoryWindow = new HistoryWindow(this);
    objHistoryWindow->setAccountDataObject(accountDataPtr); // asetetaan accountData objekti historyWindow objektiin
    connect(objHistoryWindow, &QDialog::finished, this, &cardInfo::onWindowFinished);
    QRect geometry = this->geometry();  // Tallenna sijainti ja koko
    objHistoryWindow->setGeometry(geometry);
    objHistoryWindow->open();
}

void cardInfo::closeEvent(QCloseEvent *event)
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

void cardInfo::on_btnClose_clicked()
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

    this->close();
}

