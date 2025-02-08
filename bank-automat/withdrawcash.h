#ifndef WITHDRAWCASH_H
#define WITHDRAWCASH_H

#include <QDialog>
#include <QErrorMessage>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QTimer>

#include "environment.h"
#include "accountdata.h"

namespace Ui {
class WithdrawCash;
}

class WithdrawCash : public QDialog
{
    Q_OBJECT

public:
    explicit WithdrawCash(QWidget *parent = nullptr);
    ~WithdrawCash();
    void setAccountDataObject(accountData *accountData);

private slots:
    void on_buttonOther_clicked();
    void on_button20_clicked();
    void on_button40_clicked();
    void on_button50_clicked();
    void on_button100_clicked();
    void on_withdrawCash_clicked();
    void handleWithdrawResponse(QNetworkReply *reply);

private:
    int MINBILLSIZE = 20; // pienin seteli mitä automaatti antaa muun määrän nostoon (esim 2x20€ = ok, 1x5 tai 3x5 = ei ok)
    int cashAmount = 0; // muuttuja jossa arvo muuttuu näppäinten/muun määrän perusteella
    Ui::WithdrawCash *ui;
    QNetworkAccessManager *manager;
    QString withdrawUrl = Environment::base_url() + "/procedures/withdraw";
    void sendWithdrawRequest(int amount);
    accountData* myAccountDataObject = nullptr;
    QTimer *timeoutTimer;
};

#endif // WITHDRAWCASH_H
