#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStandardItemModel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QStandardItemModel* model = new QStandardItemModel(4,4,this);

    QStandardItem* item = new QStandardItem("123");
    model->setItem(0,0,item);
    QStandardItem* item10 = new QStandardItem("10");
    QStandardItem* item11 = new QStandardItem("11");
    item->setChild(0,0,item10);
    item->setChild(0,1,item11);

    //model->setItem(1,0,item10);
    QStandardItem* item101 = new QStandardItem("101");
    item11->setChild(0,0,item101);

    ui->treeView->setModel(model);

    ui->treeView->setIndentation(10);
}

MainWindow::~MainWindow()
{
    delete ui;
}
