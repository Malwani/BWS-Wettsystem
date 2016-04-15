#include "accountliste.h"

AccountListe::AccountListe(DBHandler *p_dbHandler)
{
    this->p_dbHandler = p_dbHandler;
}

QList<Account> AccountListe::getAccounts()
{
    return this->accounts;
}

QList<Account>* AccountListe::getPAccounts() //Autor: Christoph Rehm
{
    return &accounts;
}

Account* AccountListe::getAccountP(int indexAc) //Autor: Christoph Rehm
{
    return &accounts[indexAc];
}

bool AccountListe::aktualisieren(QString* error) // Aktualisiert alle Accounteinträge aus der DB; diese Methode wird anschließend in FrmMain aufgerufen; AccountListe muss immernoch angezeigt werden, bisher nur "unsichtbar"
{
    QSqlQuery qry;

    bool ok = p_dbHandler->aktualisiereAccountListe(&qry, error); // Von qry wird mit dem '&' Operator die Adresse übergeben[was dann einen Pointer darstellt], da error schon ein Pointer ist, wird dieser so übergeben

    if(ok) // wenn in der DB-Abfrage ein Fehler passiert is steht ok auf FALSE; es wird nicht weiter verarbeitet; ok wird am Ende voa return zurückgegeben
    {
        this->accounts.clear(); // QList leeren


        while(qry.next()) // qry verarbeiten
        {
            QString benutzername = qry.value(0).toString();
            QString email        = qry.value(1).toString();
            QString vorname      = qry.value(2).toString();
            QString nachname     = qry.value(3).toString();
            QString klasse       = qry.value(4).toString();
            bool freigeschaltet  = qry.value(5).toBool();
            QString passwort     = qry.value(6).toString();
            QString regSeit      = qry.value(7).toString();

            Account tmp(benutzername, passwort, email, vorname, nachname, klasse, freigeschaltet, regSeit);
            this->accounts.append(tmp);
        }
    }

    this->ladeAdmins(error);
    this->sortiereAccounts();

    return ok; // True oder false, je nachdem ob dei DB-Abfrage erfolgreich war
}

bool AccountListe::LoginServer (QString benutzername, QString passwort, QString* error)
{
    QSqlQuery qry;
    bool ok = false;

    for(int i = 0; i < admins.length();i++)
    {
        if(benutzername != admins[i])
            *error = "Login inkorrekt.";
        else
        {
            ok = p_dbHandler->AdminPasswort(&qry, benutzername, error);

            qry.next();
            if(qry.value(0).toString() == passwort && ok) // wenn login ok und dbafrabge ok
                ok = true;
            else
            {
                *error = "Login inkorrekt.";
                ok = false;
            }
        }
    }
    return ok;
}

bool AccountListe::AccountEingabenOK(QString benutzername, QString passwort, QString vorname, QString nachname, QString email, QString klasse, QString* error)
{//Autor: Aymane Amaioua
 QString meldung; //Es existieren verschiedene Meldungen die einen Fehler zurückgeben.
 bool ok=true;
 meldung="ok";

 if(benutzername=="")
 {
     meldung="Bitte tragen Sie einen Benutzernamen ein";
 }
 if(passwort=="")
 {
     meldung="Bitte tragen Sie ein Passwort ein";
 }
 if(vorname=="")
 {
     meldung="Bitte tragen Sie Ihren Vornamen ein";
 }
 if(nachname=="")
 {
     meldung="Bitte tragen Sie Ihren Nachnamen ein";
 }
 if(email=="")
 {
     meldung="Bitte tragen Sie Ihre Email-Adresse ein";
 }
 if(klasse=="")
 {
     meldung="Bitte tragen Sie Ihre Klasse ein";
 }

 if(benutzername.size()<4) //Der Benutzername soll mindestens 4 Zeichen lang sein
 {
     meldung="Ihr Benutzername muss mindestens 4 Zeichen enthalten!";
 }

 for (int i=0; i<benutzername.size() ;i++)//Check ob anderweitgie Zeichen als Zahlen und Buchstaben im Benutzername vorhanden sind
 {
     QString zeichen=benutzername.mid(i,1);
     if(zeichen.toLower()<="a" && zeichen.toLower()>="z" || zeichen<="0" && zeichen>="9")
     {
         meldung="Im Benutzername sind Sonderzeichen nicht erlaubt!";
     }
 }

 bool addgefunden=false; //Check ob die Email Adresse überhaupt existieren könnte
 for (int i=0; i<email.size() ;i++)
 {
     QString zeichen=email.mid(i,1);
     if(zeichen=="@")
     {
         addgefunden=true;
     }
 }
if(!addgefunden)
{
    meldung="Die angegeben Email-Adresse ist ungültig bitte überprufen Sie dies!";
}

for (int i=0; i<vorname.size() ;i++)//Checkt ob anderweitgie Zeichen als Buchstaben im Vorname vorhanden sind
{
    QString zeichen=vorname.mid(i,1);
    if(zeichen.toLower()<="a" && zeichen.toLower()>="z")
    {
        meldung="Im Vorname sind Sonderzeichen oder Zahlen enthalten!";
    }
}

for (int i=0; i<nachname.size() ;i++)//Checkt ob anderweitgie Zeichen als Buchstaben im Nachname vorhanden sind
{
    QString zeichen=nachname.mid(i,1);
    if(zeichen.toLower()<="a" && zeichen.toLower()>="z")
    {
        meldung="Im Nachname sind Sonderzeichen oder Zahlen enthalten!";
    }
}

if(passwort.size()<6)//Check ob das Passwort nicht kürzer als 6 Zeichen ist.
{
    meldung="Das passwort muss mindestens 6 Zeichen lang sein!";
}

bool zahl=false; //Variable für den Check im passwort ob mindestens eine Zahl vorhanden ist
bool buchstabe=false; //Variable für den Check im passwort ob mindestens ein Buchstabe vorhaned ist
for (int i=0; i<passwort.size() ;i++)//Geht jeden einzelnen Buchstaben durch und checkt ihn
{
    QString zeichen=passwort.mid(i,1);
    if(zeichen.toLower()>="a" && zeichen.toLower()<="z")
    {
       buchstabe=true;
    }
    if(zeichen>="0" && zeichen<="9")
    {
       zahl=true;
    }
}
if(!zahl||!buchstabe)
{
    meldung="Das Passwort muss aus ein Gemisch von Zahlen und Buchstaben bestehen!";
}

if(meldung!="ok")
{
*error=meldung;
ok=false;
}
return ok;//Gibt ok also true oder eine Fehlermeldung false zurück
}


bool AccountListe::AdminEingabeOK(QString benutzername, QString passwort, QString* error)
{
    QString meldung; //Es existieren verschiedene Meldungen die einen Fehler zurückgeben.
    bool ok=true;
    meldung="ok";

    if(benutzername=="")
    {
        meldung="Bitte tragen Sie einen Benutzernamen ein";
    }
    if(passwort=="")
    {
        meldung="Bitte tragen Sie ein Passwort ein";
    }

    if(benutzername.size()< 4) //Der Benutzername soll mindestens 4 Zeichen lang sein
    {
        meldung="Ihr Benutzername muss mindestens 4 Zeichen enthalten!";
    }

    for (int i=0; i<benutzername.size() ;i++)//Check ob anderweitgie Zeichen als Zahlen und Buchstaben im Benutzername vorhanden sind
    {
        QString zeichen=benutzername.mid(i,1);
        if(zeichen.toLower()<="a" && zeichen.toLower()>="z" || zeichen<="0" && zeichen>="9")
        {
            meldung="Im Benutzername sind Sonderzeichen nicht erlaubt!";
        }
    }
    if(passwort.size()<6)//Check ob das Passwort nicht kürzer als 6 Zeichen ist.
    {
        meldung="Das passwort muss mindestens 6 Zeichen lang sein!";
    }

    bool zahl=false; //Variable für den Check im passwort ob mindestens eine Zahl vorhanden ist
    bool buchstabe=false; //Variable für den Check im passwort ob mindestens ein Buchstabe vorhaned ist
    for (int i=0; i<passwort.size() ;i++)//Geht jeden einzelnen Buchstaben durch und checkt ihn
    {
        QString zeichen=passwort.mid(i,1);
        if(zeichen.toLower()>="a" && zeichen.toLower()<="z")
        {
           buchstabe=true;
        }
        if(zeichen>="0" && zeichen<="9")
        {
           zahl=true;
        }
    }
    if(!zahl||!buchstabe)
    {
        meldung="Das Passwort muss aus ein Gemisch von Zahlen und Buchstaben bestehen!";
    }

    if(meldung!="ok")
    {
    *error=meldung;
    ok=false;
    }
    return ok;
}

bool AccountListe::AdminAnlegen(QString benutzername, QString passwort, QString* error)//Autor: Aymane Amaioua
{
    bool ok = false;

    if(this->AdminEingabeOK(benutzername, passwort, error))
    {
        bool benutzerexistiert=false;

        for (int i=0; i<admins.length()&&!benutzerexistiert;i++)
        {
            if(benutzername == admins[i])
            {
                *error = "Der gegebene Benutzername existiert bereits.";
                benutzerexistiert = true;
            }
        }

        if(!benutzerexistiert)
        {
            if(this->p_dbHandler->Adminhinzu(benutzername, passwort, error))
            {
                ok=true;
                this->admins.append(benutzername);
            }
        }
    }

    return ok;
}


bool AccountListe::loginClient(QString benutzername, QString passwort, QString* error)
{
    bool ok = true;
    bool gefunden = false;
    for(int i=0; i<accounts.length()&&!gefunden;i++)
    {

        if(benutzername == accounts[i].getBenutzername()&&passwort == accounts[i].getPasswort())
        {
                gefunden = true;
        }
    }

    if(gefunden == false)
    {
        ok = false;
        *error = "Benutzername und Passwort stimmen nicht über ein!";
    }

    return ok;
}

bool AccountListe::neuenAccountAnlegen(QString benutzername, QString passwort, QString vorname, QString nachname, QString email, QString klasse, QString* error)
{
    /*Autor Aymane Amaioua
     */
    bool ok=true;
    QString meldung;
    meldung ="ok";      /*Es existieren verschiedene Methoden die als Integer zurückgegeben werden
                        Meldung 1 Alles hat funktioniert //ok
                        Meldung 2 Email-Adresse existiert bereits!
                        Meldung 3 Benutzername existiert bereits!
                        Meldung 4 Email-Adresse und Benutzername existieren bereits!*/
    if(AccountEingabenOK(benutzername, passwort, vorname, nachname, email, klasse, error))//Check ob keine Fehler in den Eingaben sind
    {//großes if anfang
    bool emailexistiert=false;      //Variable zum Check ob die EMail-Adresse schon existiert.
    bool benutzerexistiert=false;   //Variable zum Check ob der Benutzername schon existiert.
    bool gefunden=false;            //Variable zum Check ob Email oder Benutzer schon existiert.
    bool allesgefunden=false;       //Variable zum Check ob Email und Benutzer schon existiert.

    for(int i=0; i<accounts.length()&&!allesgefunden;i++)//Schleife um alle vorhandenen accounts.
    {       //for Schleife anfang
        if(benutzername==accounts[i].getBenutzername()||email==accounts[i].getEmail())//Checkt ob EMail oder Benutzername bereits verwendet werden.
        {   //if1 anfang
            if(benutzername==accounts[i].getBenutzername())
            {
                benutzerexistiert=true;//Ist benutzerexistiert=true kommt die Fehlermeldung "Benutzername existiert bereits!"
                gefunden=true;
            }
            if(email==accounts[i].getEmail())
            {
                emailexistiert=true;//Ist emailexistiert=true kommt die Fehlermeldung "Email-Adresse existiert bereits!"
                gefunden=true;
            }
            if(benutzerexistiert&&emailexistiert)
            {
                allesgefunden=true;//Bricht die Schleife ab wenn beides existiert
            }
        }//if1 ende
    }       //for Schleife ende

    if(!gefunden)//Email Adresse und Benutzername werden noch nicht verwendet
    {
        this->p_dbHandler->registrierung(benutzername, passwort, vorname, nachname, email, klasse, error);//Daten werden in die Datenbank aufgenommen
        meldung="ok";
    }

    else
    { //else anfang
       if(emailexistiert&&benutzerexistiert)
       {
           meldung="Email-Adresse und Benutzername existieren bereits!";
       }
       else
       { //else2 anfang
           if(emailexistiert&&!benutzerexistiert)
           {
               meldung="Email-Adresse existiert bereits!";
           }
           if(!emailexistiert&&benutzerexistiert)
           {
               meldung="Benutzername existiert bereits!";
           }
       }   //else2 ende
    } //else ende

    if(meldung!="ok")
    {
    *error=meldung;
    ok=false;
    }
    if(!aktualisieren(error)) //Aktualiesiert die Liste und Checkt nochmal ob eine Abfrage möglich war
    {
        ok=false;
    }
   }//großes if ende

    else
    {
        ok=false;
    }

    this->sortiereAccounts();

    return ok; //Gibt ok also true oder die jeweilige Fehlermeldung Meldung false zurück
}

bool AccountListe::AccountFreischaltenSperren(bool freigeschaltet, QString benutzername, QString *error)//Autor: Aymane Amaioua
{
    bool ok = true;//Solange ok true ist existieren keine Fehler
    int userIndex;

    // UserIndex ermitteln
    for(int i = 0; i < this->accounts.length();i++)
    {
        if(this->accounts.at(i).getBenutzername() == benutzername)
        {
            userIndex = i;
            break;
        }
    }

    // entsprechend schalten
    if(freigeschaltet)
    {
        if(!this->p_dbHandler->Sperren(benutzername, error))
            ok = false;
        else
            this->accounts[userIndex].setFreigeschaltet(false);
    }
    else
    {
        if(!this->p_dbHandler->Freischalten(benutzername, error))
            ok = false;
        else
            this->accounts[userIndex].setFreigeschaltet(true);
    }

    return ok;
}

/*
  Funktion: Sortiert alle Accountinstanzen der AccountListe alphabetisch
  Autor: Benjamin Bardroff
  Datum: 09.04.2016
  */
void AccountListe::sortiereAccounts()
{
    // Wurde getauscht?
    bool switched;
    // aktueller/nächster Score
    QString nameA, nameB;
    Account tmpAccount("","","","","","",false, "");

    // wird verlassen sobald keine Zahlen mehr getauscht werden müssen (Sortierter Zustand)
    do
    {
        switched = false;

        // Ein Durchlauf aller Indexpostionen des Account-Arrays
        for(int i = 0; i < this->accounts.length()- 1;i++)
        {
            // "aktueller" name
            nameA = this->accounts.at(i).getNachname() + this->accounts.at(i).getVorname();
            // nächster Name
            nameB = this->accounts.at(i + 1).getNachname() + this->accounts.at(i + 1).getVorname();

            // aktuelle Score größer als nächster Score?
            if(nameA < nameB)
            {
                // Scores werden direkt im Array getauscht
                tmpAccount = this->accounts.at(i);
                this->accounts[i] = this->accounts.at(i + 1);
                this->accounts[i + 1] = tmpAccount;
                switched = true;
            }
        }
    }while( switched );
}

bool AccountListe::AccountLoeschen(QString benutzername, QString *error)//Autor: Aymane Amaioua
{
    bool ok = true;
    bool gefunden=false;

    for (int i = 0; i < accounts.length()&&!gefunden; i++)
    {
        if(benutzername == accounts[i].getBenutzername())
        {

            if(!this->p_dbHandler->loeschen(benutzername, error))
                ok = false;
            else
            {
              accounts.removeAt(i);
              gefunden=true;
            }
        }
    }

    this->sortiereAccounts();

    return ok;
}

/*
  Funktion: AccountLöschung auf Clientanfrage sowie vollständiges ErrorHandling
  Autor: Aymane, Amaioua, Benjamin Bardroff
  Datum: 09.04.2016
  */
bool AccountListe::ClientAccountLoeschen(QString benutzername, QString passwort, QString* error)
{
    bool ok = false;

    int accountIndex = -1;

    for(int i = 0; i < accounts.length();i++) // Nutzer suchen
    {
        if(benutzername == accounts[i].getBenutzername())
        {
            accountIndex = i;
            break;
        }
    }

    if(accountIndex == -1) // Wenn Nutzer nicht gefunden
        *error = "Benutzer [" + benutzername + "] nicht gefunden.";
    else if(passwort != this->accounts.at(accountIndex).getPasswort()) // wenn passwort nicht korrekt
        *error = "Passwort nicht korrekt!";
    else // wenn passwort korrekt
    {
        if(this->p_dbHandler->loeschen(benutzername, error)) // Wenn DBAbfrgae ok dann ok=true sonst bleib ok = false und erro kann ausgelsen werden
        {
            // Konnten Dtaen in DB geschrieben werden? Dann auch in AccountlistInstanz

            this->accounts.removeAt(accountIndex);
            ok = true;
        }
    }

    this->sortiereAccounts();

    return ok;
}

bool AccountListe::AdminLoeschen(QString benutzername, QString* error)//Autor: Aymane Amaioua
{
    bool ok = true;

    for(int i=0; i < admins.length(); i++)
    {
        if(benutzername == admins[i])
        {
            if(!this->p_dbHandler->adminloeschen(benutzername, error))
                ok = false;
            else
            {
                admins.removeAt(i);
                break;
            }
        }
    }

    return ok;
}

/*
  Funktion: passwortÄnderung auf Clientanfrage sowie vollständiges ErrorHandling
  Autor: Aymane, Amaioua, Benjamin Bardroff
  Datum: 09.04.2016
  */
bool AccountListe::PasswortAenderung(QString benutzername, QString passwort, QString newpasswort, QString* error)
{
    bool ok = false;

    int accountIndex = -1;

    for(int i = 0; i < accounts.length();i++) // Nutzer suchen
    {
        if(benutzername == accounts[i].getBenutzername())
        {
            accountIndex = i;
            break;
        }
    }

    if(accountIndex == -1) // Wenn Nutzer nicht gefunden
        *error = "Benutzer [" + benutzername + "] nicht gefunden.";
    else if(passwort != this->accounts.at(accountIndex).getPasswort()) // wenn passwort nicht korrekt
        *error = "Passwort nicht korrekt!";
    else // wenn passwort korrekt
    {
        if(!this->AccountEingabenOK(benutzername, newpasswort, accounts[accountIndex].getVorname(),accounts[accountIndex].getNachname(),accounts[accountIndex].getEmail(),accounts[accountIndex].getKlasse(), error)) // Sind Daten ok?
            *error = "Accountdaten ungültig.";
        else if(this->p_dbHandler->passwortaenderung(benutzername, newpasswort, error)) // Wenn DBAbfrgae ok dann ok=true sonst bleib ok = false und erro kann ausgelsen werden
        {
            // Konnten Dtaen in DB geschrieben werden? Dann auch in AccountlistInstanz

            this->accounts[accountIndex].setPass(newpasswort);
            ok = true;
        }
    }

    return ok;
}

// Autor: Aymane Amaioua, Benjamin Bardroff
Account AccountListe::SpielerInfo(QString benutzername, QString* error)
{
    const Account *tmp = NULL;

    for(int i = 0; i < accounts.length(); i++)
    {
        if(benutzername == accounts[i].getBenutzername())
        {
            tmp = &accounts.at(i);
            break;
        }

    }

    if(tmp == NULL)
        *error = "Der jeweilige Account wurde nicht gefunden!";

    return *tmp;
}

/*
  Funktion: EmailÄnderung auf Clientanfrage sowie vollständiges ErrorHandling
  Autor: Aymane, Amaioua, Benjamin Bardroff
  Datum: 09.04.2016
  */
bool AccountListe::EmailAenderung(QString benutzername, QString passwort, QString newmail, QString* error)
{
    bool ok = false;

    int accountIndex = -1;

    for(int i = 0; i < accounts.length();i++) // Nutzer suchen
    {
        if(benutzername == accounts[i].getBenutzername())
        {
            accountIndex = i;
            break;
        }
    }

    if(accountIndex == -1) // Wenn Nutzer nicht gefunden
        *error = "Benutzer [" + benutzername + "] nicht gefunden.";
    else if(passwort != this->accounts.at(accountIndex).getPasswort()) // wenn passwort nicht korrekt
        *error = "Passwort nicht korrekt!";
    else // wenn passwort korrekt
    {
        if(!this->AccountEingabenOK(benutzername, passwort, accounts[accountIndex].getVorname(),accounts[accountIndex].getNachname(),newmail,accounts[accountIndex].getKlasse(), error)) // Sind Daten ok?
            *error = "Accountdaten ungültig.";
        else if(this->p_dbHandler->emailaenderung(benutzername, newmail, error)) // Wenn DBAbfrgae ok dann ok=true sonst bleib ok = false und erro kann ausgelsen werden
        {
            // Konnten Dtaen in DB geschrieben werden? Dann auch in AccountlistInstanz

            this->accounts[accountIndex].setMail(newmail);
            ok = true;
        }
    }

    return ok;
}

bool AccountListe::AccountAendern(QString benutzername, QString newbenutzername, QString vorname, QString nachname, QString klasse, QString email, QString *error)
{
    //Autor: Aymane Amaioua
    bool ok = true;

    QString passwort="Checker12345"; //Da es unnötig ist eine zweite AccountEingabenOK methode zu schreiben gebe ich ihr einfach ein passwort das es aktzeptiert

    if(!AccountEingabenOK(newbenutzername, passwort, vorname, nachname, email, klasse, error))
        ok = false;
    else
    {
        if(!this->p_dbHandler->aendern(benutzername, newbenutzername, vorname, nachname, klasse, email, error))
            ok = false;
        else
        {
            for(int i = 0; i < accounts.length(); i++)
            {
                if(benutzername == accounts[i].getBenutzername())
                {
                    Account geaendert(newbenutzername, accounts[i].getPasswort(), email, vorname, nachname, klasse, accounts[i].getFreigeschaltet(), accounts[i].getRegDatum());
                    accounts[i] = geaendert;
                    break;
                }
            }
        }
    }

    this->sortiereAccounts();

    return ok;
}

QList<QString> AccountListe::getAdmins()//Autor: Aymane Amaioua
{
    return this->admins;
}

bool AccountListe::ladeAdmins(QString *error)//Autor: Aymane Amaioua
{
    QSqlQuery qry;

    bool ok = p_dbHandler->alleadmins(&qry, error);
    if (ok)
    {
        this->admins.clear();

        while(qry.next())
        {
            QString benutzername = qry.value(0).toString();
            admins.append(benutzername);
        }
    }
    return ok;
}
