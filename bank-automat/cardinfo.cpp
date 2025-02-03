#include "cardinfo.h"
#include "ui_cardinfo.h"
#include "accountdata.h"
#include "withdrawcash.h"
#include "transfer.h"
#include "historywindow.h"

cardInfo::cardInfo(QWidget *parent) : QDialog(parent),ui(new Ui::cardInfo)
{
    ui->setupUi(this);
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
    qDebug()<<myToken;
}

void cardInfo::on_btnData_clicked()
{
    accountData *objAccountData=new accountData(this);
    objAccountData->setUsername(username);
    objAccountData->setMyToken(myToken);
    objAccountData->open();
}


void cardInfo::on_buttonWithdrawCash_clicked()
{
    WithdrawCash *objWithdrawCash = new WithdrawCash(this);
    accountData *objAccountData = new accountData(this);
    objAccountData->setUsername(username);
    objAccountData->setMyToken(myToken);
    objWithdrawCash->setAccountDataObject(objAccountData); // asetetaan accountData objekti withdrawCash objektiin 
    objWithdrawCash->open();
}


void cardInfo::on_btnTransfer_clicked()
{
    // Tilisiirrot-nappia painettu
    Transfer *objTransfer = new Transfer(this);
    objTransfer->open();
}


void cardInfo::on_btnHistory_clicked()
{
    accountData *objAccountData=new accountData(this);
    objAccountData->setUsername(username);
    objAccountData->setMyToken(myToken);

    HistoryWindow *objHistoryWindow = new HistoryWindow(this);
    objHistoryWindow->setAccountDataObject(objAccountData);
    objHistoryWindow->open();
}

