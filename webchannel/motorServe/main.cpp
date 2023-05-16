#include "widget.h"

#include "./shared/websocketclientwrapper.h"
#include "./shared/websockettransport.h"

#include <QApplication>
#include <QDir>
#include <QFileInfo>
#include <QUrl>
#include <QWebChannel>
#include <QWebSocketServer>
#include <QMessageBox>

#include "jsapi.h"
#include "jlinkAPI.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    for(int i = 0;i<argc;i++){
//        QString string = argv[i];
//        qDebug()<<"argv"<<string;
//    }
    if(argc < 2){
        QMessageBox::warning(nullptr, "warning", "启动程序时需要输出端口号");
    }
    QString string = argv[1];
    uint16_t socketPort = static_cast<uint16_t>(string.toInt());
    qDebug()<<"socketPort"<<socketPort;

    //copy qwebchannel.js to app.exe
    QFileInfo jsFileInfo(QDir::currentPath() + "/qwebchannel.js");

    if (!jsFileInfo.exists())  //如果不存在qwebchannel.js
        QFile::copy(":/qtwebchannel/qwebchannel.js",jsFileInfo.absoluteFilePath()); //复制 qwebchannel.js到absoluteFilePath = QDir::currentPath()
    // setup the QWebSocketServer
    QWebSocketServer server(QStringLiteral("QWebChannel Standalone Example Server"), QWebSocketServer::NonSecureMode); //创建一个socket Server
//  if (!server.listen(QHostAddress::LocalHost, socketPort)) {   //使用12345这个端口
    if (!server.listen(QHostAddress("172.16.10.107"), socketPort)) {   //使用12345这个端口
        qDebug()<<("Failed to open web socket server.");        //端口使用失败,打印错误信息(类似于qDebug)
        return 1;
    }

    // wrap WebSocket clients in QWebChannelAbstractTransport objects
    WebSocketClientWrapper clientWrapper(&server);  //将webSocket Server包装在 QWebChannelAbstractTransport对象,QWebChannel必须要有QWebChannelAbstractTransport对象才能工作

    // setup the channel
    QWebChannel channel;
    QObject::connect(&clientWrapper, &WebSocketClientWrapper::clientConnected,
                     &channel, &QWebChannel::connectTo);  //将 WebSocketClientWrapper 和 QWebChannel 连接 //当有新client连接到Server时,通知QWebchannel


    jlinkAPI jlinker;
    Widget w(nullptr,&jlinker);
    jsapi jschannel(&w,&jlinker);
    channel.registerObject(QStringLiteral("jschannel"), &jschannel);  //将和js映射的qt类放入channel中

    //w.show();
    return a.exec();
}

