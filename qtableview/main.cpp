#include <QApplication>

#include "tablewidgettest.h"
#include "tableviewtest.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    tableviewtest w;
    w.show();

    TableWidgetTest t;
    t.show();

    return a.exec();
}
