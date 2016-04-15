#include "wettenliste.h"

WettenListe::WettenListe(QString* eventName, AccountListe *accountListe, DBHandler* p_dbHandler) // Autor: Christoph Rehm
{
    this->accountListe = accountListe;
    this->eventName = eventName;
    this->p_dbHandler = p_dbHandler;
}

WettenListe::~WettenListe() // Autor: Christoph Rehm
{
    int count = this->wettenListe.size();
    for (int i = count-1; i >= 0 ; i--)
    {
        delete this->wettenListe.at(i);
        this->wettenListe.removeAt(i);
    }
}

void WettenListe::ladenWetten() // Autor: Christoph Rehm
{
    QSqlQuery qry;
    QString error;
    bool ok = true;

    if(this->p_dbHandler->getWetten(&qry,&error,*this->eventName))
    {
        if(qry.first())
        {
            while(ok)
            {
                this->addWette(qry.value(1).toString(),qry.value(2).toString(),qry.value(5).toDateTime(),qry.value(6).toDateTime(),qry.value(0).toInt(),qry.value(3).toInt(),qry.value(4).toInt(),false);
                ok = qry.next();
            }
        }
    }

    for (int i = 0; i < wettenListe.size(); i++)
        this->wettenListe.at(i)->spielTippListeLaden(p_dbHandler);
}

QVector<Wette*> WettenListe::getWetten()
{
    return this->wettenListe;
}

bool WettenListe::addWette(QString m1, QString m2, QDateTime start, QDateTime ende, int spielID, int punkteM1, int punkteM2, bool datenbankEintrag) // Autor: Christoph Rehm
{
    bool ok = true;
    QString error;
    QSqlQuery qry;
    int tmpSpielID;

    if (!this->exist(m1,m2,start,ende))
    {
        if (datenbankEintrag)
        {

            if (!this->p_dbHandler->eintragenWette(&error,*this->eventName,m1,m2,start,ende,punkteM1,punkteM2))
                ok = false;

            if (!this->p_dbHandler->getSpielID(&error,&qry,*this->eventName,m1,m2,start,ende))
                ok = false;
            else
            {
                qry.first();
                tmpSpielID = qry.value(0).toInt();
            }
        }

        if (ok)
        {
            Wette* tmp = new Wette(this->accountListe,m1,m2,start,ende);

            if(!datenbankEintrag)
            {
                tmp->setSpielID(spielID);
                tmp->setErgebnis(punkteM1,punkteM2);
            }
            else
            {
                tmp->setSpielID(tmpSpielID);
            }

            this->wettenListe.append(tmp);
        }

    }
    else
        ok = false;


    return ok;
}

bool WettenListe::updateWette(int spielID, QString m1, QString m2, QDateTime start, QDateTime ende, int punkteM1, int punkteM2) // Autor: Christoph Rehm
{
    bool ok = false;
    QString error;
    int indexW;

    if(this->p_dbHandler->updateWette(&error,spielID,m1,m2,start,ende,punkteM1,punkteM2))
    {
        indexW = this->getIndexOf(spielID);
        this->wettenListe.at(indexW)->setManschaften(m1,m2);
        this->wettenListe.at(indexW)->setZeit(start,ende);
        this->wettenListe.at(indexW)->setErgebnis(punkteM1,punkteM2);
        ok = true;
    }

    return ok;
}


bool WettenListe::loescheWette(int spielID) // Autor: Christoph Rehm
{
    QString error;
    int indexW;
    bool ok = false;

    if (this->p_dbHandler->loeschenWette(&error,spielID))
    {
        indexW = this->getIndexOf(spielID);
        Wette* tmp = this->wettenListe.at(indexW);
        tmp->tippListeLoesche(p_dbHandler);
        this->wettenListe.removeAt(indexW);
        delete tmp;
        ok = true;
    }

    return ok;
}

Wette* WettenListe::getWette(int spielID) // Autor: Christoph Rehm
{
    return this->wettenListe.at(this->getIndexOf(spielID));
}

QVector<Tipp *> WettenListe::getTipps(QString benutzername) // Autor: Christoph Rehm
{
    QVector<Tipp*> tmpQV;

    for (int i = 0; i < this->wettenListe.size(); i++)
    {
        if(this->wettenListe.at(i)->getTippListe()->getIndexOf(benutzername) != -1)
            tmpQV.append(this->wettenListe.at(i)->getTipp(benutzername));
    }

    return tmpQV;
}

QVector<Wette *> WettenListe::getSpielerWetten(QString benutzername) // Autor: Christoph Rehm
{
    QVector<Wette *> tmpQV;

    for (int i = 0; i < this->wettenListe.size(); i++)
    {
        if (this->wettenListe.at(i)->hatgeTippt(benutzername))
            tmpQV.append(this->wettenListe.at(i));
    }

    return tmpQV;
}

int WettenListe::getIndexOf(int spielID) // Autor: Christoph Rehm
{
    int indexW = -1;

    for (int i = 0; i < wettenListe.size(); i++)
    {
        if (wettenListe.at(i)->getSpielID() == spielID)
        {
            indexW = i;
            break;
        }
    }

    return indexW;
}

int WettenListe::getSpielIdOf(int indexW) // Autor: Christoph Rehm
{
    return this->wettenListe.at(indexW)->getSpielID();
}

int WettenListe::getSpielIdOf(QString m1, QString m2, QDateTime start, QDateTime ende) // Autor: Christoph Rehm
{
    int spielId = -1;
    Wette* tmp;

    for (int i = 0; i < wettenListe.size(); i++)
    {
        tmp = wettenListe.at(i);
        if(tmp->getManschaftM1() == m1 && tmp->getManschaftM2() == m2 && tmp->getStart() == start && tmp->getEnde() == ende)
            spielId = tmp->getSpielID();
    }
    return spielId;
}

bool WettenListe::exist(QString m1, QString m2, QDateTime start, QDateTime ende) // Autor: Christoph Rehm
{
    bool exist = false;

    for (int i = 0; i < wettenListe.size(); i++)
    {
        if (wettenListe.at(i)->getManschaftM1() == m1 && wettenListe.at(i)->getManschaftM2() == m2)
        {
            if (wettenListe.at(i)->getStart() == start && wettenListe.at(i)->getEnde() == ende)
            {
                exist = true;
            }
        }
    }

    return exist;
}

int WettenListe::count() // Autor: Christoph Rehm
{
    return this->wettenListe.size();
}
