#ifndef LOGIN_H
#define LOGIN_H

#include <QNetworkAccessManager>
#include <QDialog>
#include <QtNetwork>
#include <QJsonDocument>

namespace Ui {
class login;
}

class login : public QDialog
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);
    ~login();
private slots:
    void loginSlot(QNetworkReply *reply);
    void on_btnLogin_clicked();
private:
    Ui::login *ui;

    QNetworkAccessManager *postManager;
    QNetworkReply *reply;
    QByteArray response_data;
};

#endif // LOGIN_H
