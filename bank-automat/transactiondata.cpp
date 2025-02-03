#include "transactiondata.h"

#include <QString>

transactiondata::transactiondata(QString transactionDateTime, QString accountIban, QString transactionType, QString a, QString b)
{
    account_iban = accountIban;
    transaction_datetime = transactionDateTime;
    //account_id = accountId;
    transaction_type = transactionType;
    amount = a;
    balance = b;
}

QString transactiondata::getAccount_iban() const
{
    return account_iban;
}

QString transactiondata::getTransaction_datetime() const
{
    return transaction_datetime;
}

//QString transactiondata::getAccount_id() const
//{
//    return account_id;
//}

QString transactiondata::getTransaction_type() const
{
    return transaction_type;
}

QString transactiondata::getAmount() const
{
    return amount;
}

QString transactiondata::getBalance() const
{
    return balance;
}

