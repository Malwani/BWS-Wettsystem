#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QMessageBox>
#include "bwsprotokoll.h"
#include "activeuser.h"

class ClientHandler : public QWidget
{
    Q_OBJECT

public:
    ClientHandler();
    ~ClientHandler();

    BwsProtokoll protokoll;
    ActiveUser activeUser;

    void setHost(QString host);
    void setPort(int port);
    bool doConnect();
    void sendData(QString data);
    QString getLastError();
    void setTryMail(QString tryMail);
    QString getTryMail() const;

private:

    QTcpSocket* client;
    QString host;
    QString lastError;
    int port;

    QString tryMail;

    void verarbeiteSR_REG   ( QString data);
    void verarbeiteSR_LOGIN ( QString data);
    void verarbeiteSR_LOGOUT( QString data);
    void verarbeiteSR_CMAIL ( QString data);
    void verarbeiteSR_CPASS ( QString data);
    void verarbeiteSR_DACC  ( QString data);
    void verarbeiteSR_UO    ( QString data);
    void verarbeiteSR_CG    ( QString data);
    void verarbeiteSR_SG    ( QString data);
    void verarbeiteSR_SUBT  ( QString data);
    void verarbeiteSR_HST   ( QString data);
    void verarbeiteSR_SELP  ( QString data);
    void verarbeiteSR_UNLI  ( QString data);
    void verarbeiteSR_INVP  ( QString data);

signals:

    void verbindungAbgebrochen();
    void login (QVector<EventInfo> events);
    void logout();
    void uebermittleLogNachricht(QString);
    void displayNewMail(QString);
    void registerSucced();

    void uebergebeMeineTipps(QVector<TipInfo>);
    void uebergebeAktuelleWetten(QVector<SpielInfo>);
    void uebergebeAusgWette(QString startZeit, QString endeZeit, int punkteA, int punkteB, int tipEins, int tipZweiA, int tipZweib);
    void uebergebeSubmitTip(bool submitOk);
    void uebergebeBestenliste(QVector<SpielerInfo>);
    void uebergebeAusgSpieler(QString vorname,QString nachname, QString klasse, QString regSeit);

    void deaktiviereTippBereich();

public slots:
    void verbindungHergestellt();
    void verbidnungGetrennt();
    void verarbeiteServerAntwort();
};

#endif // CLIENTHANDLER_H
