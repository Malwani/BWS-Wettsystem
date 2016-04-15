/*
  Modul: verbindung.h
  Funktion: 
  Autor: Christoph Rehm, Benjamin Bardroff
  Datum: 
  */

#ifndef VERBINDUNG_H
#define VERBINDUNG_H

#include <QTcpSocket>
#include <QObject>


class Verbindung : public QObject
{
    Q_OBJECT

public:
    Verbindung(QTcpSocket* client,QString ip, QObject* parent = 0);

    QString getIP();
    QTcpSocket *getClient();
    void setLoginState(bool state);
    bool getLoginState();
    void setActiveUser(QString user);
    QString getActiveUser();

private:
    QString ip;
    QString activeUser;
    bool loginState;
    QTcpSocket* client;
};

#endif // VERBINDUNG_H
