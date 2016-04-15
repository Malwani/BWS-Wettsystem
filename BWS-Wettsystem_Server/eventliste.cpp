#include "eventliste.h"


EventListe::EventListe(DBHandler *p_dbHandler, AccountListe *accountListe) // Autor: Christoph Rehm
{
    QString error;
    QSqlQuery qry;
    bool ok = true;

    this->p_dbHandler = p_dbHandler;
    this->accountListe = accountListe;

    // events aus der Datenbank holen
    //
    if(this->p_dbHandler->events(&qry,&error))
    {
        if(qry.first())
        {
            while(ok)
            {
                Event* tmp = new Event(qry.value(0).toString(),qry.value(3).toInt(),qry.value(4).toInt(),qry.value(1).toDateTime(),qry.value(2).toDateTime(),p_dbHandler,accountListe,true);
                events.append(tmp);
                ok = qry.next();
            }
        }
    }

    for (int i = 0; i < events.size(); i++)
    {
        events.at(i)->ladeListen();
    }

}

bool EventListe::eventAnlegen(QString name, int siegesTipp, int ergebnisTipp, QDateTime start, QDateTime ende, QString* error) // Autor: Christoph Rehm
{
    bool ok = true;

    for (int i = 0; i < events.size(); i++)
    {
        if (events.at(i)->getName() == name)
        {
            ok = false;
            *error = "Das Event existiert bereits!";
        }
        else
            ok = true;
    }

    if (ok)
    {
        if (this->p_dbHandler->eintragenEvent(name,start,ende,siegesTipp,ergebnisTipp,error))
        {
            Event* tmp = new Event(name,siegesTipp,ergebnisTipp,start,ende,p_dbHandler,accountListe,true);
            events.append(tmp);
            this->sortEventNachName();
            ok = true;
        }
        else
            ok = false;
    }


    return ok;
}

bool EventListe::eventAktualieren(int index, QString alterName, QString name, int siegesTipp, int ergebnisTipp, QDateTime start, QDateTime ende,QString *error) // Autor: Christoph Rehm
{
    bool ok = false;


    if (this->p_dbHandler->updateEvent(alterName, name, start, ende, siegesTipp, ergebnisTipp,error))
    {

        events.at(index)->setAlles(name,siegesTipp,ergebnisTipp,start,ende);
        events.at(index)->updatePunkteVonSpielern();
        this->sortEventNachName();
        ok = true;
    }
    else
        ok = false;


    return ok;
}

bool EventListe::eventLoeschen(int index, QString name, QString *error) // Autor: Christoph Rehm
{
    bool ok = false;

    if (this->p_dbHandler->loeschenEvent(name,error))
    {
        Event* tmpE = events.at(index);
        tmpE->wettListeLoesche();
        events.remove(index);
        delete tmpE;
        ok = true;
    }

    return ok;
}

Event* EventListe::holen(int index) // Autor: Christoph Rehm
{
    return events.at(index);
}

Event* EventListe::holen(QString name) // Autor: Christoph Rehm
{
    int indexE = -1;

    for (int i = 0; i < this->events.size(); i++)
    {
        if (this->events.at(i)->getName() == name)
        {
            indexE = i;
            break;
        }
    }

    return this->holen(indexE);
}

QVector<Wette*> EventListe::getNowWetten() // Autor: Christoph Rehm
{
    WettenListe *tmpWL;
    Wette* tmpW;
    int spielID;
    QVector<Wette*> liste;

    for (int i = 0; i < events.size(); i++)
    {
        tmpWL = this->holen(i)->getWettenListe();
        for (int y = 0; y < tmpWL->count(); y++)
        {
            spielID = tmpWL->getSpielIdOf(y);
            tmpW = tmpWL->getWette(spielID);
            if (tmpW->getStatusTippbar())
            {
                liste.append(tmpW);
            }
        }
    }

    return liste;
}

Wette* EventListe::getWette(int spielID)// Autor: Christoph Rehm
{
    Wette* tmpW;
    for (int i = 0; i < this->events.size(); i++)
    {
        if (this->events.at(i)->getWettenListe()->getIndexOf(spielID) != -1)
            tmpW = this->events.at(i)->getWette(spielID);
    }
    return tmpW;
}

QVector<Tipp*> EventListe::getTipps(QString benutzername, QString eventName) // Autor: Christoph Rehm
{
    return this->holen(eventName)->getTipps(benutzername);
}

QVector<Wette*> EventListe::getSpielerWetten(QString benutzername, QString eventName)
{
    return this->holen(eventName)->getSpielerWetten(benutzername);
}

int EventListe::getPunkte(QString benutzername, QString eventName) // Autor: Christoph Rehm
{
    return this->holen(eventName)->getPunkte(benutzername);
}


QString EventListe::getEventNameOf(int spielID) // Autor: Christoph Rehm
{
    WettenListe *tmpWL;
    QString nameE;

    for (int i = 0; i < events.size(); i++)
    {
        tmpWL = this->holen(i)->getWettenListe();
        for (int y = 0; y < tmpWL->count(); y++)
        {
            if (tmpWL->getSpielIdOf(y) == spielID)
            {
                nameE = this->holen(i)->getName();
                break;
            }
        }
    }
    return nameE;
}

void EventListe::sortEventNachName() // Autor: Christoph Rehm
{
    int count = events.count();
    bool sotiert = false;
    Event *tmp;


    for (int y = 0; y < count-1 && !sotiert; y++)
    {
        sotiert = true;
        for (int i = 0; i < (count-1-y); i++)
        {

            if (events.at(i)->getName().toUpper() > events.at(i+1)->getName().toUpper())
            {
                tmp = events.at(i);                //
                events.replace(i,events.at(i+1)); // Dreiekstausch
                events.replace(i+1,tmp);           //
                sotiert = false;
            }
        }
    }
}

int EventListe::count() // Autor: Christoph Rehm
{
    return events.size();
}
