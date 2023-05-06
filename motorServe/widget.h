#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "jlinkapi.h"
#include <QApplication>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr,jlinkAPI *debugDLL = nullptr);
    ~Widget();

    void analysisRTT();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Widget *ui;


    jlinkAPI *_debugDLL;


};

#endif // WIDGET_H
