/*
  Modul: verbindungsaufbau.h
  Funktion: 
  Autor: Christoph Rehm
  Datum: 
  */

#ifndef VERBINDUNGSAUFBAU_H
#define VERBINDUNGSAUFBAU_H

#include <QWidget>
#include <QtXml>
#include <QFile>
#include <QDebug>
#include <QMessageBox>

#include "info.h"
#include "clienthandler.h"

namespace Ui {
class Verbindungsaufbau;
}

class Verbindungsaufbau : public QWidget
{
    Q_OBJECT

public:
    explicit Verbindungsaufbau(QWidget *parent = 0);
    ~Verbindungsaufbau();

    void takeClientHandler(ClientHandler* p_clientHandler);

private slots:
    void on_btnVerbinden_clicked(); 

    void on_btnHilfe_clicked();

    void on_btnInfo_clicked();

private:
    Ui::Verbindungsaufbau *ui;
    bool xmlFileErstellen();
    ClientHandler* p_clientHandler;
    Info i;

signals:
    void openFrmMain(bool);
    void openLogin(bool);
    void openRegister(bool);

public slots:

    void showWindow(bool mode);

};

#endif // VERBINDUNGSAUFBAU_H
