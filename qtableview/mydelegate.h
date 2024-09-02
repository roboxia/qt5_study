#ifndef MYDELEGATE_H
#define MYDELEGATE_H

#include <QStyledItemDelegate>

class MyDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit MyDelegate(QObject *parent = nullptr);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    // void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    // void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    // void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    // QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    // void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;


signals:
};

#endif // MYDELEGATE_H
