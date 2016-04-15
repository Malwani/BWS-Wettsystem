/*
  Modul: verbindung.cpp
  Funktion: 
  Autor: Christoph Rehm, Benjamin Bardroff
  Datum: 
  */

#include "verbindung.h"

Verbindung::Verbindung(QTcpSocket *client, QString ip, QObject* parent) :
    QObject(parent)
{
    this->client = client;
    this->ip = ip;
    this->loginState = false;
    this->activeUser = "NONE";
}

QString Verbindung::getIP()
{
    return ip;
}

void Verbindung::setLoginState(bool state)
{
    this->loginState = state;

    if(!state)
        this->activeUser = "NONE";
}

bool Verbindung::getLoginState()
{
    return this->loginState;
}

void Verbindung::setActiveUser(QString user)
{
    this->activeUser = user;
}

QTcpSocket* Verbindung::getClient()
{
    return client;
}

QString Verbindung::getActiveUser()
{
    return this->activeUser;
}
