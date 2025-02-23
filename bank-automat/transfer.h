#ifndef TRANSFER_H
#define TRANSFER_H

#include <QDialog>
#include <QDebug>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>

#include "accountdata.h"
#include "environment.h"

namespace Ui {
class Transfer;
}

class Transfer : public QDialog
{
    Q_OBJECT

public:
    explicit Transfer(QWidget *parent = nullptr);
    ~Transfer();
    void setAccountDataObject(accountData *accountData);

private slots:
    void on_btnCompleteTransfer_clicked();
    void on_btnClose_clicked();

private:
    Ui::Transfer *ui;
    QString transferUrl = Environment::base_url() + "/procedures/transfer";
    QTimer *timeoutTimer;
    void sendTransferRequest(const QString &senderAccount, const QString &receiverAccount, double transferAmount);
    accountData* myAccountDataObject = nullptr;
};

#endif // TRANSFER_H
