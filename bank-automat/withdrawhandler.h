/*
#ifndef WITHDRAWHANDLER_H
#define WITHDRAWHANDLER_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>

class WithdrawHandler : public QObject
{
    Q_OBJECT
public:
    explicit WithdrawHandler(QObject *parent = nullptr);
    void withdrawCash(const QUrl &url);

signals:
    void withdrawSuccess();
    void withdrawFail(const QString &error);


private slots:
    void onReplyFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager *networkManager;


};

#endif // WITHDRAWHANDLER_H
*/
