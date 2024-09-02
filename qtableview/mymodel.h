#ifndef MYMODEL_H
#define MYMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>

struct MyModelData_t
{
    int index;
    QString name;
    QModelIndex myParent;
    QModelIndex myChild;
    int row;
    int column;
};


class MyModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit MyModel(QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    bool setHeaderData(int section,
                       Qt::Orientation orientation,
                       const QVariant &value,
                       int role = Qt::EditRole) override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    // Fetch data dynamically:
    //bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;

    //bool canFetchMore(const QModelIndex &parent) const override;
    //void fetchMore(const QModelIndex &parent) override;

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
private:
    QStringList m_vHeaderData;
    QStringList m_hHeaderData;

    QVector<QVector<QString>>  m_myModelData; //row<column> data
    int m_row;
    int m_column;
};


Q_DECLARE_METATYPE(MyModelData_t)

#endif // MYMODEL_H


