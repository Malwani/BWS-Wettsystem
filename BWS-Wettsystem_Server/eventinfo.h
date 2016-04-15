#ifndef EVENTINFO_H
#define EVENTINFO_H

#include <QString>

class EventInfo
{
public:
    EventInfo();

    void setEventInfo(QString eventName, QString eventStart, QString eventEnde, int preisTipsEins, int preisTipZwei);

    QString getEventName() const;
    QString getEventStart() const;
    QString getEventEnde() const;
    int getPreisTipEins() const;
    int getPreisTipZwei() const;

private:

    QString eventName;
    QString eventEnde;
    QString eventStart;
    int preisTipsEins;
    int preisTipZwei;

};

#endif // EVENTINFO_H
