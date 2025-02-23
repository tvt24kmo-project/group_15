#ifndef DEBITORCREDIT_H
#define DEBITORCREDIT_H

#include <QDialog>
#include "accountdata.h"
#include <QCloseEvent>

class login;
namespace Ui {
class DebitOrCredit;
}

class DebitOrCredit : public QDialog
{
    Q_OBJECT

public:
    explicit DebitOrCredit(QWidget *parent = nullptr, accountData *data = nullptr);
    ~DebitOrCredit();

signals:
    void debitChosen(int accountId);
    void creditChosen(int accountId);

private slots:
    void on_buttonCredit_clicked();
    void on_buttonDebit_clicked();
    void fetchAccountIds();


private:
    Ui::DebitOrCredit *ui;
    accountData *accountDataPtr;
    int debitAccountId;
    int creditAccountId;
    QNetworkAccessManager *getManager;
    QNetworkReply *reply;
    QByteArray response_data;

protected:
    void closeEvent(QCloseEvent *event) override;
};

#endif // DEBITORCREDIT_H
