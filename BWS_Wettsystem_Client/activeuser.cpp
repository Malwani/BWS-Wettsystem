#include "activeuser.h"

ActiveUser::ActiveUser()
{
    this->activeUsername = "NONE";
}

void ActiveUser::setActive(QString email, QString vorname, QString nachname, QString klasse, QString regSeit)
{
    this->email = email;
    this->vorname = vorname;
    this->nachname = nachname;
    this->klasse = klasse;
    this->regSeit = regSeit;
}

void ActiveUser::setInactive()
{
    this->activeUsername = "NONE";
    this->email.clear();
    this->vorname.clear();
    this->nachname.clear();
    this->klasse.clear();
    this->regSeit.clear();
}

void ActiveUser::setUser(QString username)
{
    this->activeUsername = username;
}

QString ActiveUser::getActiveUsername() const
{
    return this->activeUsername;
}

QString ActiveUser::getEmail() const
{
    return this->email;
}

QString ActiveUser::getKlasse() const
{
    return this->klasse;
}

QString ActiveUser::getNachname() const
{
    return this->nachname;
}

QString ActiveUser::getVorname() const
{
    return this->vorname;
}

QString ActiveUser::getRegSeit() const
{
    return this->regSeit;
}

