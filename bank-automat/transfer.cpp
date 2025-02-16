#include "transfer.h"
#include "ui_transfer.h"

Transfer::Transfer(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Transfer)
{
    ui->setupUi(this);

    // Luo ajastin ikkunalle
    timeoutTimer = new QTimer(this);
    timeoutTimer->setInterval(10000);
    timeoutTimer->start();
    // Kun aikakatkaisu tapahtuu, tämä ikkuna sulkeutuu
    connect(timeoutTimer, &QTimer::timeout, this, &Transfer::close);

}

Transfer::~Transfer()
{
    delete ui;
}

void Transfer::setAccountDataObject(accountData *accountData)
{
    myAccountDataObject = accountData;
}

void Transfer::on_btnCompleteTransfer_clicked() {
    timeoutTimer->start();  // Käynnistetään ajastin uudelleen

    QString receiverAccount = ui->lineReceiver->text();
    double transferAmount = ui->lineTransferAmount->text().toDouble();
    if(transferAmount < 0) { // Jos yrittää negatiivista siirtoa
        ui->labelTransferInfo->setText("Syötä positiivinen siirtosumma!");
    }
    else if (receiverAccount.isEmpty()) { // Jos ei ole kirjoittanut vastaanottajan tilinroa
        ui->labelTransferInfo->setText("Syötä vastaanottajan tilinumero!");
        return;
    }
    else if (transferAmount == 0 ) { // Jos ei ole kirjoittanut siirron määrää
        ui->labelTransferInfo->setText("Syötä siirron määrä!");
    }
    else if (myAccountDataObject == nullptr) { // Tarkista, että myAccountDataObject ei ole null
        qDebug() << "Virhe: myAccountDataObject on null!";
        ui->labelTransferInfo->setText("myAccountDataObject on null");
        return;
    } else {
        // Kun kaikki on kunnossa, tehdään siirto
        QString senderAccount = QString::number(myAccountDataObject->getAccountId());
        sendTransferRequest(senderAccount, receiverAccount, transferAmount);
    }
}

void Transfer::sendTransferRequest(const QString &senderAccount, const QString &receiverAccount, double transferAmount) {
    QJsonObject jsonObj;
    QNetworkRequest request(transferUrl);
    request.setRawHeader("Authorization", myAccountDataObject->getMyToken());

    // Varmistetaan, että myAccountDataObject on olemassa ja että sen tilitiedot saadaan haettua
    if (myAccountDataObject) {
        // Täytetään lähettäjän tilitiedot JSON-objektiin
        jsonObj.insert("sender_account", senderAccount); // Lähettäjän tili
        jsonObj.insert("receiver_account", receiverAccount); // Vastaanottajan tili
        jsonObj.insert("transfer_amount", transferAmount); // Siirrettävä summa
    } else {
        qDebug() << "Käyttäjän tiliä ei löytynyt";
        return;  // Jos ei löydy, lopetetaan tilisiirron lähettäminen
    }

    // Lähetetään JSON-objekti palvelimelle
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Muutetaan JSON-objekti QByteArray-muotoon
    QJsonDocument doc(jsonObj);
    QByteArray data = doc.toJson();

    // Lähetetään POST-pyyntö palvelimelle
    QNetworkReply *reply = manager->post(request, data);

    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            // Vastaus on saapunut ja se on virheetön
            QByteArray responseData = reply->readAll(); // Luetaan koko palvelimen vastaus
            qDebug() << "Vastaanotettu data:" << responseData; // Näytetään vastaanotettu data debug-ikkunassa

            // Muutetaan vastaanotettu data QJsonDocument-objektiksi
            QJsonDocument doc = QJsonDocument::fromJson(responseData);

            if (doc.isArray()) {
                // Jos vastaus on taulukko (esim. lista tileistä)
                QJsonArray jsonArray = doc.array();
                for (const QJsonValue &value : jsonArray) {
                    QJsonObject obj = value.toObject(); // Saadaan yksittäinen objekti taulukosta
                    qDebug() << "idaccount:" << obj["idaccount"].toInt();
                    qDebug() << "balance:" << obj["balance"].toString();
                }
            } else if (doc.isObject()) {
                // Jos vastaus on yksittäinen objekti (esim. tilin tiedot)
                QJsonObject jsonObj = doc.object();
                qDebug() << "idaccount:" << jsonObj["idaccount"].toInt();
                qDebug() << "balance:" << jsonObj["balance"].toString();
            } else {
                qDebug() << "Virheellinen JSON-vastaus";
            }
        } else {
            qDebug() << "Virhe: " << reply->errorString();  // Virheilmoitus
            ui->labelTransferInfo->setText("Siirto epäonnistui: " + reply->errorString());
        }
        reply->deleteLater();
    });
}

void Transfer::on_btnClose_clicked()
{
    this->close();
}
