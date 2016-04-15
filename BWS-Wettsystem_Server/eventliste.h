#ifndef EVENTLISTE_H
#define EVENTLISTE_H

#include <QVector>
#include "event.h"
#include "dbhandler.h"

class EventListe
{
public:
    EventListe(DBHandler* p_dbHandler,AccountListe* accountListe);

    bool eventAnlegen(QString name, int siegesTipp, int ergebnisTipp, QDateTime start, QDateTime ende, QString* error);
    bool eventAktualieren(int index, QString alterName, QString name, int siegesTipp, int ergebnisTipp, QDateTime start, QDateTime ende, QString* error);
    bool eventLoeschen(int index, QString name, QString* error);

    QVector<Wette*> getNowWetten(); // alle tippbaren wetten
    Wette* getWette(int spielID);
    int getPunkte(QString benutzername, QString eventName);
    QVector<Tipp*> getTipps(QString benutzername, QString eventName);
    QVector<Wette*> getSpielerWetten(QString benutzername, QString eventName);
    QString getEventNameOf(int spielID);

    Event *holen(int index);
    Event *holen(QString name);
    int count();

private:
    QVector<Event*> events;
    DBHandler* p_dbHandler;
    AccountListe* accountListe;
    void sortEventNachName();

signals:

public slots:
};

#endif // EVENTLISTE_H
