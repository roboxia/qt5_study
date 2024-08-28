#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStandardItem>
#include <QStandardItemModel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QStandardItemModel *itemModel = new QStandardItemModel(4, 2, this);
    QStandardItem *firstRow = new QStandardItem("First Row");

    ui->tableView->setModel(itemModel);

    //

}

MainWindow::~MainWindow()
{
    delete ui;
}
