#ifndef LOGIN_H
#define LOGIN_H

#include <QNetworkAccessManager>
#include <QDialog>
#include <QtNetwork>
#include <QJsonDocument>
#include <QTimer>

namespace Ui {
class login;
}

class login : public QDialog
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);
    ~login();
     void onWindowFinished();
private slots:
    void loginSlot(QNetworkReply *reply);
    void on_btnLogin_clicked();
private:
    Ui::login *ui;

    QNetworkAccessManager *postManager;
    QNetworkReply *reply;
    QByteArray response_data;
    QTimer *timeoutTimer;
};

#endif // LOGIN_H
