#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->dbf = "D:/data.db";
    connectDB();
    addMoneyDB("Thit",25000,QDate::currentDate());
    addEventDB("Mua thi mat 15k");
    closeDB();
}

MainWindow::~MainWindow(){
    delete ui;
}

/* Connect Database */
bool MainWindow::connectDB(){
    this->db = QSqlDatabase::addDatabase("QSQLITE");
    this->db.setDatabaseName(this->dbf);
    if (this->db.open()){
        this->qry = new QSqlQuery(db);

        // Create table User: USERNAME | DESCRIPTION
        this->dbs = "CREATE TABLE IF NOT EXISTS user (username VARCHAR NOT NULL PRIMARY KEY, des VARCHAR);";
        if (!this->qry->exec(this->dbs)){
            qDebug() << "Error in create table User";
            return false;
        }
        /*LOAD USER FROM TABLE*/
        this->usrList.clear();
        this->dbs = "SELECT * FROM User;";
        if (this->qry->exec(this->dbs)){
            while(this->qry->next()){
                this->usrList.push_back(this->qry->value(0).toString());
            }
        }else{
            qDebug() << "Error in reading User from database";
            return false;
        }

        // Create User data table: Money Table: ID | ITEM | MONEY | DATE
        //                         Event Table: DATETIME | EVENT
        for (QStringList::iterator it = this->usrList.begin(); it != this->usrList.end(); it++){
            // CREATE TABLE MONEY ID | ITEM | MONEY | DATE
            this->dbs = "CREATE TABLE IF NOT EXISTS " +
                    *it + "_MONEY (id VARCHAR NOT NULL PRIMARY KEY,"
                                  "item VARCHAR, money INTEGER, date DATE);";
            if (!this->qry->exec(this->dbs)){
                qDebug() << "ERROR in CREATING MONEY TABLE FOR " + *it ;
                return false;
            }
            // CREATE TABLE EVENT : DATETIME | EVENT
            this->dbs = "CREATE TABLE IF NOT EXISTS " +
                    *it + "_EVENT (datetime DATETIME, VARCHAR EVENT);";
            if (!this->qry->exec(this->dbs)){
                qDebug() << "ERROR in CREATING EVENT TABLE FOR " + *it;
                return false;
            }
        }
        if (this->usrList.count()){
            this->usr = * usrList.begin();
        }
        return true;
    }else{
        return false;
    }
}
/* Close database connection*/
void MainWindow::closeDB(){
    this->db.close();
    this->db.removeDatabase(QSqlDatabase::defaultConnection);
}

/* Generate ID for add money*/
QString MainWindow::generateID(){
    return QDateTime::currentDateTime().toString("yyyyMMddHHmmss");
}

/* Add money to Database*/
bool MainWindow::addMoneyDB(const QString & item, int money, const QDate & date){
    this->dbs = "INSERT INTO " + this->usr + "_MONEY values ('" + generateID() + "', '" + item + "', "
            + QString::number(money) + ", '" + date.toString("yyyy-MM-dd") + "');";
    if (this->qry->exec(this->dbs)){
        return true;
    }
    else{
        qDebug() << "ERROR add item";
        return false;
    }
}

/* Delete a item by ID */
bool MainWindow::delItemMoneyDB(const QString &id){
    this->dbs = "DELETE FROM " + this->usr + "_MONEY where id='" + id + "';";
    if (!this->qry->exec(this->dbs)){
        return true;
    }
    else {
        qDebug() << "ERROR in Delete Item";
        return false;
    }
}

bool MainWindow::addEventDB(const QString & event){
    this->dbs = "INSERT INTO " + this->usr + "_EVENT values ('" +
            QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") +
            "', '" + event + "');";
    if (this->qry->exec(this->dbs)){
        return true;
    }
    else {
        qDebug() << "Failed to add Event.";
        return false;
    }
}

bool MainWindow::addUser(const QString & username, const QString & description){
    this->dbs = "INSERT INTO user values ('" + username + "', '" + description + "');";
    if (this->qry->exec(this->dbs)){
        this->dbs = "CREATE TABLE IF NOT EXISTS " +
                username + "_MONEY (id VARCHAR NOT NULL PRIMARY KEY,"
                              "item VARCHAR, money INTEGER, date DATE);";
        if (!this->qry->exec(this->dbs)){
            qDebug() << "ERROR in create table money for " + username;
            return false;
        }
        this->dbs = "CREATE TABLE IF NOT EXISTS " +
                username + "_EVENT (datetime DATETIME, VARCHAR EVENT);";
        if (!this->qry->exec(this->dbs)){
            qDebug() << "Error in create table event for " + username;
            return false;
        }
        return true;
    }
    else{
        qDebug() << "Error in add user";
        return false;
    }
}

/* Remove a user */
bool MainWindow::removeUser(const QString &username){
    this->dbs = "DELETE FROM TABLE user WHERE username='" + username + "';";
    if (this->qry->exec(this->dbs)){
        this->dbs = "DROP TABLE " + username + "_MONEY;";
        if (!this->qry->exec(this->dbs)){
            qDebug() << "Cannot Drop table MONEY for " + username;
        }
        this->dbs = "DROP TABLE " + username + "_EVENT;";
        if (!this->qry->exec(this->dbs)){
            qDebug() << "Cannot Drop table Event for " + username;
        }
        return true;
    }
    else{
        return false;
    }
}
