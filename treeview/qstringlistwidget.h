#ifndef QSTRINGLISTWIDGET_H
#define QSTRINGLISTWIDGET_H

#include <QMainWindow>
#include <QStringListModel>
#include <QTreeWidget>
namespace Ui {
class QStringListWidget;
}

class QStringListWidget : public QMainWindow
{
    Q_OBJECT

public:
    explicit QStringListWidget(QWidget *parent = nullptr);
    ~QStringListWidget();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::QStringListWidget *ui;
};

#endif // QSTRINGLISTWIDGET_H
