#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStandardItem>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QDebug>
#include "mymodel.h"
#include "mydelegate.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    MyModel *model = new MyModel(this);
    model->insertRows(0,3,QModelIndex());
    model->insertColumns(0,3,QModelIndex());
    model->insertRows(1,3,QModelIndex());
    model->insertRows(model->rowCount(),5);

    MyDelegate *delegate = new MyDelegate(this);


    ui->tableView->setModel(model);
    ui->tableView->setItemDelegateForColumn(0,delegate);
    ui->tableView->setItemDelegateForColumn(1,delegate);

    QHeaderView *header = ui->tableView->verticalHeader();
    connect(header,&QHeaderView::sectionDoubleClicked,this,&MainWindow::onHeaderDoubleClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onHeaderDoubleClicked(int logicalIndex)
{
    qDebug()<<logicalIndex;

    QModelIndex index = ui->tableView->model()->index(logicalIndex + 1, 0);  // 假设查看该行的第1列
    QRect rect = ui->tableView->visualRect(index);

    if(rect.isValid())
        ui->tableView->hideRow(logicalIndex + 1);
    else
        ui->tableView->showRow(logicalIndex + 1);
}
