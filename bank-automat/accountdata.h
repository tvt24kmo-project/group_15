#ifndef ACCOUNTDATA_H
#define ACCOUNTDATA_H

#include <QDialog>
#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QTimer>

namespace Ui {
class accountData;
}

class accountData : public QDialog
{
    Q_OBJECT

public:
    explicit accountData(QWidget *parent = nullptr);
    ~accountData();

    void setUsername(const QString &newUsername);

    void setMyToken(const QByteArray &newMyToken);

    int getCustomerId();

    int getAccountId();

    QByteArray getMyToken();

    QString getUsername() const;
    
    QString getAccountIban() const;

    void fetchData();

    void setAccountId(int id);
private slots:
    void showDataSlot(QNetworkReply *reply);
private:
    Ui::accountData *ui;
    QString username;
    QByteArray myToken;

    QNetworkAccessManager *dataManager;
    QNetworkReply *reply;
    QByteArray response_data;

    int accountId = 0;
    QTimer *timeoutTimer;
    QJsonObject findAccountObject(const QJsonArray &jsonArray, int targetAccountId);
};

#endif // ACCOUNTDATA_H
