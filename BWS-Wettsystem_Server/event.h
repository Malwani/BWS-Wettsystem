#ifndef EVENT_H
#define EVENT_H


#include "eventspielerliste.h"
#include "wettenliste.h"

class Event //Autor: Christoph Rehm
{
public:
    Event(QString name, int siegesTipp, int ergebnisTipp, QDateTime start, QDateTime ende, DBHandler* p_dbHandler, AccountListe* accountListe = NULL, bool eventSpielerListeErstellen = false);
    ~Event();

    QString getName();
    int getSiegesTipp();
    int getErgebnisTipp();
    QDateTime getStart();
    QDateTime getEnde();
    QVector<Tipp*> getTipps(QString benutzername);
    QVector<Wette*> getSpielerWetten(QString benutzername);

    QVector<Wette*> getNowWetten(); // gibt tippbare wetten dises Events zurück
    Wette* getWette(int spielID);

    bool addWette(QString m1, QString m2, QDateTime start, QDateTime ende);
    int getSpielIdOfWette(QString m1, QString m2, QDateTime start, QDateTime ende);
    bool updateWette(int spielID, QString m1, QString m2, QDateTime start, QDateTime ende, int punkteM1 = -1, int punkteM2 = -1);
    bool loescheWette(int spielID);

    bool addTipp(int spielID, Account* spieler,int siegesTipp, int ergebnisTippM1, int ergebnisTippM2);
    bool updateTipp(int spielID, Account* spieler,int siegesTipp, int ergebnisTippM1, int ergebnisTippM2);

    QVector<EventSpieler*> getBestenliste();

    void ladeListen();

    void setEventSpielerListe(EventSpielerListe* eventSpielerListe);
    EventSpielerListe* getEventSpielerListe();

    WettenListe* getWettenListe();
    void wettListeLoesche();

    void setAlles(QString name, int siegesTipp, int ergebnisTipp, QDateTime start, QDateTime ende);
    void updatePunkteVonSpielern();
    void resetPunkteVonSpielern();

    int getPunkte(QString benutzername);

private:
    QString name;
    int siegesTipp;
    int ergebnisTipp;
    QDateTime start;
    QDateTime ende;
    EventSpielerListe* eventSpielerListe;
    WettenListe* wettListe;
    DBHandler* p_dbHandler;

//signals:


};

#endif // EVENT_H
