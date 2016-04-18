#include "edititem.h"
#include "ui_edititem.h"

EditItem::EditItem(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditItem)
{
    ui->setupUi(this);
    this->isChange = false;
}

EditItem::~EditItem()
{
    delete ui;
}

bool EditItem::Change(){
    return this->isChange;
}

void EditItem::setValue(const QString &itemname, int money){
    ui->leItem->setText(itemname);
    ui->spMoney->setValue(money);
}

QString EditItem::getItemName(){
    return ui->leItem->text();
}

int EditItem::getMoney(){
    return ui->spMoney->value();
}


void EditItem::on_btnOK_clicked()
{
    this->isChange = true;
    this->close();
}

void EditItem::on_btnCan_clicked()
{
    this->isChange = false;
    this->close();
}
