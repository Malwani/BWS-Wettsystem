#include "event.h"

Event::Event(QString name, int siegesTipp, int ergebnisTipp, QDateTime start, QDateTime ende, DBHandler *p_dbHandler, AccountListe *accountListe, bool eventSpielerListeErstellen) //Autor: Christoph Rehm
{
    this->name = name;
    this->siegesTipp = siegesTipp;
    this->ergebnisTipp = ergebnisTipp;
    this->start = start;
    this->ende = ende;
    this->p_dbHandler = p_dbHandler;
    this->wettListe = new WettenListe(&this->name,accountListe,p_dbHandler);
    if (eventSpielerListeErstellen)
        this->eventSpielerListe = new EventSpielerListe(&this->name,accountListe,&this->siegesTipp,&this->ergebnisTipp);
}

Event::~Event() //Autor: Christoph Rehm
{
    delete eventSpielerListe;
    delete wettListe;
}

void Event::setAlles(QString name, int siegesTipp, int ergebnisTipp, QDateTime start, QDateTime ende) //Autor: Christoph Rehm
{
    this->name = name;
    this->siegesTipp = siegesTipp;
    this->ergebnisTipp = ergebnisTipp;
    this->start = start;
    this->ende = ende;
}

void Event::ladeListen() //Autor: Christoph Rehm
{
    this->eventSpielerListe->eventListeLaden(p_dbHandler);
    this->wettListe->ladenWetten();
}

QString Event::getName() //Autor: Christoph Rehm
{
    return this->name;
}

int Event::getErgebnisTipp() //Autor: Christoph Rehm
{
    return this->ergebnisTipp;
}

int Event::getSiegesTipp() //Autor: Christoph Rehm
{
    return this->siegesTipp;
}

QDateTime Event::getStart() //Autor: Christoph Rehm
{
    return this->start;
}

QDateTime Event::getEnde() //Autor: Christoph Rehm
{
    return this->ende;
}

QVector<Tipp*> Event::getTipps(QString benutzername) //Autor: Christoph Rehm
{
    return this->wettListe->getTipps(benutzername);
}

QVector<Wette*> Event::getSpielerWetten(QString benutzername) //Autor: Christoph Rehm
{
    return this->wettListe->getSpielerWetten(benutzername);
}

QVector<Wette*> Event::getNowWetten() //Autor: Christoph Rehm
{
    int spielID;
    QVector<Wette*> tmpQV;

    for (int i = 0; i < wettListe->count(); i++)
    {
        spielID = wettListe->getSpielIdOf(i);
        if (wettListe->getWette(spielID)->getStatusTippbar())
        {
            tmpQV.append(wettListe->getWette(spielID));
        }
    }
    return tmpQV;
}

Wette* Event::getWette(int spielID) //Autor: Christoph Rehm
{
    return this->wettListe->getWette(spielID);
}

bool Event::addWette(QString m1, QString m2, QDateTime start, QDateTime ende) //Autor: Christoph Rehm
{
    return this->wettListe->addWette(m1,m2,start,ende);
}

int Event::getSpielIdOfWette(QString m1, QString m2, QDateTime start, QDateTime ende) //Autor: Christoph Rehm
{
    return this->wettListe->getSpielIdOf(m1,m2,start,ende);
}

bool Event::updateWette(int spielID, QString m1, QString m2, QDateTime start, QDateTime ende, int punkteM1, int punkteM2) //Autor: Christoph Rehm
{
    bool ok;
    ok = this->wettListe->updateWette(spielID,m1,m2,start,ende,punkteM1,punkteM2);
    this->resetPunkteVonSpielern();
    return ok;
}

bool Event::loescheWette(int spielID) //Autor: Christoph Rehm
{
    bool ok;
    ok = this->wettListe->loescheWette(spielID);
    this->resetPunkteVonSpielern();
    return ok;
}

bool Event::addTipp(int spielID, Account* spieler,int siegesTipp, int ergebnisTippM1, int ergebnisTippM2) //Autor: Christoph Rehm
{
    bool ok = false;

    if (!this->wettListe->getWette(spielID)->hatgeTippt(spieler->getBenutzername()))
    {
        if (this->wettListe->getWette(spielID)->getStatusTippbar())
        {
            this->eventSpielerListe->addEventSpieler(spieler);
            ok = this->wettListe->getWette(spielID)->getTippListe()->tippErstellen(p_dbHandler,spieler,siegesTipp,ergebnisTippM1,ergebnisTippM2);
        }
    }

    return ok;
}

bool Event::updateTipp(int spielID, Account *spieler, int siegesTipp, int ergebnisTippM1, int ergebnisTippM2) //Autor: Christoph Rehm
{
    bool ok = false;

    if (this->wettListe->getWette(spielID)->getStatusTippbar())
    {
        ok = this->wettListe->getWette(spielID)->getTippListe()->tippUpdaten(p_dbHandler,spieler,siegesTipp,ergebnisTippM1,ergebnisTippM2);
    }

    return ok;
}

QVector<EventSpieler*> Event::getBestenliste() //Autor: Christoph Rehm
{
    QVector<EventSpieler*> tmpQV;
    this->eventSpielerListe->sortPunkte();

    for (int i = 0; i < this->eventSpielerListe->count(); i++)
    {
        tmpQV.append(this->eventSpielerListe->getEventSpieler(i));
    }

    return tmpQV;
}

void Event::setEventSpielerListe(EventSpielerListe *eventSpielerListe) //Autor: Christoph Rehm
{
    this->eventSpielerListe = eventSpielerListe;
}

EventSpielerListe* Event::getEventSpielerListe() //Autor: Christoph Rehm
{
    return this->eventSpielerListe;
}

WettenListe* Event::getWettenListe() //Autor: Christoph Rehm
{
    return this->wettListe;
}

void Event::wettListeLoesche() //Autor: Christoph Rehm
{
    for (int i = (wettListe->count() -1); i >= 0; i--)
    {
        wettListe->loescheWette(wettListe->getSpielIdOf(i));
    }
}

void Event::resetPunkteVonSpielern() //Autor: Christoph Rehm; Benjamin Bardroff
{

    for (int i = 0; i < eventSpielerListe->count(); i++)
        this->eventSpielerListe->loeschePunkte(i);


    for (int i = 0; i < wettListe->count(); i++)
    {
        Wette* tmpW = this->wettListe->getWette(this->wettListe->getSpielIdOf(i));
        int pM1 = tmpW->getPunkteM1();
        int pM2 = tmpW->getPunkteM2();
        TippListe* tmpTL = tmpW->getTippListe();
        int anzTipps = tmpTL->count();

        if (pM1 != -1) // wenn das ergebniss feststeht
        {
            for (int i = 0; i < anzTipps; i++)
            {
                Account* tmpAc = tmpTL->getSpielerOf(i);
                if (tmpTL->getSiegesTipp(tmpAc) != -1) // wenn siegestipp
                {
                    if(tmpTL->getSiegesTipp(tmpAc) == 1 && pM1 > pM2)
                        this->eventSpielerListe->addSiegesTippPunkte(tmpAc);
                    else if(tmpTL->getSiegesTipp(tmpAc) == 2 && pM1 < pM2)
                        this->eventSpielerListe->addSiegesTippPunkte(tmpAc);
                    else if(tmpTL->getSiegesTipp(tmpAc) == 0 && pM1 == pM2)
                        this->eventSpielerListe->addSiegesTippPunkte(tmpAc);
                }
                else // ergebnis tipp
                {
                    if(tmpTL->getErgebnisTippM1(tmpAc) == pM1 && tmpTL->getErgebnisTippM2(tmpAc) == pM2)
                        this->eventSpielerListe->addErgebnisTippPunkte(tmpAc);
                }
            }
        }
    }
}

void Event::updatePunkteVonSpielern() //Autor: Christoph Rehm
{
    for (int i = 0; i < eventSpielerListe->count(); i++)
    {
        this->eventSpielerListe->updatePunkte(i);
    }
}

int Event::getPunkte(QString benutzername) //Autor: Christoph Rehm
{
    return this->eventSpielerListe->getPunkte(benutzername);
}
