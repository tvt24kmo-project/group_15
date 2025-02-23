#include "withdrawcash.h"
#include "ui_withdrawcash.h"


WithdrawCash::WithdrawCash(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::WithdrawCash)
{
    ui->setupUi(this);

    // Luo ajastin ikkunalle
    timeoutTimer = new QTimer(this);
    timeoutTimer->setInterval(10000);
    timeoutTimer->start();
    // Kun aikakatkaisu tapahtuu, tämä ikkuna sulkeutuu
    connect(timeoutTimer, &QTimer::timeout, this, &WithdrawCash::close);
}

void WithdrawCash::setAccountDataObject(accountData *dataObj)
{
    myAccountDataObject = dataObj;
}

WithdrawCash::~WithdrawCash()
{
    delete ui;
}

void WithdrawCash::on_buttonOther_clicked()
{
    timeoutTimer->start();  // Käynnistetään ajastin uudelleen

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
        // ja myös jos jaollinen 20+50 luvuilla, kuten 70, 120, 170 jne.
        if (cashAmount > maxWithdrawSize)
        {
            QErrorMessage *errorMessage = new QErrorMessage(this); // luodaan uusi errormessageluokka
            errorMessage->showMessage("Rahamäärä " + QString::number(cashAmount) + " on liian suuri!"); // näytetään pop up virhe
        }
        else if (cashAmount >= MINBILLSIZE && (cashAmount % MINBILLSIZE == 0 || cashAmount % 50 == 0 || cashAmount % 50 % MINBILLSIZE == 0))
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
    timeoutTimer->start();  // Käynnistetään ajastin uudelleen

    cashAmount = 20;
    ui->labelAmount->setText("20");
}


void WithdrawCash::on_button40_clicked()
{
    timeoutTimer->start();  // Käynnistetään ajastin uudelleen

    cashAmount = 40;
    ui->labelAmount->setText("40");
}


void WithdrawCash::on_button50_clicked()
{
    timeoutTimer->start();  // Käynnistetään ajastin uudelleen

    cashAmount = 50;
    ui->labelAmount->setText("50");
}


void WithdrawCash::on_button100_clicked()
{
    timeoutTimer->start();  // Käynnistetään ajastin uudelleen

    cashAmount = 100;
    ui->labelAmount->setText("100");
}


void WithdrawCash::on_withdrawCash_clicked()
{
    timeoutTimer->start();  // Käynnistetään ajastin uudelleen

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

    // hirveää soppakoodia mutta tällä estetään *taas* liian ison määrän nosto
    else if(cashAmount > maxWithdrawSize)
    {
        QErrorMessage *errorMessage = new QErrorMessage(this); // luodaan uusi errormessageluokka
        errorMessage->showMessage("Rahamäärä " + QString::number(cashAmount) + " on liian suuri!"); // näytetään pop up virhe
    }

    // VIELÄ KERRAN tarkistetaan että määrä on varmasti rajojen sisällä,
    // koska jos laittaa arvoksi esim 30 ja painaa tosi nopeasti "tee nosto" niin sen saa vietyä läpi ennen kun muun arvon tarkistuksen virheikkuna avautuu
    else if (cashAmount >= MINBILLSIZE && (cashAmount % MINBILLSIZE == 0 || cashAmount % 50 == 0 || cashAmount % 50 % MINBILLSIZE == 0))
    {
        qDebug()<<"Nostetaan " + QString::number(cashAmount) + "€"; // tieto konsoliin seurantaa varten
        ui->labelAmount->setText("0"); // nollataan labeli
        ui->lineEditCustomAmount->setText(""); // nollataan muu määrä

        sendWithdrawRequest(cashAmount); // lähetetään nostopyyntö
    }

    else
    {
        QErrorMessage *errorMessage = new QErrorMessage(this);
        errorMessage->showMessage("Syötä summa ennen nostoa");
    }
}

void WithdrawCash::sendWithdrawRequest(int amount)
{   
    QJsonObject jsonObj;
    if (myAccountDataObject)
    {
        jsonObj.insert("account_id", myAccountDataObject->getAccountId()); // hakee accountdatasta accountid:n
        jsonObj.insert("amount", cashAmount);
    }
    else
    {
        qDebug()<<"Virhe";
        return;
    }

    qDebug()<<jsonObj;
    qDebug()<<withdrawUrl;

    QNetworkRequest request(withdrawUrl);
    request.setRawHeader("Authorization", myAccountDataObject->getMyToken());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &WithdrawCash::handleWithdrawResponse);

    QJsonDocument doc(jsonObj);
    QByteArray data = doc.toJson();
    qDebug() << "Sending withdraw request:" << data;

    manager->post(request, data);

    cashAmount = 0; // nollataan luvun keruun jälkeen
}


// muutos: jos tilillä ei ole tarpeeksi rahaa, se lähettää 400 koodin mikä olisi ollut network error, joten nyt käsitellään kaikki koodit
void WithdrawCash::handleWithdrawResponse(QNetworkReply *reply)
{
    QByteArray response_data = reply->readAll();
    qDebug() << "Withdraw response:" << response_data;

    QJsonDocument jsonDoc = QJsonDocument::fromJson(response_data);
    if (!jsonDoc.isNull()) {
        QJsonObject jsonObj = jsonDoc.object();
        qDebug() << "Response JSON:" << jsonObj;
        
        QString error = jsonObj["error"].toString();
        if (!error.isEmpty()) {
            QErrorMessage *errorMessage = new QErrorMessage(this);
            if (error == "Insufficient funds.") {
                errorMessage->showMessage("Tilillä ei ole riittävästi katetta nostoa varten.");
            } else {
                errorMessage->showMessage("Nosto epäonnistui: " + error);
            }
            return;
        }

        QString message = jsonObj["message"].toString();
        if (message == "Withdrawal successful.") {
            QMessageBox::information(this, "Success", "Nosto onnistui!");
        } else {
            QErrorMessage *errorMessage = new QErrorMessage(this);
            errorMessage->showMessage("Nosto epäonnistui: " + message);
        }
    } else {
        if (reply->error() != QNetworkReply::NoError) {
            QErrorMessage *errorMessage = new QErrorMessage(this);
            errorMessage->showMessage("Verkkovirhe: " + reply->errorString());
        }
    }

    reply->deleteLater();
    manager->deleteLater();
}

void WithdrawCash::on_btnClose_clicked()
{
    WithdrawCash::close();
}

