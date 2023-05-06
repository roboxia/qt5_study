#ifndef JLINKAPI_H
#define JLINKAPI_H

#include <QLibrary>

#define JLINK_HOST_USB 0
#define JLINK_HOST_IP 1


//JLINK TIF
#define JLINKARM_TIF_JTAG	0
#define JLINKARM_TIF_SWD	1

//RESET TYPE
#define JLINKARM_RESET_TYPE_NORMAL 0
#define JLINKARM_RESET_TYPE_CORE   1
#define JLINKARM_RESET_TYPE_PIN    2


#define JLINK_CONERR_NO 0 //Connect successful
#define JLINK_CONERR_IP 1 // Error occurred during configuration of IP interface
#define JLINK_CONERR_USB 2// Error: Specific serial number not found on USB
#define JLINK_CONERR_OPEN 3 //Open jlink dll err
#define JLINK_CONERR_FILE 4 //Project file config err
#define JLINK_CONERR_DEVICE 5 //
#define JLINK_CONERR_CONNECT 6 //Could not connect to target
#define JLINK_CONERR_PARAS 7 //参数错误


//RTT CMD
#define JLINKARM_RTTERMINAL_CMD_START 0
#define JLINKARM_RTTERMINAL_CMD_STOP 1
#define JLINKARM_RTTERMINAL_CMD_GETNUMBUF 2
#define JLINKARM_RTTERMINAL_CMD_GETDESC 3

#define JLINKARM_RTTERMINAL_DIR_UP 0
#define JLINKARM_RTTERMINAL_DIR_DOWN 1

typedef unsigned int u32;
typedef unsigned short int u16;
typedef unsigned char u8;

//About set connection
typedef const char* (*JLINKARM_Open_Func_Ptr)(void);       // 定义导出函数类型
typedef bool (*JLINKARM_IsOpen_Func_Ptr)(void);
typedef void (*JLINKARM_Close_Func_Ptr)(void);
typedef char (*JLINKARM_SelectIP_Func_Ptr)(const char* sHost, int Port);
typedef int (*JLINKARM_EMU_SelectByUSBSN_Func_Ptr)(u32);   // 通过USB连接PC的不同的Jlink的SN码连接对应的Jlink

typedef int  (*JLINKARM_Connect_Func_Ptr)(void);
typedef bool  (*JLINKARM_IsConnected_Func_Ptr)(void);

typedef u32 (*JLINKARM_TIF_Select_Func_Ptr)(u32); //选择指定的目标接口
typedef void (*JLINKARM_SetSpeed_Func_Ptr)(u32);

typedef int  (*JLINKARM_ExecCommand_Func_Ptr)(const char* sIn, char* sError, int BufferSize);//添加配置信息

typedef void  (*JLINKARM_SetLogFile_Func_Ptr)(char *file); //Set log output file

//About get connection infomation
typedef u32 (*JLINKARM_GetSpeed_Func_Ptr)(void);

typedef u32 (*JLINKARM_GetSN_Func_Ptr)(void);
typedef u32 (*JLINKARM_GetId_Func_Ptr)(void);
typedef u32 (*JLINKARM_GetDeviceFamily_Func_Ptr)(void);

typedef u32 (*JLINKARM_GetDLLVersion_Func_Ptr)(void);
typedef u32 (*JLINKARM_GetHardwareVersion_Func_Ptr)(void);
typedef u32 (*JLINKARM_GetFirmwareString_Func_Ptr)(char *buff, int count);


//About read/write memory
typedef int   (*JLINKARM_ReadMem_Func_Ptr)(u32 addr, u32 len, void *buf);
typedef int (*JLINKARM_ReadMemU8_Func_Ptr)(u32 addr, u32 len, u8* pData, u8* pStatus);
typedef int (*JLINKARM_ReadMemU16_Func_Ptr)(u32 addr, u32 len, u16* pData, u8* pStatus);
typedef int (*JLINKARM_ReadMemU32_Func_Ptr)(u32 addr, u32 len, u32* pData, u8* pStatus);


//Writing Flash(ROM) memory will not take effect immediately
//1. We can use JLINKARM_Reset() to make it effective
//   The use JLINKARM_Go() to make cpu run

//2. If write memory not take effect
//   It will automatically take effect when using JLINKARM_Close()
typedef int   (*JLINKARM_WriteMem_Func_Ptr)(u32 addr, u32 len, void *buf);
typedef int   (*JLINKARM_WriteU8_Func_Ptr)(u32 addr, u8 data);
typedef int   (*JLINKARM_WriteU16_Func_Ptr)(u32 addr, u16 data);
typedef int   (*JLINKARM_WriteU32_Func_Ptr)(u32 addr, u32 data);


//About RTT
typedef int (*JLINK_RTTERMINAL_Control_Func_Ptr)(u32 Cmd, void* p);
typedef int (*JLINK_RTTERMINAL_Read_Func_Ptr)(u32 BufferIndex, char* sBuffer, u32 BufferSize);
typedef int (*JLINK_RTTERMINAL_Write_Func_Ptr)(u32 BufferIndex, const char* sBuffer, u32 BufferSize);






typedef void (*JLINKARM_Reset_Func_Ptr)(void);
typedef void (*JLINKARM_Go_Func_Ptr)(void);

typedef int   (*JLINKARM_Halt_Func_Ptr)(void);
typedef bool  (*JLINKARM_IsHalted_Func_Ptr)(void);
typedef void  (*JLINKARM_SetResetType_Func_Ptr)(int type);
typedef void  (*JLINKARM_Reset_Func_Ptr)(void);
typedef void  (*JLINKARM_Go_Func_Ptr)(void);
typedef void  (*JLINKARM_GoIntDis_Func_Ptr)(void);
typedef u32 (*JLINKARM_ReadReg_Func_Ptr)(int index);
typedef int   (*JLINKARM_WriteReg_Func_Ptr)(int index, u32 data);

typedef int   (*JLINK_EraseChip_Func_Ptr)(void);
typedef int   (*JLINK_DownloadFile_Func_Ptr)(const char* filename, u32 addr);
typedef void  (*JLINKARM_BeginDownload_Func_Ptr)(int index);
typedef void  (*JLINKARM_EndDownload_Func_Ptr)(void);

typedef struct {
 u32 HostIF;   //[necessary] Host interface used to connect to J-Link. // 0 = USB, 1 = IP
 u32 TargetIF; //[necessary] See JLINKARM_Const.h // select JTAG or SWD
 u32 SerialNo; //Serial number of J-Link we want to connect to via USB
 u32 Speed; //[necessary] Interface speed in kHz
 const char* sHost; // Points to the IPAddr / nickname of the J-Link we want to connect to.
 const char* sSettingsFile; //Don't know what it's for
 const char* sDevice; //[necessary] Target device J-Link is connected to
 const char* sScriptFile;  // J-Link script file to perform special connect etc.
} JLINK_INIT_PARAS;

typedef struct {
 u32 ConfigBlockAddress; //Address of RTT block.
 u32 Dummy0; //SBZ. Reserved for future use.
 u32 Dummy1;
 u32 Dummy2;
}JLINK_RTTERMINAL_START;

typedef struct {
 u32 InvalidateTargetCB; //If set, RTTCB will be invalidated on target.
 u32 acDummy[3]; //SBZ. Reserved for future use.
 u32 Dummy0;
 u32 Dummy1;
 u32 Dummy2;
}JLINK_RTTERMINAL_STOP;

typedef struct {
 int BufferIndex; //In: Index of the buffer to get info about
 u32 Direction; //In: Direction of the buffer. (0 = Up; 1 =Down)
 char acName[32];//Out: Array for the 0-terminated name of the buffer.
 u32 SizeOfBuffer; //Out: Size of the buffer on the target.
 u32 Flags;  //Out: Flags of the buffer.
}JLINK_RTTERMINAL_BUFDESC;



/*
 * 主要操作流程
 * 1. 打开jlink
 * 2.连接jlink的target
 * 3.设置jlink的MCU型号 . 通信速度 . jlink的通信接口
 * 4.使用jlink读取mcu的flash中的数据
*/

class jlinkAPI
{

public:
    explicit jlinkAPI(void);
    ~jlinkAPI();

    JLINKARM_GetDLLVersion_Func_Ptr JLINKARM_GetDLLVersion = nullptr;   //获取DLL版本
    JLINKARM_Open_Func_Ptr JLINKARM_Open = nullptr;               //打开设备
    JLINKARM_IsOpen_Func_Ptr JLINKARM_IsOpen = nullptr;           //是否已经打开
    JLINKARM_Close_Func_Ptr JLINKARM_Close = nullptr;             //关闭设备
    JLINKARM_TIF_Select_Func_Ptr JLINKARM_TIF_Select = nullptr;   //选择设备
    JLINKARM_SetSpeed_Func_Ptr JLINKARM_SetSpeed = nullptr;       //设置JLINK接口速度 0为自动调整
    JLINKARM_GetSpeed_Func_Ptr JLINKARM_GetSpeed = nullptr;
    JLINKARM_GetDeviceFamily_Func_Ptr JLINKARM_GetDeviceFamily = nullptr;
    JLINKARM_Reset_Func_Ptr JLINKARM_Reset = nullptr;             //复位系统
    JLINKARM_Halt_Func_Ptr JLINKARM_Halt = nullptr;               //中断程序执行，进入停止状态
    JLINKARM_Go_Func_Ptr JLINKARM_Go = nullptr;                   //执行程序

    JLINKARM_WriteMem_Func_Ptr JLINKARM_WriteMem = nullptr;       //写内存
    JLINKARM_ReadMem_Func_Ptr JLINKARM_ReadMem = nullptr;         //读内存
    JLINKARM_WriteU8_Func_Ptr JLINKARM_WriteU8 = nullptr;       //写内存
    JLINKARM_ReadMemU8_Func_Ptr JLINKARM_ReadMemU8 = nullptr;         //读内存
    JLINKARM_WriteU16_Func_Ptr JLINKARM_WriteU16 = nullptr;       //写内存
    JLINKARM_ReadMemU16_Func_Ptr JLINKARM_ReadMemU16 = nullptr;         //读内存
    JLINKARM_WriteU32_Func_Ptr JLINKARM_WriteU32 = nullptr;       //写内存
    JLINKARM_ReadMemU32_Func_Ptr JLINKARM_ReadMemU32 = nullptr;         //读内存

    //RTT
    JLINK_RTTERMINAL_Control_Func_Ptr JLINK_RTTERMINAL_Control = nullptr;
    JLINK_RTTERMINAL_Read_Func_Ptr JLINK_RTTERMINAL_Read = nullptr;
    JLINK_RTTERMINAL_Write_Func_Ptr JLINK_RTTERMINAL_Write = nullptr;


    JLINKARM_Connect_Func_Ptr JLINKARM_Connect = nullptr;
    JLINKARM_IsConnected_Func_Ptr JLINKARM_IsConnected = nullptr;
    JLINKARM_GetId_Func_Ptr JLINKARM_GetId = nullptr;
    JLINKARM_GetSN_Func_Ptr JLINKARM_GetSN = nullptr;
    JLINKARM_EMU_SelectByUSBSN_Func_Ptr JLINKARM_EMU_SelectByUSBSN = nullptr;
    JLINKARM_ExecCommand_Func_Ptr JLINKARM_ExecCommand = nullptr;
    JLINKARM_SelectIP_Func_Ptr JLINKARM_SelectIP  = nullptr;
    char connectTarget(JLINK_INIT_PARAS &paras);
    void connectClose(void);

private:
       JLINK_INIT_PARAS _config;
       QLibrary *jlink_lib;
};

#endif // JLINKAPI_H
