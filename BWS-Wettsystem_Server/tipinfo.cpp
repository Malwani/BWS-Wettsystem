#include "tipinfo.h"

TipInfo::TipInfo()
{

}

QString TipInfo::getMannschaftA() const
{
    return this->mannschaftA;
}

QString TipInfo::getMannschaftB() const
{
    return this->mannschaftB;
}

QString TipInfo::getDatum() const
{
    return this->datum;
}

bool TipInfo::getSpielStatus() const
{
    return this->spielStatus;
}

int TipInfo::getTipEins() const
{
    return this->tipEins;
}
int TipInfo::getTipZweiA() const
{
    return this->tipZweiA;
}
int TipInfo::getTipZweiB() const
{
    return this->tipZweiB;
}


void TipInfo::setTipInfo(QString mannschaftA, QString mannschaftB, QString datum, bool spielStatus, int tipEins, int tipZweiA, int tipZweiB)
{
    this->mannschaftA = mannschaftA;
    this->mannschaftB = mannschaftB;
    this->datum       = datum;
    this->spielStatus = spielStatus;
    this->tipEins     = tipEins;
    this->tipZweiA    = tipZweiA;
    this->tipZweiB    = tipZweiB;
}
