/*
  Modul: BwsProtokoll.h
  Funktion: Kommunikationsprotokoll, Deklaration
  Autor: Benjamin Bardroff
  Datum: 01.04.2016
  */

#ifndef BWSPROTOKOLL_H
#define BWSPROTOKOLL_H

#include "tipinfo.h"
#include "spielinfo.h"
#include "spielerinfo.h"
#include "eventinfo.h"

#include <QByteArray>
#include <QVector>
#include <QString>

/**
 * ser-Methoden   -> serialisieren Datenpackete; nehmen Daten als Parameter auf und erzeugen daraus nach festgelegten Vorgaben einen String, den es zu verschicken gilt
 * deser-MEthoden -> deserialisieren Datenpackete; nehmen einen String auf und ermitteln daraus nach festgelegten Vorgaben Daten, welche anschließen wieder einen Objeckt zugeteilt werden
 *
 * *** Problemstellung: ***
 * Datensätze können aus verschiedenen Datentypen bestehen. So ist eine objektorientierte Parameterübergabe als einziges Array nicht möglich.
 *
 * *** Lösungsvorschlag: ***
 * BWS-Protokoll arbeitet nur mit Strings. Das heisst, dass jegliche Daten als String dargestellt werden müssen. Es obliegt also der höheren Programminstanz Daten entsprechend
 * zu konvertieren.
 *
 * Hierbei ist es wichtig, Datensatzstrukturen;Reihenfolgen gebündelt zu erfassen um nicht den Überblick zu verlieren. Dazu wird in der Dropbox eine Exelliste angelegt,
 * in welcher für jedes Datenpaket alle RLoeihenfolgen etc. festgelegt werden.
 * Noch eleganter und auch übersichtlicher im Code wäre es, für jedes Datenpaket, welches gesendet wird alle Festlegungen Klassenintern in "BwsProtokoll" einzzlagern
 * und via. Methoden eindeutig und strukturiert abrufbar zu machen.
 * */

class BwsProtokoll
{
public:
    BwsProtokoll();

    /********************************************************************************
     *                                 PACKAGE ID'S                                 *
     ********************************************************************************/

    // *** Konstante ID-Werte für entsprechende Package-IDs *** //

    static const int AMOUNT_CLIENT_REQUESTS  = 12;
    static const int AMOUNT_SERVER_RESPONSES = 14;

    // Client-Anfragen //            // *** PACKAGE-SYNTAX *** //

    static const int CR_REG    = 0;  // BWSP~CR_REG~username~email~vorname~nachname~klasse
    static const int CR_LOGIN  = 1;  // BWSP~CR_LOGIN~username~password
    static const int CR_LOGOUT = 2;  // BWSP~CR_LOGOUT

    static const int CR_CMAIL  = 3;  // BWSP~CR_CMAIL~mail
    static const int CR_CPASS  = 4;  // BWSP~CR_CPASS~userpass~newpass
    static const int CR_DACC   = 5;  // BWSP~CR_DACC~userpass

    static const int CR_UO     = 6;  // BWSP~CR_UO
    static const int CR_CG     = 7;  // BWSP~CR_CG~event
    static const int CR_SG     = 8;  // BWSP~CR_SG~eventID~spielID
    static const int CR_SUBT   = 9;  // BWSP~CR_SUBT~eventID~spielID~tipEins["A","B","C"]~tipZweiA~tipZweiB
    static const int CR_HST    = 10; // BWSP~CR_HST~event
    static const int CR_SELP   = 11; // BWSP~CR_SELP~spielerID

    // Server-Antworten //

    static const int SR_REG    = 0;  // BWSP~SR_REG~regStatus[0/1]~error
    static const int SR_LOGIN  = 1;  // BWSP~SR_LOGIN~loginStatus[0/1]~vorname~nachname~klasse~email~regSeit~punkte~platz~error
    static const int SR_LOGOUT = 2;  // BWSP~SR_LOGOUT~logoutState~error

    static const int SR_CMAIL  = 3;  // BWSP~SR_CMAIL~changeState~error
    static const int SR_CPASS  = 4;  // BWSP~SR_CPASS~changeState~error
    static const int SR_DACC   = 5;  // BWSP~SR_DACC~deleteState~error

    static const int SR_UO     = 6;  // BWSP~SR_UO~amountTips{~eventID~spielId~TipEins~TipZweiA~TipZweiB}~error
    static const int SR_CG     = 7;  // BWSP_SR_CG~amountSpiele{~eventId~spielId~spielName}~error
    static const int SR_SG     = 8;  // BWSP~SR_SG~spielstatus~datum~startZeit~endeZeit~punkteA~punkteB~error
    static const int SR_SUBT   = 9;  // BWSP~SR_SUBT~submitOk~error
    static const int SR_HST    = 10; // BWSP~SR_HST~spielerAnzahl{spielerId~spielerName}~error
    static const int SR_SELP   = 11; // BWSP~SR_SELP~vorname~nachname~klasse~punkte~platz~error

    static const int SR_INVP   = 12; // BWSP~SINVP~error
    static const int SR_UNLI   = 13; // BWSP~SUNLI~error

    // ??? INFO ???
    // ??? Alle Pack-Methoden nehmen mindestens einen "Error-String" auf. Std-Wert: leerer String ???
    // ??? Alle Unpack-Methoden müssen das String Datenpaket (QString data) als ersten Parameter sowie einen ErrorString-pointer als zweiten Parameter aufnehmen - bei Bedarf von weiteren Rückgabewerten entsprechende Pointer aufnehmen ???

    // *** Arrays für ID-String Abfrage *** //

    QString clientRequests[BwsProtokoll::AMOUNT_CLIENT_REQUESTS];
    QString serverResponses[BwsProtokoll::AMOUNT_SERVER_RESPONSES];
    
    /********************************************************************************
     *                             PACKAGE-VERARBETUNG                              *
     ********************************************************************************/

    bool    checkValid                ( QString* data );  // prüft ob es sich um ein "BWSP"-Package handelt, sonst discard, Paket wird beschnitten
    int     getClientAnfragePackageId ( QString* data );  // ermittelt entsprechenden Index des Client-PackageIdentifiers
    int     getServerAntwortPackageId ( QString* data );  // ebenso nur für Server Packages

    /********************************************************************************
     *                             CLIENT-ANFRAGEN                                  *
     ********************************************************************************/

    // *** Pack-Methoden *** //

    QString packCR_REG   ( QString username, QString passwort, QString email, QString vorname, QString nachname, QString klasse );
    QString packCR_LOGIN ( QString username, QString password );
    QString packCR_LOGOUT();

    QString packCR_CMAIL( QString userpass, QString newMail );
    QString packCR_CPASS( QString userPass, QString newPass );
    QString packCR_DACC ( QString userPass );

    QString packCR_UO  ( QString eventName);
    QString packCR_CG  ( QString eventName );
    QString packCR_SG  ( int spielId );
    QString packCR_SUBT( int spielId, int tipEins, int tipZweiA, int tipZweiB );
    QString packCR_HST ( QString eventName );
    QString packCR_SELP( QString spielerName );

    // *** Unpack-Methoden *** //

    bool unpackCR_REG   ( QString data, QString* username, QString* passwort, QString* email, QString* vorname, QString* nachname, QString* klasse );
    bool unpackCR_LOGIN ( QString data, QString* username, QString* password ); // Client schickt schließlich einen im Client generiert MD5 Hash welcher mit dem eingetragenen MD5Hash der DB abgefragt wird

    bool unpackCR_CMAIL( QString data, QString* userpass, QString* newMail );
    bool unpackCR_CPASS( QString data, QString* userPass, QString* newPass );
    bool unpackCR_DACC ( QString data, QString* userPass );

    bool unpackCR_UO  ( QString data, QString* eventName);
    bool unpackCR_CG  ( QString data, QString* eventName );
    bool unpackCR_SG  ( QString data, int* spielId );
    bool unpackCR_SUBT( QString data, int* spielId, int* tipEins, int* tipZweiA, int* tipZweiB );
    bool unpackCR_HST ( QString data, QString* eventName );
    bool unpackCR_SELP( QString data, QString* spielerName );

    /*********************************************************************************
     *                             SERVER-ANTWORTEN                                  *
     ********************************************************************************/

    // *** Pack-Methoden *** //

    QString packSR_REG   ( bool regState, QString error = "" );
    QString packSR_LOGIN ( bool loginState, QVector<EventInfo> events, QString vorname, QString nachname, QString klasse, QString email, QString regSeit, QString error = "" );
    QString packSR_LOGOUT( bool logoutState, QString error = "" );

    QString packSR_CMAIL ( bool changed, QString error = "" );
    QString packSR_CPASS ( bool changed, QString error = "" );
    QString packSR_DACC  ( bool deleted, QString error = "" );

    QString packSR_UO    ( QVector<TipInfo> tips, QString error = "" );
    QString packSR_CG    ( QVector <SpielInfo> spiele, QString error = "" );
    QString packSR_SG    ( QString start, QString ende, int punkteA, int punkteB, int tipEins, int tipZweiA, int tipZweiB, QString error = "" );
    QString packSR_SUBT  ( bool submitOk, QString error = "" );
    QString packSR_HST   ( QVector<SpielerInfo> spieler, QString error = "" );
    QString packSR_SELP  ( QString vorname, QString nachname, QString klasse, QString regSeit, QString error = "" );

    QString packSR_INVP  ( QString error = "" );
    QString packSR_UNLI  ( QString error = "" );

    // *** Unpack-Methoden *** //

    bool unpackSR_REG   ( QString data, QString* error, bool* regState);
    bool unpackSR_LOGIN ( QString data, QString* error, bool* loginState, QVector<EventInfo>* events, QString* vorname, QString* nachname, QString* klasse, QString* email, QString* regSeit);
    bool unpackSR_LOGOUT( QString data, QString* error, bool* logoutState );

    bool unpackSR_CMAIL ( QString data, QString* error, bool* changed );
    bool unpackSR_CPASS ( QString data, QString* error, bool* changed );
    bool unpackSR_DACC  ( QString data, QString* error, bool* deleted );

    bool unpackSR_UO    ( QString data, QString* error, QVector<TipInfo>* tips );
    bool unpackSR_CG    ( QString data, QString* error, QVector <SpielInfo>* spiele );
    bool unpackSR_SG    ( QString data, QString* error, QString* start, QString* ende, int* punkteA, int* punkteB, int* tipEins, int* tipZweiA, int* tipZweiB );
    bool unpackSR_SUBT  ( QString data, QString* error, bool* submitOk);
    bool unpackSR_HST   ( QString data, QString* error, QVector<SpielerInfo>* spieler);
    bool unpackSR_SELP  ( QString data, QString* error, QString* vorname, QString* nachname, QString* regSeit, QString* klasse);

    bool unpackSR_INVP  ( QString data, QString* error );
    bool unpackSR_UNLI  ( QString data, QString* error );

private:

    void loadIdentifierStrings();
    QString getPackageIdentifierString( QString* data );  // filtert den PackageIdentifier heraus - Zeichen nach 'BWSP~XXXX~', Paket wird um Package ID beschnitten

    static const QString PROTOKOLL_ID; // Werden mit Identifier*String-Arrays global in CPP definiert
    static const QString NEXT_DATA_FIELD;
};

#endif // BWSPROTOKOLL_H
