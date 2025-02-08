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

        qDebug() << "Siirretään " + QString::number(transferAmount) + "€ tilille " + receiverAccount;
        ui->labelTransferInfo->setText("Siirto suoritettu onnistuneesti!");
    }
}
