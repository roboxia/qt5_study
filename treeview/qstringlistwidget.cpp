#include "qstringlistwidget.h"
#include "ui_qstringlistwidget.h"


QStringListWidget::QStringListWidget(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::QStringListWidget)
{
    ui->setupUi(this);

    QTreeWidgetItem *item = new QTreeWidgetItem();

    ui->treeWidget->invisibleRootItem()->insertChild(0,item);

    ui->treeWidget->setColumnCount(5);

}

QStringListWidget::~QStringListWidget()
{
    delete ui;
}

void QStringListWidget::on_pushButton_clicked()
{
    QTreeWidgetItem *item = new QTreeWidgetItem();
    ui->treeWidget->invisibleRootItem()->addChild(item);
}


void QStringListWidget::on_pushButton_2_clicked()
{
    QTreeWidgetItem *item = ui->treeWidget->currentItem();
    if(item){
        QTreeWidgetItem *child = new QTreeWidgetItem();
        item->addChild(child);
    }
}


//对于model来说只能插入行. 列数都是固定的.
//然后所有的列数, 都是用过head来指定的
//而且我觉得这种插入方式貌似更加合理一些.
