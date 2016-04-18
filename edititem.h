#ifndef EDITITEM_H
#define EDITITEM_H

#include <QDialog>

namespace Ui {
class EditItem;
}

class EditItem : public QDialog
{
    Q_OBJECT

public:
    explicit EditItem(QWidget *parent = 0);
    ~EditItem();
    bool Change();
    void setValue(const QString & itemname, int money);
    QString getItemName();
    int getMoney();
private slots:
    void on_btnOK_clicked();

    void on_btnCan_clicked();

private:
    Ui::EditItem *ui;
    bool isChange;
};

#endif // EDITITEM_H
