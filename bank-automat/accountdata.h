#ifndef ACCOUNTDATA_H
#define ACCOUNTDATA_H

#include <QDialog>
#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QJsonDocument>
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

private slots:
    void showDataSlot(QNetworkReply *reply);
    void fetchData();
private:
    Ui::accountData *ui;
    QString username;
    QByteArray myToken;

    QNetworkAccessManager *dataManager;
    QNetworkReply *reply;
    QByteArray response_data;
};

#endif // ACCOUNTDATA_H
