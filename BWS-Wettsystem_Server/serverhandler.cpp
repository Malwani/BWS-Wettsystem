/*
  Modul: ServerHandler.cpp
  Funktion: Übernimmt Netzwerkkommunikation und Authetifizirung der Benutzer
  Autor: Benjamin Bardroff, Christoph Rehm
  Datum: 16.03.2016
  */

#include "serverhandler.h"

/********************************************************************************
 *                          KONSTRUKTOR / DESTRUKTOR                            *
 ********************************************************************************/

ServerHandler::ServerHandler(QObject *parent) : QObject(parent)
{

}


// Schließt alle Verbindungen und den Server
ServerHandler::~ServerHandler()
{

}

/*********************************************************************************
 *                            VERBINDUNGS-METHODEN                               *
 ********************************************************************************/

void ServerHandler::startServer(int port)
{
    this->readMapper = new QSignalMapper(this);
    this->disconnectMapper = new QSignalMapper(this);
    this->server = new QTcpServer();

    QObject::connect(this->server,SIGNAL(newConnection()),this,SLOT(neueVerbindungAufbauen()));
    QObject::connect(readMapper      , SIGNAL(mapped(QObject*)) ,this, SLOT(neueDatenEmpfangen(QObject*)) );
    QObject::connect(disconnectMapper, SIGNAL(mapped(QObject*)), this, SLOT(verbindungsAbbruch(QObject*)) );

    this->server->listen(QHostAddress::Any,port);
    emit uebermittleLogNachricht("Server gestartet. [Port: " + QString::number(port) + "]");
}

void ServerHandler::stopServer()
{
    this->server->close();
    delete server;
    delete readMapper;
    delete disconnectMapper;
    this->loggedInUser.clear();
    emit uebermittleLogNachricht("Server gestoppt.");
}

// Wird aufgerufen sobald eine neue Verbindung eingeht
void ServerHandler::neueVerbindungAufbauen()
{    
    QTcpSocket* tmpSocket = this->server->nextPendingConnection();
    Verbindung* neueVerbindung = new Verbindung(tmpSocket, tmpSocket->peerAddress().toString());

    //Dissconnect verarbeiten
    QObject::connect(neueVerbindung->getClient(), SIGNAL(disconnected()), disconnectMapper, SLOT(map()) );
    this->disconnectMapper->setMapping(neueVerbindung->getClient(), neueVerbindung);

    //erhaltene Daten verarbeiten
    QObject::connect(neueVerbindung->getClient(),SIGNAL(readyRead()),readMapper,SLOT(map()) );
    this->readMapper->setMapping(neueVerbindung->getClient(),neueVerbindung);

    emit uebermittleLogNachricht("Verbindung wurde aufgebaut [IP: " + neueVerbindung->getIP());
}

void ServerHandler::verbindungsAbbruch(QObject* verb)
{
    Verbindung* verbindung = qobject_cast<Verbindung *>(verb);
    emit uebermittleLogNachricht("Verbindung unterbrochen [User: " + verbindung->getActiveUser() + verbindung->getIP());

    if(verbindung->getLoginState())
    {
        for(int i = 0; i < this->loggedInUser.length();i++)
        {
            if(this->loggedInUser.at(i) == verbindung->getActiveUser())
            {
                this->loggedInUser.removeAt(i);
                break;
            }
        }
    }

    delete verbindung;
}

/********************************************************************************
 *                           KOMMUNIKATIONS-METHODEN                            *
 ********************************************************************************/

void ServerHandler::takeAccountListe(AccountListe* p_accountListe)
{
    this->p_accountListe = p_accountListe;
}

void ServerHandler::takeEventListe(EventListe *p_eventListe)
{
    this->p_eventListe = p_eventListe;
}

void ServerHandler::sendeDaten(Verbindung* verbindung, QString daten)
{
    verbindung->getClient()->write(daten.toUtf8());
}

void ServerHandler::neueDatenEmpfangen(QObject* verb)
{
    QString input;
    Verbindung* verbindung = qobject_cast<Verbindung *>(verb); // übergebener QObject-Pointer ind Verbindungt-Pointer casten

    input = verbindung->getClient()->readAll(); // Daten annehmen
    this->verarbeiteAnfrage(verbindung, input);
}

void ServerHandler::verarbeiteAnfrage(Verbindung *verbindung, QString data)
{    
    if(!this->protokoll.checkValid(&data)) // wenn Paket ungültig
    {
        // Sende SINVP - ServerInvalidPackage
        this->sendeDaten(verbindung, this->protokoll.packSR_INVP());
    }
    else // Wenn Paketgültig, ermittle PackageID und prüfe Login status
    {
        // Ermittle PackageID
        int packageId = this->protokoll.getClientAnfragePackageId(&data);

        // Prüfe Login Status
        if(!verbindung->getLoginState()) //Wenn User nicht eingeloggt
        {
            switch(packageId) // Dann nur Login oder Reg gueltig; sonst UserNotLoggedIn(SUNLI)-Package
            {
            case BwsProtokoll::CR_REG:
            {
                emit uebermittleLogNachricht("CR_REG angekommen.");
                this->verarbeiteCR_REG(verbindung, data);
                break;
            }
            case BwsProtokoll::CR_LOGIN:
            {
                this->uebermittleLogNachricht("CR_LOGIN angekommen." );
                this->verarbeiteCR_LOGIN(verbindung,data);
                break;
            }
            case BwsProtokoll::CR_LOGOUT:
            {
                // nicht eingeloggte User können nicht ausloggen
                emit uebermittleLogNachricht("CR_LOGOUT angekommen[user nicht eingeloggt]");
                this->sendeDaten(verbindung, this->protokoll.packSR_UNLI());
                break;
            }
            case BwsProtokoll::CR_CMAIL:
            {
                emit uebermittleLogNachricht("CLOGOUT angekommen[user nicht eingeloggt]");
                this->sendeDaten(verbindung, this->protokoll.packSR_UNLI());
                break;
            }
            case BwsProtokoll::CR_CPASS:
            {
                emit uebermittleLogNachricht("CR_CMAIL angekommen[user nicht eingeloggt]");
                this->sendeDaten(verbindung, this->protokoll.packSR_UNLI());
                break;
            }
            case BwsProtokoll::CR_DACC:
            {
                emit uebermittleLogNachricht("CR_DACC angekommen[user nicht eingeloggt]");
                this->sendeDaten(verbindung, this->protokoll.packSR_UNLI());
                break;
            }
            case BwsProtokoll::CR_UO:
            {
                emit uebermittleLogNachricht("CR_UO angekommen[user nicht eingeloggt]");
                this->sendeDaten(verbindung, this->protokoll.packSR_UNLI());
                break;
            }
            case BwsProtokoll::CR_CG:
            {
                emit uebermittleLogNachricht("CR_CG angekommen[user nicht eingeloggt]");
                this->sendeDaten(verbindung, this->protokoll.packSR_UNLI());
                break;
            }
            case BwsProtokoll::CR_SG:
            {
                emit uebermittleLogNachricht("CR_SG angekommen[user nicht eingeloggt]");
                this->sendeDaten(verbindung, this->protokoll.packSR_UNLI());
                break;
            }
            case BwsProtokoll::CR_SUBT:
            {
                emit uebermittleLogNachricht("CR_SUBT angekommen[user nicht eingeloggt]");
                this->sendeDaten(verbindung, this->protokoll.packSR_UNLI());
                break;
            }
            case BwsProtokoll::CR_HST:
            {
                emit uebermittleLogNachricht("CR_HST angekommen[user nicht eingeloggt]");
                this->sendeDaten(verbindung, this->protokoll.packSR_UNLI());
                break;
            }
            case BwsProtokoll::CR_SELP:
            {
                emit uebermittleLogNachricht("CR_SELP angekommen[user nicht eingeloggt]");
                this->sendeDaten(verbindung, this->protokoll.packSR_UNLI());
                break;
            }
            default:
            {
                // Wenn Paket nicht bekannt, dann INVP, bei allen anderen Anfragen außer REG und LOGIN wird eine UNLI rausgegeben
                this->sendeDaten(verbindung, this->protokoll.packSR_INVP());
                break;
            }
            }// switch
        }
        else // Wenn User eingeloggt
        {
            /****** !!! HIER ALLE WEITEREN ANFRAGEN EINTRAGEN !!! *****/

            switch(packageId)
            {
            case BwsProtokoll::CR_REG:
            {
                this->uebermittleLogNachricht("CR_REG angekommen [Eingeloggte User kännen sich nicht registrieren.]");
                // Sende Invalid Package
                this->sendeDaten(verbindung, this->protokoll.packSR_INVP("Eingeloggte User können sich nicht registrieren."));
                break;
            }
            case BwsProtokoll::CR_LOGIN:
            {
                this->uebermittleLogNachricht("CR_LOGIN angekommen [User bereits eingeloggt.]");
                // Sende Invalid Package
                this->sendeDaten(verbindung, this->protokoll.packSR_INVP("User bereits eingeloggt."));
                break;
            }
            case BwsProtokoll::CR_LOGOUT:
            {
                this->uebermittleLogNachricht("CR_LOGOUT angekommen");
                this->verarbeiteCR_LOGOUT(verbindung);
                break;
            }
            case BwsProtokoll::CR_CMAIL:
            {
                this->uebermittleLogNachricht("CR_CMAIL angekommen");
                this->verarbeiteCR_CMAIL(verbindung, data);
                break;
            }
            case BwsProtokoll::CR_CPASS:
            {
                this->uebermittleLogNachricht("CR_PASS angekommen");
                this->verarbeiteCR_CPASS(verbindung, data);
                break;
            }
            case BwsProtokoll::CR_DACC:
            {
                this->uebermittleLogNachricht("CR_DACC angekommen");
                this->verarbeiteCR_DACC(verbindung, data);
                break;
            }
            case BwsProtokoll::CR_UO:
            {
                this->uebermittleLogNachricht("CR_UO angekommen");
                this->verarbeiteCR_UO(verbindung, data);
                break;
            }
            case BwsProtokoll::CR_CG:
            {
                this->uebermittleLogNachricht("CR_CG angekommen");
                this->verarbeiteCR_CG(verbindung, data);
                break;
            }
            case BwsProtokoll::CR_SG:
            {
                this->uebermittleLogNachricht("CR_SG angekommen");
                this->verarbeiteCR_SG(verbindung, data);
                break;
            }
            case BwsProtokoll::CR_SUBT:
            {
                this->uebermittleLogNachricht("CR_SUBT angekommen");
                this->verarbeiteCR_SUBT(verbindung, data);
                break;
            }
            case BwsProtokoll::CR_HST:
            {
                this->uebermittleLogNachricht("CR_HST angekommen");
                this->verarbeiteCR_HST(verbindung, data);
                break;
            }
            case BwsProtokoll::CR_SELP:
            {
                this->uebermittleLogNachricht("CR_SELP angekommen");
                this->verarbeiteCR_SELP(verbindung, data);
                break;
            }
            default:
            {
                // Sende Invalid Package
                this->sendeDaten(verbindung, this->protokoll.packSR_INVP("Paket ungültig."));
                break;
            }
            }// switch
        }
    }//else
}

/********************************************************************************
 *                                 CLIENT-ANFRAGEN                              *
 ********************************************************************************/

// Vorerst hardcodiert - anschließend mit DBHandler & entsprechenden Datenbankabfragen verbinden

void ServerHandler::verarbeiteCR_REG(Verbindung* verbindung, QString data)
{
    // user registriert sich

    QString username, email, vorname, nachname, klasse, userPass, error;

    if(!this->protokoll.unpackCR_REG(data, &username,&userPass, &email, &vorname, &nachname, &klasse))
        this->sendeDaten(verbindung,this->protokoll.packSR_INVP("CR_REG üngültig."));
    else
    {
        QString out = "Username: " + username + "\nEmail: " + email + "\nVorname: " + vorname + "\nNachname: " + nachname + "\nKlasse: " + klasse;
        emit this->uebermittleLogNachricht(out);

        if(!this->p_accountListe->neuenAccountAnlegen(username,userPass,vorname,nachname,email,klasse,&error)) // dann versuchen anzulegen
            this->sendeDaten(verbindung,this->protokoll.packSR_REG(false,error));
        else
        {
            this->sendeDaten(verbindung, this->protokoll.packSR_REG(true));
            this->uebermittleLogNachricht("Ein neuer Nutzer hat sich registriert! [" + username + "]");
        }
    }

    // Neuen User ins System eintragen mit QString feldern, username, email, vorname, nachname (inkl. prüfung auf gültuge Daten?)


    // ist alles ok: this->protokoll.packSR_REG(true);
    // fehler:       this->protokoll.packSR_REG(false, "Fehlernachricht");
}

void ServerHandler::verarbeiteCR_LOGIN(Verbindung* verbindung, QString data)
{
    // user loggt sich ein
    // DB ABfrage
    QString error = "";
    QString username, passwort;

    EventInfo tmpEventInfo;
    QVector<EventInfo> events; // user fragt alle existierenden events ab
    QString vorname = "", nachname = "", klasse = "", email = "", regSeit = "";
    bool bereitsEingeloggt = false; // wenn user in loggedIn Liste gefudnen wird dieses auf true gesetzt

    if(!this->protokoll.unpackCR_LOGIN(data,&username, &passwort))
        this->sendeDaten(verbindung,this->protokoll.packSR_INVP("CR_LOGIN üngültig."));
    else // paket konnte gelesen werden
    {
        for(int i = 0; i < this->loggedInUser.length();i++) // ermitteln ob user schon eingeloggt
        {
            if(username == this->loggedInUser.at(i))
            {
                bereitsEingeloggt = true;
                break;
            }
        }

        if(bereitsEingeloggt)  // Wenn user eingeloggt
        {
            emit this->uebermittleLogNachricht("User [" + username + "] bereits eingeloggt!");
            this->sendeDaten(verbindung, this->protokoll.packSR_LOGIN(false,events,vorname,nachname,klasse,email,regSeit, "User [" + username + "] bereits eingeloggt!"));
        }
        else
        {
            //  *** AccountLogin Abfrage ***

            if(!this->p_accountListe->loginClient(username, passwort, &error))
                this->sendeDaten(verbindung, this->protokoll.packSR_LOGIN(false,events,vorname,nachname, klasse, email,regSeit, "Login inkorrekt."));
            else
            {   // user konnte sich authetifizieren
                QString error;
                int userIndex;

                // LoginAccount holen
                for(int i = 0; i < this->p_accountListe->getAccounts().length();i++)
                {
                    if(this->p_accountListe->getAccounts().at(i).getBenutzername() == username)
                    {
                        userIndex = i;
                        break;
                    }
                }

                // wenn user nicht freigeschaltet
                if(!this->p_accountListe->getAccounts().at(userIndex).getFreigeschaltet())
                {
                    error = "Nutzer [" + username + "] nicht freigeschaltet.";
                    this->sendeDaten(verbindung, this->protokoll.packSR_LOGIN(false,events,vorname,nachname, klasse, email,regSeit, error));
                }
                else // sonst Alle Daten holen
                {
                    vorname  = this->p_accountListe->getAccounts().at(userIndex).getVorname(); // Account Daten
                    nachname = this->p_accountListe->getAccounts().at(userIndex).getNachname();
                    klasse   = this->p_accountListe->getAccounts().at(userIndex).getKlasse();
                    email    = this->p_accountListe->getAccounts().at(userIndex).getEmail();
                    regSeit  = this->p_accountListe->getAccounts().at(userIndex).getRegDatum();

                    for(int i = 0; i < this->p_eventListe->count();i++) // Event Daten
                    {
                        tmpEventInfo.setEventInfo(this->p_eventListe->holen(i)->getName(), this->p_eventListe->holen(i)->getStart().toString("ddMMyyyy"), this->p_eventListe->holen(i)->getEnde().toString("ddMMyyyy")
                                                  , this->p_eventListe->holen(i)->getSiegesTipp(), this->p_eventListe->holen(i)->getErgebnisTipp());
                        events.push_back(tmpEventInfo);
                    }

                    verbindung->setLoginState(true); // loginStae erst auf false, wenn Login ok auf true setzen
                    verbindung->setActiveUser(username); // usernamen setzen
                    this->loggedInUser.append(username); // User in loggedInUser hinterlegen

                    // Daten senden
                    this->sendeDaten(verbindung, this->protokoll.packSR_LOGIN(verbindung->getLoginState(),events, vorname, nachname, klasse, email, regSeit ,error));
                    this->uebermittleLogNachricht("User [" + this->p_accountListe->getAccounts().at(userIndex).getBenutzername() + "] hat sich eingeloggt.");
                }
            }
        }
    }
}

void ServerHandler::verarbeiteCR_LOGOUT(Verbindung *verbindung)
{
    // ausloggen
    emit this->uebermittleLogNachricht("User [" + verbindung->getActiveUser() + "] hat sich ausgeloggt");

    for(int i = 0; i < this->loggedInUser.length();i++)
    {
        if(verbindung->getActiveUser() == this->loggedInUser.at(i))
        {
            this->loggedInUser.removeAt(i);
            break;
        }
    }

    verbindung->setLoginState(false);
    verbindung->setActiveUser("NONE");

    this->sendeDaten(verbindung, this->protokoll.packSR_LOGOUT(true, "Kein Error")); // kein Error
}

void ServerHandler::verarbeiteCR_CMAIL(Verbindung* verbindung, QString data)
{
    QString userpass, newMail, error = "";

    if(!this->protokoll.unpackCR_CMAIL(data, &userpass, &newMail))
        this->sendeDaten(verbindung,this->protokoll.packSR_INVP("CR_CMAIL üngültig."));
    else
    {
        QString out = "Passwort: " + userpass + "\nMail: " + newMail;
        emit this->uebermittleLogNachricht(out);

        if(!this->p_accountListe->EmailAenderung(verbindung->getActiveUser(), userpass, newMail, &error))
            this->sendeDaten(verbindung, this->protokoll.packSR_CMAIL(false,error));
        else
            this->sendeDaten(verbindung, this->protokoll.packSR_CMAIL(true));

        // Erst Passwort abgleich / wenn korrekt MEail änderung - bei Fehlern enstprechende Error msg in error speichern
    }
}

void ServerHandler::verarbeiteCR_CPASS(Verbindung* verbindung, QString data)
{
    QString userpass, newpass, error = "Kein Error";

    if(!this->protokoll.unpackCR_CPASS(data, &userpass, &newpass))
        this->sendeDaten(verbindung,this->protokoll.packSR_INVP("CR_CPASS üngültig."));
    else
    {
        QString out = "Passwort: " + userpass + "\nNeues Passwort" + newpass;
        emit this->uebermittleLogNachricht(out);

        if(!this->p_accountListe->PasswortAenderung(verbindung->getActiveUser(), userpass, newpass, &error))
            this->sendeDaten(verbindung, this->protokoll.packSR_CPASS(false,error));
        else
            this->sendeDaten(verbindung, this->protokoll.packSR_CPASS(true));

        // Erst passwort abgleich, wenn userpass korrekt dann newpass übernehmen
        // this->sendeDaten(verbindung, this->protokoll.packSR_CPASS(true/false, error)); // wie bei Change mail, entrpechende fehlermeldung in error speichern
    }
}

void ServerHandler::verarbeiteCR_DACC(Verbindung* verbindung, QString data)
{
    QString userpass, error = "Kein Error";

    if(!this->protokoll.unpackCR_DACC(data,&userpass))
        this->sendeDaten(verbindung,this->protokoll.packSR_INVP("CR_DACC ungültig."));
    else
    {
        QString out = "Passwort: " + userpass;

        emit this->uebermittleLogNachricht(out);
        // Passwort abfrage, entsprechend errormsg in error speichern

        if(!this->p_accountListe->ClientAccountLoeschen(verbindung->getActiveUser(), userpass,&error))
            this->sendeDaten(verbindung, this->protokoll.packSR_DACC(false,error));
        else
            this->sendeDaten(verbindung,this->protokoll.packSR_DACC(true));
    }
}

void ServerHandler::verarbeiteCR_UO(Verbindung* verbindung, QString data)
{
    QString eventName, error = "Kein Fehler";

    if(!this->protokoll.unpackCR_UO(data, &eventName))
        this->sendeDaten(verbindung, this->protokoll.packSR_INVP("CR_UO ungültig."));
    else
    {
        QVector<TipInfo> tipInfos;
        TipInfo tmpTipInfo;

        QVector<Wette*> userWetten  = this->p_eventListe->getSpielerWetten(verbindung->getActiveUser(), eventName);

        for(int i = 0; i < userWetten.length(); i++)
        {
            int tipEins = userWetten.at(i)->getTipp(verbindung->getActiveUser())->getSiegesTipp();
            int tipZweiA = userWetten.at(i)->getTipp(verbindung->getActiveUser())->getErgebnisTippM1();
            int tipZweiB = userWetten.at(i)->getTipp(verbindung->getActiveUser())->getErgebnisTippM2();

            tmpTipInfo.setTipInfo(userWetten.at(i)->getManschaftM1(), userWetten.at(i)->getManschaftM2(),
                                  userWetten.at(i)->getStart().toString("dd.MM.yyyy") + " - " + userWetten.at(i)->getEnde().toString("dd.MM.yyyy"), userWetten.at(i)->getStatusTippbar()
                                  , tipEins,tipZweiA,tipZweiB);
            tipInfos.push_back(tmpTipInfo);
        }

        QString paket = this->protokoll.packSR_UO(tipInfos, error);
        this->sendeDaten(verbindung, paket );
    }
}

void ServerHandler::verarbeiteCR_CG(Verbindung* verbindung, QString data)
{
    // Liste alle Spiele eines Events ausgeben (spielinfo - eventname, Spielname)

    QVector<SpielInfo> spiele;
    SpielInfo tmpSpielInfo;
    QString event, error = "Kein Fehler.";

    if(!this->protokoll.unpackCR_CG(data, &event))
        this->sendeDaten(verbindung, this->protokoll.packSR_INVP("CR_CG ungültig."));
    else
    {

        QVector<Wette*> eventWetten = this->p_eventListe->holen(event)->getWettenListe()->getWetten();

        for(int i = 0; i < eventWetten.length();i++)
        {
            tmpSpielInfo.setSpielInfo(eventWetten.at(i)->getSpielID(), eventWetten.at(i)->getManschaftM1(), eventWetten.at(i)->getManschaftM2()
                                      , eventWetten.at(i)->getStart().toString("dd.MM.yyyy") + " - " +  eventWetten.at(i)->getEnde().toString("dd.MM.yyyy"), eventWetten.at(i)->getStatusTippbar());
            spiele.push_back(tmpSpielInfo);
        }

        QString paket = this->protokoll.packSR_CG(spiele, error);

        this->sendeDaten(verbindung, paket );
    }
}

void ServerHandler::verarbeiteCR_SG(Verbindung* verbindung, QString data)
{
    int spielId;

    if(!this->protokoll.unpackCR_SG(data, &spielId))
            this->sendeDaten(verbindung, this->protokoll.packSR_INVP("CR_SG ungültig."));
    else
    {
        Wette* selWette = this->p_eventListe->getWette(spielId);

        int aufSiegGetippt = -1;
        int aufErgGetipptA = -1;
        int aufErgGetipptB = -1;

        if(selWette->hatgeTippt(verbindung->getActiveUser())) // hat user auf Wette getippt?
        {
            aufSiegGetippt = selWette->getTipp(verbindung->getActiveUser())->getSiegesTipp();
            aufErgGetipptA = selWette->getTipp(verbindung->getActiveUser())->getErgebnisTippM1();
            aufErgGetipptB = selWette->getTipp(verbindung->getActiveUser())->getErgebnisTippM2();
        }

        this->sendeDaten(verbindung, this->protokoll.packSR_SG(selWette->getStart().toString("dd.MM.yyyy hh:mm"),selWette->getEnde().toString("dd.MM.yyyy hh:mm")
                                                               ,selWette->getPunkteM1(),selWette->getPunkteM2(),aufSiegGetippt,aufErgGetipptA,aufErgGetipptB)); // Tips müssen standrdwerte haben, da nur eine variante getippt werden kann
    }

    // ausgewähltes Spiel - nähere Informationsfelder (bei soppelklick auf Spiel)
}

void ServerHandler::verarbeiteCR_SUBT(Verbindung* verbindung, QString data)
{
    // User gibt Tip ab

    int spielId, tipEins, tipZweiA, tipZweiB;

    if(!this->protokoll.unpackCR_SUBT(data,&spielId, &tipEins, &tipZweiA, &tipZweiB))
        this->sendeDaten(verbindung, this->protokoll.packSR_INVP("CR_SUBT ungültig."));
    else
    {
        Account* acc = NULL;

        for(int i = 0; i < this->p_accountListe->getAccounts().length(); i++)
        {
            if(this->p_accountListe->getAccounts().at(i).getBenutzername() == verbindung->getActiveUser())
                acc = this->p_accountListe->getAccountP(i);
        }

        if(acc == NULL)
            this->sendeDaten(verbindung, this->protokoll.packSR_SUBT(false, "Nutzer nicht gefunden!"));
        else
        {
            if(!this->p_eventListe->holen(this->p_eventListe->getEventNameOf(spielId))->addTipp(spielId, acc, tipEins,tipZweiA,tipZweiB))
                this->sendeDaten(verbindung, this->protokoll.packSR_SUBT(false,"Tipp konnte nicht abgegeben werden."));
            else
                this->sendeDaten(verbindung, this->protokoll.packSR_SUBT(true));
        }
    }
}

void ServerHandler::verarbeiteCR_HST(Verbindung* verbindung, QString data)
{
    // Bestenliste ausgeben, Namen und Punkte in absteigender Reihenfolge

    QVector<SpielerInfo> spieler;
    QString eventName, error = "Kein Error";

    if(!this->protokoll.unpackCR_HST(data,&eventName))
        this->sendeDaten(verbindung, this->protokoll.packSR_INVP("CR_HST ungültig."));
    else
    {
        QVector<EventSpieler*> eventSpieler = this->p_eventListe->holen(eventName)->getBestenliste();
        SpielerInfo tmpSpielerInfo;

        for(int i = 0; i < eventSpieler.length(); i++)
        {
            tmpSpielerInfo.setSpielerInfo(eventSpieler.at(i)->getSpieler()->getBenutzername(), eventSpieler.at(i)->getPunkte());
            spieler.push_back(tmpSpielerInfo);
        }

        this->sendeDaten(verbindung, this->protokoll.packSR_HST(spieler, error));
    }
}

void ServerHandler::verarbeiteCR_SELP(Verbindung* verbindung, QString data)
{
    // anhanf von Spielernamen, näher Spielerinfos ausgeben
    QString spielerName, error;

    if(!this->protokoll.unpackCR_SELP(data, &spielerName))
        this->sendeDaten(verbindung, this->protokoll.packSR_INVP("CR_SELP ungültig."));
    else
    {
        Account tmp = this->p_accountListe->SpielerInfo(spielerName, &error);
        this->sendeDaten(verbindung, this->protokoll.packSR_SELP(tmp.getVorname(), tmp.getNachname(), tmp.getKlasse(),tmp.getRegDatum(),error));
    }
}
