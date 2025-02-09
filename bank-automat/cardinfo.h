#ifndef CARDINFO_H
#define CARDINFO_H


#include "qdialog.h"
#include <QTimer>

namespace Ui {
class cardInfo;
}

class cardInfo : public QDialog
{

    Q_OBJECT
public:
    explicit cardInfo(QWidget *parent = nullptr);
    ~cardInfo();
    void setUsername(const QString &newUsername);

    void setMyToken(const QByteArray &newMyToken);

    void onWindowFinished();

private slots:
    void on_btnData_clicked();

    void on_buttonWithdrawCash_clicked();

    void on_btnTransfer_clicked();

    void on_btnHistory_clicked();

private:
    Ui::cardInfo *ui;
    QString username;
    QByteArray myToken;
    QTimer *timeoutTimer;
};

#endif // CARDINFO_H
