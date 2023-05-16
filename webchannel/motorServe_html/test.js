var script = document.createElement('script');
script.setAttribute('type','text/javascript');
script.setAttribute('src','./qwebchannel.js');
document.getElementsByTagName('head')[0].appendChild(script);


var jschannel;

let receiveQtValueHandle = null;


function receiveQtValue(callback){
    receiveQtValueHandle = callback;
}


//BEGIN SETUP
function connectServerApp(port) { 
    var socket = new WebSocket(`ws://172.16.10.107:${port}`); //创建socket

    socket.onclose = function() {   
        console.error("web channel closed");
    };
    socket.onerror = function(error) {
        console.error("web channel error: " + error);
    }; 
    socket.onopen = function() {
        new QWebChannel(socket, function(channel) {  //创建一个 QWebChannel,输入socket
            // make core object accessible globally
            jschannel = channel.objects.jschannel;   //获取channel的对象,赋值对象
            //jschannel.qtSendReg.connect(receiveQtValueHandle);  //qt向js通信,qt的信号连接js的槽函数,进行通信,该函数自动触发
        });
    }
}


function exitServerApp()
{
    jschannel.exitServerApp();
}

//调用读取函数直接获取到数据可以吗?
//不行的,一个是槽函数,一个是信号,想要获取到数据,必须是通过emit进行

function jsWriteReg(addr,message)
{
    jschannel.jsWriteReg(addr,message,function(returnValue){ 
        console.log(returnValue);
    });
}


function jsReadReg(addr)
{
    jschannel.jsReadReg(addr);
}

function jlinkConnect()
{
    jschannel.jlinkConnect();
}

function closeJlink()
{
    jschannel.closeJlink();
}
//END SETUP
//打开我的app
//连接服务器
//连接jlink
