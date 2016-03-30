#ifndef EDITDATE_H
#define EDITDATE_H

#include <QDialog>
#include <QDate>

namespace Ui {
class EditDate;
}

class EditDate : public QDialog
{
    Q_OBJECT

public:
    explicit EditDate(QWidget *parent = 0);
    ~EditDate();
    void setValue(const QDate & date);
    QDate getValue();
    bool change();
private slots:
    void onDateChanged();
    void on_btnOK_clicked();
    void on_btnCal_clicked();

private:
    Ui::EditDate *ui;
    bool isChange;
};

#endif // EDITDATE_H
