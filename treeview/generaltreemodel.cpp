#include "generaltreemodel.h"
#include <QDebug>



RowItem::RowItem()
{
    m_columnCount = 0;
    m_parentItem = nullptr;
    m_colunmData.clear();
    m_childItem.clear();
}



RowItem::~RowItem()
{
    //删除所有 child
    int count = m_childItem.size();
    for(int i = 0;i<count;++i){
        RowItem *item = m_childItem.takeAt(0);
        if(item){
            delete item;
        }
    }

    //确认parent中没有自己. 以此保证直接delete自己, 也能够正确的操作表格
    if(m_parentItem){
        //从parent中取消掉自己的索引
        m_parentItem->takeChild(this);
    }
}

int RowItem::row() const
{
    if(m_parentItem){
        for(int i = 0;i < m_parentItem->m_childItem.size();++i){
            if(this == m_parentItem->m_childItem.at(i)){
                return i;
            }
        }
        return -1;
    }
    return -1;
}

int RowItem::rowCount() const
{
    return m_childItem.size();
}

int RowItem::columnCount() const
{
    return m_columnCount;
}



int RowItem::level()
{
    int level = -1;
    RowItem *par = this->m_parentItem;
    while(par){
        level++;
        par = par->m_parentItem;
    }
    return level;
}



bool RowItem::setData(int column, const QVariant &value, int role)
{
    if(role == EditRole)
        role = DisplayRole;
    if(!m_colunmData.contains(role)){
        QVariantList list;
        list.reserve(m_columnCount);
        for (int var = 0; var < m_columnCount; ++var) {
            list.append(QVariant());
        }
        m_colunmData.insert(role,list);
    }
    if(column >= m_colunmData[role].size()){
        return false;
    }
    m_colunmData[role][column] = value;
    return true;
}

QVariant RowItem::data(int column, int role) const
{
    if(role == EditRole)
        role = DisplayRole;
    if(!m_colunmData.contains(role)){
        return QVariant();
    }
    if(column >= m_colunmData[role].size())
        return QVariant();
    return m_colunmData[role][column];
}

RowItem *RowItem::child(int row) const
{
    if(row < 0 || row >= m_childItem.size()){
        return nullptr;
    }
    return m_childItem[row];
}

RowItem *RowItem::parent() const
{
    return m_parentItem;
}


RowItem *RowItem::fromIndex(const QModelIndex &index)
{
    if(!index.isValid())
        return nullptr;
    return reinterpret_cast<RowItem*>(index.internalPointer());
}


bool RowItem::minsertColumns(int column, int count)
{
    //范围限制
    if(count <= 0|| column < 0 || column > m_columnCount){
        return false;
    }

    //修改自身
    for(int i = 0;i<m_colunmData.size();++i){
        for(int j =0;j<count;++j){
            m_colunmData[i].insert(column + j,QVariant());
        }
    }
    m_columnCount += count;

    //子项变更
    for(int i = 0;i<m_childItem.size();++i){
        m_childItem[i]->minsertColumns(column,count);
    }
    return true;
}


bool RowItem::mremoveColumns(int column, int count)
{
    //范围限制
    if(count <= 0 || column >= m_columnCount || column < 0 ||
        column + count > m_columnCount){
        return false;
    }

    //删除数据
    for(int i = 0;i<m_colunmData.size();++i){
        for(int j =0;j<count;++j){
            m_colunmData[i].removeAt(column);
        }
    }
    m_columnCount -= count;

    //子项变更
    for(int i = 0;i<m_childItem.size();++i){
        m_childItem[i]->mremoveColumns(column,count);
    }
    return true;
}


bool RowItem::insertChild(int row, RowItem *child)
{
    if(child->m_parentItem == this){
        return true;
    }
    if(row < 0 || row > m_childItem.size()){
        return false;
    }
    if(child->m_parentItem){
        return false;
    }
    child->m_parentItem = this;
    //设置column.如果原来的column过少, 则增加. 过多则删除末尾. 一致则不操作
    if(child->m_columnCount < m_columnCount){
        child->minsertColumns(child->m_columnCount,m_columnCount - child->m_columnCount);
    }else if(child->m_columnCount > m_columnCount){
        child->mremoveColumns(m_columnCount,child->m_columnCount - m_columnCount);
    }

    m_childItem.insert(row,child);
    return true;
}

bool RowItem::addChild(RowItem *child)
{
    return insertChild(m_childItem.size(),child);
}


QList<RowItem *> RowItem::takeChildren(int row, int count)
{
    QList<RowItem *> items;
    //限制范围
    if(count <= 0 || row >= m_childItem.size() || row < 0
        || row + count > m_childItem.size()){
        return items;
    }

    for(int i = 0;i<count;++i){
        m_childItem[row]->m_parentItem = nullptr; //无父项
        items.append(m_childItem[row]);
        m_childItem.removeAt(row);
    }
    return items;
}

bool RowItem::takeChild(RowItem *child)
{
    int index = m_childItem.indexOf(child);
    if(index < 0){
        return false;
    }
    return takeChildren(index,1).size() > 0;
}


bool RowItem::minsertRows(int row, int count)
{
    for(int i = 0;i < count;++i){
        RowItem *item = new RowItem();
        if(insertChild(row + i,item) == false)
            return false;
    }
    return true;
}


void RowItem::mremoveRows(int row, int count)
{
    QList<RowItem *> items = takeChildren(row,count);
    for(int i = 0;i<items.size();++i){
        delete items[i];
    }
}



GeneralTreeModel::GeneralTreeModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    m_rootItem = new RowItem();
}



QVariant GeneralTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::EditRole){
        role = Qt::DisplayRole;
    }
    if (role != Qt::DisplayRole)
        return QVariant();
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
    if(role == Qt::EditRole){
        role = Qt::DisplayRole;
    }
    if (role != Qt::DisplayRole)
        return false;

    if (value != headerData(section, orientation, role)) {
        // FIXME: Implement me!
        if(section < 0){
            return false;
        }
        if(orientation == Qt::Horizontal){
            if(section >= m_horizontalHeaders.size()){
                for(int i = m_horizontalHeaders.size();i<=section;++i){
                    m_horizontalHeaders.append("");
                }
            }
            m_horizontalHeaders[section] = value.toString();
        }else if(orientation == Qt::Vertical){
            if(section >= m_verticalHeaders.size()){
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
        return createIndex(row,column, RowItem::fromIndex(parent)->child(row));
    }else{//根节点
        return createIndex(row,column,m_rootItem->child(row));
    }
    return QModelIndex();
}


QModelIndex GeneralTreeModel::parent(const QModelIndex &index) const
{
    // FIXME: Implement me!
    if(index.isValid()){
        RowItem* childItem = RowItem::fromIndex(index);
        RowItem* parentItem = childItem->parent();
        if(parentItem == nullptr || parentItem == m_rootItem){
            return QModelIndex();
        }else{
            return createIndex(parentItem->row(),0,parentItem);
        }
    }
    return QModelIndex();
}

int GeneralTreeModel::rowCount(const QModelIndex &parent) const
{
    if(!parent.isValid()){
        return m_rootItem->rowCount();
    }
    // FIXME: Implement me!
    RowItem* item = RowItem::fromIndex(parent);
    qDebug()<<item->rowCount();
    return item->rowCount();
}

int GeneralTreeModel::columnCount(const QModelIndex &parent) const
{
    if(!parent.isValid()){
        return m_rootItem->columnCount();
    }
    // FIXME: Implement me!
    RowItem* item = RowItem::fromIndex(parent);
    return item->columnCount();
}

QVariant GeneralTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // FIXME: Implement me!
    RowItem* item = RowItem::fromIndex(index);
    return item->data(index.column(),role);
}

bool GeneralTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid()){
        return false;
    }
    // FIXME: Implement me!
    RowItem* item = RowItem::fromIndex(index);
    return item->setData(index.column(),value,role);

}

Qt::ItemFlags GeneralTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;
    // FIXME: Implement me!

    RowItem* item = RowItem::fromIndex(index);
    if(item){
        //qDebug()<<index;
        return QAbstractItemModel::flags(index) | item->data(index.column(),RowItem::ItemFlagsData).value<Qt::ItemFlags>();

    }
    return QAbstractItemModel::flags(index); // FIXME: Implement me!
}

bool GeneralTreeModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    if(parent.isValid()){
        RowItem* parentItem = RowItem::fromIndex(parent);
        parentItem->minsertRows(row,count);
    }else{
        rootItem()->minsertRows(row,count);
    }

    endInsertRows();
    return true;
}




bool GeneralTreeModel::insertColumns(int column, int count, const QModelIndex &parent)
{
    if(column < 0 || column > columnCount(parent) || count <= 0){
        return false;
    }
    beginInsertColumns(parent, column, column + count - 1);
    // FIXME: Implement me!
    m_rootItem->minsertColumns(column,count);
    endInsertColumns();
    return true;
}


bool GeneralTreeModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if(row < 0 || row > rowCount(parent) || count <= 0){
        return false;
    }
    beginRemoveRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    if(parent.isValid()){
        RowItem* parentItem = RowItem::fromIndex(parent);
        parentItem->mremoveRows(row,count);
    }else{
        m_rootItem->mremoveRows(row,count);
    }
    endRemoveRows();
    return true;
}

bool GeneralTreeModel::removeColumns(int column, int count, const QModelIndex &parent)
{
    if(column < 0 || column > columnCount(parent) || count <= 0){
        return false;
    }
    beginRemoveColumns(parent, column, column + count - 1);
    // FIXME: Implement me!
    m_rootItem->mremoveColumns(column,count);
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
    return RowItem::fromIndex(index(row,0,parent));
}

void GeneralTreeModel::removeRow(int row, const QModelIndex &parent)
{
    removeRows(row,1,parent);
}
void GeneralTreeModel::removeRow(QModelIndex index)
{
    removeRow(index.row(),index.parent());
}

void GeneralTreeModel::appendRow(const QModelIndex &parent)
{
    insertRows(rowCount(parent),1,parent);
}

RowItem *GeneralTreeModel::item(int row, const QModelIndex &parent)
{
    return RowItem::fromIndex(index(row,0,parent));
}


