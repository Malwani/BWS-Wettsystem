#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QDesktopServices>
#include <QUrl>
#include "dbhandler.h"
#include "info.h"
#include "accountliste.h"
#include <QCryptographicHash>

namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(DBHandler* p_dbHandler, AccountListe* p_accountListe, QWidget *parent = 0);
    ~Login();

private:
    Ui::Login *ui;
    DBHandler* p_dbHandler;
    AccountListe* p_accountListe;
    Info i;

signals:
    void openFrmMain(bool);


private slots:
    void on_btnLogin_clicked();

    void on_btnInfo_clicked();

    void on_btnHilfe_clicked();

public slots:
    void showWindow(bool mode);


};

#endif // LOGIN_H
