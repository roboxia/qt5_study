#ifndef GENERALTREEWIDGET_H
#define GENERALTREEWIDGET_H

#include <QMainWindow>
#include "generaltreemodel.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class GeneralTreeWidget;
}
QT_END_NAMESPACE

class GeneralTreeWidget : public QMainWindow
{
    Q_OBJECT

public:
    GeneralTreeWidget(QWidget *parent = nullptr);
    ~GeneralTreeWidget();

public slots:
    //void onDataChanged(

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::GeneralTreeWidget *ui;

    GeneralTreeModel* model;
};
#endif // GENERALTREEWIDGET_H
