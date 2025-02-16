#include "cardinfo.h"
#include "ui_cardinfo.h"
#include "accountdata.h"
#include "withdrawcash.h"
#include "transfer.h"
#include "historywindow.h"

cardInfo::cardInfo(QWidget *parent, accountData *data) : 
    QDialog(parent)
    , ui(new Ui::cardInfo)
    , accountDataPtr(data)
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

    // Välitetään accountDataPtr Transfer-ikkunaan
    objTransfer->setAccountDataObject(accountDataPtr);

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

    HistoryWindow *objHistoryWindow = new HistoryWindow(this);
    objHistoryWindow->setAccountDataObject(accountDataPtr); // asetetaan accountData objekti historyWindow objektiin
    connect(objHistoryWindow, &QDialog::finished, this, &cardInfo::onWindowFinished);
    QRect geometry = this->geometry();  // Tallenna sijainti ja koko
    objHistoryWindow->setGeometry(geometry);
    objHistoryWindow->open();
}


void cardInfo::on_btnClose_clicked()
{
    cardInfo::close();
}

