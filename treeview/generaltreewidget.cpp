#include "generaltreewidget.h"
#include "ui_generaltreewidget.h"

#include "generaltreemodel.h"
#include <QDebug>


GeneralTreeWidget::GeneralTreeWidget(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GeneralTreeWidget)
{
    ui->setupUi(this);
    model = new GeneralTreeModel(this);

    for(int i =0;i<10;i++){
        model->setHeaderData(i,Qt::Vertical,"123");
        model->setHeaderData(i,Qt::Horizontal,"123");
    }


    ui->treeView->setModel(model);
    model->insertColumns(0,5);
}

GeneralTreeWidget::~GeneralTreeWidget()
{
    delete ui;
}

void GeneralTreeWidget::on_pushButton_clicked()
{
    model->insertRows(model->rowCount(),1);
    // QModelIndex index = ui->treeView->currentIndex();
    // smodel->insertRows(index.row()+1,1,index.parent());

}

void GeneralTreeWidget::on_pushButton_2_clicked()
{
    QModelIndex index = ui->treeView->currentIndex();
    index = model->index(index.row(),0,index.parent());


    if(index.isValid()){
        model->insertRows(model->rowCount(index),1,index);
    }

    // index = smodel->index(index.row(),0,index.parent());
    // if(index.isValid()){
    //     smodel->insertRows(0,1,index);
    // }

}
