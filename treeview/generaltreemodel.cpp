#include "generaltreemodel.h"
#include <QDebug>
GeneralTreeModel::GeneralTreeModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    m_rootItem = new TableRowItem(nullptr,-1);
}

QVariant GeneralTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(role);
    // FIXME: Implement me!
    if(section < 0){
        return QVariant();
    }
    if(orientation == Qt::Horizontal){
        if(section < m_horizontalHeaders.size()){
            return m_horizontalHeaders[section];
        }
    }else if(orientation == Qt::Vertical){

        if(section < m_verticalHeaders.size()){
            return m_verticalHeaders[section];
        }
    }
    return QVariant();
}

bool GeneralTreeModel::setHeaderData(int section,
                                     Qt::Orientation orientation,
                                     const QVariant &value,
                                     int role)
{
    Q_UNUSED(role);
    if (value != headerData(section, orientation, role)) {
        // FIXME: Implement me!
        if(section < 0){
            return false;
        }
        if(orientation == Qt::Horizontal){
            if(section > m_horizontalHeaders.size()){
                for(int i = m_horizontalHeaders.size();i<=section;++i){
                    m_horizontalHeaders.append("");
                }
            }
            m_horizontalHeaders[section] = value.toString();
        }else if(orientation == Qt::Vertical){
            if(section > m_verticalHeaders.size()){
                for(int i = m_verticalHeaders.size();i<=section;++i){
                    m_verticalHeaders.append("");
                }
            }
            m_verticalHeaders[section] = value.toString();
        }

        emit headerDataChanged(orientation, section, section);
        return true;
    }
    return false;
}

QModelIndex GeneralTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    // FIXME: Implement me!
    if(parent.isValid()){
        TableRowItem* parentItem = static_cast<TableRowItem*>(parent.internalPointer());
        TableRowItem* childItem = parentItem->childItem(row);
        if(childItem){
            return createIndex(row,column,childItem);
        }
    }else{//根节点
        TableRowItem* childItem = m_rootItem->childItem(row);
        if(childItem){
            return createIndex(row,column,childItem);
        }
    }
    return QModelIndex();
}


QModelIndex GeneralTreeModel::parent(const QModelIndex &index) const
{
    // FIXME: Implement me!
    if(index.isValid()){
        TableRowItem* childItem = static_cast<TableRowItem*>(index.internalPointer());
        TableRowItem* parentItem = childItem->parentItem();
        if(parentItem){
            return createIndex(parentItem->childItemIndex(childItem),0,parentItem);
        }
    }
    return QModelIndex();
}

int GeneralTreeModel::rowCount(const QModelIndex &parent) const
{
    if(!parent.isValid()){
        return m_rootItem->childCountCount();
    }
    // FIXME: Implement me!
    return static_cast<TableRowItem*>(parent.internalPointer())->childCountCount();
}

int GeneralTreeModel::columnCount(const QModelIndex &parent) const
{
    if(!parent.isValid()){
        return m_rootItem->columnCount();
    }
    // FIXME: Implement me!
    return static_cast<TableRowItem*>(parent.internalPointer())->columnCount();
}

QVariant GeneralTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // FIXME: Implement me!
    TableRowItem* item = static_cast<TableRowItem*>(index.internalPointer());
    return item->data(index.column(),role);
}

bool GeneralTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid()){
        return false;
    }
    if (data(index, role) != value) {
        // FIXME: Implement me!
        bool res = false;
        TableRowItem* item = static_cast<TableRowItem*>(index.internalPointer());
        res = item->setData(index.column(),value,role);
        if(res ){
            emit dataChanged(index, index, {role});
            return true;
        }
    }
    return false;
}

Qt::ItemFlags GeneralTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;
    TableRowItem* item = static_cast<TableRowItem*>(index.internalPointer());
    if(item->checkable(index.column())){
        return QAbstractItemModel::flags(index) | Qt::ItemIsEditable | Qt::ItemIsUserCheckable; // FIXME: Implement me!
    }
    if(item->isValid(index.column())){
        return QAbstractItemModel::flags(index) | Qt::ItemIsEditable; // FIXME: Implement me!
    }
    return QAbstractItemModel::flags(index); // FIXME: Implement me!
}

bool GeneralTreeModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    if(parent.isValid()){
        TableRowItem* parentItem = static_cast<TableRowItem*>(parent.internalPointer());
        for(int i = 0;i<count;++i){
            //构造函数自动添加到父级中.
            new TableRowItem(parentItem,row);
        }
    }else{
        for(int i = 0;i<count;++i){
            new TableRowItem(m_rootItem,row);
        }
    }
    endInsertRows();
    return true;
}

bool GeneralTreeModel::insertColumns(int column, int count, const QModelIndex &parent)
{
    beginInsertColumns(parent, column, column + count - 1);
    // FIXME: Implement me!
    m_rootItem->insertColumns(column,count);
    endInsertColumns();
    return true;
}

bool GeneralTreeModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    TableRowItem* parentItem = nullptr;
    if(parent.isValid()){
        parentItem = static_cast<TableRowItem*>(parent.internalPointer());
    }else{
        parentItem = m_rootItem;
    }
    for(int i = 0;i<count;++i){
        TableRowItem* childItem = parentItem->takeChildItem(row);
        if(childItem){
            delete childItem;
        }
    }
    endRemoveRows();
    return true;
}

bool GeneralTreeModel::removeColumns(int column, int count, const QModelIndex &parent)
{
    beginRemoveColumns(parent, column, column + count - 1);
    // FIXME: Implement me!
    m_rootItem->removeColumns(column,count);
    endRemoveColumns();
    return true;
}

TableRowItem *GeneralTreeModel::rootItem() const
{
    return m_rootItem;
}

