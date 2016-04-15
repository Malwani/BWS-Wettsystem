#include "spielerinfo.h"

SpielerInfo::SpielerInfo()
{
}

QString SpielerInfo::getSpielerName() const
{
    return this->spielerName;
}

int SpielerInfo::getPunkte() const
{
    return this->punkte;
}

void SpielerInfo::setSpielerInfo(QString spielerName, int punkte)
{
    this->spielerName = spielerName;
    this->punkte = punkte;
}
