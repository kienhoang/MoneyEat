#include "log.h"
#include "ui_log.h"

Log::Log(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Log)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->plainTextEdit);
}

Log::~Log(){
    delete ui;
}

void Log::addEvent(const QString &str){
    ui->plainTextEdit->insertPlainText(str + '\n');
}

void Log::clear(){
    ui->plainTextEdit->clear();
}
