#ifndef TRANSACTIONDATA_H
#define TRANSACTIONDATA_H

#include <QString>

class transactiondata
{
public:
    transactiondata(QString transactionDatetime, QString accountIban, QString transactionType, QString a, QString b);


    QString getAccount_iban() const;
    QString getTransaction_datetime() const;
    //QString getAccount_id() const;
    QString getTransaction_type() const;
    QString getAmount() const;
    QString getBalance() const;

private:

    QString account_iban;
    QString transaction_datetime;
    //QString account_id;
    QString transaction_type;
    QString amount;
    QString balance;
};

#endif // TRANSACTIONDATA_H
