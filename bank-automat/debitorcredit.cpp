#include "debitorcredit.h"
#include "ui_debitorcredit.h"

DebitOrCredit::DebitOrCredit(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DebitOrCredit)
{
    ui->setupUi(this);
}

DebitOrCredit::~DebitOrCredit()
{
    delete ui;
}

void DebitOrCredit::on_buttonCredit_clicked()
{
    qDebug()<<"creditbutton clicked";
}


void DebitOrCredit::on_buttonDebit_clicked()
{
    qDebug()<<"debitbutton clicked";
}

