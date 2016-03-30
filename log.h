#ifndef LOG_H
#define LOG_H

#include <QMainWindow>

namespace Ui {
class Log;
}

class Log : public QMainWindow
{
    Q_OBJECT

public:
    explicit Log(QWidget *parent = 0);
    ~Log();
    void addEvent(const QString & str);
    void clear();
private:
    Ui::Log *ui;
};

#endif // LOG_H
