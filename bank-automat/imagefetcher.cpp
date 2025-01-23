#include "imagefetcher.h"

ImageFetcher::ImageFetcher(QObject *parent)
    : QObject(parent), networkManager(new QNetworkAccessManager(this)) {} // luodaan networkmanager kun ImageFetcher luodaan

void ImageFetcher::fetchImage(const QUrl &url) // fetchImage funktio joka ottaa urlin parametrina ja lähettää sen networkmanagerille
{
    QNetworkRequest request(url);                                                                    // luodaan QNetworkRequest joka ottaa urlin parametrina
    connect(networkManager, &QNetworkAccessManager::finished, this, &ImageFetcher::onReplyFinished); // yhdistetään networkmanagerin signaali 'finished' ImageFetcherin slotiin 'onReplyFinished'
  //connect(sender, signal, receiver, method)
    networkManager->get(request);                                                                    // lähetetään HTTP GET request networkmanagerille
}

void ImageFetcher::onReplyFinished(QNetworkReply *reply) // onReplyFinished funktio joka ottaa network replyn parametrina ja emitoi signaaleja riippuen onko replyn errori NoError vai ei
{
    if (reply->error() == QNetworkReply::NoError) // jos network replyn errori on NoError
    {
        QByteArray imageData = reply->readAll(); // luetaan network replyn data ja tallennetaan se imageData muuttujaan
        QPixmap pixmap;                          // luodaan pixmap

        if (pixmap.loadFromData(imageData)) // jos loadFromData onnistui niin emitoidaan imageFetched signaali
        {
            emit imageFetched(pixmap); // emitoidaan imageFetched signaali ja lähetetään pixmap parametrina
        }
        else
        {                                                 // jos loadFromData palauttaa false niin emitoidaan fetchFailed signaali
            emit fetchFailed("Kuvan lataus epäonnistui"); // emitoidaan fetchFailed signaali ja virheviesti
        }
    }
    else
    {                                           // jos replyn errori ei ole NoError niin emitoidaan fetchFailed signaali
        emit fetchFailed(reply->errorString()); // emitoidaan fetchFailed signaali ja virheviesti QNetworkReplyn errorStringillä
    }
    reply->deleteLater(); // poistetaan reply 
}
