#ifndef HISTORYWINDOW_H
#define HISTORYWINDOW_H

#include <QDialog>
#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include "transactiondata.h"
#include "accountdata.h"
#include "environment.h"

namespace Ui {
class HistoryWindow;
}

class HistoryWindow : public QDialog
{
    Q_OBJECT

public:
    explicit HistoryWindow(QWidget *parent = nullptr);
    ~HistoryWindow();
    void setAccountDataObject(accountData *accountData);

    QString getListaa() const;

    void setListaa(const QString &newListaa);

private slots:
    void on_btnUpdateTransactions_clicked();

    void getTransactionsSlot (QNetworkReply *reply);

    void on_btnReadPrev_clicked();

    void on_btnReadNext_clicked();

    void on_btnClose_clicked();

private:
    Ui::HistoryWindow *ui;
    QList<transactiondata> TDL;
    QNetworkAccessManager *getManager;
    QNetworkReply *reply;
    QByteArray response_data;
    QString listaa = "0";
    accountData* myAccountDataObject;
    QString transactionsUrl = Environment::base_url() + "/transactions/";
    //QString username;
    //QByteArray myToken;

};

#endif // HISTORYWINDOW_H
