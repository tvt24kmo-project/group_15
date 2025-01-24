#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "imagefetcher.h" // käytetään kuvan noutamiseen tehtyä luokkaa

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // nämä slotit kuuntelee ImageFetcherin signaaleja
    void onImageFetched(const QPixmap &pixmap); // slot onImageFetched joka ottaa QPixmapin parametrina ja asettaa sen labeliin
    void onFetchFailed(const QString &error); // slot onFetchFailed joka ottaa QStringin parametrina ja asettaa sen labeliin

    void on_buttonLogin_clicked();

    void on_pushButton_clicked();

    void on_buttonStart_clicked();

private:
    Ui::MainWindow *ui;
    ImageFetcher *imageFetcher; // luodaan olio luokasta ImageFetcher
};


#endif // MAINWINDOW_H
