#include "spielinfo.h"

SpielInfo::SpielInfo()
{

}

QString SpielInfo::getDatum() const
{
    return this->datum;
}

int SpielInfo::getSpielId() const
{
    return this->spielId;
}

QString SpielInfo::getMannschaftA() const
{
    return this->mannschaftA;
}

QString SpielInfo::getMannschaftB() const
{
    return this->mannschaftB;
}

bool SpielInfo::getSpielStatus() const
{
    return this->spielStatus;
}

void SpielInfo::setSpielInfo(int spielId, QString mannschaftA, QString mannschaftB, QString datum, bool spielStatus)
{
    this->spielId = spielId;
    this->mannschaftA = mannschaftA;
    this->mannschaftB = mannschaftB;
    this->datum = datum;
    this->spielStatus = spielStatus;
}
