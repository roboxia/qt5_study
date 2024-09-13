#include "rowitem.h"
#include <QDebug>

RowItem::RowItem(RowItem *parentItem, RowItem *indexItem)
    :m_parentItem(parentItem)
    ,m_level{0}
    ,m_columnCount{0}
{
    if(parentItem!=nullptr){
        insertColumns(0,parentItem->columnCount());
        int index = 0;
        if(indexItem == nullptr){
            index = parentItem->childCount(); //插入到最后
        }else{
            index = parentItem->childItemIndex(indexItem);
        }
        parentItem->insertChildItem(index,this);
    }

    //计算层级
    for(RowItem *item = parentItem;item != nullptr;item = item->parentItem()){
        ++m_level;
    }
}

RowItem::RowItem(RowItem *parentItem, int index)
: m_parentItem(parentItem)
    ,m_level{0}
    ,m_columnCount{0}
{
    if(parentItem!=nullptr){
        insertColumns(0,parentItem->columnCount());
        parentItem->insertChildItem(index,this);
    }

    //计算层级
    for(RowItem *item = parentItem;item != nullptr;item = item->parentItem()){
        ++m_level;
    }
}


//删除自身的时候, 从parent中删除自己
//删除自身的时候需要处理好上下关系就行.
RowItem::~RowItem()
{
    int count = m_childItem.size();
    for(int i = 0;i<count;++i){
        m_childItem[i]->deleteSelf();
    }

    if(m_parentItem){
        //从parent中取消掉自己的索引
        m_parentItem->takeChildItem(this);
    }
}

void RowItem::insertColumns(int column, int count)
{
    if(count <= 0 ){
        return;
    }
    if(column < 0)
       column = 0;
    if(column > m_columnCount){
        column = m_columnCount;
    }

    for(int i = 0;i<m_colunmData.size();++i){
        for(int j =0;j<count;++j){
            m_colunmData[i].insert(column,QVariant());
        }
    }

    m_columnCount += count;

    for(int i = 0;i<m_childItem.size();++i){
        m_childItem[i]->insertColumns(column,count);
    }
}

void RowItem::removeColumns(int column, int count)
{
    if(count <= 0 || column >= m_columnCount){
        return;
    }
    if(column < 0)
        column = 0;
    if(column + count > m_columnCount){
        count = m_columnCount - column;
    }

    for(int i = 0;i<m_colunmData.size();++i){
        for(int j =0;j<count;++j){
            m_colunmData[i].removeAt(column);
        }
    }
    m_columnCount -= count;

    for(int i = 0;i<m_childItem.size();++i){
        m_childItem[i]->removeColumns(column,count);
    }
}

bool RowItem::setData(int column, const QVariant &value, int role)
{
    if(column >= m_columnCount){
        return false;
    }
    if(role == DisplayRole)
        role = EditRole;
    if(role == CheckEnableRole && !roleMap.contains(role)) //第一次使能check,为state赋初值
        setData(column,false,CheckStateRole);

    if(!roleMap.contains(role)){
        QVariantList list;
        list.reserve(m_columnCount);
        for (int var = 0; var < m_columnCount; ++var) {
            list.append(QVariant());
        }
        m_colunmData.append(list);
        roleMap.insert(role,m_colunmData.size()-1);
    }
    int index = roleMap[role];
    m_colunmData[index][column] = value;
    return true;
}

QVariant RowItem::data(int column, int role) const
{
    if(column >= m_columnCount){
        return QVariant();
    }
    if(role == DisplayRole)
        role = EditRole;
    if(!roleMap.contains(role)){
        return QVariant();
    }
    int index = roleMap[role];
    return m_colunmData[index][column];
}



RowItem* RowItem::parentItem() const
{
    return m_parentItem;
}

QVector<RowItem*> RowItem::childItems() const
{
    return m_childItem;
}

RowItem *RowItem::childItem(int index) const
{
    if(index >= m_childItem.size() || index < 0){
        return nullptr;
    }
    return m_childItem[index];
}

int RowItem::childItemIndex(RowItem *childItem) const
{
    for(int i = 0;i < m_childItem.size();++i){
        if(m_childItem[i] == childItem){
            return i;
        }
    }
    return -11;
}

RowItem* RowItem::takeChildItem(int index)
{
    if(index >= m_childItem.size() || index < 0){
        return nullptr;
    }
    RowItem *item = m_childItem[index];
    m_childItem.remove(index);
    return item;
}

int RowItem::takeChildItem(RowItem *childItem)
{
    for(int i = 0;i < m_childItem.size();++i){
        if(m_childItem[i] == childItem){
            m_childItem.remove(i);
            return i;
        }
    }
    return -11;
}

bool RowItem::insertChildItem(int index, RowItem *item)
{
    if(index < 0){
        index = 0;
    }
    if(index > m_childItem.size()){
        index = m_childItem.size();
    }
    if(item->parentItem() != nullptr){
        item->parentItem()->takeChildItem(item);
    }

    m_childItem.insert(index,item);
    return true;
}


void RowItem::deleteSelf()
{
    if(m_parentItem){
        m_parentItem->takeChildItem(this);
        m_parentItem = nullptr;
    }
    delete this;
}

void RowItem::deleteChildItem(int index)
{
    if(index >= m_childItem.size() || index < 0){
        return;
    }
    RowItem *item = m_childItem[index];
    item->deleteSelf();
}


int RowItem::childCount()
{
    return m_childItem.size();
}

int RowItem::childCountCount()
{
    int count = m_childItem.size();
    for(int i = 0;i < m_childItem.size();++i){
        count += m_childItem[i]->childCountCount();
    }
    return count;
}

int RowItem::index()
{
    if(m_parentItem){
        return m_parentItem->childItemIndex(this);
    }
    return -1;
}

void RowItem::setCheckable(int column, bool checkable)
{
    setData(column,checkable,CheckEnableRole);
}

bool RowItem::checkable(int column) const
{
    QVariant var =  data(column,CheckEnableRole);
    return var.toBool();
}

void RowItem::setIcon(int column, const QIcon &icon)
{
    setData(column,icon,DecorationRole);
}

QIcon RowItem::icon(int column) const
{
    QVariant var = data(column,DecorationRole);
    return var.value<QIcon>();
}

bool RowItem::isValid(int column)
{
    if(column >= m_columnCount || column < 0){
        return false;
    }
    return data(column).isValid();
}

int RowItem::level()
{
    return m_level;
}


void RowItem::clearData()
{
    m_colunmData.clear();
    roleMap.clear();
}

int RowItem::columnCount() const
{
    return m_columnCount;
}
