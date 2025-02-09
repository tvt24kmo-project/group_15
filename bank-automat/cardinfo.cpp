#include "cardinfo.h"
#include "ui_cardinfo.h"
#include "accountdata.h"
#include "withdrawcash.h"
#include "transfer.h"
#include "historywindow.h"

cardInfo::cardInfo(QWidget *parent) : QDialog(parent),ui(new Ui::cardInfo)
{
    ui->setupUi(this);

    // Luo ajastin ikkunalle
    timeoutTimer = new QTimer(this);
    timeoutTimer->setInterval(30000);
    timeoutTimer->start();
    // Kun aikakatkaisu tapahtuu, tämä ikkuna sulkeutuu
    connect(timeoutTimer, &QTimer::timeout, this, &cardInfo::close);

}

cardInfo::~cardInfo()
{
    delete ui;
}

void cardInfo::setUsername(const QString &newUsername)
{
    username = newUsername;
    ui->labelUsername->setText(username);
}

void cardInfo::setMyToken(const QByteArray &newMyToken)
{
    myToken = newMyToken;
    // qDebug()<<myToken;
}

void cardInfo::onWindowFinished(){
    timeoutTimer->start();  // Käynnistetään ajastin uudelleen
}

void cardInfo::on_btnData_clicked()
{
    timeoutTimer->stop(); // pysäytä ajastin
    accountData *objAccountData=new accountData(this);
    objAccountData->setUsername(username);
    objAccountData->setMyToken(myToken);
    connect(objAccountData, &QDialog::finished, this, &cardInfo::onWindowFinished);
    objAccountData->open();

}

void cardInfo::on_buttonWithdrawCash_clicked()
{
    timeoutTimer->stop(); // pysäytä ajastin
    WithdrawCash *objWithdrawCash = new WithdrawCash(this);
    accountData *objAccountData = new accountData(this);
    objAccountData->setUsername(username);
    objAccountData->setMyToken(myToken);
    objWithdrawCash->setAccountDataObject(objAccountData); // asetetaan accountData objekti withdrawCash objektiin
    connect(objWithdrawCash, &QDialog::finished, this, &cardInfo::onWindowFinished);
    objWithdrawCash->open();
}


void cardInfo::on_btnTransfer_clicked()
{
    timeoutTimer->stop(); // pysäytä ajastin
    // Tilisiirrot-nappia painettu
    Transfer *objTransfer = new Transfer(this);
    connect(objTransfer, &QDialog::finished, this, &cardInfo::onWindowFinished);
    objTransfer->open();
}


void cardInfo::on_btnHistory_clicked()
{
    timeoutTimer->stop(); // pysäytä ajastin
    accountData *objAccountData=new accountData(this);
    objAccountData->setUsername(username);
    objAccountData->setMyToken(myToken);

    HistoryWindow *objHistoryWindow = new HistoryWindow(this);
    objHistoryWindow->setAccountDataObject(objAccountData);
    connect(objHistoryWindow, &QDialog::finished, this, &cardInfo::onWindowFinished);
    objHistoryWindow->open();
}

