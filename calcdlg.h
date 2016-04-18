#ifndef CALCDLG_H
#define CALCDLG_H

#include <QDialog>

namespace Ui {
class CalcDlg;
}

class CalcDlg : public QDialog
{
    Q_OBJECT

public:
    explicit CalcDlg(QWidget *parent = 0);
    ~CalcDlg();
    void setValue(int val);
private:
    Ui::CalcDlg *ui;
};

#endif // CALCDLG_H
