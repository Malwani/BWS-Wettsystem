#ifndef EVENTSPIELER_H
#define EVENTSPIELER_H

#include "account.h"

class EventSpieler
{
public:
    EventSpieler(Account* pSpieler);

    void setPunkte(int punkte);
    void setRichdigeErgebnisTipps(int anzahl);
    void setRichdigeSiegesTipps(int anzahl);
    void addErgebnisTipp(int punkte);
    void addSiegesTipp(int punkte);
    void subErgebnisTipp(int punkte);
    void subSiegesTipp(int punkte);

    int getPunkte();
    int getRichdigeErgebnisTipps();
    int getRichdigeSiegesTipps();
    Account* getSpieler();

private:
    int punkte;
    int richdigeErgebnisTipps;
    int richdigeSiegesTipps;
    Account* pSpieler;


};

#endif // EVENTSPIELER_H
