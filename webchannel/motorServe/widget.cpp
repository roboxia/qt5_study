#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QMessageBox>

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
Widget::Widget(QWidget *parent,jlinkAPI *debugDLL) :
    QWidget(parent),
    ui(new Ui::Widget),
    _debugDLL(debugDLL)
{
    ui->setupUi(this);



}

Widget::~Widget()
{
    //qDebug()<<"~widgets";

    delete ui;
}

/**
 * @brief Widget::analysisRTT 解析RTT数据
 *
 */
void Widget::analysisRTT()
{

}


void Widget::on_pushButton_clicked()
{
     qApp->quit();
}


void Widget::on_pushButton_2_clicked()
{
    JLINK_RTTERMINAL_BUFDESC buff;
    JLINK_RTTERMINAL_START start;
    start.ConfigBlockAddress = 0;
    buff.BufferIndex = 1;
    buff.Direction = JLINKARM_RTTERMINAL_DIR_UP;
    char buff2[30] = {0};
    _debugDLL->JLINK_RTTERMINAL_Control(JLINKARM_RTTERMINAL_CMD_START,reinterpret_cast<void*>(&start));
    _debugDLL->JLINK_RTTERMINAL_Control(JLINKARM_RTTERMINAL_CMD_GETNUMBUF,reinterpret_cast<void*>(&buff));
    qDebug()<<buff.BufferIndex;
    qDebug()<<buff.Direction;
    qDebug()<<buff.acName;
    qDebug()<<buff.SizeOfBuffer;
    qDebug()<<buff.Flags;
    _debugDLL->JLINK_RTTERMINAL_Read(1,buff2,30);
    QString send(buff2);
    qDebug()<<send;
}

