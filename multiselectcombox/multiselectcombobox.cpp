
#include "MultiSelectComboBox.h"
#include <QLineEdit>
#include <QEvent>
#include <QScrollBar>
#include <QDebug>
#include <QMouseEvent>


MultiSelectComboBox::MultiSelectComboBox(QWidget* aParent) :
    QComboBox(aParent),
    mListWidget(new QListWidget(this)),
    mLineEdit(new QLineEdit(this)),
    mSearchBar(new QLineEdit(this))
{
    QListWidgetItem* curItem = new QListWidgetItem(mListWidget);
    mSearchBar->setPlaceholderText("Search..");
    mSearchBar->setClearButtonEnabled(true);
    mListWidget->addItem(curItem);
    mListWidget->setItemWidget(curItem, mSearchBar);

    mLineEdit->setReadOnly(true);
    // mLineEdit->installEventFilter(this);


    setModel(mListWidget->model());
    setView(mListWidget);
    setLineEdit(mLineEdit);

    connect(mSearchBar, &QLineEdit::textChanged, this, &MultiSelectComboBox::onSearch);


    connect(this, &QComboBox::currentTextChanged, this, [this](QString text){
        //note: combox内部会自动将文本框中的内容设置为当前check的那个item.
        //所以每次改变文本之后都需要将其更新回来.

        //禁止发出currentTextChanged信号
        QSignalBlocker blocker(this);
        mLineEdit->setText(currentText().join(";"));
    });

    connect(mListWidget, &QListWidget::itemPressed, this, [=](QListWidgetItem *item){
        qDebug()<<"itemPressed";
        if(item->checkState() == Qt::Unchecked){
            item->setCheckState(Qt::Checked);
        }else{
            item->setCheckState(Qt::Unchecked);
        }
    });
}

void MultiSelectComboBox::showPopup()
{

    //固定选择数量, 解决搜索后显示问题
    //不知道什么原因, 在构造函数中, 设置这个值, 会导致鼠标放在后面一项时自动下滑
    //setMinimumHeight 设置的高度, 必须必 setMaxVisibleItems 占用的高度小, 否则也会自动下滑
    setMaxVisibleItems(10);
    int maxVisibleRows =9;
    int rowHeight = mListWidget->sizeHintForRow(0);
    mListWidget->setMinimumHeight(maxVisibleRows * rowHeight);


    QComboBox::showPopup();
    //搜索框中文输入问题
    this->view()->activateWindow();
}

void MultiSelectComboBox::hidePopup()
{

    this->setCurrentIndex(mListWidget->currentRow());

    int width = this->width();
    int height = mListWidget->height();
    int x = QCursor::pos().x() - mapToGlobal(geometry().topLeft()).x() + geometry().x();
    int y = QCursor::pos().y() - mapToGlobal(geometry().topLeft()).y() + geometry().y();
    if (x >= 0 && x <= width && y >= this->height() && y <= height + this->height())
    {
        // Item was clicked, do not hide popup
    }
    else
    {
        QComboBox::hidePopup();
    }
}



void MultiSelectComboBox::addItem(const QString& aText, const QVariant& aUserData)
{
    Q_UNUSED(aUserData);
    QListWidgetItem* listWidgetItem = new QListWidgetItem(mListWidget);
    mListWidget->addItem(listWidgetItem);
    listWidgetItem->setCheckState(Qt::Unchecked);
    listWidgetItem->setText(aText);
}

QStringList MultiSelectComboBox::currentText()
{
    QStringList slist;
    for(int i = 1; i < mListWidget->count(); i++)
    {
        QListWidgetItem* item = mListWidget->item(i);
        if(item->checkState() == Qt::Checked)
        {
            slist.append(item->text());
        }
    }
    return slist;
}

void MultiSelectComboBox::addItems(const QStringList& aTexts)
{
    for(const auto& string : aTexts)
    {
        addItem(string);
    }
}

int MultiSelectComboBox::count() const
{
    int count = mListWidget->count() - 1;// Do not count the search bar
    if(count < 0)
    {
        count = 0;
    }
    return count;
}



void MultiSelectComboBox::onSearch(const QString& aSearchString)
{
    for(int i = 1; i < mListWidget->count(); i++)
    {
        QString text = mListWidget->item(i)->text();
        if(text.contains(aSearchString))
        {
            mListWidget->item(i)->setHidden(false);
        }
        else
        {
            mListWidget->item(i)->setHidden(true); //隐藏
        }
    }
}




void MultiSelectComboBox::SetSearchBarPlaceHolderText(const QString& aPlaceHolderText)
{
    mSearchBar->setPlaceholderText(aPlaceHolderText);
}

void MultiSelectComboBox::SetPlaceHolderText(const QString& aPlaceHolderText)
{
    mLineEdit->setPlaceholderText(aPlaceHolderText);
}

void MultiSelectComboBox::clear()
{
    mListWidget->clear();
    QListWidgetItem* curItem = new QListWidgetItem(mListWidget);
    mSearchBar = new QLineEdit(this);
    mSearchBar->setPlaceholderText("Search..");
    mSearchBar->setClearButtonEnabled(true);
    mListWidget->addItem(curItem);
    mListWidget->setItemWidget(curItem, mSearchBar);

    connect(mSearchBar, &QLineEdit::textChanged, this, &MultiSelectComboBox::onSearch);
}

bool MultiSelectComboBox::eventFilter(QObject* aObject, QEvent* aEvent)
{
    // if(aObject == mLineEdit && aEvent->type() == QEvent::MouseButtonRelease) {
    //     QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(aEvent);
    //     if(mouseEvent->button() == Qt::LeftButton){
    //         showPopup();
    //         return true;
    //     }
    // }
    return QComboBox::eventFilter(aObject, aEvent);
}




void MultiSelectComboBox::setCurrentText(const QString& aText)
{
    Q_UNUSED(aText);
}

void MultiSelectComboBox::setCurrentText(const QStringList& aText)
{
    int count = mListWidget->count();

    for (int i = 1; i < count; ++i)
    {
        QListWidgetItem *item = mListWidget->item(i);
        if(aText.contains(item->text())){
            item->setCheckState(Qt::Checked);
        }
    }
}

void MultiSelectComboBox::ResetSelection()
{
    int count = mListWidget->count();

    for (int i = 1; i < count; ++i)
    {
        QListWidgetItem *item = mListWidget->item(i);
        item->setCheckState(Qt::Unchecked);
    }
}
