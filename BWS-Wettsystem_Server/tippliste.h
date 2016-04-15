#ifndef TIPPLISTE_H
#define TIPPLISTE_H

#include "tipp.h"

class TippListe
{
public:
    TippListe(int* spielID, AccountListe* accountListe);
    ~TippListe();

    void tippListeLaden(DBHandler* p_dbHandler);
    bool tippErstellen(DBHandler* p_dbHandler, Account *spieler, int siegesTipp, int ergebnisTippM1, int ergebnisTippM2);
    bool tippUpdaten(DBHandler* p_dbHandler, Account *spieler, int siegesTipp, int ergebnisTippM1, int ergebnisTippM2);
    bool tippLoeschen(DBHandler* p_dbHandler, Account *spieler);

    int getIndexOf(Account* spieler);
    int getIndexOf(QString benutzername);
    Account *getSpielerOf(int indexS);
    int getSiegesTipp(Account* spieler);
    int getErgebnisTippM1(Account* spieler);
    int getErgebnisTippM2(Account* spieler);

    Tipp* getTipp(QString benutzername);

    int count();

private:
    int* spielID;
    QVector<Tipp*> tipps;
    AccountListe *accountListe;
    QList<Account> *accounts;
};

#endif // TIPPLISTE_H
