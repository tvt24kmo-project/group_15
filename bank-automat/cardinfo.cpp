#include "cardinfo.h"
#include "ui_cardinfo.h"
#include "accountdata.h"
#include "withdrawcash.h"

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
    objWithdrawCash->open();
}

