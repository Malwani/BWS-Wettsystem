#ifndef EVENTSPIELERLISTE_H
#define EVENTSPIELERLISTE_H

#include "dbhandler.h"
#include "eventspieler.h"
#include "accountliste.h"
#include <QVector>

class EventSpielerListe
{
public:
    EventSpielerListe(QString* eventName, AccountListe* accountListe, int* siegesPunkte, int* ergebnisPunkte);
    ~EventSpielerListe();

    bool addEventSpieler(Account* spieler);

    void entferneEventSpieler(Account* spieler);
    void entferneEventSpieler(int index);

    void eventListeLaden(DBHandler *p_dbHandler);

    void setPunkte(Account* spieler, int punkte);
    void setPunkte(int index, int punkte);

    void addSiegesTippPunkte(Account* spieler);
    void addSiegesTippPunkte(int index);
    void addErgebnisTippPunkte(int index);
    void addErgebnisTippPunkte(Account* spieler);

    void subSiegesTippPunkte(Account* spieler);
    void subSiegesTippPunkte(int index);
    void subErgebnisTippPunkte(int index);
    void subErgebnisTippPunkte(Account* spieler);

    void updatePunkte(Account* spieler);
    void updatePunkte(int index);

    void loeschePunkte(Account* spieler);
    void loeschePunkte(int index);

    int getPunkte(QString benutzername);

    void sortBNamen();
    void sortPunkte();

    int count();

    int getIndexOf(Account* spieler);
    int getIndexOf(QString benutzername);

    EventSpieler* getEventSpieler(int index);
    EventSpieler* getEventSpieler(Account* spieler);

private:
    QVector<EventSpieler*> eventSpieler;
    QString* eventName;
    int* siegesPunkte;
    int* ergebnisPunkte;
    AccountListe *accountListe;
    QList<Account> *accounts;

signals:

public slots:
};

#endif // EVENTSPIELERLISTE_H
