/*
  Modul: DbHandler.h
  Funktion: Verwaltung der Datenbankzugriffe
  Autor: Aymane Amaioua, Christoph Rehm, benjamin Bardroff
  Datum: 01.04.2016
  */

#ifndef DBHANDLER_H
#define DBHANDLER_H

#include <QtSql/QSqlQuery>
#include <QtSql/QSqlDatabase>
#include <QFile>
#include <QVariant>
#include <QDateTime>
#include "account.h"
#include <QCryptographicHash>

class FrmMain;
class AccountListe;

class DBHandler
{
public:
    DBHandler();

    /********************************************************************************
     *                        NETZWERK-DATENBANK-ABFRAGEN                           *
     ********************************************************************************/

    bool tryAdminLogin(QString username, QString passwort, QString* error);
    bool tryUserLogin(QString username, QString passwort, QString* error); // Abfragen sind public, in diesen werden dann immer die private open und close methoden aufgerufen
    bool registrierung(QString benutzername, QString passwort, QString vorname, QString nachname, QString email, QString klasse, QString* error);


    /********************************************************************************
     *                      SERVERINTERNE DATENBANK-ABFRAGEN                        *
     ********************************************************************************/

    // Accountverwaltung
    bool aktualisiereAccountListe(QSqlQuery* p_qry, QString* error);
    bool Freischalten(QString benutzername, QString* error);//Schaltet den jeweiligen User frei.
    bool Sperren(QString benutzername, QString* error);//Sperrt den jeweiligen User.
    bool loeschen(QString benutzername, QString* error);
    bool aendern(QString benutzername, QString newbenutzername, QString vorname, QString nachname, QString klasse, QString email, QString* error);
    bool alleadmins(QSqlQuery* p_qry, QString* error);
    bool adminloeschen(QString benutzername, QString* error);
    bool Adminhinzu(QString benutzername, QString passwort, QString* error);
    bool emailaenderung(QString benutzername, QString newmail, QString* error);
    bool passwortaenderung(QString benutzername, QString newpasswort, QString* error);
    bool AdminPasswort(QSqlQuery* p_qry, QString benutzername, QString* error);
    /*
     * Diese Methode muss von außen (In dem Fall in der Klasse Accountiste::aktualisieren()) ausgeführt werden.
     * Der DBHandler macht seine Datenbankabfrage intern, da als Parameter ein Pointer zur accountListe(QList) von der Kl
     */

    // für events
    bool events(QSqlQuery* p_qry, QString* error); // gibt die Atribute der Events(Name, Beginn, Ende, SiegestippPunkte, ErgebnistippPunkte) zurück
    bool eintragenEvent(QString name, QDateTime beginn, QDateTime ende, int siegesTipp, int ergebnisTipp, QString* error);
    bool loeschenEvent(QString eventName, QString* error);
    bool updateEvent(QString nameAlt, QString nameNeu, QDateTime beginn, QDateTime ende, int siegesTipp, int ergebnisTipp, QString* error);

    // für eventspielerliste
    bool getEventSpieler(QSqlQuery* p_qry, QString* error, QString eventName); // gibt daten zurück um einen EventSpieler erstellen zu können

    //für wettenListe
    bool getWetten(QSqlQuery* p_qry, QString* error, QString eventName); //gibt daten für wetten zurück
    bool eintragenWette(QString* error, QString eventName, QString m1, QString m2, QDateTime start, QDateTime ende, int punkteM1, int punkteM2);
    bool getSpielID(QString* error, QSqlQuery* p_qry, QString eventName, QString m1, QString m2, QDateTime start, QDateTime ende);
    bool updateWette(QString* error, int spielID, QString m1, QString m2, QDateTime start, QDateTime ende, int punkteM1, int punkteM2);
    bool loeschenWette(QString* error, int spielID);

    // für tippListe
    bool getTipps(QSqlQuery* p_qry, QString* error, int spielID);
    bool eintragenTipp(QString* error, QString benutzerName, int spielID, int siegesTipp, int ergebnisTippM1, int ergebnisTippM2);
    bool updateTipp(QString* error, QString benutzerName, int spielID, int siegesTipp, int ergebnisTippM1, int ergebnisTippM2);
    bool loescheTipp(QString *error, QString benutzerName, int spielID);

    // für Mannschaften
    bool mannschaften(QSqlQuery* p_qry, QString* error); // gibt die Namen der Mannschaften zurück
    bool eintragenMannschaft(QString mannschaftsName, QString* error); // eintragen einer neuen Mannschaft
    bool loeschenMannschaft(QString mannschaftsName, QString* error); // löschen einer Mannschaft
    bool updateMannschaft(QString mannschaftsNameAlt, QString mannschaftsNameNeu, QString* error); // Aktualisiert den Namen

private:

    QSqlDatabase wettenDB;

    bool openDB(); // Ich habe die open und close Methoden auf private gesetzt, so versichert man, dass DBAbfragen nur DBHandler intern gelöst werden
    bool closeDB();
    bool createDB();
    bool DBExists();
};

#endif // DBHANDLER_H
