#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"editdate.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->dbf = "D:/data.db";
    log = new Log(this);
    connectDB();
    initWidget();
    loadMoneyTree();
    loadEventToLog();
}

MainWindow::~MainWindow(){
    closeDB();
    delete log;
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
        loadUserList();
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
bool MainWindow::addMoneyDB(const QString & item, int money, const QDate & date, const QString & id){
    this->dbs = "INSERT INTO " + this->usr + "_MONEY values ('" + id + "', '" + item + "', "
            + QString::number(money) + ", '" + date.toString("yyyy-MM-dd") + "');";
    if (this->qry->exec(this->dbs)){
        return true;
    }
    else {
        qDebug() << "ERROR add item";
        return false;
    }
}

/* Delete a item by ID */
bool MainWindow::delItemMoneyDB(const QString &id){
    this->dbs = "DELETE FROM " + this->usr + "_MONEY where id='" + id + "';";
    if (this->qry->exec(this->dbs)){
        return true;
    }
    else {
        qDebug() << "ERROR in Delete Item";
        return false;
    }
}

/* Add event to Database */
bool MainWindow::addEventDB(const QDateTime & datetime, const QString & event){
    this->dbs = "INSERT INTO " + this->usr + "_EVENT values ('" +
            datetime.toString("yyyy-MM-dd HH:mm:ss") +
            "', '" + event + "');";
    if (this->qry->exec(this->dbs)){
        return true;
    }
    else {
        qDebug() << "Failed to add Event.";
        return false;
    }
}

/* Add user to Database */
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
    else {
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
    else {
        return false;
    }
}

/* Load User List */
void MainWindow::loadUserList(){
    /*LOAD USER FROM TABLE*/
    this->usrList.clear();
    this->dbs = "SELECT * FROM User;";
    if (this->qry->exec(this->dbs)){
        while(this->qry->next()){
            this->usrList.push_back(this->qry->value(0).toString());
        }
    }
    else  {
        qDebug() << "Error in reading User from database";
    }
}

/* Load user list to widget */
void MainWindow::reloadUser(){
    loadUserList();
    ui->cmUser->addItems(this->usrList);
}

/* Init Widget, connect Widgets */
void MainWindow::initWidget(){
    reloadUser();
    ui->date->setDate(QDate::currentDate());
    ui->calendar->setSelectedDate(QDate::currentDate());
    connect(ui->date,SIGNAL(dateChanged(QDate)),ui->calendar,SLOT(setSelectedDate(QDate)));
    connect(ui->calendar,SIGNAL(selectionChanged()),this,SLOT(onCalendarChange()));
    ui->moneytree->setColumnCount(3);
    QTreeWidgetItem * header = ui->moneytree->headerItem();
    header->setText(0,"Date");
    header->setText(1,"Total Money");
    header->setText(2,"ID");
    loadMoneyTree();
}

/* Add Date - Toplevel Item into moneytree */
QTreeWidgetItem *MainWindow::addDateMoneyTree(const QDate &date){
    int count = ui->moneytree->topLevelItemCount();
    for (int i = 0; i < count; i++){
        if (ui->moneytree->topLevelItem(i)->text(0) == date.toString("dd/MM/yyyy")){
            return ui->moneytree->topLevelItem(i);
        }
    }
    QTreeWidgetItem * item = new QTreeWidgetItem;
    item->setText(0,date.toString("dd/MM/yyyy"));
    ui->moneytree->addTopLevelItem(item);
    return item;
}

/* Add Money tree - Return this Item */
QTreeWidgetItem * MainWindow::addItemMoneyTree(const QString &item, int money, const QDate & date, const QString & id){
    QTreeWidgetItem * moneyitem = new QTreeWidgetItem;
    moneyitem->setText(0,item);
    moneyitem->setText(1,QString::number(money));
    moneyitem->setText(2,id);
    addDateMoneyTree(date)->addChild(moneyitem);
    ui->moneytree->setCurrentItem(moneyitem);
    updateMoneyTree(moneyitem->parent());
    return moneyitem;
}

/* Load Money Tree */
void MainWindow::loadMoneyTree(){
    QString item;
    int money;
    QString id;
    QDate date;
    ui->moneytree->clear();
    this->dbs = "SELECT * FROM " + this->usr + "_MONEY;";
    if (this->qry->exec(this->dbs)){
        while (qry->next()){
            id = qry->value(0).toString();
            item = qry->value(1).toString();
            money = qry->value(2).toInt();
            date = qry->value(3).toDate();
            addItemMoneyTree(item,money,date,id);
        }
    }
    updateMoneyTree();
}

/* Update Money for Money Tree */
void MainWindow::updateMoneyTree(QTreeWidgetItem *item){
    if (item){
        if (item->parent()){
            item = item->parent();
        }
        int count = item->childCount();
        int total = 0;
        for (int i = 0; i < count; i++){
            total += item->child(i)->text(1).toInt();
        }
        item->setText(1,QString::number(total));
    }
    else {
        int TopCount = ui->moneytree->topLevelItemCount();
        QTreeWidgetItem * topItem ;
        int childCount ;
        int total;
        for (int i = 0; i < TopCount; i++){
            topItem = ui->moneytree->topLevelItem(i);
            childCount = topItem->childCount();
            total = 0;
            for (int j = 0; j < childCount; j++){
                total += topItem->child(j)->text(1).toInt();
            }
            topItem->setText(1,QString::number(total));
        }
    }
}

/* Load Event to Log */
void MainWindow::loadEventToLog(){
    this->dbs = "SELECT * FROM " + this->usr + "_EVENT;";
    log->clear();
    if (this->qry->exec(this->dbs)){
        while (qry->next()){
            log->addEvent(qry->value(0).toString() + " | " + qry->value(1).toString());
        }
    }
}


/* On calendar click change */
void MainWindow::onCalendarChange(){
    ui->date->setDate(ui->calendar->selectedDate());
}

void MainWindow::on_btnAdd_clicked(){
    QString id ;
    QString item;
    QDate date;
    int money;
    if (ui->leItem->text() != ""){
        item = ui->leItem->text();
        money = ui->spMoney->value();
        date = ui->date->date();
        id = generateID();
        if (addMoneyDB(item,money,date,id)){
            QDateTime curnentDateTime = QDateTime::currentDateTime();
            addItemMoneyTree(item,money,date,id);
            addEventDB(curnentDateTime,"Added: " + item
                       + " $ "+ QString::number(money)
                       + " $ " + date.toString("dd/MM/yyyy"));
            ui->leItem->setText("");
            ui->spMoney->setValue(0);
            log->addEvent(curnentDateTime.toString("yyyy-MM-dd hh:mm:ss")
                          + " | Added: " + item
                          + " $ " + QString::number(money)
                          + " $ " + date.toString("dd/MM/yyyy"));
        }
        else {
            QMessageBox::critical(this,"ERROR","An Error occur, cannot add new item");
            qDebug() << "ERROR: Cannot add new item";
        }
    }
    else {
        QMessageBox::critical(this,"ERROR","Item field cannot be empty");
        qDebug() << "ERROR: Item field cannot be empty";
    }
}

void MainWindow::on_btnDelete_clicked(){
    QTreeWidgetItem * item = ui->moneytree->currentItem();
    QTreeWidgetItem * parent = item->parent();
    if (parent){
        /* If it has parent Item */
        if (QMessageBox::question(this,"Delete Item","Are You Sure Want To Delete This Item '" + item->text(0) +"' ?") == QMessageBox::Yes){
            if (delItemMoneyDB(item->text(2))){
                QDateTime currentDateTime = QDateTime::currentDateTime();
                addEventDB(currentDateTime, "Deleted: " + item->text(0)
                           + " $ " + item->text(1)
                           + " $ " + item->parent()->text(0));
                log->addEvent(currentDateTime.toString("yyyy-MM-dd hh:mm:ss")
                              + "Deleted: " + item->text(0)
                              + " $ " + item->text(1)
                              + " $ " + item->parent()->text(0));
                delete item;
                if (parent->childCount() == 0){
                    delete parent;
                }
                else {
                    updateMoneyTree(parent);
                }
            }
        }
    }
    else {
        /* if item is TopLevelItem */
        if (QMessageBox::question(this,"Delete Item","Are You Sure Want To Delete ALL Item in '"+ item->text(0) +"' ?") == QMessageBox::Yes){
            int childCount = item->childCount();
            QDateTime currentDateTime;
            for (int i = 0; i < childCount; i++){
                if (delItemMoneyDB(item->child(i)->text(2))){
                    currentDateTime = QDateTime::currentDateTime();
                    addEventDB(currentDateTime, "Deleted: " + item->child(i)->text(0)
                               + " $ " + item->child(i)->text(1)
                               + " $ " + item->text(0));
                    log->addEvent(currentDateTime.toString("yyyy-MM-dd hh:mm:ss")
                                  + " | Deleted: " + item->child(i)->text(0)
                                  + " $ " + item->child(i)->text(1)
                                  + " $ " + item->text(0));
                }
            }
            while (item->childCount()){
                delete item->child(0);
            }
            delete item;
        }
    }
}

void MainWindow::on_btnLoadUser_clicked(){
    this->usr = ui->cmUser->currentText();
    loadMoneyTree();
    loadEventToLog();
}

void MainWindow::on_btnSort_clicked(){

}

void MainWindow::on_btnLog_clicked(){
    log->show();
}

void MainWindow::on_btnEdit_clicked(){
    QTreeWidgetItem * item = ui->moneytree->currentItem();
    if (item->parent()){

    }
    else {
        QDate date = QDate::fromString(ui->moneytree->currentItem()->text(0),"dd/MM/yyyy");
        EditDate editdate(this);
        editdate.setValue(date);
        editdate.exec();
        if (editdate.change()){
            QDate newdate = editdate.getValue();
            this->dbs = "UPDATE " + this->usr +  "_money SET date = '" + newdate.toString("yyyy-MM-dd")
                    + "' WHERE date = '" + date.toString("yyyy-MM-dd") + "';";
            //qDebug() << this->dbs;
            if (qry->exec(this->dbs)){
                QDateTime currentDateTime = QDateTime::currentDateTime();
                int count = item->childCount();
                item->setText(0,newdate.toString("dd/MM/yyyy"));
                for (int i = 0; i < count; i++){
                    addEventDB(currentDateTime, "Edited: " + item->child(i)->text(0)
                               + " $ " + item->child(i)->text(1)
                               + " $ " + date.toString("dd/MM/yyyy")
                               + " -> " + newdate.toString("dd/MM/yyyy"));
                    log->addEvent(currentDateTime.toString("yyyy-MM-dd hh:mm:ss") +
                                  " | Edited: " + item->child(i)->text(0) +
                                  " $ " + item->child(i)->text(1) +
                                  " $ " + date.toString("dd/MM/yyyy") +
                                  " -> " + newdate.toString("dd/MM/yyyy"));
                }
            }
            else {
                qDebug() << "Error in edit date";
            }
        }
    }
}
