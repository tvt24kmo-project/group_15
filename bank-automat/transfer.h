#ifndef TRANSFER_H
#define TRANSFER_H

#include <QDialog>
#include <QDebug>

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

private:
    Ui::Transfer *ui;
};

#endif // TRANSFER_H
