#include "clienthandler.h"

ClientHandler::ClientHandler()
{

}

void ClientHandler::setPort(int port)
{
    this->port = port;
}

void ClientHandler::setHost(QString host)
{
    this->host = host;
}

void ClientHandler::setTryMail(QString tryMail)
{
    this->tryMail = tryMail;
}

QString ClientHandler::getTryMail() const
{
    return this->tryMail;
}

ClientHandler::~ClientHandler()
{
    this->client->close();
    delete client;
}

bool ClientHandler::doConnect()
{   
    this->client = new QTcpSocket(this);

    QObject::connect(client, SIGNAL(connected())   , this, SLOT(verbindungHergestellt())   );
    QObject::connect(client, SIGNAL(disconnected()), this, SLOT(verbidnungGetrennt())      );
    QObject::connect(client, SIGNAL(readyRead())   , this, SLOT(verarbeiteServerAntwort()) );

    bool connected = false;
    this->client->connectToHost(this->host,this->port);

    if(!client->waitForConnected(5000))
    {
       this->lastError = this->client->errorString();
    }
    else
        connected = true;

    return connected;
}

void ClientHandler::verbindungHergestellt()
{
    this->uebermittleLogNachricht("Verbunden!");
}

void ClientHandler::verbidnungGetrennt()
{
    this->client->close();

    emit this->verbindungAbgebrochen();
    this->uebermittleLogNachricht("Verbindung zum Server wurde getrennt.");
}

void ClientHandler::verarbeiteServerAntwort() // Einegehende Serverantwort wird hier verarbeitet
{
    QString data = this->client->readAll();

    if(!this->protokoll.checkValid(&data))
    {
        // Fehlernachricht
        this->uebermittleLogNachricht("Fehler: ServerAntwort ungültig.");
    }
    else
    {
        // Ermittle PackageID
        int packageId = this->protokoll.getServerAntwortPackageId(&data);

        switch(packageId)
        {
        case BwsProtokoll::SR_REG:
        {
            this->uebermittleLogNachricht("SR_REG angekommen.");
            this->verarbeiteSR_REG(data);
            break;
        }
        case BwsProtokoll::SR_LOGIN:
        {
            this->uebermittleLogNachricht("SR_LOGIN angekommen.");
            this->verarbeiteSR_LOGIN(data);
            break;
        }
        case BwsProtokoll::SR_LOGOUT:
        {
            this->uebermittleLogNachricht("SR_LOGOUT angekommen.");
            this->verarbeiteSR_LOGOUT(data);
            break;
        }
        case BwsProtokoll::SR_CMAIL:
        {
            this->uebermittleLogNachricht("SR_CMAIL angekommen.");
            this->verarbeiteSR_CMAIL(data);
            break;
        }
        case BwsProtokoll::SR_CPASS:
        {
            this->uebermittleLogNachricht("SR_CPASS angekommen.");
            this->verarbeiteSR_CPASS(data);
            break;
        }
        case BwsProtokoll::SR_DACC:
        {
            this->uebermittleLogNachricht("SR_DACC angekommen.");
            this->verarbeiteSR_DACC(data);
            break;
        }
        case BwsProtokoll::SR_UO:
        {
            this->uebermittleLogNachricht("SR_UO angekommen.");
            this->verarbeiteSR_UO(data);
            break;
        }
        case BwsProtokoll::SR_CG:
        {
            this->uebermittleLogNachricht("SR_CG angekommen.");
            this->verarbeiteSR_CG(data);
            break;
        }
        case BwsProtokoll::SR_SG:
        {
            this->uebermittleLogNachricht("SR_SG angekommen.");
            this->verarbeiteSR_SG(data);
            break;
        }
        case BwsProtokoll::SR_SUBT:
        {
            this->uebermittleLogNachricht("SR_SUBT angekommen.");
            this->verarbeiteSR_SUBT(data);
            break;
        }
        case BwsProtokoll::SR_HST:
        {
            this->uebermittleLogNachricht("SR_HST angekommen.");
            this->verarbeiteSR_HST(data);
            break;
        }
        case BwsProtokoll::SR_SELP:
        {
            this->uebermittleLogNachricht("SR_SELP angekommen.");
            this->verarbeiteSR_SELP(data);
            break;
        }
        case BwsProtokoll::SR_UNLI:
        {
            this->uebermittleLogNachricht("SR_UNLI angekommen.");
            this->verarbeiteSR_UNLI(data);
            break;
        }
        case BwsProtokoll::SR_INVP:
        {
            this->uebermittleLogNachricht("SR_INVP angekommen.");
            this->verarbeiteSR_INVP(data);
            break;
        }
        } // switch
    } // else
}// funktion

void ClientHandler::sendData(QString data)
{
    this->client->write(data.toUtf8());
}

QString ClientHandler::getLastError()
{
    return this->lastError;
}

/********************************************************************************
 *                           VERABREITUNGS-METHODEN                             *
 ********************************************************************************/

void ClientHandler::verarbeiteSR_REG(QString data)
{
    QString error;
    bool regState;

    this->protokoll.unpackSR_REG(data, &error, &regState);

    if(!regState)
        QMessageBox::information(this, "Fehler!", error);
    else
    {
        QMessageBox::information(this, "Erfolg!", "Sie haben sich erfolgreich registriert!");
        emit this->registerSucced();
    }

    QString out = "RegState: " + QString::number(regState) + "\nError: " + error;
    emit uebermittleLogNachricht(out);
}

void ClientHandler::verarbeiteSR_LOGIN(QString data)
{
    // Daten die Server übermittelt
    bool loginState;
    QString vorname,nachname,klasse, email,regSeit;
    QVector<EventInfo> events;
    QString error;

    // Entpacken
    if(!this->protokoll.unpackSR_LOGIN(data,&error,&loginState,&events,&vorname,&nachname,&klasse,&email,&regSeit))
        emit this->uebermittleLogNachricht("SR_LOGIN ungültig.");
    else
    {
        if(loginState)
        {
            this->activeUser.setActive(email,vorname,nachname,klasse,regSeit);
            QMessageBox::information(this,"Login Erfolgreich!", "Sie haben sich erfolgreich eingeloggt.");
            emit this->login(events);
        }
        else
        {
            this->activeUser.setInactive();
            QMessageBox::information(this,"Login nicht möglich!", error);
        }
    }
}

void ClientHandler::verarbeiteSR_LOGOUT(QString data)
{
    QString error;
    bool logoutState;

    if(!this->protokoll.unpackSR_LOGOUT(data,&error,&logoutState))
        QMessageBox::critical(this,"Fehler!", "SR_LOGOUT ungültig.");
    else
        QMessageBox::information(this,"Erfolg!", "Sie haben sich erfolgreich ausgeloggt.");

    emit this->logout();
}

void ClientHandler::verarbeiteSR_CMAIL(QString data)
{
    QString error, out;
    bool mailChanged;

    if(!this->protokoll.unpackSR_CMAIL(data,&error,&mailChanged))
        QMessageBox::critical(this, "Fehler!", "SR_CMAIL ungültig.");
    else
    {
        out = "Mailchanged: " + QString::number(mailChanged) + "\nError: " + error;
        emit this->uebermittleLogNachricht(out);

        if(!mailChanged)
        {
            this->tryMail.clear();
            QMessageBox::information(this,"Fehler!", "Ihre Email konnte nicht geändert werden:\n" + error );
        }
        else
        {
            QMessageBox::information(this,"Erfolg!", "Ihre Email wurde erfolgreich geändert.");

            emit this->displayNewMail(this->tryMail);
        }
    }
}

void ClientHandler::verarbeiteSR_CPASS(QString data)
{
    QString error, out;
    bool passChanged;

    if(!this->protokoll.unpackSR_CPASS(data,&error,&passChanged))
        QMessageBox::critical(this, "Fehler!", "SR_CPASS ungültig.");
    else
    {
        out = "PassChanged: " + QString::number(passChanged) + "\nError: " + error;
        emit this->uebermittleLogNachricht(out);

        if(!passChanged)
            QMessageBox::information(this,"Fehler", "Passwort konnte nicht geändert werden:\n" + error);
        else
            QMessageBox::information(this,"Erfolg!", "Passwort wurde geändert.");
    }
}

void ClientHandler::verarbeiteSR_DACC(QString data)
{
    QString error, out;
    bool accountDel;

    if(!this->protokoll.unpackSR_DACC(data,&error,&accountDel))
        QMessageBox::critical(this, "Fehler!", "SR_CPASS ungültig.");
    else
    {
        out = "AccountDEL: " + QString::number(accountDel) + "\nError: " + error;
        emit this->uebermittleLogNachricht(out);

        if(!accountDel)
            QMessageBox::information(this,"Information", "Account konnte nicht gelöscht werden:\n" + error);
        else
        {
            QMessageBox::information(this,"Erfolg!", "Account wurde gelöscht.");
            this->sendData(this->protokoll.packCR_LOGOUT());
        }
    }
}

void ClientHandler::verarbeiteSR_UO(QString data)
{
    QString error;
    QVector<TipInfo> tips;

    if(!this->protokoll.unpackSR_UO(data, &error, &tips))
        error = "SR_UO ungültig.";
    else
        emit this->uebergebeMeineTipps(tips);
}

void ClientHandler::verarbeiteSR_CG(QString data)
{
    QString error;
    QVector<SpielInfo> aktSpiele;

    if(!this->protokoll.unpackSR_CG(data, &error, &aktSpiele))
        QMessageBox::critical(this,"Fehler!", "SR_CG ungültig. | " + error);
    else
        emit this->uebergebeAktuelleWetten(aktSpiele);
}

void ClientHandler::verarbeiteSR_SG(QString data)
{
    QString error, out;

    QString start, ende;
    int punkteA, punkteB, tipEins, tipZweiA, tipZweiB ;

    // !!! Datum wurde schon über SpielInfo uebergeben, ggf abändern //

    if(!this->protokoll.unpackSR_SG(data,&error,&start, &ende,&punkteA,&punkteB,&tipEins, &tipZweiA, &tipZweiB))
        out = "SR_DACC ungültig.";
    else
        emit this->uebergebeAusgWette(start,ende, punkteA, punkteB, tipEins, tipZweiA, tipZweiB);

    this->uebermittleLogNachricht(out);
}

void ClientHandler::verarbeiteSR_SUBT(QString data)
{
    QString error, out;
    bool submitOk;

    if(!this->protokoll.unpackSR_SUBT(data, &error,&submitOk))
        QMessageBox::critical(this, "Fehler!", "SR_SUBT ungültig.");
    else
    {
        emit this->deaktiviereTippBereich();
        QMessageBox::information(this,"Erfolg!", "Tip wurde eingetragen.");
    }

    emit this->uebermittleLogNachricht(out);
}

void ClientHandler::verarbeiteSR_HST(QString data)
{
    QString error;
    QVector<SpielerInfo> spieler;

    if(!this->protokoll.unpackSR_HST(data,&error,&spieler))
        QMessageBox::critical(this,"Fehler!", "SR_HST ungültig. - " + error);
    else
        this->uebergebeBestenliste(spieler);
}

void ClientHandler::verarbeiteSR_SELP(QString data)
{
    QString out,error, vorname, nachname, klasse, regSeit;

    if(!this->protokoll.unpackSR_SELP(data, &error, &vorname, &nachname, &regSeit, &klasse))
        out = "SR_SELP ungültig.";
    else
        emit this->uebergebeAusgSpieler(vorname,nachname,klasse,regSeit);
}

void ClientHandler::verarbeiteSR_UNLI(QString data)
{
    QString error, out;

    if(!this->protokoll.unpackSR_UNLI(data,&error))
        out = "SR_UNLI ungültig.";
    else
        emit this->uebermittleLogNachricht("UserNotLoggedIn: " + error);
}

void ClientHandler::verarbeiteSR_INVP(QString data)
{
    QString error, out;

    if(!this->protokoll.unpackSR_INVP(data,&error))
        out = "SR_INVP ungültig.";
    else
        emit this->uebermittleLogNachricht("Invalid Package: " + error);
}
