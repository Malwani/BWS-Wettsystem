/*
  Modul: account.cpp
  Funktion: Methoden der Klasse Account
  Autor: Christoph Rehm
  Datum: 2016-03-09
  */

#ifndef SPIELER_H
#define SPIELER_H

#include<QString>

class Account
{
public:
    Account(QString benutzername,QString passwort, QString email, QString vorname, QString nachname, QString klasse, bool freigeschaltet, QString regSeit);

    void setScore(int punkte);
    void setListenplatz(int platz);
    void setLoggedIn(bool mode);
    void setFreigeschaltet(bool frei);
    void setMail(QString newMail);
    void setPass(QString newPass);

    int getPunkte() const;
    bool isLoggedIn() const;
    int getListenplatz() const;

    QString getBenutzername()const;
    QString getEmail()const;
    QString getVorname()const;
    QString getNachname()const;
    QString getKlasse()const;
    QString getRegDatum()const;
    QString getPasswort() const;
    bool getFreigeschaltet()const;

private:
    QString benutzername;
    QString email;
    QString regDatum;
    QString vorname;
    QString nachname;
    QString klasse;
    QString passwort;
    bool freigeschaltet;
    int punkte;
    int platz;
    bool loggedIn;
};

#endif // SPIELER_H
