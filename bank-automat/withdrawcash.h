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
    void on_buttonOther_clicked();

    void on_button20_clicked();

    void on_button40_clicked();

    void on_button60_clicked();

    void on_button100_clicked();

private:
    int MINBILLSIZE = 20; // pienin seteli mitä automaatti antaa muun määrän nostoon (esim 2x20€ = ok, 1x5 tai 3x5 = ei ok)
    int cashAmount = 0; // muuttuja jossa arvo muuttuu näppäinten/muun määrän perusteella
    Ui::WithdrawCash *ui;
};

#endif // WITHDRAWCASH_H
