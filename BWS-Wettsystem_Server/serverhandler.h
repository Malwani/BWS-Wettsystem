/*
  Modul: ServerHandler.h
  Funktion: 
  Autor: Benjamin Bardroff, Christoph Rehm
  Datum: 
  */

#ifndef ServerHandler_H
#define ServerHandler_H

#include <QWidget>
#include <QtNetwork>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include <QString>

#include "verbindung.h"
#include "bwsprotokoll.h"
#include "accountliste.h"
#include "eventliste.h"

class ServerHandler : public QObject
{
    Q_OBJECT

private:

    QTcpServer* server;
    QSignalMapper* readMapper;
    QSignalMapper* disconnectMapper;
    BwsProtokoll protokoll;
    AccountListe* p_accountListe;
    EventListe* p_eventListe;

    QVector<QString> loggedInUser;

    void verarbeiteAnfrage(Verbindung* verbindung, QString data);

    void verarbeiteCR_REG   (Verbindung* verbindung, QString data);
    void verarbeiteCR_LOGIN (Verbindung* verbindung, QString data);
    void verarbeiteCR_LOGOUT(Verbindung* verbindung);
    void verarbeiteCR_CMAIL (Verbindung* verbindung, QString data);
    void verarbeiteCR_CPASS (Verbindung* verbindung, QString data);
    void verarbeiteCR_DACC  (Verbindung* verbindung, QString data);
    void verarbeiteCR_UO    (Verbindung* verbindung, QString data);
    void verarbeiteCR_CG    (Verbindung* verbindung, QString data);
    void verarbeiteCR_SG    (Verbindung* verbindung, QString data);
    void verarbeiteCR_SUBT  (Verbindung* verbindung, QString data);
    void verarbeiteCR_HST   (Verbindung* verbindung, QString data);
    void verarbeiteCR_SELP  (Verbindung* verbindung, QString data);

public:
    ServerHandler( QObject* parent = 0);
    ~ServerHandler();

    void startServer(int port);
    void stopServer();
    void sendeDaten(Verbindung* verbindung, QString daten);
    void takeAccountListe(AccountListe* p_accountListe);
    void takeEventListe(EventListe* p_eventListe);

signals:
    void uebermittleLogNachricht(QString daten); // übermittelt ServerHandler interne Daten nach "außen"

private slots:
    void neueVerbindungAufbauen();
    void neueDatenEmpfangen(QObject* verb);
    void verbindungsAbbruch(QObject* verb);

public slots:

};

#endif // ServerHandler_H
