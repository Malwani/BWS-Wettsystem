#include "eventspielerliste.h"

EventSpielerListe::EventSpielerListe(QString *eventName, AccountListe *accountListe, int *siegesPunkte, int *ergebnisPunkte) //Autor: Christoph Rehm
{
    this->eventName = eventName;
    this->siegesPunkte = siegesPunkte;
    this->ergebnisPunkte = ergebnisPunkte;
    this->accountListe = accountListe;
    accounts = accountListe->getPAccounts();

}

EventSpielerListe::~EventSpielerListe() //Autor: Christoph Rehm
{
    int index = this->eventSpieler.size();
    for (int i = index-1; i >= 0 ; i--)
    {
        delete this->eventSpieler.at(i);
        this->eventSpieler.removeAt(i);
    }
}

void EventSpielerListe::eventListeLaden(DBHandler *p_dbHandler) //Autor: Christoph Rehm
{

    QSqlQuery qry;
    QString error;
    bool ok = true;
    Account* tmpAc;

    if (p_dbHandler->getEventSpieler(&qry,&error,*eventName))
    {
        if(qry.first())
        {
            while(ok)
            {

                for (int i = 0; i < accounts->count(); i++)
                {
                    if (qry.value(0).toString() == accounts->at(i).getBenutzername())
                    {
                        tmpAc = accountListe->getAccountP(i);
                        this->addEventSpieler(tmpAc);
                        break;
                    }
                }

                if (qry.value(5).toInt() != (-1)) // wenn es ein ergebniss gibt
                {
                    int spielM1 = qry.value(4).toInt();
                    int spielM2 = qry.value(5).toInt();

                    if (qry.value(1).toInt() != 0) // wenn er siegestipp hat
                    {
                        if (qry.value(1).toInt() == 1 && spielM1 > spielM2)
                            this->addSiegesTippPunkte(tmpAc);
                        if (qry.value(1).toInt() == 2 && spielM1 < spielM2)
                            this->addSiegesTippPunkte(tmpAc);
                    }
                    else
                    {
                        int tippM1 = qry.value(2).toInt();
                        int tippM2 = qry.value(3).toInt();

                        if(spielM1 == tippM1 && spielM2 == tippM2) // wenn der Tipp dem Ergebniss übereinstimmt
                            this->addErgebnisTippPunkte(tmpAc);
                    }
                }

                // übergebrn tmpES->getSpieler()
                ok = qry.next();
            }
        }
    }
}

bool EventSpielerListe::addEventSpieler(Account *spieler) //Autor: Christoph Rehm
{
    bool ok = false;

    if (this->getIndexOf(spieler) == -1)
    {
        EventSpieler* tmp = new EventSpieler(spieler);
        this->eventSpieler.append(tmp);
        ok = true;
    }

    return ok;
}

void EventSpielerListe::entferneEventSpieler(Account *spieler) // Autor: Christoph Rehm
{
    this->entferneEventSpieler(this->getIndexOf(spieler));
}

void EventSpielerListe::entferneEventSpieler(int index) // Autor: Christoph Rehm
{
    EventSpieler* tmp = eventSpieler.at(index);
    eventSpieler.removeAt(index);
    delete tmp;
}


void EventSpielerListe::setPunkte(Account *spieler, int punkte) //Autor: Christoph Rehm
{
    this->setPunkte(this->getIndexOf(spieler),punkte);
}

void EventSpielerListe::setPunkte(int index, int punkte) //Autor: Christoph Rehm
{
    this->eventSpieler.at(index)->setPunkte(punkte);
}



void EventSpielerListe::addErgebnisTippPunkte(int index) //Autor: Christoph Rehm
{
    this->eventSpieler.at(index)->addErgebnisTipp(*ergebnisPunkte);
}

void EventSpielerListe::addErgebnisTippPunkte(Account *spieler) //Autor: Christoph Rehm
{
    this->addErgebnisTippPunkte(this->getIndexOf(spieler));
}



void EventSpielerListe::addSiegesTippPunkte(int index) //Autor: Christoph Rehm
{
    this->eventSpieler.at(index)->addSiegesTipp(*siegesPunkte);
}

void EventSpielerListe::addSiegesTippPunkte(Account *spieler) //Autor: Christoph Rehm
{
    this->addSiegesTippPunkte(this->getIndexOf(spieler));
}


void EventSpielerListe::subErgebnisTippPunkte(int index) //Autor: Christoph Rehm
{
   this->eventSpieler.at(index)->subErgebnisTipp(*ergebnisPunkte);
}

void EventSpielerListe::subErgebnisTippPunkte(Account *spieler) //Autor: Christoph Rehm
{
    this->subErgebnisTippPunkte(this->getIndexOf(spieler));
}


void EventSpielerListe::subSiegesTippPunkte(int index) //Autor: Christoph Rehm
{
    this->eventSpieler.at(index)->subSiegesTipp(*siegesPunkte);
}

void EventSpielerListe::subSiegesTippPunkte(Account *spieler) //Autor: Christoph Rehm
{
    this->subSiegesTippPunkte(this->getIndexOf(spieler));
}


void EventSpielerListe::updatePunkte(Account *spieler) //Autor: Christoph Rehm
{
    this->updatePunkte(this->getIndexOf(spieler));
}

void EventSpielerListe::updatePunkte(int index) //Autor: Christoph Rehm
{
    int rSigesTipps = this->eventSpieler.at(index)->getRichdigeSiegesTipps();
    int rErgebnisTipps = this->eventSpieler.at(index)->getRichdigeErgebnisTipps();

    int punkte = (rSigesTipps * *this->siegesPunkte) + (rErgebnisTipps * *this->ergebnisPunkte);

    this->eventSpieler.at(index)->setPunkte(punkte);

}


void EventSpielerListe::loeschePunkte(int index) //Autor: Christoph Rehm
{
    this->eventSpieler.at(index)->setPunkte(0);
    this->eventSpieler.at(index)->setRichdigeErgebnisTipps(0);
    this->eventSpieler.at(index)->setRichdigeSiegesTipps(0);
}

void EventSpielerListe::loeschePunkte(Account *spieler) //Autor: Christoph Rehm
{
    this->loeschePunkte(this->getIndexOf(spieler));
}


int EventSpielerListe::getPunkte(QString benutzername) //Autor: Christoph Rehm
{
    return this->eventSpieler.at(this->getIndexOf(benutzername))->getPunkte();
}

void EventSpielerListe::sortBNamen() //Autor: Christoph Rehm
{
    EventSpieler* tmp;
    int count = eventSpieler.size();
    bool sotiert = false;

    for (int y = 0; y < count-1 && !sotiert; y++)
    {
        sotiert = true;
        for (int i = 0; i < (count-1-y); i++)
        {
            if (eventSpieler.at(i)->getSpieler()->getBenutzername().toUpper() > eventSpieler.at(i+1)->getSpieler()->getBenutzername().toUpper())
            {
                tmp = eventSpieler.at(i);                     //
                eventSpieler.replace(i,eventSpieler.at(i+1)); // Dreiekstausch
                eventSpieler.replace(i+1,tmp);                //
                sotiert = false;
            }
        }
    }
}

void EventSpielerListe::sortPunkte() //Autor: Christoph Rehm
{
    EventSpieler* tmp;
    int count = eventSpieler.size();
    bool sotiert = false;

    for (int y = 0; y < count-1 && !sotiert; y++)
    {
        sotiert = true;
        for (int i = 0; i < (count-1-y); i++)
        {
            if (eventSpieler.at(i)->getPunkte() < eventSpieler.at(i+1)->getPunkte())
            {
                tmp = eventSpieler.at(i);                     //
                eventSpieler.replace(i,eventSpieler.at(i+1)); // Dreiekstausch
                eventSpieler.replace(i+1,tmp);                //
                sotiert = false;
            }
        }
    }
}

EventSpieler* EventSpielerListe::getEventSpieler(int index) //Autor: Christoph Rehm
{
    return this->eventSpieler.at(index);
}

EventSpieler* EventSpielerListe::getEventSpieler(Account *spieler) //Autor: Christoph Rehm
{
    return this->getEventSpieler(this->getIndexOf(spieler));
}


int EventSpielerListe::count() //Autor: Christoph Rehm
{
    return this->eventSpieler.count();
}

int EventSpielerListe::getIndexOf(Account *spieler) //Autor: Christoph Rehm
{
    int indexOf = -1;

    for (int i = 0; i < this->eventSpieler.size(); i++)
    {
        if (spieler->getBenutzername() == eventSpieler.at(i)->getSpieler()->getBenutzername())
        {
            indexOf = i;
            break;
        }
    }

    return indexOf;
}

int EventSpielerListe::getIndexOf(QString benutzername) //Autor: Christoph Rehm
{
    int indexOf = -1;

    for (int i = 0; this->eventSpieler.size(); i++)
    {
        if (benutzername == eventSpieler.at(i)->getSpieler()->getBenutzername())
        {
            indexOf = i;
            break;
        }
    }

    return indexOf;
}
