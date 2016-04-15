#include "tippliste.h"

TippListe::TippListe(int *spielID, AccountListe *accountListe) // Autor: Christoph Rehm
{
    this->spielID = spielID;
    this->accountListe = accountListe;
    this->accounts = accountListe->getPAccounts();
}

TippListe::~TippListe() // Autor: Christoph Rehm
{
    int index = this->tipps.size();
    for (int i = index-1; i >= 0 ; i--)
    {
        delete this->tipps.at(i);
        this->tipps.removeAt(i);
    }
}

void TippListe::tippListeLaden(DBHandler *p_dbHandler) // Autor: Christoph Rehm
{
    bool ok = true;
    QString error;
    QString tmpName;
    QSqlQuery qry;
    int indexAc;

    if (p_dbHandler->getTipps(&qry,&error,*spielID))
    {
        if(qry.first())
        {
            while(ok)
            {
                tmpName = qry.value(0).toString();
                for (int i = 0; accounts->size(); i++)
                {
                    if (accounts->at(i).getBenutzername() == tmpName)
                    {
                        indexAc = i;
                        break;
                    }
                }

                Tipp* tmp = new Tipp(accountListe->getAccountP(indexAc),qry.value(1).toInt(),qry.value(2).toInt(),qry.value(3).toInt());
                tipps.append(tmp);
                ok = qry.next();
            }
        }
    }
}

bool TippListe::tippErstellen(DBHandler *p_dbHandler, Account *spieler, int siegesTipp, int ergebnisTippM1, int ergebnisTippM2) // Autor: Christoph Rehm
{
    bool ok = false;
    QString error;

    if (p_dbHandler->eintragenTipp(&error,spieler->getBenutzername(), *spielID,siegesTipp,ergebnisTippM1,ergebnisTippM2))
    {
        Tipp* tmp = new Tipp(spieler,siegesTipp,ergebnisTippM1,ergebnisTippM2);
        this->tipps.append(tmp);
        ok = true;
    }

    return ok;
}

bool TippListe::tippUpdaten(DBHandler *p_dbHandler, Account *spieler, int siegesTipp, int ergebnisTippM1, int ergebnisTippM2) // Autor: Christoph Rehm
{
    bool ok = false;
    QString error;
    int indexSp = this->getIndexOf(spieler);

    if (p_dbHandler->updateTipp(&error,spieler->getBenutzername(),*spielID,siegesTipp,ergebnisTippM1,ergebnisTippM2))
    {
        tipps.at(indexSp)->setErgebnisTippM1(ergebnisTippM1);
        tipps.at(indexSp)->setErgebnisTippM2(ergebnisTippM2);
        tipps.at(indexSp)->setSiegesTipp(siegesTipp);
        ok = true;
    }

    return ok;
}

bool TippListe::tippLoeschen(DBHandler *p_dbHandler, Account *spieler)
{
    bool ok = false;
    QString error;
    int indexT = this->getIndexOf(spieler);

    if (p_dbHandler->loescheTipp(&error,spieler->getBenutzername(),*this->spielID))
    {
        Tipp* tmp = this->tipps.at(indexT);
        this->tipps.removeAt(indexT);
        delete tmp;
        ok = true;
    }
    return ok;
}


int TippListe::getSiegesTipp(Account *spieler) // Autor: Christoph Rehm
{
    return this->tipps.at(this->getIndexOf(spieler))->getSiegesTipp();
}

int TippListe::getErgebnisTippM1(Account *spieler) // Autor: Christoph Rehm
{
    return this->tipps.at(this->getIndexOf(spieler))->getErgebnisTippM1();
}

int TippListe::getErgebnisTippM2(Account *spieler) // Autor: Christoph Rehm
{
    return this->tipps.at(this->getIndexOf(spieler))->getErgebnisTippM2();
}

Tipp* TippListe::getTipp(QString benutzername) // Autor: Christoph Rehm
{
    return this->tipps.at(this->getIndexOf(benutzername));
}

int TippListe::getIndexOf(Account *spieler) // Autor: Christoph Rehm
{
    int indexTipp = -1;

    for (int i = 0; i < tipps.size(); i++)
    {
        if (tipps.at(i)->getSpieler()->getBenutzername() == spieler->getBenutzername())
        {
            indexTipp = i;
            break;
        }
    }

    return indexTipp;
}

int TippListe::getIndexOf(QString benutzername) // Autor: Christoph Rehm
{
    int indexTipp = -1;

    for (int i = 0; i < tipps.size(); i++)
    {
        if (tipps.at(i)->getSpieler()->getBenutzername() == benutzername)
        {
            indexTipp = i;
            break;
        }
    }

    return indexTipp;
}

Account *TippListe::getSpielerOf(int indexS) // Autor: Christoph Rehm
{
    return this->tipps.at(indexS)->getSpieler();
}

int TippListe::count() // Autor: Christoph Rehm
{
    return this->tipps.size();
}
