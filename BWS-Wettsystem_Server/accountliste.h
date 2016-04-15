#ifndef SPIELERLISTE_H
#define SPIELERLISTE_H

#include "dbhandler.h"
#include "account.h"
#include <QMessageBox>
#include <QListWidget>

class FrmMain;

class AccountListe
{
public:

    AccountListe(DBHandler* p_dbHeandler);

    bool aktualisieren(QString* error); // ruft entsprechende DBHandler Methode auf
    bool neuenAccountAnlegen(QString benutzername, QString passwort, QString vorname, QString nachname, QString email, QString klasse, QString* error);
    bool AccountEingabenOK(QString benutzername, QString passwort, QString vorname, QString nachname, QString email, QString klasse, QString* error);
    bool AccountFreischaltenSperren(bool freischalten, QString benutzername, QString *error);
    bool AccountLoeschen(QString benutzername, QString *error);
    bool AccountAendern(QString benutzername, QString newbenutzername, QString vorname, QString nachname, QString klasse, QString email, QString *error);
    bool AdminSortieren(QString *error);
    bool AdminLoeschen(QString benutzername, QString* error);
    bool AdminAnlegen(QString benutzername, QString passwort, QString* error);
    bool ladeAdmins(QString *error);
    bool AdminEingabeOK(QString benutzername, QString passwort, QString* error);
    bool EmailAenderung(QString benutzername, QString passwort, QString newmail, QString* error);
    bool PasswortAenderung(QString benutzername, QString passwort, QString newpasswort, QString* error);
    bool ClientAccountLoeschen(QString benutzername, QString passwort, QString* error);
    bool loginClient(QString benutzername, QString passwort, QString* error);
    bool LoginServer (QString benutzername, QString passwort, QString* error);

    Account SpielerInfo(QString benutzername, QString* error);
    QList<QString> getAdmins();
    QList<Account> getAccounts();
    QList<Account>* getPAccounts();
    Account* getAccountP(int indexAc);

private:

    void sortiereAccounts(); // immer alphabetisch

    QList<Account> accounts;
    QList<QString> admins;
    DBHandler* p_dbHandler;

};

#endif // SPIELERLISTE_H
