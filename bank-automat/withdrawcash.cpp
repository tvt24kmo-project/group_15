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

void WithdrawCash::on_buttonOther_clicked()
{
    QString Qamount = ui->lineEditCustomAmount->text(); // haetaan muun rahamäärän luku
    qDebug()<<"Käytetään muuta määrää: " + Qamount + "€"; // tieto konsoliin seurantaa varten

    bool ok; // qt moment
    cashAmount = Qamount.toInt(&ok); // viedään intiksi ku qstring ei toimi if statementissä

    if (!ok) // jos intiksi konverttaaminen ei toimi (jos syötetään esim kirjaimia)
    {
        qDebug()<<"convert to int failed";
        QErrorMessage *errorMessage = new QErrorMessage(this); // luodaan uusi errormessageluokka
        errorMessage->showMessage("Virheellinen arvo, syötä kokonaisluku kenttään"); // näytetään pop up virhe
    }

    else if (ok)// jos intiksi muunto menee läpi
    {
        // jos jaollinen minimi setelillä (20) tai 50 setelillä (pakosta tosi aina 50€+ summilla, joten OR min ja 50 välillä ei aiheuta ongelmia)
        if (cashAmount%MINBILLSIZE == 0 || cashAmount%50 == 0)
        {
            qDebug()<<"Jaollinen " + QString::number(MINBILLSIZE) + ":llä(pienin seteli), tai 50€"; // qstring moment
            ui->labelAmount->setText(QString::number(cashAmount)); // muunnetaan rahamäärä qstringiksi ja labeliin
        }
        else // jos rahamäärää ei voi pilkkoa minimiseteliin (20) tai 50€ seteleihin
        {
            qDebug()<<"Ei jaollinen" + QString::number(MINBILLSIZE) + ":llä (pienin seteli), tai 50€";
            QErrorMessage *errorMessage = new QErrorMessage(this); // luodaan uusi errormessageluokka
            errorMessage->showMessage("Rahamäärää " + QString::number(cashAmount) + " ei voi jakaa seteleiksi!"); // näytetään pop up virhe
        }
    }


}
/*
 * jos painetaan mitä tahansa rahamäärää
 * päivitä noston määrän muuttuja
 * näytä uusi arvo labelissa
*/

void WithdrawCash::on_button20_clicked()
{
    cashAmount = 20;
    ui->labelAmount->setText("20");
}


void WithdrawCash::on_button40_clicked()
{
    cashAmount = 40;
    ui->labelAmount->setText("40");
}


void WithdrawCash::on_button50_clicked()
{
    cashAmount = 50;
    ui->labelAmount->setText("50");
}


void WithdrawCash::on_button100_clicked()
{
    cashAmount = 100;
    ui->labelAmount->setText("100");
}


void WithdrawCash::on_withdrawCash_clicked()
{
    if(cashAmount < 0) // jos yrittää negatiivista lukua
    {
        QErrorMessage *errorMessage = new QErrorMessage(this);
        errorMessage->showMessage("Negatiivista nostoa ei voi tehdä");
    }
    else if (cashAmount == 0 )
    {
        QErrorMessage *errorMessage = new QErrorMessage(this);
        errorMessage->showMessage("Syötä noston määrä ensin");
    }
    else
    {
        qDebug()<<"Nostetaan " + QString::number(cashAmount) + "€"; // tieto konsoliin seurantaa varten
        ui->labelAmount->setText("0"); // nollataan labeli
        ui->lineEditCustomAmount->setText(""); // nollataan muu määrä

        sendWithdrawRequest(cashAmount); // lähetetään nostopyyntö
    }
}

void WithdrawCash::sendWithdrawRequest(int amount)
{
    QJsonObject jsonObj;
    jsonObj.insert("account_id", 7);
    jsonObj.insert("amount", cashAmount);
    qDebug()<<jsonObj;
    qDebug()<<withdrawUrl;
    cashAmount = 0; // nollataan luvun keruun jälkeen
}

