/****************************************************************************
**
** Copyright (C) 2016 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com, author Milian Wolff <milian.wolff@kdab.com>
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtWebChannel module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "dialog.h"
#include "core.h"
#include "../shared/websocketclientwrapper.h"
#include "../shared/websockettransport.h"

#include <QApplication>
#include <QDesktopServices>
#include <QDialog>
#include <QDir>
#include <QFileInfo>
#include <QUrl>
#include <QWebChannel>
#include <QWebSocketServer>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    QFileInfo jsFileInfo(QDir::currentPath() + "/qwebchannel.js");//获取qwebchannel.js的信息

    if (!jsFileInfo.exists())  //如果不存在qwebchannel.js
        QFile::copy(":/qtwebchannel/qwebchannel.js",jsFileInfo.absoluteFilePath()); //复制 qwebchannel.js到absoluteFilePath = QDir::currentPath()
    // setup the QWebSocketServer
    QWebSocketServer server(QStringLiteral("QWebChannel Standalone Example Server"), QWebSocketServer::NonSecureMode); //创建一个socket Server
    if (!server.listen(QHostAddress::LocalHost, 12345)) {   //使用12345这个端口
        qFatal("Failed to open web socket server.");        //端口使用失败,打印错误信息(类似于qDebug)
        return 1;
    }

    // wrap WebSocket clients in QWebChannelAbstractTransport objects
    WebSocketClientWrapper clientWrapper(&server);  //将webSocket Server包装在 QWebChannelAbstractTransport对象,QWebChannel必须要有QWebChannelAbstractTransport对象才能工作

    // setup the channel
    QWebChannel channel;
    QObject::connect(&clientWrapper, &WebSocketClientWrapper::clientConnected,
                     &channel, &QWebChannel::connectTo);  //将 WebSocketClientWrapper 和 QWebChannel 连接 //当有新client连接到Server时,通知QWebchannel

    // setup the UI
    Dialog dialog;

    // setup the core and publish it to the QWebChannel
    Core core(&dialog);
    channel.registerObject(QStringLiteral("core"), &core);  //将和js映射的qt类放入channel中

    // open a browser window with the client HTML page
//    QUrl url = QUrl::fromLocalFile(BUILD_DIR "/index.html");
//    QDesktopServices::openUrl(url);

//    dialog.displayMessage(Dialog::tr("Initialization complete, opening browser at %1.").arg(url.toDisplayString()));
    dialog.show();

    return app.exec();
}
