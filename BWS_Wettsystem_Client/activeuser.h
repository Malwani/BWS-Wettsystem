#ifndef ACTIVEUSER_H
#define ACTIVEUSER_H

#include <QString>

class ActiveUser
{
public:
    ActiveUser();

private:

    QString activeUsername;
    QString email;
    QString vorname;
    QString nachname;
    QString klasse;
    QString regSeit;

public:

    void setUser(QString username);
    void setActive(QString email, QString vorname, QString nachname, QString klasse, QString regSeit);
    void setInactive();

    QString getActiveUsername() const;
    QString getEmail() const;
    QString getVorname() const;
    QString getNachname() const;
    QString getKlasse() const;
    QString getRegSeit() const;

};

#endif // ACTIVEUSER_H
