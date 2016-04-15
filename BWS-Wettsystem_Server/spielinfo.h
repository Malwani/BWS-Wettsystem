#ifndef SPIELINFO_H
#define SPIELINFO_H

#include <QString>

class SpielInfo
{

private:

    int     spielId;
    QString mannschaftA;
    QString mannschaftB;
    QString datum;
    bool    spielStatus;

public:
    SpielInfo();

    int getSpielId() const;
    QString getMannschaftA() const;
    QString getMannschaftB() const;
    QString getDatum()const;
    bool getSpielStatus() const;

    void setSpielInfo(int spielId, QString mannschaftA, QString mannschaftB, QString datum, bool spielStatus);
};


#endif // SPIELINFO_H
