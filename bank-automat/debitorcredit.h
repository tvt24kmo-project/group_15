#ifndef DEBITORCREDIT_H
#define DEBITORCREDIT_H

#include <QDialog>

namespace Ui {
class DebitOrCredit;
}

class DebitOrCredit : public QDialog
{
    Q_OBJECT

public:
    explicit DebitOrCredit(QWidget *parent = nullptr);
    ~DebitOrCredit();

private slots:
    void on_buttonCredit_clicked();

    void on_buttonDebit_clicked();

private:
    Ui::DebitOrCredit *ui;
};

#endif // DEBITORCREDIT_H
