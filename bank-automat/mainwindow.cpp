#include "cardinfo.h"
#include "environment.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    QString url = "http://localhost:3000/images/" + filename; // kuvan sijainti ja nimi muuttujasta
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


void MainWindow::on_btnLogin_clicked()
{

    QJsonObject jsonObj;
    jsonObj.insert("cardnumber",ui->textUsername->text());
    jsonObj.insert("pinhash",ui->textPassword->text());

    QString site_url=Environment::base_url()+"/login";
    QNetworkRequest request(site_url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    postManager = new QNetworkAccessManager(this);
    connect(postManager,SIGNAL(finished(QNetworkReply*)), this, SLOT(loginSlot(QNetworkReply*)));
    reply = postManager->post(request, QJsonDocument(jsonObj).toJson());
}

void MainWindow::loginSlot(QNetworkReply *reply)
{

    response_data=reply->readAll();
    if(response_data.length()<2){
        qDebug()<<"Palvelin ei vastaa";
        ui->labelInfo->setText("Palvelin ei vastaa!");
    }
    else {
        if(response_data=="-11"){
            ui->labelInfo->setText("Tietokanta virhe!");
        }
        else {
            if(response_data!="false" && response_data.length()>20) {
                ui->labelInfo->setText("Kirjautuminen OK");
                QByteArray myToken="Bearer "+response_data;
                cardInfo *objCardInfo= new cardInfo(this);
                objCardInfo->setUsername(ui->textUsername->text());
                objCardInfo->setMyToken(myToken);
                objCardInfo->open();
            }
            else {
                ui->labelInfo->setText("Väärä tunnus/salasana");
            }

        }

    }

    reply->deleteLater();
    postManager->deleteLater();
}


