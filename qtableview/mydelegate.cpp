#include "mydelegate.h"
#include <QLabel>
#include <QSpinBox>
#include <QStyleOptionProgressBar>
#include <QApplication>
MyDelegate::MyDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{}

//该函数是创建一个自定义编辑器. 只有在双击单元格进行编辑的时候才会创建这个控件. 如果要修改静态显示效果应该使用paint函数
QWidget *MyDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSpinBox *sb = new QSpinBox(parent);
    sb->setMaximum(100);
    sb->setMinimum(0);
    sb->setFrame(false);

    return sb;

}

void MyDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QSpinBox *sb = static_cast<QSpinBox*>(editor);
    int value = index.data().toInt();
    sb->setValue(value);
}

void MyDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QSpinBox *sb = static_cast<QSpinBox*>(editor);
    int value = sb->value();
    model->setData(index,value);
}

void MyDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

void MyDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column() == 1)
    {
        int progress = index.data().toInt();
        QStyleOptionProgressBar progressBarOption;
        progressBarOption.rect = option.rect;
        progressBarOption.minimum = 0;
        progressBarOption.maximum = 100;
        progressBarOption.progress = progress;
        progressBarOption.text = QString::number(progress) + "%";
        progressBarOption.textVisible = true;

        QApplication::style()->drawControl(QStyle::CE_ProgressBar,
                                           &progressBarOption, painter);
    } else{
        QStyledItemDelegate::paint(painter, option, index);
    }
}

QSize MyDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    return QStyledItemDelegate::sizeHint(option, index);
}

