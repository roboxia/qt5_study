#include "jsapi.h"
#include <QDebug>

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>


jsapi::jsapi(QObject *parent,jlinkAPI *debugDLL) :
    QObject(parent),
  _debugDLL(debugDLL)
{

}

jsapi::~jsapi()
{
     _debugDLL->connectClose();
}

QString jsapi::jsWriteReg(QString addr, QString value)
{
    uint32_t a;
    uint32_t v;
    if(addr.indexOf("0x")!=-1 ||addr.indexOf("0X")!=-1){
        a = static_cast<uint32_t>(addr.toInt(nullptr,16));
    }
    else {
        a = static_cast<uint32_t>(addr.toInt(nullptr,10));
    }

    if(value.indexOf("0x")!=-1 ||value.indexOf("0X")!=-1){
        v = static_cast<uint32_t>(value.toInt(nullptr,16));
    }
    else {
        v = static_cast<uint32_t>(value.toInt(nullptr,10));
    }

    _debugDLL->JLINKARM_WriteU32(a,v);
    qDebug()<<"jsWriteReg";
    return "return value";
}

QString jsapi::jsReadReg(QString addr)
{
    uint32_t a;
    uint16_t v;
    if(addr.indexOf("0x")!=-1 ||addr.indexOf("0X")!=-1){
        a = static_cast<uint32_t>(addr.toInt(nullptr,16));
    }
    else {
        a = static_cast<uint32_t>(addr.toInt(nullptr,10));
    }
    _debugDLL->JLINKARM_ReadMemU16(a ,1,&v,nullptr);
    QString sv = QString::number(v);
    QString errcode = "0";
    QString info = "success";

    QString message = "code:"+errcode+","
            +"msg:"+info+","
            +"data:["+sv+"]";
    //emit qtSendReg(message);
    return message;
}

void jsapi::exitServerApp()
{
    qApp->quit();
}

QByteArray jsapi::jlinkConnect(QByteArray config)
{
    char code = JLINK_CONERR_NO;

    JLINK_INIT_PARAS Paras;
    memset(&Paras, 0, sizeof(JLINK_INIT_PARAS));

    QJsonParseError jsonError;
    QJsonDocument jsonConfig = QJsonDocument::fromJson(config, &jsonError);
    if( !jsonConfig.isNull() && (jsonError.error == QJsonParseError::NoError)) //解析未发生错误
    {
        if(jsonConfig.isObject())
        {
            QJsonObject jsobject = jsonConfig.object();

            Paras.HostIF = static_cast<u32>(jsobject.value("hostIF").toInt());
            Paras.TargetIF = static_cast<u32>(jsobject.value("targetIF").toInt());
            Paras.SerialNo = static_cast<u32>(jsobject.value("serialNo").toInt());
            Paras.Speed = static_cast<u32>(jsobject.value("speed").toInt());

            QByteArray arrayBuff1 = jsobject.value("sSettingsFile").toString().toLatin1();
            Paras.sSettingsFile = arrayBuff1.data();

            QByteArray arrayBuff2 = jsobject.value("sDevice").toString().toLatin1();
            Paras.sDevice =arrayBuff2.data();

            QByteArray arrayBuff3 = jsobject.value("sScriptFile").toString().toLatin1();
            Paras.sScriptFile =arrayBuff3.data();

//            qDebug()<<"config"<<config;
//            qDebug()<<"Paras"<<Paras.HostIF;
//            qDebug()<<"Paras"<<Paras.TargetIF;
//            qDebug()<<"Paras"<<Paras.SerialNo;
//            qDebug()<<"Paras"<<Paras.sSettingsFile;
//            qDebug()<<"Paras"<<Paras.sDevice;
//            qDebug()<<"Paras"<<Paras.sScriptFile;
            code = _debugDLL->connectTarget(Paras);
        }
    }
    else {
        code = JLINK_CONERR_PARAS;
    }

    QJsonDocument document;
    QJsonObject object;
    object.insert("code",code);
    if(code == JLINK_CONERR_NO){
        object.insert("msg","success");
    }
    else {
        object.insert("msg","failed");
    }


    QJsonArray array;
    object.insert("data",array);
    document.setObject(object);

    return  document.toJson(QJsonDocument::Compact);
}


void jsapi::closeJlink()
{
    _debugDLL->connectClose();
}

void jsapi::openRTT()
{

}

void jsapi::closeRTT()
{

}

void jsapi::openRTTPlot()
{

}

void jsapi::closeRTTPlot()
{

}

void jsapi::openRegPlot()
{

}

void jsapi::closeRegPlot()
{

}
