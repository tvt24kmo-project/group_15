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

    QByteArray myToken; // tallennetaan tokeni tähän (käytetään kortin tarkistuksessa ENNEN varsinaista kirjautumista)

    QNetworkAccessManager *postManager;
    QNetworkReply *reply;
    QByteArray response_data;
    QTimer *timeoutTimer;
    int checkCardType(); // 0 = normi, 1 = multi
    int checkCardStatus(); // 0 = ei lukittu, 1 = lukittu
    int wrongAttemptsCounter=0; // seurataan montako kertaa väärä tunnus/salasana on syötetty
    int fetchAttempts(); // haetaan jo olemassa olevien väärin syötettyjen yritysten määrä (jos käynnistetään uudelleen tai yritetään joskus myöhemmin)
    void sendAttemptToServer(int wrongAttmept); // lähetetään palvelimelle tieto väärästä tunnuksesta/salasanasta
};

#endif // LOGIN_H
