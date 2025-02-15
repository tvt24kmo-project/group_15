#ifndef CARDINFO_H
#define CARDINFO_H


#include "qdialog.h"
#include <QTimer>
#include "accountdata.h"
#include "imagefetcher.h"

namespace Ui {
class cardInfo;
}

class cardInfo : public QDialog
{

    Q_OBJECT
public:
    explicit cardInfo(QWidget *parent = nullptr, accountData *data = nullptr);
    ~cardInfo();
    void setUsername(const QString &newUsername);

    void setMyToken(const QByteArray &newMyToken);

    void onWindowFinished();

private slots:
    void on_btnData_clicked();

    void on_buttonWithdrawCash_clicked();

    void on_btnTransfer_clicked();

    void on_btnHistory_clicked();

    void on_btnClose_clicked();

    void onImageFetched(const QPixmap &pixmap); // slot onImageFetched joka ottaa QPixmapin parametrina ja asettaa sen labeliin
    void onFetchFailed(const QString &error); // slot onFetchFailed joka ottaa QStringin parametrina ja asettaa sen labeliin



private:
    Ui::cardInfo *ui;
    QString username;
    QByteArray myToken;
    QTimer *timeoutTimer;
    accountData *accountDataPtr;
    ImageFetcher *imageFetcher;
};

#endif // CARDINFO_H
