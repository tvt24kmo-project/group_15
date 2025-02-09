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

void Transfer::on_btnCompleteTransfer_clicked() {
    timeoutTimer->start();  // Käynnistetään ajastin uudelleen

    QString receiverAccount = ui->lineReceiver->text();
    double transferAmount = ui->lineTransferAmount->text().toDouble();
    if(transferAmount < 0) { // Jos yrittää negatiivista siirtoa
        ui->labelTransferInfo->setText("Syötä positiivinen siirtosumma!");
    }
    else if (receiverAccount.isEmpty()) {
        ui->labelTransferInfo->setText("Syötä vastaanottajan tilinumero!");
        return;
    }
    else if (transferAmount == 0 ) {
        ui->labelTransferInfo->setText("Syötä siirron määrä!");
    }
    else {
        qDebug()<<"Siirretään " + QString::number(transferAmount) + "€ tilille" ; // Tieto konsoliin seurantaa varten
        ui->lineTransferAmount->setText("0"); // Nollataan labeli
        ui->labelTransferInfo->clear(); // Tyhjennetään mahdolliset aiemmat virheilmoitukset
        ui->labelTransferInfo->setText("Siirto suoritettu onnistuneesti!");
    }
}

void Transfer::sendTransferRequest(const QString &senderAccount, const QString &receiverAccount, double transferAmount) {
    QJsonObject transferData;
    transferData["sender_account"] = senderAccount; // Lähettäjän tili
    transferData["receiver_account"] = receiverAccount;
    transferData["transfer_amount"] = transferAmount;

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest request(QUrl("http://localhost:3000/transfer"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonDocument doc(transferData);
    QByteArray data = doc.toJson();
    QNetworkReply *reply = manager->post(request, data);

    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            ui->labelTransferInfo->setText("Siirto suoritettu onnistuneesti!");
        } else {
            ui->labelTransferInfo->setText("Siirto epäonnistui: " + reply->errorString());
        }
        reply->deleteLater(); // Vapautetaan vastaus
    });
}
