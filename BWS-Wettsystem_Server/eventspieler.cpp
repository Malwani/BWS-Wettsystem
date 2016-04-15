#include "eventspieler.h"

EventSpieler::EventSpieler(Account *pSpieler) //Autor: Christoph Rehm
{
    this->pSpieler = pSpieler;
    this->richdigeErgebnisTipps = 0;
    this->richdigeSiegesTipps = 0;
    this->punkte = 0;
}

int EventSpieler::getPunkte() //Autor: Christoph Rehm
{
    return this->punkte;
}

Account* EventSpieler::getSpieler() //Autor: Christoph Rehm
{
    return this->pSpieler;
}

int EventSpieler::getRichdigeErgebnisTipps() //Autor: Christoph Rehm
{
    return this->richdigeErgebnisTipps;
}

int EventSpieler::getRichdigeSiegesTipps() //Autor: Christoph Rehm
{
    return this->richdigeSiegesTipps;
}

void EventSpieler::setPunkte(int punkte) //Autor: Christoph Rehm
{
    this->punkte = punkte; //SQL zumm eintragen der punkte
}

void EventSpieler::setRichdigeErgebnisTipps(int anzahl) //Autor: Christoph Rehm
{
    this->richdigeErgebnisTipps = anzahl;
}

void EventSpieler::setRichdigeSiegesTipps(int anzahl) //Autor: Christoph Rehm
{
    this->richdigeSiegesTipps = anzahl;
}

void EventSpieler::addErgebnisTipp(int punkte) //Autor: Christoph Rehm
{
    this->punkte += punkte;
    this->richdigeErgebnisTipps++;
}

void EventSpieler::addSiegesTipp(int punkte) //Autor: Christoph Rehm
{
    this->punkte += punkte;
    this->richdigeSiegesTipps++;
}

void EventSpieler::subErgebnisTipp(int punkte) //Autor: Christoph Rehm
{
    this->punkte -= punkte;
    this->richdigeErgebnisTipps--;
}

void EventSpieler::subSiegesTipp(int punkte) //Autor: Christoph Rehm
{
    this->punkte -= punkte;
    this->richdigeSiegesTipps--;
}
