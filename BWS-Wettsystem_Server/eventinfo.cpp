#include "eventinfo.h"

EventInfo::EventInfo()
{

}

void EventInfo::setEventInfo(QString eventName, QString eventStart, QString eventEnde, int preisTipsEins, int preisTipZwei)
{
    this->eventName = eventName;
    this->eventStart = eventStart;
    this->eventEnde = eventEnde;
    this->preisTipsEins = preisTipsEins;
    this->preisTipZwei = preisTipZwei;
}

QString EventInfo::getEventName() const
{
    return this->eventName;
}

QString EventInfo::getEventStart() const
{
    return this->eventStart;
}

QString EventInfo::getEventEnde() const
{
    return this->eventEnde;
}

int EventInfo::getPreisTipEins() const
{
    return this->preisTipsEins;
}

int EventInfo::getPreisTipZwei() const
{
    return this->preisTipZwei;
}
