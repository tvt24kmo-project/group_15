#ifndef IMAGEFETCHER_H
#define IMAGEFETCHER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPixmap>

/*
 * ImageFetcher luokka
 *
 * Tämä luokka käsittelee kuvan lataamisen URLista Qt Network moduulilla
 *
 * Emitoi signaalin onnistuneesta kuvan latauksesta pixmapilla tai virheistä
 * Helpottaa kuvien hakemista ja vähentää uudelleenkirjoittamista
 *
 * Ominaisuudet:
 * Lähettää HTTP GET pyynnön kuvan hakemista varten urlista
 * Emitoi signaalin kun kuva on haettu ja ladattu pixmappiin
 * Emitoi signaalin jos tulee virheitä
 *
 * Käyttö:
 * luo ImageFetcher olio
 * kutsu fetchImage() metodia URLilla parametrina qurl("url")
 * yhdistä imageFetched ja fetchFailed signaalit
*/


class ImageFetcher : public QObject
{
    Q_OBJECT
public:
    explicit ImageFetcher(QObject *parent = nullptr);
    void fetchImage(const QUrl &url); // fetchImage funktio joka ottaa urlin parametrina ja lähettää sen networkmanagerille

signals: 
    // signaalit emitoidaan kun kuva on haettu ja ladattu pixmapiin tai jos tulee virheitä 
    void imageFetched(const QPixmap &pixmap); // signaali imageFetched joka ottaa QPixmapin parametrina ja emitoi sen 
    void fetchFailed(const QString &error); // signaali fetchFailed joka ottaa QStringin parametrina ja emitoi sen

private slots:
    void onReplyFinished(QNetworkReply *reply); // slot onReplyFinished joka ottaa network replyn parametrina ja emitoi signaaleja riippuen onko replyn errori NoError vai ei

private:
    QNetworkAccessManager *networkManager; // luo networkmanagerin ImageFetcherin yhteydessä
};

#endif // IMAGEFETCHER_H
