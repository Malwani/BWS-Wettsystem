/*
  Modul: DbHandler.cpp
  Funktion: Verwaltung der Datenbankzugriffe
  Autor: Aymane Amaioua, Christoph Rehm, benjamin Bardroff
  Datum: 01.04.2016
  */

#include "dbhandler.h"

/********************************************************************************
 *                              INIT-METHODEN                                   *
 ********************************************************************************/

DBHandler::DBHandler()
{
    createDB();
}

bool DBHandler::createDB()
{
    if(!DBExists())
    {
        QString adminPass = "adminPass123";
        QString hashedAdminPass(QCryptographicHash::hash(adminPass.toUtf8(),QCryptographicHash::Md5).toHex());

        QString path = "db.sqlite";
        QString tblManschaft = "CREATE TABLE 'tbl_Mannschaft' ('ID_Mannschaft' INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL  UNIQUE , 'Name' CHAR NOT NULL  UNIQUE )";
        QString tblSpiel = "CREATE TABLE 'tbl_Spiel' ('ID_Spiel' INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL  UNIQUE ,'ID_Event' INTEGER NOT NULL, 'ID_Mannschaft1' INTEGER NOT NULL , 'ID_Mannschaft2' INTEGER NOT NULL , 'PunkteMannschaft1' INTEGER, 'PunkteMannschaft2' INTEGER, 'Beginn' DATETIME NOT NULL , 'Ende' DATETIME NOT NULL )";
        QString tblEvent = "CREATE TABLE 'tbl_Event' ('ID_Event' INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL  UNIQUE , 'Name' CHAR NOT NULL  UNIQUE , 'Beginn' DATETIME NOT NULL , 'Ende' DATETIME NOT NULL, 'SiegestippPunkte' INTEGER NOT NULL , 'ErgebnistippPunkte' INTEGER NOT NULL  )";
        QString tblUser = "CREATE TABLE 'tbl_User' ('ID_User' INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL  UNIQUE , 'Benutzername' CHAR NOT NULL  UNIQUE , 'Passwort' CHAR NOT NULL DEFAULT abcde12345, 'Email' CHAR NOT NULL  UNIQUE , 'Vorname' CHAR NOT NULL , 'Nachname' CHAR NOT NULL , 'Klasse' CHAR NOT NULL , 'registriert_seit' DATETIME NOT NULL  DEFAULT CURRENT_DATE , 'freigeschaltet' BOOL NOT NULL DEFAULT false )";
        QString tblAdmin = "CREATE TABLE 'tbl_Admin' ('ID_Admin' INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL  UNIQUE , 'Benutzername' CHAR NOT NULL  UNIQUE , 'Passwort' CHAR NOT NULL)";
        QString tblTipptAuf = "CREATE TABLE 'tbl_TipptAuf' ('ID_User' INTEGER NOT NULL , 'ID_Spiel' INTEGER NOT NULL , 'Siegestipp' INTEGER, 'ErgebnistippM1' INTEGER, 'ErgebnistippM2' INTEGER)";

        QString stdAdmin = "INSERT INTO tbl_Admin (Benutzername, Passwort) VALUES('Admin', '"+hashedAdminPass+"')";
        this->wettenDB = QSqlDatabase::addDatabase("QSQLITE");
        this->wettenDB.setDatabaseName(path);
        this->wettenDB.open();

        QSqlQuery query;

        query.exec(tblManschaft);
        query.exec(tblSpiel);
        query.exec(tblEvent);
        query.exec(tblUser);
        query.exec(tblAdmin);
        query.exec(tblTipptAuf);
        query.exec(stdAdmin);
        wettenDB.close();

        return true;
    }
    if(DBExists())
    {
        this->wettenDB = QSqlDatabase::addDatabase("QSQLITE");
        this->wettenDB.setDatabaseName("db.sqlite");
        this->wettenDB.open();

        return false;
    }
}

bool DBHandler::DBExists()
{
    bool dbExists = false;

    QFile db;
    db.setFileName("db.sqlite");
    if(db.exists())
    {
        dbExists = true;
    }

    return dbExists;
}

bool DBHandler::openDB()
{
    bool isOpen = false;
    wettenDB.open();

    if(this->wettenDB.isOpen())
        isOpen = true;

    return isOpen;
}

bool DBHandler::closeDB()
{
    wettenDB.close();
    wettenDB = QSqlDatabase();
    wettenDB.removeDatabase(QSqlDatabase::defaultConnection);

    return true;
}

/********************************************************************************
 *                      SERVERINTERNE DATENBANK-ABFRAGEN                        *
 ********************************************************************************/

// !!! Alle Datenbankabfragen-Methoden sind gleich aufgebaut. Der Rückgabetyp ist immer bool und gibt an ob etwas schief gelaufen ist oder nicht. !!!
// !!! Als Parameter werden auch immer nur die selbigen Parameter aufgenommen !!!
// !!! Dabei handelt es sich um 2 Pointer:
// !!!
// !!! - qry ~ Hierbei handelt es sich einfach um die qry, welche das SQLStatement erzeugt hat, dieses wird viva Pointer übergeben und eine Ebene höher (also z.B in der Klasse AccountListe oder FrmMain) verarbeitet.
// !!! - error ~ Über den QString-Pointer wird eine mögliche Fehlermeldung übergeben, welche ebenfalls eine Ebene höher, falls der Rückgabewert FALSE ist ausgegeben wird.

// Rückgabewert gibt an ob ein Fehler entstanden ist. Ergebnis-Query wird über pointer übergeben. Entsprechende Fehlernachricht wird ebenfalls über den QString-Pointer übergeben.
bool DBHandler::aktualisiereAccountListe(QSqlQuery* p_qry, QString* error)
{
    bool ok = false;

    if(!this->openDB())
        *error = "Datenbank konnte nicht geöffnet werden.";
    else
    {


        QSqlQuery qry(this->wettenDB);
        qry.prepare("SELECT Benutzername, Email, Vorname, Nachname, Klasse, freigeschaltet, passwort, registriert_seit FROM tbl_User");

        if(!qry.exec())
            *error = "Datenbankabfrage konnte nicht ausgeführt werden.";
        else
        {
            *p_qry = qry;
            ok = true;
        }
    }

    return ok;
}

bool DBHandler::Freischalten(QString benutzername, QString* error)//Autor: Aymane Amaioua
{
    bool ok = false;

    if(!this->openDB())
        *error = "Datenbank konnte nicht geöffnet werden.";
    else
    {
        QSqlQuery qry(this->wettenDB);
        qry.prepare("UPDATE tbl_User SET freigeschaltet= 'true' WHERE Benutzername= '"+benutzername+"'");
        //Setzt den jeweiligen User im Bereich Freigeschaltet auf true


        if(!qry.exec())
            *error = "Datenbankabfrage konnte nicht ausgeführt werden.";
        else
            ok = true;
    }

    return ok;

}

bool DBHandler::Sperren(QString benutzername, QString* error)//Autor: Aymane Amaioua
{
    bool ok = false;

    if(!this->openDB())
        *error = "Datenbank konnte nicht geöffnet werden.";
    else
    {
        QSqlQuery qry(this->wettenDB);
        qry.prepare("UPDATE tbl_User SET freigeschaltet= 'false' WHERE Benutzername= '"+benutzername+"'");
        //Setzt den jeweiligen User im Bereich Freigeschaltet auf false

        if(!qry.exec())
            *error = "Datenbankabfrage konnte nicht ausgeführt werden.";
        else
            ok = true;
    }

    return ok;
}

bool DBHandler::events(QSqlQuery *p_qry, QString *error) // Autoren: Christoph Rehm
{
    bool ok = false;

    if(!this->openDB())
        *error = "Datenbank konnte nicht geöffnet werden.";
    else
    {
        QSqlQuery qry(this->wettenDB);
        qry.prepare("SELECT Name, Beginn, Ende, SiegestippPunkte, ErgebnistippPunkte FROM tbl_Event ORDER BY UPPER(Name)");

        if(!qry.exec())
            *error = "Datenbankabfrage konnte nicht ausgeführt werden.";
        else
        {
            *p_qry = qry;
            ok = true;
        }
    }

    return ok;
}

bool DBHandler::eintragenEvent(QString name, QDateTime beginn, QDateTime ende, int siegesTipp, int ergebnisTipp, QString *error) //Autor: Christoph Rehm
{
    bool ok = false;

    if(!this->openDB())
            *error = "Datenbank konnte nicht geöffnet werden.";
        else
    {
        QSqlQuery qry(this->wettenDB);
        qry.prepare("INSERT INTO tbl_Event (Name, Beginn, Ende, SiegestippPunkte, ErgebnistippPunkte) VALUES ('" + name + "', '" + beginn.toString("yyyy-MM-dd hh:mm:ss") + "', '" + ende.toString("yyyy-MM-dd hh:mm:ss") + "', " + QString::number(siegesTipp) + ", " + QString::number(ergebnisTipp) + ")");

        if(!qry.exec())
            *error = "Datenbankabfrage konnte nicht ausgeführt werden.";
        else
            ok = true;
    }

    return ok;
}

bool DBHandler::updateEvent(QString nameAlt, QString nameNeu, QDateTime beginn, QDateTime ende, int siegesTipp, int ergebnisTipp, QString *error) //Autor: Christoph Rehm
{
    bool ok = false;

    if(!this->openDB())
            *error = "Datenbank konnte nicht geöffnet werden.";
        else
        {
        QSqlQuery qry(this->wettenDB);
        qry.prepare("UPDATE tbl_Event SET Name = '" + nameNeu + "', Beginn = '" + beginn.toString("yyyy-MM-dd hh:mm:ss") + "', Ende = '" + ende.toString("yyyy-MM-dd hh:mm:ss") + "', SiegestippPunkte = " + QString::number(siegesTipp) + ",ErgebnistippPunkte = "+ QString::number(ergebnisTipp) +"  WHERE Name = '" + nameAlt + "'");

        if(!qry.exec())
            *error = "Datenbankabfrage konnte nicht ausgeführt werden.";
        else
            ok = true;
        }

    return ok;
}

bool DBHandler::loeschenEvent(QString eventName, QString *error) //Autor: Christoph Rehm
{
    bool ok = false;

    if(!this->openDB())
            *error = "Datenbank konnte nicht geöffnet werden.";
        else
        {
        QSqlQuery qry(this->wettenDB);
        qry.prepare("DELETE FROM tbl_Event WHERE Name = '" + eventName + "'");

        if(!qry.exec())
            *error = "Datenbankabfrage konnte nicht ausgeführt werden.";
        else
            ok = true;
        }

    return ok;
}

bool DBHandler::mannschaften(QSqlQuery *p_qry, QString *error) // Autoren: Christoph Rehm
{
    bool ok = false;

    if(!this->openDB())
        *error = "Datenbank konnte nicht geöffnet werden.";
    else
    {
        QSqlQuery qry(this->wettenDB);
        qry.prepare("SELECT Name FROM tbl_Mannschaft ORDER BY UPPER(Name)");

        if(!qry.exec())
            *error = "Datenbankabfrage konnte nicht ausgeführt werden.";
        else
        {
            *p_qry = qry;
            ok = true;
        }
    }

    return ok;
}

bool DBHandler::eintragenMannschaft(QString mannschaftsName, QString *error) //Autor: Christoph Rehm
{
    bool ok = false;

    if(!this->openDB())
            *error = "Datenbank konnte nicht geöffnet werden.";
        else
    {
        QSqlQuery qry(this->wettenDB);
        qry.prepare("INSERT INTO tbl_Mannschaft (Name) VALUES ('" + mannschaftsName + "')");

        if(!qry.exec())
            *error = "Datenbankabfrage konnte nicht ausgeführt werden.";
        else
            ok = true;
    }

    return ok;
}

bool DBHandler::loeschenMannschaft(QString mannschaftsName, QString *error) //Autor: Christoph Rehm
{
    bool ok = false;

    if(!this->openDB())
            *error = "Datenbank konnte nicht geöffnet werden.";
        else
        {
        QSqlQuery qry(this->wettenDB);
        qry.prepare("DELETE FROM tbl_Mannschaft WHERE Name = '" + mannschaftsName + "'");

        if(!qry.exec())
            *error = "Datenbankabfrage konnte nicht ausgeführt werden.";
        else
            ok = true;
        }

    return ok;
}

bool DBHandler::updateMannschaft(QString mannschaftsNameAlt, QString mannschaftsNameNeu, QString *error) //Autor: Christoph Rehm
{
    bool ok = false;

    if(!this->openDB())
            *error = "Datenbank konnte nicht geöffnet werden.";
        else
        {
        QSqlQuery qry(this->wettenDB);
        qry.prepare("UPDATE tbl_Mannschaft SET Name = '" + mannschaftsNameNeu + "' WHERE Name = '" + mannschaftsNameAlt + "'");

        if(!qry.exec())
            *error = "Datenbankabfrage konnte nicht ausgeführt werden.";
        else
            ok = true;
        }

    return ok;
}

bool DBHandler::getEventSpieler(QSqlQuery *p_qry, QString *error, QString eventName) //Autor: Christoph Rehm
{
    bool ok = false;

    if(!this->openDB())
        *error = "Datenbank konnte nicht geöffnet werden.";
    else
    {
        QSqlQuery qry(this->wettenDB);
        qry.prepare("SELECT tbl_User.Benutzername, tbl_TipptAuf.Siegestipp, tbl_TipptAuf.ErgebnistippM1,tbl_TipptAuf.ErgebnistippM2, tbl_Spiel.PunkteMannschaft1, tbl_Spiel.PunkteMannschaft2 FROM tbl_User INNER JOIN tbl_TipptAuf ON tbl_User.ID_User = tbl_TipptAuf.ID_User INNER JOIN tbl_Spiel ON tbl_TipptAuf.ID_Spiel = tbl_Spiel.ID_Spiel INNER JOIN tbl_Event ON tbl_Spiel.ID_Event = tbl_Event.ID_Event WHERE tbl_Event.Name = '" + eventName + "' ");

        if(!qry.exec())
            *error = "Datenbankabfrage konnte nicht ausgeführt werden.";
        else
        {
            *p_qry = qry;
            ok = true;
        }
    }

    return ok;
}

bool DBHandler::getWetten(QSqlQuery *p_qry, QString *error, QString eventName) //Autor: Christoph Rehm
{
    bool ok = false;

    if(!this->openDB())
        *error = "Datenbank konnte nicht geöffnet werden.";
    else
    {
        QSqlQuery qry(this->wettenDB);
        QString sQry = "SELECT tbl_Spiel.ID_Spiel, (SELECT tbl_Mannschaft.Name FROM tbl_Mannschaft WHERE tbl_Spiel.ID_Mannschaft1 = tbl_Mannschaft.ID_Mannschaft) AS M1,(SELECT tbl_Mannschaft.Name FROM tbl_Mannschaft WHERE tbl_Spiel.ID_Mannschaft2 = tbl_Mannschaft.ID_Mannschaft) AS M2, tbl_Spiel.PunkteMannschaft1, tbl_Spiel.PunkteMannschaft2, tbl_Spiel.Beginn, tbl_Spiel.Ende";
                sQry += " FROM tbl_Spiel";
                sQry += " INNER JOIN tbl_Event";
                sQry += " ON tbl_Spiel.ID_Event = tbl_Event.ID_Event";
                sQry += " WHERE tbl_Event.Name = '"+eventName+"'";
        qry.prepare(sQry);

        if(!qry.exec())
            *error = "Datenbankabfrage konnte nicht ausgeführt werden.";
        else
        {
            *p_qry = qry;
            ok = true;
        }
    }

    return ok;
}

bool DBHandler::eintragenWette(QString *error,QString eventName, QString m1, QString m2, QDateTime start, QDateTime ende, int punkteM1, int punkteM2) //Autor: Christoph Rehm
{
    bool ok = false;

    if(!this->openDB())
            *error = "Datenbank konnte nicht geöffnet werden.";
        else
    {
        QSqlQuery qry(this->wettenDB);
        QString sQry = "INSERT INTO tbl_Spiel (ID_Event, ID_Mannschaft1,ID_Mannschaft2,PunkteMannschaft1,PunkteMannschaft2,Beginn, Ende)";
                sQry += " VALUES ((SELECT ID_Event FROM tbl_Event WHERE Name =  '"+eventName+"'),";
                sQry += " (SELECT ID_Mannschaft FROM tbl_Mannschaft WHERE Name = '"+m1+"'),";
                sQry += " (SELECT ID_Mannschaft FROM tbl_Mannschaft WHERE Name = '"+m2+"'),";
                sQry += " "+QString::number(punkteM1)+","+QString::number(punkteM2)+",'"+start.toString("yyyy-MM-dd hh:mm:ss")+"','"+ende.toString("yyyy-MM-dd hh:mm:ss")+"')";

        qry.prepare(sQry);

        if(!qry.exec())
            *error = "Datenbankabfrage konnte nicht ausgeführt werden.";
        else
            ok = true;
    }

    return ok;
}

bool DBHandler::getSpielID(QString *error,QSqlQuery* p_qry, QString eventName, QString m1, QString m2, QDateTime start, QDateTime ende) //Autor: Christoph Rehm
{
    bool ok = false;

    if(!this->openDB())
        *error = "Datenbank konnte nicht geöffnet werden.";
    else
    {
        QSqlQuery qry(this->wettenDB);
        QString sQry = "SELECT tbl_Spiel.ID_Spiel";
                sQry += " FROM tbl_Spiel";
                sQry += " WHERE ID_Event = (SELECT ID_Event FROM tbl_Event WHERE Name = '"+eventName+"')";
                sQry += " AND ID_Mannschaft1 = (SELECT ID_Mannschaft FROM tbl_Mannschaft WHERE Name = '"+m1+"')";
                sQry += " AND ID_Mannschaft2 = (SELECT ID_Mannschaft FROM tbl_Mannschaft WHERE Name = '"+m2+"')";
                sQry += " AND Beginn = '"+start.toString("yyyy-MM-dd hh:mm:ss")+"'";
                sQry += " AND Ende = '"+ende.toString("yyyy-MM-dd hh:mm:ss")+"'";

        qry.prepare(sQry);

        if(!qry.exec())
            *error = "Datenbankabfrage konnte nicht ausgeführt werden.";
        else
        {
            *p_qry = qry;
            ok = true;
        }
    }

    return ok;
}

bool DBHandler::updateWette(QString *error, int spielID, QString m1, QString m2, QDateTime start, QDateTime ende, int punkteM1, int punkteM2) //Autor: Christoph Rehm
{
    bool ok = false;

    if(!this->openDB())
            *error = "Datenbank konnte nicht geöffnet werden.";
        else
        {
        QSqlQuery qry(this->wettenDB);
        QString sQry = "UPDATE tbl_Spiel";
                sQry += " SET ID_Mannschaft1 = (SELECT ID_Mannschaft FROM tbl_Mannschaft WHERE Name = '"+m1+"'),";
                sQry += " ID_Mannschaft2 = (SELECT ID_Mannschaft FROM tbl_Mannschaft WHERE Name = '"+m2+"'),";
                sQry += " PunkteMannschaft1 = "+QString::number(punkteM1)+", PunkteMannschaft2 = "+QString::number(punkteM2)+", Beginn = '"+start.toString("yyyy-MM-dd hh:mm:ss")+"', Ende = '"+ende.toString("yyyy-MM-dd hh:mm:ss")+"'";
                sQry += " WHERE ID_Spiel = "+QString::number(spielID);

        qry.prepare(sQry);

        if(!qry.exec())
            *error = "Datenbankabfrage konnte nicht ausgeführt werden.";
        else
            ok = true;
        }

    return ok;
}

bool DBHandler::loeschenWette(QString *error, int spielID) //Autor: Christoph Rehm
{
    bool ok = false;

    if(!this->openDB())
            *error = "Datenbank konnte nicht geöffnet werden.";
        else
        {
        QSqlQuery qry(this->wettenDB);
        QString sQry = "DELETE FROM tbl_Spiel WHERE ID_Spiel = "+QString::number(spielID);

        qry.prepare(sQry);

        if(!qry.exec())
            *error = "Datenbankabfrage konnte nicht ausgeführt werden.";
        else
            ok = true;
        }

    return ok;
}

bool DBHandler::eintragenTipp(QString *error, QString benutzerName, int spielID, int siegesTipp, int ergebnisTippM1, int ergebnisTippM2) //Autor: Christoph Rehm
{
    bool ok = false;

    if(!this->openDB())
            *error = "Datenbank konnte nicht geöffnet werden.";
        else
    {
        QSqlQuery qry(this->wettenDB);
        qry.prepare("INSERT INTO tbl_TipptAuf (ID_User, ID_Spiel, Siegestipp, ErgebnistippM1, ErgebnistippM2) SELECT tbl_User.ID_User, "+QString::number(spielID)+", "+QString::number(siegesTipp)+", "+QString::number(ergebnisTippM1)+" ,"+QString::number(ergebnisTippM2)+"  FROM tbl_User WHERE tbl_User.Benutzername = '"+benutzerName+"'");

        if(!qry.exec())
            *error = "Datenbankabfrage konnte nicht ausgeführt werden.";
        else
            ok = true;
    }

    return ok;
}

bool DBHandler::updateTipp(QString *error, QString benutzerName, int spielID, int siegesTipp, int ergebnisTippM1, int ergebnisTippM2)
{
    bool ok = false;

    if(!this->openDB())
            *error = "Datenbank konnte nicht geöffnet werden.";
        else
    {
        QString strQry = "UPDATE tbl_TipptAuf";
                strQry += " SET Siegestipp = "+QString::number(siegesTipp)+", ErgebnistippM1 = "+QString::number(ergebnisTippM1)+", ErgebnistippM2 = "+QString::number(ergebnisTippM2);
                strQry += " WHERE ID_Spiel = "+QString::number(spielID);
                strQry += " AND ID_User = (SELECT ID_User FROM tbl_User WHERE Benutzername = '"+benutzerName+"')";

        QSqlQuery qry(this->wettenDB);
        qry.prepare(strQry);

        if(!qry.exec())
            *error = "Datenbankabfrage konnte nicht ausgeführt werden.";
        else
            ok = true;
    }

    return ok;
}

bool DBHandler::loescheTipp(QString *error, QString benutzerName, int spielID) //Autor: Christoph Rehm
{
    bool ok = false;

    if(!this->openDB())
            *error = "Datenbank konnte nicht geöffnet werden.";
        else
    {
        QString strQry = "DELETE FROM tbl_TipptAuf";
                strQry += " WHERE ID_Spiel = "+QString::number(spielID);
                strQry += " AND ID_User = (SELECT ID_User FROM tbl_User WHERE Benutzername = '"+benutzerName+"')";

        QSqlQuery qry(this->wettenDB);
        qry.prepare(strQry);

        if(!qry.exec())
            *error = "Datenbankabfrage konnte nicht ausgeführt werden.";
        else
            ok = true;
    }

    return ok;
}

bool DBHandler::getTipps(QSqlQuery *p_qry, QString *error, int spielID) //Autor: Christoph Rehm
{
    bool ok = false;

    if(!this->openDB())
        *error = "Datenbank konnte nicht geöffnet werden.";
    else
    {
        QString stQry = "SELECT tbl_User.Benutzername, tbl_TipptAuf.Siegestipp, tbl_TipptAuf.ErgebnistippM1, tbl_TipptAuf.ErgebnistippM2";
                stQry += " FROM tbl_TipptAuf";
                stQry += " INNER JOIN tbl_User";
                stQry += " ON tbl_User.ID_User = tbl_TipptAuf.ID_User";
                stQry += " WHERE tbl_TipptAuf.ID_Spiel = "+QString::number(spielID)+"";
        QSqlQuery qry(this->wettenDB);
        qry.prepare(stQry);

        if(!qry.exec())
            *error = "Datenbankabfrage konnte nicht ausgeführt werden.";
        else
        {
            *p_qry = qry;
            ok = true;
        }
    }

    return ok;
}

bool DBHandler::adminloeschen(QString benutzername, QString* error)//Autor: Aymane Amaioua
{
    bool ok = false;

    if(!this->openDB())
        *error = "Datenbank konnte nicht geöffnet werden.";
    else
    {
        QSqlQuery qry(this->wettenDB);
        qry.prepare("DELETE  FROM tbl_Admin WHERE Benutzername='" + benutzername+"'");

        if(!qry.exec())
            *error = "Datenbankabfrage konnte nicht ausgeführt werden.";
        else
            ok = true;
    }
    return ok;
}

bool DBHandler::alleadmins(QSqlQuery* p_qry, QString* error)//Autor: Aymane Amaioua
{
    bool ok = false;

    if(!this->openDB())
        *error = "Datenbank konnte nicht geöffnet werden.";
    else
    {
        QSqlQuery qry(this->wettenDB);
        qry.prepare("SELECT Benutzername FROM tbl_Admin");

        if(!qry.exec())
            *error = "Datenbankabfrage konnte nicht ausgeführt werden.";
        else
        {
            *p_qry = qry;
            ok = true;
        }
    }

    return ok;
}

bool DBHandler::AdminPasswort(QSqlQuery* p_qry, QString benutzername, QString* error)
{
    bool ok = false;
    if(!this->openDB())
       *error = "Datenbank konnte nicht geöffnet werden.";
    else
    {
        QSqlQuery qry(this->wettenDB);
        qry.prepare("SELECT Passwort FROM tbl_Admin WHERE Benutzername='"+benutzername+"'");

        if(!qry.exec())
            *error = "Datenbankabfrage konnte nicht ausgeführt werden.";
        else
        {
            *p_qry = qry;
            ok = true;
        }
    }
    return ok;
}

bool DBHandler::Adminhinzu(QString benutzername, QString passwort, QString* error)//Autor: Aymane Amaioua
{
    bool ok = false;
    if(!this->openDB())
        *error = "Datenbank konnte nicht geöffnet werden.";
    else
    {
        QSqlQuery qry(this->wettenDB);
        qry.prepare("INSERT INTO tbl_Admin (Benutzername, Passwort) VALUES('"+benutzername+"','"+passwort+"')");

        if(!qry.exec())
            *error = "Datenbankabfrage konnte nicht ausgeführt werden.";
        else
            ok = true;
    }

    return ok;
}

/********************************************************************************
 *                        NETZWERK-DATENBANK-ABFRAGEN                           *
 ********************************************************************************/

bool DBHandler::registrierung(QString benutzername, QString passwort, QString vorname, QString nachname, QString email, QString klasse, QString *error)//Neuer benutzer wird angelegt
{//Autor: Aymane Amaioua
    bool ok = false;

    if(!this->openDB())
        *error = "Datenbank konnte nicht geöffnet werden.";
    else
    {
        QSqlQuery qry(this->wettenDB);
        qry.prepare("INSERT INTO tbl_User (Benutzername, Passwort, Vorname, Nachname, Email, Klasse) VALUES('"+benutzername+"','"+passwort+"','"+vorname+"','"+nachname+"','"+email+"','"+klasse+"')");

        if(!qry.exec())
            *error = "Datenbankabfrage konnte nicht ausgeführt werden.";
        else
        {
            ok = true;
        }
    }

    return ok;
}

bool DBHandler::loeschen(QString benutzername, QString* error)//Autor: Aymane Amaioua
{
    bool ok = false;

    if(!this->openDB())
        *error = "Datenbank konnte nicht geöffnet werden.";
    else
    {
        QSqlQuery qry(this->wettenDB);
        qry.prepare("DELETE FROM tbl_User WHERE Benutzername='" + benutzername+"'");
        if(!qry.exec())
            *error = "Datenbankabfrage konnte nicht ausgeführt werden.";
        else
        {
            ok = true;
        }
    }
    return ok;
}

bool DBHandler::aendern(QString benutzername, QString newbenutzername, QString vorname, QString nachname, QString klasse, QString email, QString* error)
{//Autor: Aymane Amaioua
    bool ok = false;
    if(!this->openDB())
        *error = "Datenbank konnte nicht geöffnet werden.";
    else
    {
        QSqlQuery qry(this->wettenDB);
        qry.prepare("UPDATE tbl_User SET Benutzername = '"+newbenutzername+"', Vorname ='"+vorname+"', Nachname='"+nachname+"', Klasse='"+klasse+"', Email='"+email+"'  WHERE  Benutzername='"+benutzername+"'");
        if(!qry.exec())
            *error = "Datenbankabfrage konnte nicht ausgeführt werden.";
        else
        {
            ok = true;
        }
    }
    return ok;
}

bool DBHandler::emailaenderung(QString benutzername, QString newmail, QString* error)
{
    bool ok = false;
    if(!this->openDB())
        *error = "Datenbank konnte nicht gröffnet werden.";
    else
    {
        QSqlQuery qry(this->wettenDB);
        qry.prepare("UPDATE tbl_User SET Email='"+newmail+"' WHERE Benutzername='"+benutzername+"'");
        if(!qry.exec())
            *error = "Datenbankabfrage konnte nicht ausgeführt werden.";
        else
        {
            ok = true;
        }
        return ok ;
    }
}

bool DBHandler::passwortaenderung(QString benutzername, QString newpasswort, QString* error)
{
    bool ok = false;
    if(!this->openDB())
        *error = "Datenbank konnte nicht gröffnet werden.";
    else
    {
        QSqlQuery qry(this->wettenDB);
        qry.prepare("UPDATE tbl_User SET Passwort='"+newpasswort+"' WHERE Benutzername='"+benutzername+"'");
        if(!qry.exec())
            *error = "Datenbankabfrage konnte nicht ausgeführt werden.";
        else
        {
            ok = true;
        }
        return ok ;
    }
}
