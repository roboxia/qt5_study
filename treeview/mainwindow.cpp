#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStandardItemModel>
#include "generaltreemodel.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
#if 1
    QStandardItemModel* model = new QStandardItemModel(4,4,this);


    QStandardItem* item = new QStandardItem("123");
    model->setItem(0,0,item);
    QStandardItem* item10 = new QStandardItem("10");
    QStandardItem* item11 = new QStandardItem("11");
    item->setChild(0,0,item10);
    item->setChild(1,0,item11);

    //model->setItem(1,0,item10);
    QStandardItem* item101 = new QStandardItem("101");
    QStandardItem* item102 = new QStandardItem("102");
    item10->setChild(0,0,item101);
    item11->setChild(0,0,item101);


    ui->treeView->setModel(model);

    ui->treeView->setIndentation(10);

    QStandardItem *root = model->invisibleRootItem();
    QModelIndex index = root->index();
    qDebug() << "root index: " << index;

#else
    GeneralTreeModel* model = new GeneralTreeModel(this);
    ui->treeView->setModel(model);
    model->insertColumns(0,2);
    model->insertRows(0,10);



    //model->setData(model->index(2,0,model->index(0,0)),"12355");

#endif
}

MainWindow::~MainWindow()
{
    delete ui;
}
