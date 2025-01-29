#include "withdrawcash.h"
#include "ui_withdrawcash.h"
#include <QErrormessage>


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
    qDebug()<<"Yritetään nostaa rahaa: " + Qamount + "€";

    bool ok; // qt abstraction moment
    int amount = Qamount.toInt(&ok); // viedään intiksi ku qstring ei toimi if statementissä

    if (!ok) // jos intiksi konverttaaminen ei toimi (jos syötetään esim kirjaimia)
    {
        qDebug()<<"convert to int failed";
        QErrorMessage *errorMessage = new QErrorMessage(this);
        errorMessage->showMessage("Virheellinen arvo, syötä kokonaisluku kenttään");
    }

    else // jos intiksi muunto menee läpi
    {
        // jos jaollinen minimi setelillä (tällä hetkellä 20) tai 50 setelillä (pakosta tosi aina 50€+ summilla, joten or statement ei haittaa)
        if (amount%MINBILLSIZE == 0 || amount%50 == 0)
        {
            qDebug()<<"Jaollinen " + QString::number(MINBILLSIZE) + ":llä(pienin seteli), tai 50€"; // qstring moment
        }
        else
        {
            qDebug()<<"Ei jaollinen" + QString::number(MINBILLSIZE) + ":llä (pienin seteli), tai 50€";
        }
    }


}

