#ifndef TABLEITEM_H
#define TABLEITEM_H

#include <QObject>
#include <QVector>
#include <QVariant>
#include <QIcon>


using namespace Qt;


//层级数据结构. 主要就是说要有层级信息.
//正常索引过程中全部是按照层级进行索引?

class TableRowItem
{
public:
    enum UserRoles
    {
        CheckEnableRole = UserRole + 1,

    };

    //如果存在parentItem, 自动同步 columnCount
    TableRowItem(TableRowItem *parentItem = nullptr,TableRowItem* indexItem = nullptr);
    explicit  TableRowItem(TableRowItem *parentItem = nullptr,int index = -1);


    ~TableRowItem();

    //同步设置所以子项.
    void insertColumns(int column,int count = 1);
    void removeColumns(int column,int count = 1);

    //默认DisplayRole和EditRole是一样的.
    bool setData(int column, const QVariant &value, int role = DisplayRole);
    QVariant data(int column, int role = DisplayRole) const;


    //item
    TableRowItem* parentItem() const;
    QVector<TableRowItem*> childItems() const;
    TableRowItem* childItem(int index) const;
    int childItemIndex(TableRowItem* childItem) const;

    //一般不能直接变更item子项. 需要通过model操作. 否则会显示不正常.
    TableRowItem* takeChildItem(int index);
    int takeChildItem(TableRowItem* childItem);


    //可以使用 deleteSelf 和 delete来删除自己. 删除过程会将自己从父级中删除.并且删除所有子项.
    void deleteSelf();
    void deleteChildItem(int index);


    //计数
    int childCount();
    int childCountCount();
    //在父级中的index(subRow)
    int index();
    //获取当前处于什么层级
    int level();

    //设置指定role的快捷函数.
    void setCheckable(int column,bool checkable);
    bool checkable(int column) const;
    void setIcon(int column,const QIcon &icon);
    QIcon icon(int column) const;
    bool isValid(int column);

    void clearData();

    int columnCount() const;

private:
    bool insertChildItem(int index,TableRowItem* item);
    //该类中最好都通过 setData 和 columnData来操作数据, 以保证 roleMap 的正确性.
    //不然需要校验 roleMap 的正确性.
    //<roles<column>>
    QVector<QVariantList> m_colunmData;
    QMap<int,int> roleMap;

    TableRowItem* m_parentItem;
    QVector<TableRowItem*> m_childItem;

    int m_level; //记录当前层级, 用于在table中显示不同的层级效果.
    int m_columnCount;
};

#endif // TABLEITEM_H
