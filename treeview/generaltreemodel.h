#ifndef GENERALTREEMODEL_H
#define GENERALTREEMODEL_H

#include <QAbstractItemModel>
#include <QVector>
#include <QVariant>
#include <QIcon>

using namespace Qt;

class GeneralTreeModel;



class RowItem
{
public:
    enum UserRoles
    {
        CheckEnableRole = UserRole + 1,
    };

    //如果存在parentItem, 自动同步 columnCount
    RowItem();
    RowItem(RowItem *parentItem);
    RowItem(RowItem *parentItem,int row);
    ~RowItem();

    //同步设置所有子项. model中应该以root节点进行设置
    void insertColumns(int column,int count = 1);
    void removeColumns(int column,int count = 1);

    //item的parent必须为nullptr, 否则跳过添加
    void insertRows(int row,QList<RowItem*> items); //可以插入数据, 但是没法通知model. model是保护函数
    void insertRows(int row, int count);
    void insertRow(int row,RowItem* item);
    void appendRows(QList<RowItem*> items);
    void appendRow(RowItem* item);

    void removeRows(int row,int count);
    void removeRow(int row);

    void setChild(int row,RowItem* item);

    RowItem *takeChild(int row);
    int takeChild(RowItem* item);
    QList<RowItem*> takeChildren();

    RowItem *child(int row);
    QList<RowItem*> children() const;
    RowItem *parent();


    //只有根节点需要设置model.其他节点自动同步
    QAbstractItemModel *model() const;
    //返回自身的index
    QModelIndex index(int column) const;
    //返回子项的index
    QModelIndex indexChild(int row,int column) const;

    int row();
    int childRow(RowItem* item) const;
    int rowCount();
    int columnCount() const;
    int level();

    //默认DisplayRole和EditRole是一样的.
    bool setData(int column, const QVariant &value, int role = DisplayRole);
    QVariant data(int column, int role = DisplayRole) const;


    //设置指定role的快捷函数.
    void setCheckable(int column,bool checkable);
    bool checkable(int column) const;

    void setIcon(int column,const QIcon &icon);
    QIcon icon(int column) const;
    bool isValid(int column);

    //清空自身的数据
    void clearData();
    //清空自身和子项的数据. 嵌套清空
    void clearDataChild();

    static RowItem* fromViod(void* ptr);
protected:
    //需要对root节点赋值
    GeneralTreeModel *m_model;
    RowItem* m_parentItem;
    friend class GeneralTreeModel;

private:
    //该类中最好都通过 setData 和 columnData来操作数据, 以保证 roleMap 的正确性.
    //不然需要校验 roleMap 的正确性.
    //<roles<column>>  [roles][column]
    QList<QVariantList> m_colunmData;
    QMap<int,int> roleMap;
    QList<RowItem*> m_childItem;

    //记录当前层级. root节点为 -1. 0为可见的 top 层
    int m_level;
    int m_columnCount;
};

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

    RowItem *rootItem() const;

    //append
    //remove一行
    //inster一行

    //不构造index获取数据. 直接通过行号
    //快速指定父列设置数据...
    RowItem *insertRow(int row,const QModelIndex &parent = QModelIndex());
    void removeRow(int row,const QModelIndex &parent = QModelIndex());
    void appendRow(const QModelIndex &parent = QModelIndex());

    RowItem *item(int row,int column,const QModelIndex &parent = QModelIndex());
protected:
    friend class RowItem;


private:
    RowItem *m_rootItem;
    QStringList m_horizontalHeaders;
    QStringList m_verticalHeaders;


};

#endif // GENERALTREEMODEL_H
