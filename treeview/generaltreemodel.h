#ifndef GENERALTREEMODEL_H
#define GENERALTREEMODEL_H

#include <QAbstractItemModel>
#include <QVector>
#include <QVariant>
#include <QIcon>

using namespace Qt;

class GeneralTreeModel;


/**
 *1. RowItem只作为一个单纯的树状链表进行使用. 不能操作Model的相关功能.
 *2. RowItem的数据链表操作,都需要从父类进行操作. 每一次 new 一个新的对象. 都表示一个新的根节点(无parent) 但是可以设置paent
 *直接全部在model里面进行操作了. 就不使用这个item了.
 */
class RowItem
{
public:
    enum UserData
    {
        ItemFlagsData = UserRole + 1,
        EnableIndexRowData, //记录Item的Display数据是否使用index的行号代替
    };

    RowItem();

    ~RowItem();


    //自身所在parnet下的 行号
    int row() const;
    //子行的数量. 不嵌套
    int rowCount() const;
    int columnCount() const;

    //当前所处的层级
    int level();

    //如果传入无效index, 返回nullptr
    static RowItem* fromIndex(const QModelIndex &index);
protected:
    friend class GeneralTreeModel;

    //自身及child设置columns
    bool minsertColumns(int column,int count = 1);
    bool mremoveColumns(int column,int count = 1);

    //要求child的parent为nullptr.
    //1. 设置child的parent = this
    //3. 更新child的column
    bool insertChild(int row,RowItem *child);
    bool addChild(RowItem *child);
    QList<RowItem*> takeChildren(int row, int count =1);
    bool takeChild(RowItem *child);

    //查询数据
    bool setData(int column, const QVariant &value, int role = DisplayRole);
    QVariant data(int column, int role = DisplayRole) const;

    //通过这两个函数只能修改或者获取数据和属性, 不能修改父,或者子
    RowItem *child(int row) const;
    RowItem *parent() const;


    //变更chrild  //删除, 取出
    bool minsertRows(int row,int count = 1); //插入操作的实际操作者
    void mremoveRows(int row,int count = 1);
private:
    int m_columnCount;

    //role,columnData
    QMap<int,QVariantList> m_colunmData;

    RowItem* m_parentItem;
    QList<RowItem*> m_childItem;
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
    //从 row 开始, 新增 count 行数据. 将新增行数按照正常顺序插入到原表中
    //如果 parent 为空, 则在根节点添加
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool insertRows(int row, QList<RowItem*> items, const QModelIndex &parent = QModelIndex());
    bool insertColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;


    // Remove data:
    QList<RowItem*> takeChildren(int row, int count ,const QModelIndex &parent = QModelIndex());
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
    void removeRow(QModelIndex index);
    void appendRow(const QModelIndex &parent = QModelIndex());

    RowItem *item(int row,const QModelIndex &parent = QModelIndex());

    void clear();

protected:
    friend class RowItem;


private:
    RowItem *m_rootItem;
    QStringList m_horizontalHeaders;
    QStringList m_verticalHeaders;


};

#endif // GENERALTREEMODEL_H
