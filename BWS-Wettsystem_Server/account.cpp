/*
  Modul: Account.h
  Funktion: Header der Klasse Account
  Autor: Christoph Rehm
  Datum: 2016-03-09
  */

#include "account.h"

Account::Account(QString benutzername, QString passwort, QString email, QString vorname, QString nachname, QString klasse, bool freigeschaltet, QString regSeit)
{
    this->benutzername = benutzername;
    this->email = email;
    this->vorname = vorname;
    this->nachname = nachname;
    this->klasse = klasse;
    this->freigeschaltet = freigeschaltet;
    this->passwort = passwort;
    this->loggedIn = false;
    this->regDatum = regSeit;
}

void Account::setScore(int punkte)
{
    this->punkte = punkte;
}

void Account::setListenplatz(int platz)
{
    this->platz = platz;
}

void Account::setLoggedIn(bool mode)
{
    this->loggedIn = mode;
}

int Account::getPunkte() const
{
    return this->punkte;
}

QString Account::getPasswort() const
{
    return this->passwort;
}

bool Account::isLoggedIn() const
{
    return this->loggedIn;
}

int Account::getListenplatz() const
{
    return this->platz;
}

QString Account::getBenutzername() const
{
    return this->benutzername;
}

QString Account::getEmail() const
{
    return this->email;
}

QString Account::getVorname() const
{
    return this->vorname;
}

QString Account::getNachname() const
{
    return this->nachname;
}

QString Account::getKlasse() const
{
    return this->klasse;
}

bool Account::getFreigeschaltet() const
{
    return this->freigeschaltet;
}

QString Account::getRegDatum() const
{
    return this->regDatum;
}

// Author: Benjamin Bardroff, 10,04,2016
void Account::setMail(QString newMail)
{
    this->email = newMail;
}

void Account::setPass(QString newPass)
{
    this->passwort = newPass;
}

void Account::setFreigeschaltet(bool frei)
{
    this->freigeschaltet = frei;
}
