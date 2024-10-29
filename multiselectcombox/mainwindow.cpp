#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "multiselectcombobox.h"
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QHBoxLayout *layout = new QHBoxLayout(ui->centralwidget);


    MultiSelectComboBox *com = new MultiSelectComboBox(this);
    for(int i = 0;i<100;++i){
        com->addItem(QString::number(i));
    }
    layout->addWidget(com);

    QComboBox *com2 = new QComboBox(this);
    com2->addItems({"1","2","3"});
    layout->addWidget(com2);


}

MainWindow::~MainWindow()
{
    delete ui;
}
