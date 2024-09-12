#ifndef GENERALTREEMODEL_H
#define GENERALTREEMODEL_H

#include <QAbstractItemModel>
#include "tableitem.h"

//根节点是一个不可见节点, 用于管理所有的节点 ,和QStandardItemModel的根节点一样
//所有操作过程中使用parent的函数, 如果parent = QModelIndex() 则表示是根节点
class GeneralTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit GeneralTreeModel(QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    bool setHeaderData(int section,
                       Qt::Orientation orientation,
                       const QVariant &value,
                       int role = Qt::EditRole) override;

    // Basic functionality:
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    // Add data:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool insertColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;

    // Remove data:
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;

    TableRowItem *rootItem() const;

    //append
    //remove一行
    //inster一行

    //不构造index获取数据. 直接通过行号
    //快速指定父列设置数据...



private:
    TableRowItem *m_rootItem;
    QStringList m_horizontalHeaders;
    QStringList m_verticalHeaders;
};

#endif // GENERALTREEMODEL_H
