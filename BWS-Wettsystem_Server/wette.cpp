#include "wette.h"

Wette::Wette(AccountListe *accountListe, QString m1, QString m2, QDateTime start, QDateTime ende) // Autor: Christoph Rehm
{
    this->tippListe = new TippListe(&this->spielID,accountListe);
    this->ManschaftM1 = m1;
    this->ManschaftM2 = m2;
    this->start = start;
    this->ende = ende;
    this->punkteM1 = -1;
    this->punkteM2 = -1;
}

Wette::~Wette() // Autor: Christoph Rehm
{
    delete tippListe;
}

void Wette::setSpielID(int spielID) // Autor: Christoph Rehm
{
    this->spielID = spielID;
}

void Wette::spielTippListeLaden(DBHandler *p_dbHandler) // Autor: Christoph Rehm
{
    this->tippListe->tippListeLaden(p_dbHandler);
}

void Wette::setManschaften(QString m1, QString m2) // Autor: Christoph Rehm
{
    this->ManschaftM1 = m1;
    this->ManschaftM2 = m2;
}

void Wette::setErgebnis(int punkteM1, int punkteM2) // Autor: Christoph Rehm
{
    this->punkteM1 = punkteM1;
    this->punkteM2 = punkteM2;
}

void Wette::setZeit(QDateTime start, QDateTime ende) // Autor: Christoph Rehm
{
    this->start = start;
    this->ende = ende;
}

int Wette::getPunkteM1() // Autor: Christoph Rehm
{
    return this->punkteM1;
}

int Wette::getPunkteM2() // Autor: Christoph Rehm
{
    return this->punkteM2;
}

QString Wette::getManschaftM1() // Autor: Christoph Rehm
{
    return this->ManschaftM1;
}

QString Wette::getManschaftM2() // Autor: Christoph Rehm
{
    return this->ManschaftM2;
}

QDateTime Wette::getStart() // Autor: Christoph Rehm
{
    return this->start;
}

QDateTime Wette::getEnde() // Autor: Christoph Rehm
{
    return this->ende;
}

int Wette::getSpielID() // Autor: Christoph Rehm
{
    return this->spielID;
}

bool Wette::getStatusTippbar() // Autor: Christoph Rehm
{
    bool tippbar = false;
    QDateTime tmpDT;
    tmpDT = tmpDT.currentDateTime();

    if(this->start <= tmpDT && this->ende >= tmpDT)
    {
        tippbar = true;
    }

    return tippbar;
}

bool Wette::hatgeTippt(QString benutzername) // Autor: Christoph Rehm
{
    bool hatGeTippt = false;

    for (int i = 0; i < this->tippListe->count(); i++)
    {
        if (this->tippListe->getIndexOf(benutzername) != -1)
            hatGeTippt = true;
    }
    return hatGeTippt;
}

Tipp* Wette::getTipp(QString benutzername) // Autor: Christoph Rehm
{
    return this->tippListe->getTipp(benutzername);
}

TippListe* Wette::getTippListe() // Autor: Christoph Rehm
{
    return this->tippListe;
}

void Wette::tippListeLoesche(DBHandler* p_dbHandler) // Autor: Christoph Rehm
{
    for (int i = (tippListe->count()-1); i >= 0; i--)
    {
        tippListe->tippLoeschen(p_dbHandler,tippListe->getSpielerOf(i));
    }
}
