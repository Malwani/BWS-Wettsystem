#include "tipp.h"

Tipp::Tipp(Account *spieler, int siegesTipp, int ergebnisTippM1, int ergebnisTippM2) // Autor: Christoph Rehm
{
    this->spieler = spieler;
    this->siegesTipp = siegesTipp;
    this->ergebnisTippM1 = ergebnisTippM1;
    this->ergebnisTippM2 = ergebnisTippM2;
}

void Tipp::setErgebnisTippM1(int tipp) // Autor: Christoph Rehm
{
    this->ergebnisTippM1 = tipp;
}

void Tipp::setErgebnisTippM2(int tipp) // Autor: Christoph Rehm
{
    this->ergebnisTippM2 = tipp;
}

void Tipp::setSiegesTipp(int tipp) // Autor: Christoph Rehm
{
    this->siegesTipp = tipp;
}

Account* Tipp::getSpieler() // Autor: Christoph Rehm
{
    return this->spieler;
}

int Tipp::getErgebnisTippM1() // Autor: Christoph Rehm
{
    return this->ergebnisTippM1;
}

int Tipp::getErgebnisTippM2() // Autor: Christoph Rehm
{
    return this->ergebnisTippM2;
}

int Tipp::getSiegesTipp() // Autor: Christoph Rehm
{
    return this->siegesTipp;
}
