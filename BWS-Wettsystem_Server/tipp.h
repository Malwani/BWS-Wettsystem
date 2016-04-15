#ifndef TIPP_H
#define TIPP_H

#include "accountliste.h"

class Tipp
{
public:
    Tipp(Account* spieler, int siegesTipp, int ergebnisTippM1, int ergebnisTippM2);

    Account* getSpieler();
    int getSiegesTipp();
    int getErgebnisTippM1();
    int getErgebnisTippM2();

    void setErgebnisTippM1(int tipp);
    void setErgebnisTippM2(int tipp);
    void setSiegesTipp(int tipp);

private:
    int siegesTipp;
    int ergebnisTippM1;
    int ergebnisTippM2;
    Account* spieler;
};

#endif // TIPP_H
