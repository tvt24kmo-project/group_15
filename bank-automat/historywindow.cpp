#include "historywindow.h"
#include "ui_historywindow.h"
#include "environment.h"
#include <QStandardItemModel>
#include <QDebug>
#include "cardinfo.h"

#include <transactiondata.h>



HistoryWindow::HistoryWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::HistoryWindow)
{
    ui->setupUi(this);
    // Luo ajastin ikkunalle
    timeoutTimer = new QTimer(this);
    timeoutTimer->setInterval(10000);
    timeoutTimer->start();
    // Kun aikakatkaisu tapahtuu, tämä ikkuna sulkeutuu
    connect(timeoutTimer, &QTimer::timeout, this, &cardInfo::close);
}
void HistoryWindow::setAccountDataObject(accountData *accountData)
{
    myAccountDataObject = accountData;
}

HistoryWindow::~HistoryWindow()
{
    delete ui;
}


void HistoryWindow::on_btnUpdateTransactions_clicked()
{
    timeoutTimer->start();
    //Testauksen jälkeen base url environmentista
    //qDebug()<<"Transactionin sivu: ";
    //qDebug()<<transactionsUrl;
    //if (myAccountDataObject) {
    //    auto accountId = myAccountDataObject->getAccountId(); //Nonfunction
    //    auto webtoken = myAccountDataObject->getMyToken();
    //    auto nimi = myAccountDataObject->getUsername();
        //qDebug() << "Account ID:" << accountId;
        //qDebug() << "WEBtken : " << webtoken;
        //qDebug() << "WEBtken : " << nimi;
    //} else {
    //    qDebug() << "Error: myAccountDataObject is NULL!";
    //}

    //QJsonObject jsonObj;
    //jsonObj.insert("account_id", myAccountDataObject->getUsername());
    //qDebug()<<"json läpäisi ";
    //qDebug()<<jsonObj;
    //qDebug()<<transactionsUrl;
    

    // Vaihda tässä getUsername() tai getAccountIban() sen mukaan mikä versio tietokannasta itsellä on
    // jos käytät linux serveriä, pitää olla getAccountIban
    QNetworkRequest request(transactionsUrl+myAccountDataObject->getAccountIban()+"/"+listaa);
    //qDebug() << "Final URL: " << transactionsUrl + myAccountDataObject->getUsername() + "/" + listaa;

    request.setRawHeader("Authorization",myAccountDataObject->getMyToken());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    getManager = new QNetworkAccessManager(this);

    connect(getManager, SIGNAL(finished (QNetworkReply*)), this, SLOT(getTransactionsSlot(QNetworkReply*)));

    reply = getManager->get(request);


}


void HistoryWindow::getTransactionsSlot (QNetworkReply *reply)
{
    response_data=reply->readAll();
    //qDebug()<<"DATA : "+response_data;


    QJsonDocument json_doc = QJsonDocument::fromJson(response_data);
    QJsonArray json_array = json_doc.array();
    //qDebug()<<response_data.length();
    QList<transactiondata> transactionsList;
    foreach (const QJsonValue &value, json_array) {
        QJsonObject json_obj = value.toObject();
        QString datetime = json_obj["aika"].toString();
        QString account = QString::number(json_obj["tililtä"].toInt());
        QString type = json_obj["tapahtuma"].toString();
        QString amount = json_obj["määrä"].toString();
        //qDebug()<<account;
        QString balance = json_obj["saldo"].toString();
        //qDebug()<<balance;
        transactionsList.append(transactiondata(datetime, account, type, amount, balance));
        //qDebug()<<transactionsList[0].getAmount();
        //TULEVA DATA ON PELKKÄÄ STRINGIÄ

        QStandardItemModel *table_model = new QStandardItemModel(transactionsList.size(),5);
        table_model->setHeaderData(0, Qt::Horizontal, QObject::tr("aika"));
        table_model->setHeaderData(1, Qt::Horizontal, QObject::tr("Tilinumero"));
        table_model->setHeaderData(2, Qt::Horizontal, QObject::tr("Tilitapahtuma"));
        table_model->setHeaderData(3, Qt::Horizontal, QObject::tr("määrä"));
        table_model->setHeaderData(4, Qt::Horizontal, QObject::tr("saldo"));


        for (int row = 0; row < transactionsList.size(); ++row) {
            QStandardItem *transaction_datetime = new QStandardItem(transactionsList[row].getTransaction_datetime());
            table_model->setItem(row, 0, transaction_datetime);
            QStandardItem *account_iban = new QStandardItem(transactionsList[row].getAccount_iban());
            table_model->setItem(row, 1, account_iban);
            QStandardItem *transaction_type = new QStandardItem(transactionsList[row].getTransaction_type());
            table_model->setItem(row, 2, transaction_type);
            QStandardItem *amount = new QStandardItem(transactionsList[row].getAmount());
            table_model->setItem(row, 3, amount);
            QStandardItem *balance = new QStandardItem(transactionsList[row].getBalance());
            table_model->setItem(row, 4, balance);
        }

        ui->tableTransactions->setModel(table_model);
        ui->tableTransactions->resizeColumnsToContents();

    }

    //ui->tableTransactions->setModel(table_model);
    //ui->textBooks->setText(book);

    reply->deleteLater();
    getManager->deleteLater();
}


void HistoryWindow::on_btnReadNext_clicked()
{
    timeoutTimer->start(); // nollaa aika

    if (response_data.length() < 300){
    } else {
        int num = listaa.toInt();
        num +=5;
        HistoryWindow::listaa = QString::number(num);
        //qDebug()<<listaa;

        HistoryWindow::on_btnUpdateTransactions_clicked();
    }
}


void HistoryWindow::on_btnReadPrev_clicked()
{
    timeoutTimer->start(); // // Käynnistetään ajastin uudelleen

    if (listaa.toInt() <= 0){
    } else {
        int num = listaa.toInt();
        num -=5;
        HistoryWindow::listaa = QString::number(num);
        //qDebug()<<listaa;
        HistoryWindow::on_btnUpdateTransactions_clicked();
    }
}

void HistoryWindow::setListaa(const QString &newListaa)
{
    listaa = newListaa;
}

QString HistoryWindow::getListaa() const
{
    return listaa;
}


void HistoryWindow::on_btnClose_clicked()
{
    HistoryWindow::close();
}

