#ifndef TRANSFER_H
#define TRANSFER_H

#include <QDialog>
#include <QDebug>
#include <QTimer>
namespace Ui {
class Transfer;
}

class Transfer : public QDialog
{
    Q_OBJECT

public:
    explicit Transfer(QWidget *parent = nullptr);
    ~Transfer();

private slots:
    void on_btnCompleteTransfer_clicked();

    void on_btnClose_clicked();

private:
    Ui::Transfer *ui;
    QTimer *timeoutTimer;
};

#endif // TRANSFER_H
