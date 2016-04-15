/*
  Modul: BwsProtokoll.cpp
  Funktion: Kommunikationsprotokoll, Definition
  Autor: Benjamin Bardroff
  Datum: 01.04.2016
  */

#include "bwsprotokoll.h"

// *** Konstruktor *** //

BwsProtokoll::BwsProtokoll()
{
    this->loadIdentifierStrings();
}


/********************************************************************************
 *                                PACKAGE-VERARBEITUNG                          *
 ********************************************************************************/

// *** Konstanten *** //

const QString BwsProtokoll::PROTOKOLL_ID = "BWSP";

// am besten Zeichen die nicht auf Tastatur liegen
const QString BwsProtokoll::NEXT_DATA_FIELD = "~";

// *** Methoden *** //

/*
  Funktion: Lädt entsprechende Paket-IdentikatorStrings in angegebener Reihenfolge
  Autor: Benjamin Bardroff
  Datum: 01.04.2016
  */
void BwsProtokoll::loadIdentifierStrings()
{
    // laden der ClientAnfragenID-Strings

    this->clientRequests[BwsProtokoll::CR_REG]    = "CR_REG";
    this->clientRequests[BwsProtokoll::CR_LOGIN]  = "CR_LOGIN";
    this->clientRequests[BwsProtokoll::CR_LOGOUT] = "CR_LOGOUT";
    this->clientRequests[BwsProtokoll::CR_CMAIL]  = "CR_CMAIL";
    this->clientRequests[BwsProtokoll::CR_CPASS]  = "CR_CPASS";
    this->clientRequests[BwsProtokoll::CR_DACC]   = "CR_DACC";
    this->clientRequests[BwsProtokoll::CR_UO]     = "CR_UO";
    this->clientRequests[BwsProtokoll::CR_CG]     = "CR_CG";
    this->clientRequests[BwsProtokoll::CR_SG]     = "CR_SG";
    this->clientRequests[BwsProtokoll::CR_SUBT]   = "CR_SUBT";
    this->clientRequests[BwsProtokoll::CR_HST]    = "CR_HST";
    this->clientRequests[BwsProtokoll::CR_SELP]   = "CR_SELP";


    // laden der ServerAntwortenID-Strings

    this->serverResponses[BwsProtokoll::SR_REG]    = "SR_REG";
    this->serverResponses[BwsProtokoll::SR_LOGIN]  = "SR_LOGIN";
    this->serverResponses[BwsProtokoll::SR_LOGOUT] = "SR_LOGOUT";
    this->serverResponses[BwsProtokoll::SR_CMAIL]  = "SR_CMAIL";
    this->serverResponses[BwsProtokoll::SR_CPASS]  = "SR_CPASS";
    this->serverResponses[BwsProtokoll::SR_DACC]   = "SR_DACC";
    this->serverResponses[BwsProtokoll::SR_UO]     = "SR_UO";
    this->serverResponses[BwsProtokoll::SR_CG]     = "SR_CG";
    this->serverResponses[BwsProtokoll::SR_SG]     = "SR_SG";
    this->serverResponses[BwsProtokoll::SR_SUBT]   = "SR_SUBT";
    this->serverResponses[BwsProtokoll::SR_HST]    = "SR_HST";
    this->serverResponses[BwsProtokoll::SR_SELP]   = "SR_SELP";
    this->serverResponses[BwsProtokoll::SR_INVP]   = "SR_INVP";
    this->serverResponses[BwsProtokoll::SR_UNLI]   = "SR_UNLI";
}

/*
  Funktion: Prüft ob Datenpaket gültig ist und beschneidet es um den ProtokollIdentikator ("BWSP")
  Autor: Benjamin Bardroff
  Datum: 01.04.2016
  */
bool BwsProtokoll::checkValid(QString* data) // Prüft ob es sich um ein gültiges BWSP-Paket handelt, bescheidet das Paket umd 'BWSP~' - Wird als erste Validierungsinstanz aufgerufen
{
    bool ok = false;

    if(data->mid(0, BwsProtokoll::PROTOKOLL_ID.length() ) == BwsProtokoll::PROTOKOLL_ID) // Prüft ob die ersten 5 Zeichen "BWSP" sind
    {
        data->remove(0, BwsProtokoll::PROTOKOLL_ID.length() + BwsProtokoll::NEXT_DATA_FIELD.length() ); // Beschneidet das Paket um "BWSP~"
        ok = true;
    }

    return ok;
}

/*
  Funktion: Ermittelt den PaketIdentikator und beschneidet das Paket um diesen
  Autor: Benjamin Bardroff
  Datum: 01.04.2016
  */
QString BwsProtokoll::getPackageIdentifierString(QString* data)
{
    QString identifier = "";
    QString zeichen    = "";

    for(int i = 0; i < data->length(); i++)
    {
         zeichen = data->mid(i,1);

        if( zeichen == BwsProtokoll::NEXT_DATA_FIELD) // Wenn TRENNER erreicht
        {
            data->remove(0,i + 1); // Paket beschneiden, erster Trenner fällt weg
            break;                 // Schleife verlassen
        }
        else
            identifier += zeichen; // Sonst aktuelles Zeichen dem Identifier zuweisen
    }

    return identifier;
}

/*
  Funktion: ermittelt Index eines ServerResponse PaketIdentikator
  Autor: Benjamin Bardroff
  Datum: 01.04.2016
  */
int BwsProtokoll::getServerAntwortPackageId(QString* data) // gibt den PaketIDentifier für ServerAntworten zurück
{
    QString idStr = this->getPackageIdentifierString(data);

    int id;

    for(int i = 0; i < BwsProtokoll::AMOUNT_SERVER_RESPONSES; i++)
    {
        if(idStr == this->serverResponses[i])
            id = i;
    }

    return id;
}

/*
  Funktion: ermittelt Index eines ClientReqeust PaketIdentikator
  Autor: Benjamin Bardroff
  Datum: 01.04.2016
  */
int BwsProtokoll::getClientAnfragePackageId(QString* data) // gibt den PaketIDentifier für ClientAnfragen zurück
{
    QString idStr = this->getPackageIdentifierString(data);
    int id;

    for(int i = 0; i < BwsProtokoll::AMOUNT_CLIENT_REQUESTS; i++)
    {
        if(idStr == this->clientRequests[i])
            id = i;
    }

    return id;
}

/********************************************************************************
 *                             CLIENT-ANFRAGEN                                  *
 ********************************************************************************/

// *** Pack-Methoden *** //

/*
  Funktion: Serialisiert Daten einer ClientRegisterRequest
  Autor: Benjamin Bardroff
  Datum: 01.04.2016
  */
QString BwsProtokoll::packCR_REG(QString username, QString passwort, QString email, QString vorname, QString nachname, QString klasse)
{
    return BwsProtokoll::PROTOKOLL_ID
            + BwsProtokoll::NEXT_DATA_FIELD
            + this->clientRequests[BwsProtokoll::CR_REG]
            + BwsProtokoll::NEXT_DATA_FIELD
            + username
            + BwsProtokoll::NEXT_DATA_FIELD
            + passwort
            + BwsProtokoll::NEXT_DATA_FIELD
            + email
            + BwsProtokoll::NEXT_DATA_FIELD
            + vorname
            + BwsProtokoll::NEXT_DATA_FIELD
            + nachname
            + BwsProtokoll::NEXT_DATA_FIELD
            + klasse;
}

/*
  Funktion: Serialisiert Daten einer ClientLoginRequest
  Autor: Benjamin Bardroff
  Datum: 01.04.2016
  */
QString BwsProtokoll::packCR_LOGIN(QString username, QString password)
{
    return BwsProtokoll::PROTOKOLL_ID
            + BwsProtokoll::NEXT_DATA_FIELD
            + this->clientRequests[BwsProtokoll::CR_LOGIN]
            + BwsProtokoll::NEXT_DATA_FIELD
            + username
            + BwsProtokoll::NEXT_DATA_FIELD
            + password;
}

/*
  Funktion: Serialisiert Daten einer ClientLogoutRequest
  Autor: Benjamin Bardroff
  Datum: 01.04.2016
  */
QString BwsProtokoll::packCR_LOGOUT()
{
    return BwsProtokoll::PROTOKOLL_ID
            + BwsProtokoll::NEXT_DATA_FIELD
            + this->clientRequests[BwsProtokoll::CR_LOGOUT]
            + BwsProtokoll::NEXT_DATA_FIELD; // legt Ende des Package-ID-Strings, für VerarbeitungsMethoden, fest.
}

/*
  Funktion: Serialisiert Daten einer ClientChangeMailRequest
  Autor: Benjamin Bardroff
  Datum: 01.04.2016
  */
QString BwsProtokoll::packCR_CMAIL(QString userpass, QString newMail)
{
    return BwsProtokoll::PROTOKOLL_ID
            + BwsProtokoll::NEXT_DATA_FIELD
            + this->clientRequests[BwsProtokoll::CR_CMAIL]
            + BwsProtokoll::NEXT_DATA_FIELD
            + userpass
            + BwsProtokoll::NEXT_DATA_FIELD
            + newMail;
}

/*
  Funktion: Serialisiert Daten einer ClientChangePasswordRequest
  Autor: Benjamin Bardroff
  Datum: 01.04.2016
  */
QString BwsProtokoll::packCR_CPASS(QString userPass, QString newPass)
{
    return BwsProtokoll::PROTOKOLL_ID
            + BwsProtokoll::NEXT_DATA_FIELD
            + this->clientRequests[BwsProtokoll::CR_CPASS]
            + BwsProtokoll::NEXT_DATA_FIELD
            + userPass
            + BwsProtokoll::NEXT_DATA_FIELD
            + newPass;
}

/*
  Funktion: Serialisiert Daten einer ClientDeleteAccountRequest
  Autor: Benjamin Bardroff
  Datum: 01.04.2016
  */
QString BwsProtokoll::packCR_DACC(QString userPass)
{
    return BwsProtokoll::PROTOKOLL_ID
            + BwsProtokoll::NEXT_DATA_FIELD
            + this->clientRequests[BwsProtokoll::CR_DACC]
            + BwsProtokoll::NEXT_DATA_FIELD
            + userPass;
}

/*
  Funktion: Serialisiert Daten einer ClientUserOverviewRequest
  Autor: Benjamin Bardroff
  Datum: 01.04.2016
  */
QString BwsProtokoll::packCR_UO(QString eventName)
{
    return BwsProtokoll::PROTOKOLL_ID
            + BwsProtokoll::NEXT_DATA_FIELD
            + this->clientRequests[BwsProtokoll::CR_UO]
            + BwsProtokoll::NEXT_DATA_FIELD
            + eventName;
}

/*
  Funktion: Serialisiert Daten einer ClientCurrentGamesRequest
  Autor: Benjamin Bardroff
  Datum: 01.04.2016
  */
QString BwsProtokoll::packCR_CG(QString eventName)
{
    return BwsProtokoll::PROTOKOLL_ID
            + BwsProtokoll::NEXT_DATA_FIELD
            + this->clientRequests[BwsProtokoll::CR_CG]
            + BwsProtokoll::NEXT_DATA_FIELD
            + eventName;
}

/*
  Funktion: Serialisiert Daten einer ClientSelectedGameRequest
  Autor: Benjamin Bardroff
  Datum: 01.04.2016
  */
QString BwsProtokoll::packCR_SG(int spielId)
{
    return BwsProtokoll::PROTOKOLL_ID
            + BwsProtokoll::NEXT_DATA_FIELD
            + this->clientRequests[BwsProtokoll::CR_SG]
            + BwsProtokoll::NEXT_DATA_FIELD
            + QString::number(spielId);
}

/*
  Funktion: Serialisiert Daten einer ClientSubmitTipRequest
  Autor: Benjamin Bardroff
  Datum: 01.04.2016
  */
QString BwsProtokoll::packCR_SUBT(int spielId, int tipEins, int tipZweiA, int tipZweiB)
{
    return BwsProtokoll::PROTOKOLL_ID
            + BwsProtokoll::NEXT_DATA_FIELD
            + this->clientRequests[BwsProtokoll::CR_SUBT]
            + BwsProtokoll::NEXT_DATA_FIELD
            + QString::number(spielId)
            + BwsProtokoll::NEXT_DATA_FIELD
            + QString::number(tipEins)
            + BwsProtokoll::NEXT_DATA_FIELD
            + QString::number(tipZweiA)
            + BwsProtokoll::NEXT_DATA_FIELD
            + QString::number(tipZweiB);
}

/*
  Funktion: Serialisiert Daten einer ClientHighscoreTableRequest
  Autor: Benjamin Bardroff
  Datum: 01.04.2016
  */
QString BwsProtokoll::packCR_HST(QString eventName)
{
    return BwsProtokoll::PROTOKOLL_ID
            + BwsProtokoll::NEXT_DATA_FIELD
            + this->clientRequests[BwsProtokoll::CR_HST]
            + BwsProtokoll::NEXT_DATA_FIELD
            + eventName;
}

/*
  Funktion: Serialisiert Daten einer ClientSelectedPlayerRequest
  Autor: Benjamin Bardroff
  Datum: 01.04.2016
  */
QString BwsProtokoll::packCR_SELP(QString spielerName)
{
    return BwsProtokoll::PROTOKOLL_ID
            + BwsProtokoll::NEXT_DATA_FIELD
            + this->clientRequests[BwsProtokoll::CR_SELP]
            + BwsProtokoll::NEXT_DATA_FIELD
            + spielerName;
}


// *** Unpack-Methoden *** //

/*
  Funktion: Deserialisiert Daten einer ClientRegisterRequest
  Autor: Benjamin Bardroff
  Datum: 01.04.2016
  */
bool BwsProtokoll::unpackCR_REG(QString data, QString *username, QString *passwort, QString *email, QString *vorname, QString *nachname, QString *klasse)
{
    bool ok = true;
    int dataField = 1; // aktuelles Datenfeld (start: 1)

    QString character = "";

    for(int i = 0; i < data.length();i++) // Datenpaket druchlaufen
    {
        character = data.mid(i,1);

        if(character == BwsProtokoll::NEXT_DATA_FIELD) // Wenn TRENNER gefunden, auf nächsten Datenfeld springen
            dataField++;
        else                     // sonst aktuelles Datenfeld beschreiben
        {
            switch(dataField)
            {
            case 1: *username += character; break;
            case 2: *passwort += character; break;
            case 3: *email    += character; break;
            case 4: *vorname  += character; break;
            case 5: *nachname += character; break;
            case 6: *klasse   += character; break;
            default: ok = false; break;
            }
        }
    }

    return ok;
}

/*
  Funktion: Deserialisiert Daten einer ClientLoginRequest
  Autor: Benjamin Bardroff
  Datum: 01.04.2016
  */
bool BwsProtokoll::unpackCR_LOGIN(QString data, QString *username, QString *password)
{
    bool ok = true;
    int dataField = 1;

    QString character = "";

    for(int i = 0; i < data.length();i++)
    {
        character = data.mid(i,1);

        if(character == BwsProtokoll::NEXT_DATA_FIELD)
            dataField++;
        else
        {
            switch(dataField)
            {
            case 1: *username += character; break;
            case 2: *password += character; break;
            default: ok = false; break;
            }
        }
    }

    return ok;
}

/*
  Funktion: Deserialisiert Daten einer ClientChangeMailRequest
  Autor: Benjamin Bardroff
  Datum: 01.04.2016
  */
bool BwsProtokoll::unpackCR_CMAIL(QString data, QString *userpass, QString *newMail)
{
    bool ok = true;
    int dataField = 1;

    QString character = "";

    for(int i = 0; i < data.length();i++)
    {
        character = data.mid(i,1);

        if(character == BwsProtokoll::NEXT_DATA_FIELD)
            dataField++;
        else
        {
            switch(dataField)
            {
            case 1: *userpass += character; break;
            case 2: *newMail     += character; break;
            default: ok = false; break;
            }
        }
    }

    return ok;
}

/*
  Funktion: Deserialisiert Daten einer ClientChangePasswordRequest
  Autor: Benjamin Bardroff
  Datum: 01.04.2016
  */
bool BwsProtokoll::unpackCR_CPASS(QString data, QString* userPass, QString* newPass)
{
    bool ok = true;
    int dataField = 1;

    QString character = "";

    for(int i = 0; i < data.length();i++)
    {
        character = data.mid(i,1);

        if(character == BwsProtokoll::NEXT_DATA_FIELD)
            dataField++;
        else
        {
            switch(dataField)
            {
            case 1: *userPass += character; break;
            case 2: *newPass  += character; break;
            default: ok = false; break;
            }
        }
    }

    return ok;
}

/*
  Funktion: Deserialisiert Daten einer ClientDeleteAccountRequest
  Autor: Benjamin Bardroff
  Datum: 01.04.2016
  */
bool BwsProtokoll::unpackCR_DACC(QString data, QString *userPass)
{
    bool ok = true;
    int dataField = 1;

    QString character = "";

    for(int i = 0; i < data.length();i++)
    {
        character = data.mid(i,1);

        if(character == BwsProtokoll::NEXT_DATA_FIELD)
            dataField++;
        else
        {
            switch(dataField)
            {
            case 1: *userPass += character; break;
            default: ok = false; break;
            }
        }
    }

    return ok;
}

/*
  Funktion: Deserialisiert Daten einer ClientUserOverviewRequests
  Autor: Benjamin Bardroff
  Datum: 08.04.2016
  */
bool BwsProtokoll::unpackCR_UO(QString data, QString *eventName)
{
    bool ok = true;
    int dataField = 1;

    QString character = "";

    for(int i = 0; i < data.length();i++)
    {
        character = data.mid(i,1);

        if(character == BwsProtokoll::NEXT_DATA_FIELD)
            dataField++;
        else
        {
            switch(dataField)
            {
            case 1: *eventName += character; break;
            default: ok = false; break;
            }
        }
    }

    return ok;
}

/*
  Funktion: Deserialisiert Daten einer ClientCurrentGamesRequest
  Autor: Benjamin Bardroff
  Datum: 01.04.2016
  */
bool BwsProtokoll::unpackCR_CG(QString data, QString *eventName)
{
    bool ok = true;
    int dataField = 1;

    QString character = "";

    for(int i = 0; i < data.length();i++)
    {
        character = data.mid(i,1);

        if(character == BwsProtokoll::NEXT_DATA_FIELD)
            dataField++;
        else
        {
            switch(dataField)
            {
            case 1: *eventName += character; break;
            default: ok = false; break;
            }
        }
    }

    return ok;
}

/*
  Funktion: Deserialisiert Daten einer ClientSelectedGameRequest
  Autor: Benjamin Bardroff
  Datum: 01.04.2016
  */
bool BwsProtokoll::unpackCR_SG(QString data, int *spielId)
{
    bool ok = true;
    int dataField = 1;

    QString character  = "";
    QString strSpielId = "";

    for(int i = 0; i < data.length();i++)
    {
        character = data.mid(i,1);

        if(character == BwsProtokoll::NEXT_DATA_FIELD)
            dataField++;
        else
        {
            switch(dataField)
            {
            case 1:  strSpielId += character; break; // Int erst als ziffern aufnehmen
            default: ok = false; break;
            }
        }
    }

    *spielId = strSpielId.toInt(); // dann we vollständig umkonvertieren

    return ok;
}

/*
  Funktion: Deserialisiert Daten einer ClientSubmitTipRequest
  Autor: Benjamin Bardroff
  Datum: 01.04.2016
  */
bool BwsProtokoll::unpackCR_SUBT(QString data, int* spielId, int* tipEins, int* tipZweiA, int* tipZweiB)
{
    bool ok = true;
    int dataField = 1;

    QString character   = "";

    QString strSpielId  = "";
    QString strTipEins  = "";
    QString strTipZweiA = "";
    QString strTipZweiB = "";

    for(int i = 0; i < data.length();i++)
    {
        character = data.mid(i,1);

        if(character == BwsProtokoll::NEXT_DATA_FIELD)
            dataField++;
        else
        {
            switch(dataField)
            {
            case 1: strSpielId  += character; break;
            case 2: strTipEins  += character; break; // String direkt übernehmen
            case 3: strTipZweiA += character; break; // Integers erst als Zahl ermitteln
            case 4: strTipZweiB += character; break;
            default: ok = false;              break;
            }
        }
    }

    // dann konvertieren und übernehmen
    *spielId  = strSpielId.toInt();
    *tipEins  = strTipEins.toInt();
    *tipZweiA = strTipZweiA.toInt();
    *tipZweiB = strTipZweiB.toInt();

    return ok;
}

/*
  Funktion: Deserialisiert Daten einer ClientHighscoreTableRequest
  Autor: Benjamin Bardroff
  Datum: 01.04.2016
  */
bool BwsProtokoll::unpackCR_HST(QString data, QString *eventName)
{
    bool ok = true;
    int dataField = 1;

    QString character = "";

    for(int i = 0; i < data.length();i++)
    {
        character = data.mid(i,1);

        if(character == BwsProtokoll::NEXT_DATA_FIELD)
            dataField++;
        else
        {
            switch(dataField)
            {
            case 1: *eventName += character; break;
            default: ok = false; break;
            }
        }
    }

    return ok;
}

/*
  Funktion: Deserialisiert Daten einer ClientSelectedPlayerRequest
  Autor: Benjamin Bardroff
  Datum: 01.04.2016
  */
bool BwsProtokoll::unpackCR_SELP(QString data, QString *spielerName)
{
    bool ok = true;
    int dataField = 1;

    QString character = "";

    for(int i = 0; i < data.length();i++)
    {
        character = data.mid(i,1);

        if(character == BwsProtokoll::NEXT_DATA_FIELD)
            dataField++;
        else
        {
            switch(dataField)
            {
            case 1: *spielerName += character; break;
            default: ok = false; break;
            }
        }
    }

    return ok;
}

/********************************************************************************
 *                             SERVER-ANTWORTEN                                  *
 ********************************************************************************/

// *** Pack-Methoden *** //

/*
  Funktion: Serialisiert Daten einer ServerRegisterResponse
  Autor: Benjamin Bardroff
  Datum: 01.04.2016
  */
QString BwsProtokoll::packSR_REG(bool regState, QString error)
{
    return BwsProtokoll::PROTOKOLL_ID
            + BwsProtokoll::NEXT_DATA_FIELD
            + this->serverResponses[BwsProtokoll::SR_REG]
            + BwsProtokoll::NEXT_DATA_FIELD
            + QString::number(regState)
            + BwsProtokoll::NEXT_DATA_FIELD
            + error;
}

/*
  Funktion: Serialisiert Daten einer ServerLoginResponse
  Autor: Benjamin Bardroff
  Datum: 01.04.2016
  */
QString BwsProtokoll::packSR_LOGIN(bool loginState, QVector<EventInfo> events, QString vorname, QString nachname, QString klasse, QString email, QString regSeit, QString error )
{
    QString out = BwsProtokoll::PROTOKOLL_ID
        + BwsProtokoll::NEXT_DATA_FIELD
        + this->serverResponses[BwsProtokoll::SR_LOGIN]
        + BwsProtokoll::NEXT_DATA_FIELD
        + QString::number(loginState)
        + BwsProtokoll::NEXT_DATA_FIELD
        + QString::number(events.length());

    for(int i = 0; i < events.length(); i++) // Vector einfügen
    {
        out += BwsProtokoll::NEXT_DATA_FIELD
                + events.at(i).getEventName()
                + BwsProtokoll::NEXT_DATA_FIELD
                + events.at(i).getEventStart()
                + BwsProtokoll::NEXT_DATA_FIELD
                + events.at(i).getEventEnde()
                + BwsProtokoll::NEXT_DATA_FIELD
                + QString::number(events.at(i).getPreisTipEins())
                + BwsProtokoll::NEXT_DATA_FIELD
                + QString::number(events.at(i).getPreisTipZwei());
    }

    out += BwsProtokoll::NEXT_DATA_FIELD
        + vorname
        + BwsProtokoll::NEXT_DATA_FIELD
        + nachname
        + BwsProtokoll::NEXT_DATA_FIELD
        + klasse
        + BwsProtokoll::NEXT_DATA_FIELD
        + email
        + BwsProtokoll::NEXT_DATA_FIELD
        + regSeit
        + BwsProtokoll::NEXT_DATA_FIELD
        + error;

    return out;
}

/*
  Funktion: Serialisiert Daten einer ServerLogoutResponse
  Autor: Benjamin Bardroff
  Datum: 01.04.2016
  */
QString BwsProtokoll::packSR_LOGOUT(bool logoutState, QString error)
{
    return BwsProtokoll::PROTOKOLL_ID
            + BwsProtokoll::NEXT_DATA_FIELD
            + this->serverResponses[BwsProtokoll::SR_LOGOUT]
            + BwsProtokoll::NEXT_DATA_FIELD
            + QString::number(logoutState)
            + BwsProtokoll::NEXT_DATA_FIELD
            + error;
}

/*
  Funktion: Serialisiert Daten einer ServerChangeMailResponse
  Autor: Benjamin Bardroff
  Datum: 01.04.2016
  */
QString BwsProtokoll::packSR_CMAIL(bool changed, QString error)
{
    return BwsProtokoll::PROTOKOLL_ID
            + BwsProtokoll::NEXT_DATA_FIELD
            + this->serverResponses[BwsProtokoll::SR_CMAIL]
            + BwsProtokoll::NEXT_DATA_FIELD
            + QString::number(changed)
            + BwsProtokoll::NEXT_DATA_FIELD
            + error;
}

/*
  Funktion: Serialisiert Daten einer ServerChangePasswordResponse
  Autor: Benjamin Bardroff
  Datum: 01.04.2016
  */
QString BwsProtokoll::packSR_CPASS(bool changed, QString error)
{
    return BwsProtokoll::PROTOKOLL_ID
            + BwsProtokoll::NEXT_DATA_FIELD
            + this->serverResponses[BwsProtokoll::SR_CPASS]
            + BwsProtokoll::NEXT_DATA_FIELD
            + QString::number(changed)
            + BwsProtokoll::NEXT_DATA_FIELD
            + error;
}

/*
  Funktion: Serialisiert Daten einer ServerDeleteAccountResponse
  Autor: Benjamin Bardroff
  Datum: 01.04.2016
  */
QString BwsProtokoll::packSR_DACC(bool deleted, QString error)
{
    return BwsProtokoll::PROTOKOLL_ID
            + BwsProtokoll::NEXT_DATA_FIELD
            + this->serverResponses[BwsProtokoll::SR_DACC]
            + BwsProtokoll::NEXT_DATA_FIELD
            + QString::number(deleted)
            + BwsProtokoll::NEXT_DATA_FIELD
            + error;
}

/*
  Funktion: Serialisiert Daten einer ServerUserOverviewResponse
  Autor: Benjamin Bardroff
  Datum: 01.04.2016
  */
QString BwsProtokoll::packSR_UO( QVector<TipInfo> tips, QString error)
{
    QString out = BwsProtokoll::PROTOKOLL_ID // Kopf erstellen
            + BwsProtokoll::NEXT_DATA_FIELD
            + this->serverResponses[BwsProtokoll::SR_UO]
            + BwsProtokoll::NEXT_DATA_FIELD
            + QString::number(tips.length());

    for(int i = 0; i < tips.length(); i++) // Vector einfügen
    {
        out += BwsProtokoll::NEXT_DATA_FIELD
                + tips.at(i).getMannschaftA()
                + BwsProtokoll::NEXT_DATA_FIELD
                + tips.at(i).getMannschaftB()
                + BwsProtokoll::NEXT_DATA_FIELD
                + tips.at(i).getDatum()
                + BwsProtokoll::NEXT_DATA_FIELD
                + QString::number(tips.at(i).getSpielStatus())
                + BwsProtokoll::NEXT_DATA_FIELD
                + QString::number(tips.at(i).getTipEins())
                + BwsProtokoll::NEXT_DATA_FIELD
                + QString::number(tips.at(i).getTipZweiA())
                + BwsProtokoll::NEXT_DATA_FIELD
                + QString::number(tips.at(i).getTipZweiB());
    }

    out += BwsProtokoll::NEXT_DATA_FIELD // Error anhängen
            + error;

    return out;
}

/*
  Funktion: Serialisiert Daten einer ServerCurrentGamesResponse
  Autor: Benjamin Bardroff
  Datum: 01.04.2016
  */
QString BwsProtokoll::packSR_CG(QVector<SpielInfo> spiele, QString error)
{
    QString out = BwsProtokoll::PROTOKOLL_ID // Kopf erstellen
            + BwsProtokoll::NEXT_DATA_FIELD
            + this->serverResponses[BwsProtokoll::SR_CG]
            + BwsProtokoll::NEXT_DATA_FIELD
            + QString::number(spiele.length());

    for(int i = 0; i < spiele.length(); i++) // Vector einfügen
    {
        out += BwsProtokoll::NEXT_DATA_FIELD
                + QString::number(spiele.at(i).getSpielId())
                + BwsProtokoll::NEXT_DATA_FIELD
                + spiele.at(i).getMannschaftA()
                + BwsProtokoll::NEXT_DATA_FIELD
                + spiele.at(i).getMannschaftB()
                + BwsProtokoll::NEXT_DATA_FIELD
                + spiele.at(i).getDatum()
                + BwsProtokoll::NEXT_DATA_FIELD
                + QString::number(spiele.at(i).getSpielStatus());
    }

    out += BwsProtokoll::NEXT_DATA_FIELD // Error anhängen
            + error;

    return out;
}

/*
  Funktion: Serialisiert Daten einer ServerSelectedGameResponse
  Autor: Benjamin Bardroff
  Datum: 01.04.2016
  */
QString BwsProtokoll::packSR_SG(QString start, QString ende, int punkteA, int punkteB, int tipEins, int tipZweiA, int tipZweiB, QString error)
{
    return BwsProtokoll::PROTOKOLL_ID
            + BwsProtokoll::NEXT_DATA_FIELD
            + this->serverResponses[BwsProtokoll::SR_SG]
            + BwsProtokoll::NEXT_DATA_FIELD
            + start
            + BwsProtokoll::NEXT_DATA_FIELD
            + ende
            + BwsProtokoll::NEXT_DATA_FIELD
            + QString::number(punkteA)
            + BwsProtokoll::NEXT_DATA_FIELD
            + QString::number(punkteB)
            + BwsProtokoll::NEXT_DATA_FIELD
            + QString::number(tipEins)
            + BwsProtokoll::NEXT_DATA_FIELD
            + QString::number(tipZweiA)
            + BwsProtokoll::NEXT_DATA_FIELD
            + QString::number(tipZweiB)
            + BwsProtokoll::NEXT_DATA_FIELD
            + error;
}

/*
  Funktion: Serialisiert Daten einer ServerSubmitTipResponse
  Autor: Benjamin Bardroff
  Datum: 01.04.2016
  */
QString BwsProtokoll::packSR_SUBT(bool submitOk, QString error)
{
    return BwsProtokoll::PROTOKOLL_ID
            + BwsProtokoll::NEXT_DATA_FIELD
            + this->serverResponses[BwsProtokoll::SR_SUBT]
            + BwsProtokoll::NEXT_DATA_FIELD
            + QString::number(submitOk)
            + BwsProtokoll::NEXT_DATA_FIELD
            + error;
}

/*
  Funktion: Serialisiert Daten einer ServerHighscoreTableResponse
  Autor: Benjamin Bardroff
  Datum: 01.04.2016
  */
QString BwsProtokoll::packSR_HST(QVector<SpielerInfo> spieler, QString error)
{
    QString out = BwsProtokoll::PROTOKOLL_ID // Kopf erstellen
            + BwsProtokoll::NEXT_DATA_FIELD
            + this->serverResponses[BwsProtokoll::SR_HST]
            + BwsProtokoll::NEXT_DATA_FIELD
            + QString::number(spieler.length());

    for(int i = 0; i < spieler.length(); i++) // Vector einfügen
    {
        out += BwsProtokoll::NEXT_DATA_FIELD
                + spieler.at(i).getSpielerName()
                + BwsProtokoll::NEXT_DATA_FIELD
                + QString::number(spieler.at(i).getPunkte());
    }

    out += BwsProtokoll::NEXT_DATA_FIELD // Error anhängen
            + error;

    return out;
}

/*
  Funktion: Serialisiert Daten einer ServerSelectedPlayerResponse
  Autor: Benjamin Bardroff
  Datum: 01.04.2016
  */
QString BwsProtokoll::packSR_SELP(QString vorname, QString nachname, QString klasse, QString regSeit, QString error)
{
    return BwsProtokoll::PROTOKOLL_ID
            + BwsProtokoll::NEXT_DATA_FIELD
            + this->serverResponses[BwsProtokoll::SR_SELP]
            + BwsProtokoll::NEXT_DATA_FIELD
            + vorname
            + BwsProtokoll::NEXT_DATA_FIELD
            + nachname
            + BwsProtokoll::NEXT_DATA_FIELD
            + klasse
            + BwsProtokoll::NEXT_DATA_FIELD
            + regSeit
            + BwsProtokoll::NEXT_DATA_FIELD
            + error;
}

/*
  Funktion: Serialisiert Daten einer ServerInvalidPackageResponse
  Autor: Benjamin Bardroff
  Datum: 01.04.2016
  */
QString BwsProtokoll::packSR_INVP(QString error)
{
    return BwsProtokoll::PROTOKOLL_ID
            + BwsProtokoll::NEXT_DATA_FIELD
            + this->serverResponses[BwsProtokoll::SR_INVP]
            + BwsProtokoll::NEXT_DATA_FIELD
            + error;
}

/*
  Funktion: Serialisiert Daten einer ServerUserNotLoggedInResponse
  Autor: Benjamin Bardroff
  Datum: 01.04.2016
  */
QString BwsProtokoll::packSR_UNLI(QString error)
{
    return BwsProtokoll::PROTOKOLL_ID
            + BwsProtokoll::NEXT_DATA_FIELD
            + this->serverResponses[BwsProtokoll::SR_UNLI]
            + BwsProtokoll::NEXT_DATA_FIELD
            + error;
}

// *** Unpack-Methoden *** //

/*
  Funktion: Deserialisiert Daten einer ServerRegisterResponse
  Autor: Benjamin Bardroff
  Datum: 01.04.2016
  */
bool BwsProtokoll::unpackSR_REG(QString data, QString *error, bool *regState)
{
    bool ok = true;
    int dataField = 1;

    QString character = "";

    for(int i = 0; i < data.length();i++)
    {
        character = data.mid(i,1);

        if(character == BwsProtokoll::NEXT_DATA_FIELD)
            dataField++;
        else
        {
            switch(dataField)
            {
            case 1: *regState  = character.toInt(); break; // direkte Wertuebernahme, da boll immer "1" oder "0"
            case 2: *error    += character;         break;
            default: ok = false; break;
            }
        }
    }

    return ok;
}

/*
  Funktion: Deserialisiert Daten einer ServerLoginResponse
  Autor: Benjamin Bardroff
  Datum: 08.04.2016
  */
bool BwsProtokoll::unpackSR_LOGIN(QString data, QString *error, bool *loginState, QVector<EventInfo> *events, QString *vorname, QString *nachname, QString *klasse, QString *email, QString *regSeit)
{


    bool ok = true;
    int index = 0;
    int dataField        = 1; // Zeigt auf aktuelles Datenfeld -> loginState
    int amountDataStruct = 0; // zeigt auf wieviele DataStructs vorliegen
    int dataStructField  = 1; // Zeigt auf aktuelles Datafield in DataStruct
    int currDataStruct   = 1; // Zeigt auf aktuelles DataStruct

    QString character = "";
    QString strAmountDataStruct = "";

    // Felder für TipInfo
    EventInfo tmpEventInfo;

    QString strEventName  = "";
    QString strEventStart = "";
    QString strEventEnde  = "";
    QString strPreisTippA = "";
    QString strPreisTippB = "";

    events->clear(); // Vector leeren

    *loginState = data.mid(index++,1).toInt(); // loginStatus ermitteln; erstes Datenfeld
    dataField++; // auf nächstes Datenfeld springen

    while(dataField == 2) // Amount an Datastructs auslesen (2. Datenfeld)
    {
        character = data.mid(++index,1); // index um 1 erhöhen und diesen verwenden (da sonst index auf NEXT_DATA_FIELD zeigt)

        if(character == BwsProtokoll::NEXT_DATA_FIELD)
            dataField++;
        else
            strAmountDataStruct += character;
    }

    amountDataStruct = strAmountDataStruct.toInt(); // anzahl an datenstrukturen konverteiren int int

    if(amountDataStruct == 0 ) // wenn keine eEvents geliefert, dasnn direkt mit vorname weitermachen (index 4)
        dataField = 4;

    if(!loginState)
    {
        dataField = 9; // wenn user nicht eingeloggt, nur error feld
        index += 5; // Leere felder überspringen
    }

    while(dataField == 3) // datastructs auslesen (Datenfeld 3)
    {
        character = data.mid(++index,1); // Index inkrement dann verwenden da Index auf NEXT_DATA_FIELD zeigt

        if(character == BwsProtokoll::NEXT_DATA_FIELD)
            dataStructField++;
        else
        {
            switch(dataStructField)
            {
            case 1: strEventName  += character; break;
            case 2: strEventStart += character; break;
            case 3: strEventEnde  += character; break;
            case 4: strPreisTippA += character; break;
            case 5: strPreisTippB += character; break;
            default: ok = false; break;
            }
        }

        if(dataStructField > 5 ) // Wenn alle Felder des DataStruct durchlaufen
        {
            tmpEventInfo.setEventInfo(strEventName, strEventStart, strEventEnde, strPreisTippA.toInt(), strPreisTippB.toInt()); // Daten in TipInfo laden
            events->push_back(tmpEventInfo); // Tip Info in Vector speichern

            strEventName.clear(); // tmpStrings leerren
            strEventStart.clear();
            strEventEnde.clear();
            strPreisTippA.clear();
            strPreisTippB.clear();

            currDataStruct++;    // auf nächstes Datastruct zeigen
            dataStructField = 1; // feld für nächstes DataStruct zurücksetzen

            if(currDataStruct > amountDataStruct)
                dataField++; // wenn letztes DataStruct behandlet auf error springen
        }
    }

    for(int i = ++index; i < data.length(); i++) // restliche Datenfelder durchlaufen
    {
        character = data.mid(i,1); // nächsten char holen

        if(character == BwsProtokoll::NEXT_DATA_FIELD)
            dataField++;
        else
        {
            switch(dataField)
            {
            case 4: *vorname  += character; break;
            case 5: *nachname += character; break;
            case 6: *klasse   += character; break;
            case 7: *email    += character; break;
            case 8: *regSeit  += character; break;
            case 9: *error    += character; break;
            default: ok = false; break;
            }
        }
    }

    return ok;
}

/*
  Funktion: Deserialisiert Daten einer ServerLogoutResponse
  Autor: Benjamin Bardroff
  Datum: 01.04.2016
  */
bool BwsProtokoll::unpackSR_LOGOUT(QString data, QString *error, bool *logoutState)
{
    bool ok = true;
    int dataField = 1;

    QString character = "";

    for(int i = 0; i < data.length();i++)
    {
        character = data.mid(i,1);

        if(character == BwsProtokoll::NEXT_DATA_FIELD)
            dataField++;
        else
        {
            switch(dataField)
            {
            case 1: *logoutState = character.toInt(); break; // direkte Wertuebernahme, da boll immer "1" oder "0"
            case 2: *error      += character;         break;
            default: ok = false; break;
            }
        }
    }

    return ok;
}

/*
  Funktion: Deserialisiert Daten einer ServerChangeMailResponse
  Autor: Benjamin Bardroff
  Datum: 01.04.2016
  */
bool BwsProtokoll::unpackSR_CMAIL(QString data, QString *error, bool *changed)
{
    bool ok = true;
    int dataField = 1;

    QString character = "";

    for(int i = 0; i < data.length();i++)
    {
        character = data.mid(i,1);

        if(character == BwsProtokoll::NEXT_DATA_FIELD)
            dataField++;
        else
        {
            switch(dataField)
            {
            case 1: *changed = character.toInt(); break; // direkte Wertuebernahme, da boll immer "1" oder "0"
            case 2: *error  += character;         break;
            default: ok = false; break;
            }
        }
    }

    return ok;
}

/*
  Funktion: Deserialisiert Daten einer ServerChangePasswordResponse
  Autor: Benjamin Bardroff
  Datum: 01.04.2016
  */
bool BwsProtokoll::unpackSR_CPASS(QString data, QString *error, bool *changed)
{
    bool ok = true;
    int dataField = 1;

    QString character = "";

    for(int i = 0; i < data.length();i++)
    {
        character = data.mid(i,1);

        if(character == BwsProtokoll::NEXT_DATA_FIELD)
            dataField++;
        else
        {
            switch(dataField)
            {
            case 1: *changed = character.toInt(); break; // direkte Wertuebernahme, da boll immer "1" oder "0"
            case 2: *error  += character;         break;
            default: ok = false; break;
            }
        }
    }

    return ok;
}

/*
  Funktion: Deserialisiert Daten einer ServerDeleteAccountResponse
  Autor: Benjamin Bardroff
  Datum: 01.04.2016
  */
bool BwsProtokoll::unpackSR_DACC(QString data, QString *error, bool *deleted)
{
    bool ok = true;
    int dataField = 1;

    QString character = "";

    for(int i = 0; i < data.length();i++)
    {
        character = data.mid(i,1);

        if(character == BwsProtokoll::NEXT_DATA_FIELD)
            dataField++;
        else
        {
            switch(dataField)
            {
            case 1: *deleted = character.toInt(); break; // direkte Wertuebernahme, da boll immer "1" oder "0"
            case 2: *error  += character;         break;
            default: ok = false; break;
            }
        }
    }

    return ok;
}

/*
  Funktion: Deserialisiert Daten einer ServerUserOvierviewResponse
  Autor: Benjamin Bardroff
  Datum: 01.04.2016
  */
bool BwsProtokoll::unpackSR_UO(QString data, QString *error, QVector<TipInfo> *tips)
{
    bool ok = true;
    int index = 0;
    int dataField        = 1; // Zeigt auf aktuelles Datenfeld
    int amountDataStruct = 0; // zeigt auf wieviele DataStructs vorliegen
    int dataStructField  = 1; // Zeigt auf aktuelles Datafield in DataStruct
    int currDataStruct   = 1; // Zeigt auf aktuelles DataStruct

    QString character = "";
    QString strAmountDataStruct = "";

    // Felder für TipInfo
    TipInfo tmpTipInfo;

    QString strMannchafftA = "";
    QString strMannschaftB = "";
    QString strDatum       = "";
    bool    spielStatus;
    QString strTipEins     = "";
    QString strTipZweiA    = "";
    QString strTipZweiB    = "";

    tips->clear(); // Vector leeren

    while(dataField == 1) // Amount an Datastructs auslesen
    {
        character = data.mid(index++,1); // index verwenden und dann um 1 erhöhen

        if(character == BwsProtokoll::NEXT_DATA_FIELD)
            dataField++;
        else
            strAmountDataStruct += character;
    }

    amountDataStruct = strAmountDataStruct.toInt(); // anzahl an datenstrukturen annehmen

    if(amountDataStruct == 0) // wenn keine DataStructs direkt auf error springen
        dataField = 3;

    while(dataField == 2) // datastructs auslesen
    {
        character = data.mid(index++,1); // index verwenden und dann um 1 erhöhen

        if(character == BwsProtokoll::NEXT_DATA_FIELD)
            dataStructField++;
        else
        {
            switch(dataStructField)
            {
            case 1: strMannchafftA += character;         break;
            case 2: strMannschaftB += character;         break;
            case 3: strDatum       += character;         break;
            case 4: spielStatus     = character.toInt(); break;
            case 5: strTipEins     += character;         break;
            case 6: strTipZweiA    += character;         break;
            case 7: strTipZweiB    += character;         break;
            default: ok = false;                         break;
            }
        }

        if(dataStructField > 7 ) // Wenn alle Felder des DataStruct durchlaufen
        {
            tmpTipInfo.setTipInfo(strMannchafftA, strMannschaftB, strDatum, spielStatus, strTipEins.toInt(), strTipZweiA.toInt(), strTipZweiB.toInt()); // Daten in TipInfo laden
            tips->push_back(tmpTipInfo); // Tip Info in Vector speichern

            strMannchafftA.clear();
            strMannschaftB.clear();
            strDatum.clear();
            strTipEins.clear();
            strTipZweiA.clear();
            strTipZweiB.clear();

            currDataStruct++;    // auf nächstes Datastruct zeigen
            dataStructField = 1; // feld für nächstes DataStruct zurücksetzen

            if(currDataStruct > amountDataStruct)
                dataField++; // wenn letztes DataStruct behandlet auf error springen
        }
    }

    if(dataField == 3) // error feld auslesen
    {
        while(index < data.length())
        {
            character = data.mid(++index,1); // index verwenden und dann um 1 erhöhen
            *error += character;
        }
    }

    return ok;
}

/*
  Funktion: Deserialisiert Daten einer ServerCurrentGamesResponse
  Autor: Benjamin Bardroff
  Datum: 01.04.2016
  */
bool BwsProtokoll::unpackSR_CG(QString data, QString *error, QVector<SpielInfo> *spiele)
{
    bool ok = true;
    int index = 0;
    int dataField        = 1; // Zeigt auf aktuelles Datenfeld
    int amountDataStruct = 0; // zeigt auf wieviele DataStructs vorliegen
    int dataStructField  = 1; // Zeigt auf aktuelles Datafield in DataStruct
    int currDataStruct   = 1; // Zeigt auf aktuelles DataStruct

    QString character = "";
    QString strAmountDataStruct = "";

    SpielInfo tmpSpielInfo;

    QString strSpielId      = "";
    QString strMannschafftA = "";
    QString strMannschafftB = "";
    QString strDatum        = "";
    bool spielStatus;

    spiele->clear(); // Vector leeren

    while(dataField == 1) // Amount an Datastructs auslesen
    {
        character = data.mid(index++,1); // index verwenden und dann um 1 erhöhen

        if(character == BwsProtokoll::NEXT_DATA_FIELD)
            dataField++;
        else
            strAmountDataStruct += character;
    }

    amountDataStruct = strAmountDataStruct.toInt(); // anzahl an datenstrukturen annehmen

    if(amountDataStruct == 0) // wenn keine DataStructs
        dataField = 3;

    while(dataField == 2) // datastructs auslesen
    {
        character = data.mid(index++,1); // index verwenden und dann um 1 erhöhen

        if(character == BwsProtokoll::NEXT_DATA_FIELD)
            dataStructField++;
        else
        {
            switch(dataStructField)
            {
            case 1: strSpielId      += character; break;
            case 2: strMannschafftA += character; break;
            case 3: strMannschafftB += character; break;
            case 4: strDatum        += character; break;
            case 5: spielStatus = character.toInt(); break;
            default: ok = false; break;
            }
        }

        if(dataStructField > 5 ) // Wenn alle Felder des DataStruct durchlaufen
        {
            // Daten in TipInfo laden
            tmpSpielInfo.setSpielInfo(strSpielId.toInt(), strMannschafftA, strMannschafftB, strDatum, spielStatus);
            spiele->push_back(tmpSpielInfo);
            // Tip Info in Vector speichern

            strSpielId.clear(); // tmpString leerren
            strMannschafftA.clear();
            strMannschafftB.clear();
            strDatum.clear();

            currDataStruct++;    // auf nächstes Datastruct zeigen
            dataStructField = 1; // feld für nächstes DataStruct zurücksetzen

            if(currDataStruct > amountDataStruct)
                dataField++; // wenn letztes DataStruct behandlet auf error springen
        }
    }

    if(dataField == 3) // error feld auslesen
    {
        while(index < data.length())
        {
            character = data.mid(++index,1); // index verwenden und dann um 1 erhöhen
            *error += character;
        }
    }

    return ok;
}

/*
  Funktion: Deserialisiert Daten einer ServerSelectedGameResponse
  Autor: Benjamin Bardroff
  Datum: 08.04.2016
  */
bool BwsProtokoll::unpackSR_SG(QString data, QString *error, QString *start, QString *ende, int *punkteA, int *punkteB, int *tipEins, int *tipZweiA, int *tipZweiB)
{
    bool ok = true;
    int dataField = 1;

    QString character = "";

    QString strPunkteA  = "";
    QString strPunkteB  = "";
    QString strTipEins  = "";
    QString strTipZweiA = "";
    QString strTipZweiB = "";

    for(int i = 0; i < data.length();i++)
    {
        character = data.mid(i,1);

        if(character == BwsProtokoll::NEXT_DATA_FIELD)
            dataField++;
        else
        {
            switch(dataField)
            {
            case 1: *start += character; break;
            case 2: *ende  += character; break;
            case 3: strPunkteA += character; break;
            case 4: strPunkteB += character; break;
            case 5: strTipEins += character; break;
            case 6: strTipZweiA += character; break;
            case 7: strTipZweiB += character; break;
            case 8: *error      += character; break;
            default: ok = false; break;
            }
        }
    }

    *punkteA  = strPunkteA.toInt();
    *punkteB  = strPunkteB.toInt();
    *tipEins  = strTipEins.toInt();
    *tipZweiA = strTipZweiA.toInt();
    *tipZweiB = strTipZweiB.toInt();

    return ok;
}

/*
  Funktion: Deserialisiert Daten einer ServerSubmitTipResponse
  Autor: Benjamin Bardroff
  Datum: 01.04.2016
  */
bool BwsProtokoll::unpackSR_SUBT(QString data, QString *error, bool *submitOk)
{
    bool ok = true;
    int dataField = 1;

    QString character = "";

    for(int i = 0; i < data.length();i++)
    {
        character = data.mid(i,1);

        if(character == BwsProtokoll::NEXT_DATA_FIELD)
            dataField++;
        else
        {
            switch(dataField)
            {
            case 1: *submitOk = character.toInt(); break; // direkte Wertuebernahme, da boll immer "1" oder "0"
            case 2: *error   += character;         break;
            default: ok = false; break;
            }
        }
    }

    return ok;
}

/*
  Funktion: Deserialisiert Daten einer ServerHighscoreTableResponse
  Autor: Benjamin Bardroff
  Datum: 01.04.2016
  */
bool BwsProtokoll::unpackSR_HST(QString data, QString *error, QVector<SpielerInfo> *spieler)
{
    bool ok = true;
    int index = 0;
    int dataField        = 1; // Zeigt auf aktuelles Datenfeld
    int amountDataStruct = 0; // zeigt auf wieviele DataStructs vorliegen
    int dataStructField  = 1; // Zeigt auf aktuelles Datafield in DataStruct
    int currDataStruct   = 1; // Zeigt auf aktuelles DataStruct

    QString character = "";
    QString strAmountDataStruct = "";

    SpielerInfo tmpSpielerInfo;
    QString strSpielerName = "";
    QString strPunkte      = "";

    spieler->clear(); // Vector leeren

    while(dataField == 1) // Amount an Datastructs auslesen
    {
        character = data.mid(index++,1); // index verwenden und dann um 1 erhöhen

        if(character == BwsProtokoll::NEXT_DATA_FIELD)
            dataField++;
        else
            strAmountDataStruct += character;
    }

    amountDataStruct = strAmountDataStruct.toInt(); // anzahl an datenstrukturen annehmen

    if(amountDataStruct == 0) // wenn keine DataStructs dann direkt zu error
        dataField = 3;

    while(dataField == 2) // datastructs auslesen
    {
        character = data.mid(index++,1); // index verwenden und dann um 1 erhöhen

        if(character == BwsProtokoll::NEXT_DATA_FIELD)
            dataStructField++;
        else
        {
            switch(dataStructField)
            {
            case 1: strSpielerName += character; break;
            case 2: strPunkte      += character; break;
            default: ok = false; break;
            }
        }

        if(dataStructField > 2 ) // Wenn alle Felder des DataStruct durchlaufen
        {
            // Daten in TipInfo laden
            tmpSpielerInfo.setSpielerInfo(strSpielerName, strPunkte.toInt());
            spieler->push_back(tmpSpielerInfo);
            // Tip Info in Vector speichern

            strSpielerName.clear(); // tmpString leerren
            strPunkte.clear();

            currDataStruct++;    // auf nächstes Datastruct zeigen
            dataStructField = 1; // feld für nächstes DataStruct zurücksetzen

            if(currDataStruct > amountDataStruct)
                dataField++; // wenn letztes DataStruct behandlet auf error springen
        }
    }

    if(dataField == 3) // error feld auslesen
    {
        while(index < data.length())
        {
            character = data.mid(index++,1); // index verwenden und dann um 1 erhöhen
            *error += character;
        }
    }

    return ok;
}

/*
  Funktion: Deserialisiert Daten einer ServerSelectedPlayerResponse
  Autor: Benjamin Bardroff
  Datum: 01.04.2016
  */
bool BwsProtokoll::unpackSR_SELP(QString data, QString *error, QString *vorname, QString *nachname, QString *regSeit, QString *klasse)
{
    bool ok = true;
    int dataField = 1;

    QString character = "";

    for(int i = 0; i < data.length();i++)
    {
        character = data.mid(i,1);

        if(character == BwsProtokoll::NEXT_DATA_FIELD)
            dataField++;
        else
        {
            switch(dataField)
            {
            case 1: *vorname  += character; break; // direkte Wertuebernahme, da boll immer "1" oder "0"
            case 2: *nachname += character; break;
            case 3: *klasse   += character; break;
            case 4: *regSeit  += character; break;
            case 5: *error    += character; break;
            default: ok = false; break;
            }
        }
    }

    return ok;
}

/*
  Funktion: Deserialisiert Daten einer ServerInvalidPackageResponse
  Autor: Benjamin Bardroff
  Datum: 01.04.2016
  */
bool BwsProtokoll::unpackSR_INVP(QString data, QString* error)
{
    bool ok = true;
    int dataField = 1;

    QString character = "";

    for(int i = 0; i < data.length();i++)
    {
        character = data.mid(i,1);

        if(character == BwsProtokoll::NEXT_DATA_FIELD)
            dataField++;
        else
        {
            switch(dataField)
            {
            case 1: *error += character; break;
            default: ok = false; break;
            }
        }
    }

    return ok;
}

/*
  Funktion: Deserialisiert Daten einer ServerUserNotLoggedInResponse
  Autor: Benjamin Bardroff
  Datum: 01.04.2016
  */
bool BwsProtokoll::unpackSR_UNLI(QString data, QString* error)
{
    bool ok = true;
    int dataField = 1;

    QString character = "";

    for(int i = 0; i < data.length();i++)
    {
        character = data.mid(i,1);

        if(character == BwsProtokoll::NEXT_DATA_FIELD)
            dataField++;
        else
        {
            switch(dataField)
            {
            case 1: *error += character; break;
            default: ok = false; break;
            }
        }
    }

    return ok;
}
