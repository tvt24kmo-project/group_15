#include "withdrawcash.h"
#include "ui_withdrawcash.h"


WithdrawCash::WithdrawCash(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::WithdrawCash)
{
    ui->setupUi(this);
}

WithdrawCash::~WithdrawCash()
{
    delete ui;
}

void WithdrawCash::on_buttonWithdrawOther_clicked()
{
    QString Qamount = ui->lineEditCustomAmount->text(); // haetaan muun rahamäärän luku
    qDebug()<<"Yritetään nostaa rahaa:" + Qamount + "€";

    bool ok; // qt abstraction moment
    int amount = Qamount.toInt(&ok); // viedään intiksi ku qstring ei toimi if statementissä

    if (!ok)
    {
        qDebug()<<"convert to int failed";
    }

    else
    {
        if (amount%MINBILLSIZE == 0) // jos
        {
            qDebug()<<"Jaollinen " + QString::number(MINBILLSIZE) + ":llä(pienin seteli)"; // qstring moment
        }
        else
        {
            qDebug()<<"Ei jaollinen 20:llä ";
        }
    }


}

