#ifndef JSAPI_H
#define JSAPI_H

#include <QObject>
#include "jlinkapi.h"
#include <QApplication>

class jsapi : public QObject
{
    Q_OBJECT
public:
    explicit jsapi(QObject *parent = nullptr,jlinkAPI *debugDLL = nullptr);
    ~jsapi();
//Qt向Js通信使用信号
signals:
   // void qtSendReg(const QString &message);


//Js向Qt通信使用槽函数
public slots:

    QString jsWriteReg(QString addr,QString value);  //接收函数,难道是js直接调用该函数?


    QString jsReadReg(QString addr);  //接收函数,难道是js直接调用该函数?

    //关闭QT服务器程序
    void exitServerApp();

    //Jlink连接相关
    QByteArray jlinkConnect(QByteArray config);
    void closeJlink();

    //RTT连接相关
    void openRTT();
    void closeRTT();

    //显示RTT波形
    void openRTTPlot();
    void closeRTTPlot();

    //显示某个寄存器的波形
    void openRegPlot();
    void closeRegPlot();
private:
    jlinkAPI *_debugDLL;

};

#endif // JSAPI_H
