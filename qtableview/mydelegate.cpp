#include "mydelegate.h"
#include <QLabel>
MyDelegate::MyDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{}

QWidget *MyDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QLabel *icon = new QLabel("good",parent);
    return icon;

}

