#include "mymodel.h"

MyModel::MyModel(QObject *parent)
    : QAbstractTableModel(parent)
, m_row(0)
, m_column(0)
{
    qRegisterMetaType<MyModelData_t>("MyModelData_t");
}

//不太确定一般会传递一些什么角色过来
QVariant MyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
    if(orientation == Qt::Orientation::Vertical && role == Qt::DisplayRole ){
        return m_vHeaderData[section];
    }else if(orientation == Qt::Orientation::Horizontal && role == Qt::DisplayRole){
        return m_hHeaderData[section];
    }
    return QVariant();
}


int MyModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

    // FIXME: Implement me!
    return m_row;
}

int MyModel::columnCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

    // FIXME: Implement me!
    return m_column;
}

QVariant MyModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if(index.row() >= m_row || index.column() >= m_column)
        return QVariant();

    // FIXME: Implement me!
    if(role == Qt::DisplayRole){
        MyModelData_t data = m_myModelData[index.row()][index.column()];
        return QVariant::fromValue(data);
    }

    return QVariant();
}

Qt::ItemFlags MyModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

bool MyModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid()){
        return false;
    }
    if(role == Qt::EditRole){
        if(index.row() < m_row && index.column() < m_column){
            m_myModelData[index.row()][index.column()] = value.value<MyModelData_t>();
            emit dataChanged(index,index);
            return true;
        }
    }
    return false;
}

bool MyModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if(role == Qt::EditRole){
        if(orientation == Qt::Orientation::Vertical){
            if(section < m_row)
                m_vHeaderData[section] = value.toString();
        }else if(orientation == Qt::Orientation::Horizontal){
            if(section < m_column)
                m_hHeaderData[section] = value.toString();
        }
        emit headerDataChanged(orientation,section,section);
        return true;
    }
    return false;
}

bool MyModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(QModelIndex(),row,row+count-1);
    for(int i = 0; i < count; i++){
        QVector<MyModelData_t> data;
        for(int j = 0; j < m_column; j++){
            MyModelData_t temp;
            temp.index = 0;
            temp.name = "";
            temp.myParent = QModelIndex();
            temp.myChild = QModelIndex();
            temp.row = i;
            temp.column = j;
            data.push_back(temp);
        }
        m_myModelData.insert(row,data);
    }
    m_row += count;
    endInsertRows();
    return true;
}

bool MyModel::insertColumns(int column, int count, const QModelIndex &parent)
{
    beginInsertColumns(QModelIndex(),column,column+count-1);
    for(int i = 0; i < m_row; i++){
        for(int j = 0; j < count; j++){
            MyModelData_t temp;
            temp.index = 0;
            temp.name = "";
            temp.myParent = QModelIndex();
            temp.myChild = QModelIndex();
            temp.row = i;
            temp.column = column + j;
            m_myModelData[i].insert(column + j,temp);
        }
    }
    m_column += count;
    endInsertColumns();
    return true;
}

bool MyModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(QModelIndex(),row,row+count-1);
    for(int i = 0; i < count; i++){
        m_myModelData.remove(row); //remove 之后row会自动减一, 所以每次都remove row即可
    }
    m_row -= count;
    endRemoveRows();
    return true;
}

bool MyModel::removeColumns(int column, int count, const QModelIndex &parent)
{
    beginRemoveColumns(QModelIndex(),column,column+count-1);
    for(int i = 0; i < m_row; i++){
        for(int j = 0; j < count; j++){
            m_myModelData[i].remove(column);
        }
    }
    m_column -= count;
    endRemoveColumns();
    return true;
}
