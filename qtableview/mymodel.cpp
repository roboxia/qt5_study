#include "mymodel.h"

MyModel::MyModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    qRegisterMetaType<MyModelData_t>("MyModelData_t");
}

//不太确定一般会传递一些什么角色过来
QVariant MyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
    //校验section是否在范围
    if(orientation == Qt::Orientation::Vertical && role == Qt::DisplayRole ){
        if(section < m_vHeaderData.size())
            return m_vHeaderData[section];
    }else if(orientation == Qt::Orientation::Horizontal && role == Qt::DisplayRole){
        if(section < m_hHeaderData.size())
            return m_hHeaderData[section];
    }
    return QVariant();
}


int MyModel::rowCount(const QModelIndex &parent) const
{
    // FIXME: Implement me!
    Q_UNUSED(parent);
    return m_vHeaderData.size();
}

int MyModel::columnCount(const QModelIndex &parent) const
{


    // FIXME: Implement me!
    Q_UNUSED(parent);
    return m_hHeaderData.size();
}

QVariant MyModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if(m_myModelData.isEmpty())
        return QVariant();

    //校验index是否正常
    if(index.row() >= m_myModelData.size() || index.column() >= m_myModelData[0].size())
        return QVariant();

    // FIXME: Implement me!
    if(role == Qt::DisplayRole || role == Qt::EditRole){
        QString data = m_myModelData[index.row()][index.column()];
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
    if(m_myModelData.isEmpty()){
        return false;
    }

    if(index.row() >= m_myModelData.size() || index.column() >= m_myModelData[0].size())
        return false;

    if(role == Qt::EditRole){
        m_myModelData[index.row()][index.column()] = value.toString();
        emit dataChanged(index,index);
        return true;
    }
    return false;
}

bool MyModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if(role == Qt::EditRole){
        if(orientation == Qt::Orientation::Vertical){
            if(section >= m_vHeaderData.size())
                return false;
            m_vHeaderData[section] = value.toString();
        }else if(orientation == Qt::Orientation::Horizontal){
            if(section >= m_hHeaderData.size())
                return false;
            m_hHeaderData[section] = value.toString();
        }
        emit headerDataChanged(orientation,section,section);
        return true;
    }
    return false;
}

bool MyModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    beginInsertRows(QModelIndex(),row,row+count-1);
    int m_column = m_hHeaderData.size();
    for(int i = 0; i < count; i++){
        QVector<QString> data;
        for(int j = 0; j < m_column; j++){
            QString temp = "";
            data.push_back(temp);
        }
        m_myModelData.insert(row,data);
    }
    for(int i = row; i < count + row; i++){
        m_vHeaderData.insert(i,QString("row"));
    }
    endInsertRows();
    return true;
}

bool MyModel::insertColumns(int column, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    beginInsertColumns(QModelIndex(),column,column+count-1);
    int m_row = m_vHeaderData.size();
    for(int i = 0; i < m_row; i++){
        for(int j = column; j < count + column; ++j){
            m_myModelData[i].insert(j,"");
        }
    }
    for(int i = column; i < count + column; i++){
        m_hHeaderData.insert(i,QString("column"));
    }
    endInsertColumns();
    return true;
}

bool MyModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    beginRemoveRows(QModelIndex(),row,row+count-1);
    for(int i = 0; i < count; i++){
        m_myModelData.remove(row); //remove 之后row会自动减一, 所以每次都remove row即可
        m_vHeaderData.removeAt(row);
    }
    endRemoveRows();
    return true;
}

bool MyModel::removeColumns(int column, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    beginRemoveColumns(QModelIndex(),column,column+count-1);
    int m_row = m_vHeaderData.size();
    for(int i = 0; i < m_row; i++){
        for(int j = 0; j < count; j++){
            m_myModelData[i].remove(column);
            m_hHeaderData.removeAt(column);
        }
    }
    endRemoveColumns();
    return true;
}
