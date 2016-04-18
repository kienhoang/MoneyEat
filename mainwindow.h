#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QDateTime>
#include <QMessageBox>
#include <QTreeWidget>
#include <log.h>
#include <QFileDialog>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    /* For Database */
    bool connectDB(); // Connect Database
    void closeDB();   // Close conection database
    QString generateID();   // Generate ID for insert money
    bool addMoneyDB(const QString & item, int money,const QDate & date, const QString & id); // Add money to Database
    bool delItemMoneyDB(const QString & id); // Delete a Item from Database
    bool addEventDB(const QDateTime & datetime, const QString & event);  // Add a event to Database
    bool addUser(const QString  & username, const QString & description); // Add a user
    bool removeUser(const QString & username);  // Remove a user
    void loadUserList();
    void changeDB(const QString & dbf);
    /* For Widgets */
    void loadInfo();
    void reloadUser();
    void initWidget();
    QTreeWidgetItem * addDateMoneyTree(const QDate & date);
    QTreeWidgetItem * addItemMoneyTree(const QString &item, int money, const QDate & date, const QString & id);
    void loadMoneyTree();
    void updateMoneyTree(QTreeWidgetItem * item = 0);
    void loadEventToLog();

private slots:
    void onCalendarChange();
    void on_btnAdd_clicked();
    void on_btnDelete_clicked();
    void on_btnLoadUser_clicked();
    void on_btnSort_clicked();
    void on_btnLog_clicked();
    void on_btnEdit_clicked();

    void on_btnCalc_clicked();

    void on_actionExit_triggered();

    void on_actionNew_Database_triggered();

    void on_actionConnect_Database_2_triggered();

private:
    Ui::MainWindow *ui;

    /*User*/
    QString usr; // Current username
    QStringList usrList; // List of Username: loaded by connectDB()

    /*For Database*/
    QSqlDatabase db; // Database
    QSqlQuery * qry; // Database Query
    QString dbf;   // Database file
    QString dbs;   // Query String
    /* Log Window */
    Log * log;

};

#endif // MAINWINDOW_H
