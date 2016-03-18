#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QDateTime>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    /*For Database */
    bool connectDB();
    void closeDB();
    QString generateID();
    bool addMoneyDB(const QString & item, int money,const QDate & date);
    bool delItemMoneyDB(const QString & id);
    bool addEventDB(const QString & event);
    bool addUser(const QString  & username, const QString & description);
    bool removeUser(const QString & username);

private:
    Ui::MainWindow *ui;

    /*User*/
    QString usr;
    QStringList usrList;

    /*For Database*/
    QSqlDatabase db; // Database
    QSqlQuery * qry; // Database Query
    QString dbf;   // Database file
    QString dbs;   // Query String

};

#endif // MAINWINDOW_H
