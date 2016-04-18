#include "calcdlg.h"
#include "ui_calcdlg.h"

CalcDlg::CalcDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CalcDlg)
{
    ui->setupUi(this);
    ui->lcdNumber->setDigitCount(10);
}

CalcDlg::~CalcDlg()
{
    delete ui;
}
void CalcDlg::setValue(int val){
    ui->lcdNumber->display(val);
}
