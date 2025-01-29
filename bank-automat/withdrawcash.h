#ifndef WITHDRAWCASH_H
#define WITHDRAWCASH_H

#include <QDialog>

namespace Ui {
class WithdrawCash;
}

class WithdrawCash : public QDialog
{
    Q_OBJECT

public:
    explicit WithdrawCash(QWidget *parent = nullptr);
    ~WithdrawCash();

private slots:
    void on_buttonWithdrawOther_clicked();

private:
    int MINBILLSIZE = 20; // pienin seteli mitä automaatti antaa muun määrän nostoon (esim 2x20 = ok, 1x5 tai 3x5 = ei ok)
    Ui::WithdrawCash *ui;
};

#endif // WITHDRAWCASH_H
