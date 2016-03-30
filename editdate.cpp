#include "editdate.h"
#include "ui_editdate.h"

EditDate::EditDate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditDate)
{
    ui->setupUi(this);
    connect(ui->date,SIGNAL(dateChanged(QDate)),ui->calendar,SLOT(setSelectedDate(QDate)));
    connect(ui->calendar,SIGNAL(selectionChanged()),this,SLOT(onDateChanged()));
    this->isChange = false;
}

EditDate::~EditDate(){
    delete ui;
}

void EditDate::onDateChanged(){
    ui->date->setDate(ui->calendar->selectedDate());
}

void EditDate::setValue(const QDate &date){
    ui->date->setDate(date);
    ui->calendar->setSelectedDate(date);
}

QDate EditDate::getValue(){
    return ui->date->date();
}

bool EditDate::change(){
    return this->isChange;
}

void EditDate::on_btnOK_clicked(){
    this->isChange = true;
    this->close();
}

void EditDate::on_btnCal_clicked(){
    this->isChange = false;
    this->close();
}
