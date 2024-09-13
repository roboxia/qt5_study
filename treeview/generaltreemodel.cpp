#include "generaltreemodel.h"
#include <QDebug>
GeneralTreeModel::GeneralTreeModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    m_rootItem = new RowItem(nullptr,-1);
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
        RowItem* parentItem = static_cast<RowItem*>(parent.internalPointer());
        RowItem* childItem = parentItem->childItem(row);
        if(childItem){
            return createIndex(row,column,childItem);
        }
    }else{//根节点
        RowItem* childItem = m_rootItem->childItem(row);
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
        RowItem* childItem = static_cast<RowItem*>(index.internalPointer());
        RowItem* parentItem = childItem->parentItem();
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
    return static_cast<RowItem*>(parent.internalPointer())->childCountCount();
}

int GeneralTreeModel::columnCount(const QModelIndex &parent) const
{
    if(!parent.isValid()){
        return m_rootItem->columnCount();
    }
    // FIXME: Implement me!
    return static_cast<RowItem*>(parent.internalPointer())->columnCount();
}

QVariant GeneralTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // FIXME: Implement me!
    RowItem* item = static_cast<RowItem*>(index.internalPointer());
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
        RowItem* item = static_cast<RowItem*>(index.internalPointer());
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
    RowItem* item = static_cast<RowItem*>(index.internalPointer());
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
        RowItem* parentItem = static_cast<RowItem*>(parent.internalPointer());
        for(int i = 0;i<count;++i){
            //构造函数自动添加到父级中.
            new RowItem(parentItem,row);
        }
    }else{
        for(int i = 0;i<count;++i){
            new RowItem(m_rootItem,row);
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
    RowItem* parentItem = nullptr;
    if(parent.isValid()){
        parentItem = static_cast<RowItem*>(parent.internalPointer());
    }else{
        parentItem = m_rootItem;
    }
    for(int i = 0;i<count;++i){
        RowItem* childItem = parentItem->takeChildItem(row);
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

RowItem *GeneralTreeModel::rootItem() const
{
    return m_rootItem;
}

RowItem *GeneralTreeModel::insertRow(int row, const QModelIndex &parent)
{
    insertRows(row,1,parent);
    return static_cast<RowItem*>(index(row,0,parent).internalPointer());
}

void GeneralTreeModel::removeRow(int row, const QModelIndex &parent)
{
    removeRows(row,1,parent);
}

void GeneralTreeModel::appendRow(const QModelIndex &parent)
{
    insertRows(rowCount(parent),1,parent);
}

RowItem *GeneralTreeModel::item(int row, int column, const QModelIndex &parent)
{
    return static_cast<RowItem*>(index(row,column,parent).internalPointer());
}

