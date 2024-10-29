#ifndef MULTISELECTCOMBOBOX_H
#define MULTISELECTCOMBOBOX_H

#include <QComboBox>
#include <QListWidget>

class MultiSelectComboBox : public QComboBox
{
    Q_OBJECT

public:
    MultiSelectComboBox(QWidget* aParent = Q_NULLPTR);
    void addItem(const QString& aText, const QVariant& aUserData = QVariant());
    void addItems(const QStringList& aTexts);
    QStringList currentText();
    int count() const;
    void showPopup() override;
    void hidePopup() override;
    void SetSearchBarPlaceHolderText(const QString& aPlaceHolderText);
    void SetPlaceHolderText(const QString& aPlaceHolderText);
    void ResetSelection();


signals:
    void selectionChanged();

public slots:
    void clear();
    void setCurrentText(const QString& aText);
    void setCurrentText(const QStringList& aText);


protected:
    bool eventFilter(QObject* aObject, QEvent* aEvent) override;

private:
    void onSearch(const QString& aSearchString);

    QListWidget* mListWidget;
    QLineEdit* mLineEdit;
    QLineEdit* mSearchBar;
};


#endif // MULTISELECTCOMBOBOX_H
