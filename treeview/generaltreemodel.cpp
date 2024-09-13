#include "generaltreemodel.h"
#include <QDebug>


RowItem::RowItem()
{
    m_level = -1;
    m_columnCount = 0;
    m_parentItem = nullptr;
    m_model = nullptr;
    m_colunmData.clear();
    m_childItem.clear();
}

RowItem::RowItem(RowItem *parentItem)
{
    m_level = -1;
    m_columnCount = 0;
    m_parentItem = nullptr;
    m_model = nullptr;
    m_colunmData.clear();
    m_childItem.clear();

    if(parentItem){
        parentItem->appendRow(this);
    }
}

RowItem::RowItem(RowItem *parentItem,int row)
{
    m_level = -1;
    m_columnCount = 0;
    m_parentItem = nullptr;
    m_model = nullptr;
    m_colunmData.clear();
    m_childItem.clear();

    if(parentItem){
        parentItem->insertRow(row,this);
    }
}

RowItem::~RowItem()
{
    //删除所有 child
    int count = m_childItem.size();
    for(int i = 0;i<count;++i){
        RowItem *item = takeChild(0);
        if(item){
            delete item;
        }
    }

    //确认parent中没有自
    if(m_parentItem){
        //从parent中取消掉自己的索引
        m_parentItem->takeChild(this);
    }
}

void RowItem::insertColumns(int column, int count)
{
    //范围限制
    if(count <= 0 ){
        return;
    }
    if(column < 0)
        column = 0;
    if(column > m_columnCount){
        column = m_columnCount;
    }

    //表格同步
    if(m_model){
        QModelIndex index = QModelIndex();
        if(m_parentItem){
            index = m_parentItem->index(0);
        }
        m_model->beginInsertColumns(index,column,column+count-1);
    }
    //插入数据
    for(int i = 0;i<m_colunmData.size();++i){
        for(int j =0;j<count;++j){
            m_colunmData[i].insert(column + j,QVariant());
        }
    }

    m_columnCount += count;

    for(int i = 0;i<m_childItem.size();++i){
        m_childItem[i]->insertColumns(column,count);
    }

    if(m_model != nullptr){
        m_model->endInsertColumns();
    }
}

void RowItem::removeColumns(int column, int count)
{
    //范围限制
    if(count <= 0 || column >= m_columnCount){
        return;
    }
    if(column < 0)
        column = 0;
    if(column + count > m_columnCount){
        count = m_columnCount - column;
    }

    //表格同步
    if(m_model){
        QModelIndex index = QModelIndex();
        if(m_parentItem){
            index = m_parentItem->index(0);
        }
        m_model->beginRemoveColumns(index,column,column+count-1);
    }
    //删除数据
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


void RowItem::insertRows(int row, QList<RowItem *> items)
{
    //限制范围
    if(row < 0)
        row = 0;
    if(row > m_childItem.size()){
        row = m_childItem.size();
    }
    //同步表格
    if(m_model){
        QModelIndex index = QModelIndex();
        if(m_parentItem){
            index = m_parentItem->index(0);
        }
        m_model->beginInsertRows(index,row,row+items.size()-1);
    }
    for(int i = 0;i<items.size();++i){
        if(items[i]->parent() != nullptr){
            continue;
        }
        items[i]->m_parentItem = this;
        items[i]->m_model = this->m_model;
        //对齐column
        if(items[i]->m_columnCount < this->m_columnCount){
            items[i]->insertColumns(items[i]->m_columnCount,this->m_columnCount - items[i]->m_columnCount);
        }
        m_childItem.insert(row+i,items[i]);

        //统计层级
        items[i]->m_level = m_level + 1;
    }
}

void RowItem::insertRows(int row, int count)
{
    QList<RowItem*> items;
    for(int i = 0;i<count;++i){
        items.append(new RowItem());
    }
    insertRows(row,items);
}

void RowItem::insertRow(int row, RowItem *item)
{
    //限制范围
    insertRows(row,{item});
}

void RowItem::appendRows(QList<RowItem *> items)
{
    insertRows(m_childItem.size(),items);
}

void RowItem::appendRow(RowItem *item)
{
    insertRows(m_childItem.size(),{item});
}

void RowItem::removeRows(int row, int count)
{
    //限制范围
    if(count <= 0 || row >= m_childItem.size()){
        return;
    }
    if(row < 0)
        row = 0;
    if(row + count > m_childItem.size()){
        count = m_childItem.size() - row;
    }
    //同步表格
    if(m_model){
        QModelIndex index = QModelIndex();
        if(m_parentItem){
            index = m_parentItem->index(0);
        }
        m_model->beginRemoveRows(index,row,row+count-1);
    }
    for(int i = 0;i<count;++i){
        RowItem *item = takeChild(row);
        if(item){
            delete item;
        }
    }

    if(m_model){
        m_model->endRemoveRows();
    }
}

void RowItem::removeRow(int row)
{
    removeRows(row,1);
}

void RowItem::setChild(int row, RowItem *item)
{
    if(row >= m_childItem.size()){
        insertRows(row,row-m_childItem.size()); // 使 size = row.此时 使用 row最为序号插入是在最后一个
        insertRow(row,item);
    }else{
        //删除掉原row.
        RowItem *ritem = takeChild(row);
        if(ritem){
            delete ritem;
        }
        //重新插入该item
        insertRow(row,item);
    }
}

RowItem *RowItem::takeChild(int row)
{
    if(row >= m_childItem.size() || row < 0){
        return nullptr;
    }
    RowItem *item = m_childItem[row];
    m_childItem.removeAt(row);
    return item;
}

int RowItem::takeChild(RowItem *item)
{
    for(int i = 0;i<m_childItem.size();++i){
        if(m_childItem[i] == item){
            m_childItem.removeAt(i);
            return i;
        }
    }
    return -1;
}

QList<RowItem *> RowItem::takeChildren()
{
    QList<RowItem *> items = m_childItem;
    m_childItem.clear();
    return items;
}

RowItem *RowItem::child(int row)
{
    if(row >= m_childItem.size() || row < 0){
        return nullptr;
    }
    return m_childItem[row];
}

QList<RowItem *> RowItem::children() const
{
    return m_childItem;
}

RowItem *RowItem::parent()
{
    return m_parentItem;
}

int RowItem::row()
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

int RowItem::childRow(RowItem *item) const
{
    for(int i = 0;i < m_childItem.size();++i){
        if(m_childItem.at(i) == item){
            return i;
        }
    }
    return -1;
}

int RowItem::rowCount()
{
    return m_childItem.size();
}

int RowItem::columnCount() const
{
    return m_columnCount;
}


int RowItem::level()
{
    return m_level;
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
    if(column >= m_colunmData[index].size()){
        for(int i = m_colunmData[index].size();i<=column;++i){
            m_colunmData[index].append(QVariant());
        }
    }

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
    if(column >= m_colunmData[index].size())
        return QVariant();

    return m_colunmData[index][column];
}



QAbstractItemModel *RowItem::model() const
{
    return m_model;
}



QModelIndex RowItem::index(int column) const
{
    if(m_parentItem == nullptr){
        return QModelIndex();
    }
    int row = m_parentItem->childRow(const_cast<RowItem*>(this));
    void *ptr = const_cast<RowItem*>(this);

    return m_model->createIndex(row,column, ptr); //自己在父级中的位置
}

QModelIndex RowItem::indexChild(int row, int column) const
{
    if(row >= m_childItem.size() || row < 0){
        return QModelIndex();
    }
    return m_model->createIndex(row,column,m_childItem[row]);
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




void RowItem::clearData()
{
    roleMap.clear();
    m_colunmData.clear();
}

void RowItem::clearDataChild()
{
    clearData();
    for(int i = 0;i<m_childItem.size();++i){
        m_childItem[i]->clearDataChild();
    }
}

RowItem *RowItem::fromViod(void *ptr)
{
    return static_cast<RowItem*>(ptr);
}





GeneralTreeModel::GeneralTreeModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    m_rootItem = new RowItem();
    m_rootItem->m_model = this;
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
        return parentItem->indexChild(row,column);
    }else{//根节点
        return m_rootItem->indexChild(row,column);
    }
    return QModelIndex();
}


QModelIndex GeneralTreeModel::parent(const QModelIndex &index) const
{
    // FIXME: Implement me!
    if(index.isValid()){
        RowItem* childItem = static_cast<RowItem*>(index.internalPointer());
        RowItem* parentItem = childItem->parent();
        if(parentItem){
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
    return static_cast<RowItem*>(parent.internalPointer())->rowCount();
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
    // beginInsertRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    if(parent.isValid()){
        RowItem* parentItem = static_cast<RowItem*>(parent.internalPointer());
        parentItem->insertRows(row,count);
    }else{
        rootItem()->insertRows(row,count);
    }
    // endInsertRows();
    return true;
}

bool GeneralTreeModel::insertColumns(int column, int count, const QModelIndex &parent)
{
    // beginInsertColumns(parent, column, column + count - 1);
    // FIXME: Implement me!
    m_rootItem->insertColumns(column,count);
    // endInsertColumns();
    return true;
}

bool GeneralTreeModel::removeRows(int row, int count, const QModelIndex &parent)
{
    // beginRemoveRows(parent, row, row + count - 1);
    // FIXME: Implement me!

    if(parent.isValid()){
        RowItem* parentItem = static_cast<RowItem*>(parent.internalPointer());
        parentItem->removeRows(row,count);
    }else{
        m_rootItem->removeRows(row,count);
    }

    // endRemoveRows();
    return true;
}

bool GeneralTreeModel::removeColumns(int column, int count, const QModelIndex &parent)
{
    //beginRemoveColumns(parent, column, column + count - 1);
    // FIXME: Implement me!
    m_rootItem->removeColumns(column,count);
    //endRemoveColumns();
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

