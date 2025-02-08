#include "mainwindow.h"
#include "login.h"
#include "ui_mainwindow.h"
#include "environment.h"
#include <Login.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , imageFetcher(new ImageFetcher(this)) // luodaan ImageFetcher mainwindowin yhteydessä
{
    ui->setupUi(this);


    // yhdistetään ImageFetcherin signaalit mainwindowin slotteihin
    connect(imageFetcher, &ImageFetcher::imageFetched, this, &MainWindow::onImageFetched);
    connect(imageFetcher, &ImageFetcher::fetchFailed, this, &MainWindow::onFetchFailed);
  //connect(sender, signal, receiver, method)

    // haetaan kuva backendistä
    QString filename = "mainwindow.png"; // joustava tapa hakea kuvia backendistä
    QString url = Environment::base_url() + "/images/" + filename; // kuvan sijainti ja nimi muuttujasta
    imageFetcher->fetchImage(QUrl(url));
} // kutsutaan fetchImage funktiota ja annetaan url parametrina

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::onImageFetched(const QPixmap &pixmap) // kun kuva on haettu niin asetetaan se labeliin 
{
    ui->labelMainWindowImg->setPixmap(pixmap); // asetetaan pixmap labeliin
    ui->labelMainWindowImg->setScaledContents(true); // skaalataan kuva labeliin sopivaksi
}

void MainWindow::onFetchFailed(const QString &error) // jos haku epäonnistuu niin asetetaan labeliin virheilmoitus
{
    ui->labelMainWindowImg->setText("Kuvan haku epäonnistui: " + error); // jos kuva ei lataudu niin asetetaan labeliin virheilmoitus (siiretäänkö QDebugiin mieluummin??)
}

void MainWindow::on_btnStart_clicked()
{
    //Aloita-painiketta painettu
    login *objLogin=new login(this);
    objLogin->open();
}
