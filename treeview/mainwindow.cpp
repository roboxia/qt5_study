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
#if 0
    QStandardItemModel* model = new QStandardItemModel(4,4,this);


    QStandardItem* item = new QStandardItem("123");
    model->setItem(0,0,item);
    QStandardItem* item10 = new QStandardItem("10");
    QStandardItem* item11 = new QStandardItem("11");
    item->setChild(0,0,item10);
    item->setChild(0,1,item11);

    //model->setItem(1,0,item10);
    QStandardItem* item101 = new QStandardItem("101");
    QStandardItem* item102 = new QStandardItem("102");
    item10->setChild(0,0,item101);

    model->setItem(3,1,item102);

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

    QModelIndex index = model->index(0,0);
    model->insertRows(0,3,index);

    QModelIndex index2 = model->index(0,0,index);
    model->insertRows(0,3,index2);

    TableRowItem* item = static_cast<TableRowItem*>(index2.internalPointer());




    model->setData(model->index(0,0),"123");
    model->setData(model->index(0,1),"1233");
    model->setData(model->index(0,2),"12333");

    TableRowItem *otem2 = static_cast<TableRowItem*>(model->index(0,0).internalPointer());

    model->setData(model->index(1,0),"12344");
    model->setData(model->index(1,1),"123444");
    model->setData(model->index(1,2),"1234444");

    //model->setData(model->index(2,0,model->index(0,0)),"12355");

#endif
}

MainWindow::~MainWindow()
{
    delete ui;
}
