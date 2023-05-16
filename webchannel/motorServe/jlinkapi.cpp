#include "jlinkapi.h"
#include <QDebug>
#include <string.h>

jlinkAPI::jlinkAPI(void)
{
    //构造函数初始化时，new QLibaray时传入使用的dll文件
    //jlink_lib = new QLibrary("JLink_x64.dll");
    jlink_lib = new QLibrary("JLinkARM.dll");

    if(jlink_lib->load()){
        qDebug()<<"成功";
        JLINKARM_Open =  reinterpret_cast<JLINKARM_Open_Func_Ptr>(jlink_lib->resolve("JLINKARM_Open"));
        JLINKARM_IsOpen = reinterpret_cast<JLINKARM_IsOpen_Func_Ptr>(jlink_lib->resolve("JLINKARM_IsOpen"));
        JLINKARM_Close = reinterpret_cast<JLINKARM_Close_Func_Ptr>(jlink_lib->resolve("JLINKARM_Close"));
        JLINKARM_ExecCommand = reinterpret_cast<JLINKARM_ExecCommand_Func_Ptr>(jlink_lib->resolve("JLINKARM_ExecCommand"));
        JLINKARM_GetDLLVersion = reinterpret_cast<JLINKARM_GetDLLVersion_Func_Ptr>(jlink_lib->resolve("JLINKARM_GetDLLVersion"));
        JLINKARM_TIF_Select = reinterpret_cast<JLINKARM_TIF_Select_Func_Ptr>(jlink_lib->resolve("JLINKARM_TIF_Select"));
        JLINKARM_SetSpeed = reinterpret_cast<JLINKARM_SetSpeed_Func_Ptr>(jlink_lib->resolve("JLINKARM_SetSpeed"));
        JLINKARM_GetSpeed = reinterpret_cast<JLINKARM_GetSpeed_Func_Ptr>(jlink_lib->resolve("JLINKARM_GetSpeed"));
        JLINKARM_GetDeviceFamily = reinterpret_cast<JLINKARM_GetDeviceFamily_Func_Ptr>(jlink_lib->resolve("JLINKARM_GetDeviceFamily"));
        JLINKARM_Connect = reinterpret_cast<JLINKARM_Connect_Func_Ptr>(jlink_lib->resolve("JLINKARM_Connect"));
        JLINKARM_IsConnected = reinterpret_cast<JLINKARM_IsConnected_Func_Ptr>(jlink_lib->resolve("JLINKARM_IsConnected"));
        JLINKARM_GetId = reinterpret_cast<JLINKARM_GetId_Func_Ptr>(jlink_lib->resolve("JLINKARM_GetId"));
        JLINKARM_GetSN = reinterpret_cast<JLINKARM_GetSN_Func_Ptr>(jlink_lib->resolve("JLINKARM_GetSN"));
        JLINKARM_Reset = reinterpret_cast<JLINKARM_Reset_Func_Ptr>(jlink_lib->resolve("JLINKARM_Reset"));
        JLINKARM_Halt = reinterpret_cast<JLINKARM_Halt_Func_Ptr>(jlink_lib->resolve("JLINKARM_Halt"));

        JLINKARM_WriteMem = reinterpret_cast<JLINKARM_WriteMem_Func_Ptr>(jlink_lib->resolve("JLINKARM_WriteMem"));
        JLINKARM_ReadMem = reinterpret_cast<JLINKARM_ReadMem_Func_Ptr>(jlink_lib->resolve("JLINKARM_ReadMem"));
        JLINKARM_WriteU8 = reinterpret_cast<JLINKARM_WriteU8_Func_Ptr>(jlink_lib->resolve("JLINKARM_WriteU8"));
        JLINKARM_ReadMemU8 = reinterpret_cast<JLINKARM_ReadMemU8_Func_Ptr>(jlink_lib->resolve("JLINKARM_ReadMemU8"));
        JLINKARM_WriteU16 = reinterpret_cast<JLINKARM_WriteU16_Func_Ptr>(jlink_lib->resolve("JLINKARM_WriteU16"));
        JLINKARM_ReadMemU16 = reinterpret_cast<JLINKARM_ReadMemU16_Func_Ptr>(jlink_lib->resolve("JLINKARM_ReadMemU16"));
        JLINKARM_WriteU32 = reinterpret_cast<JLINKARM_WriteU32_Func_Ptr>(jlink_lib->resolve("JLINKARM_WriteU32"));
        JLINKARM_ReadMemU32 = reinterpret_cast<JLINKARM_ReadMemU32_Func_Ptr>(jlink_lib->resolve("JLINKARM_ReadMemU32"));

        JLINKARM_SelectIP = reinterpret_cast<JLINKARM_SelectIP_Func_Ptr>(jlink_lib->resolve("JLINKARM_SelectIP"));

        JLINKARM_Go = reinterpret_cast<JLINKARM_Go_Func_Ptr>(jlink_lib->resolve("JLINKARM_Go"));

        //RTT
        JLINK_RTTERMINAL_Control = reinterpret_cast<JLINK_RTTERMINAL_Control_Func_Ptr>(jlink_lib->resolve("JLINK_RTTERMINAL_Control"));
        JLINK_RTTERMINAL_Read = reinterpret_cast<JLINK_RTTERMINAL_Read_Func_Ptr>(jlink_lib->resolve("JLINK_RTTERMINAL_Read"));
        JLINK_RTTERMINAL_Write = reinterpret_cast<JLINK_RTTERMINAL_Write_Func_Ptr>(jlink_lib->resolve("JLINK_RTTERMINAL_Write"));

//        JLINK_EraseChip = reinterpret_cast<JLINK_EraseChip_Func_Ptr>(jlink_lib->resolve("JLINK_EraseChip"));
        qDebug()<<"解析函数完成";
    }
    else
    {
        qDebug()<<"加载dll失败!!";
        qDebug()<<jlink_lib->errorString();
    }
}



jlinkAPI::~jlinkAPI()
{
    jlink_lib->unload();
}


char jlinkAPI::connectTarget(JLINK_INIT_PARAS &paras)
{
    _config = paras;
    int r;
    char acIn[0x400];
    char acOut[0x400];
    const char* sError;
    if (paras.HostIF == JLINK_HOST_IP) {
        //
        // If sHost is NULL, J-Link selection dialog will pop-up
        // on JLINKARM_Open() / JLINKARM_OpenEx(), showing all
        // emulators that have been found in the network
        // Passing 0 as port selects the default port (19020).
        //
        r = JLINKARM_SelectIP(paras.sHost, 0);
        if (r == 1) {
            return JLINK_CONERR_IP; // Error occurred during configuration of IP interface
        }
    } else { // Connect via USB
    //
    // Was a specific serial number set we shall to connect to?
    //
        if (paras.SerialNo) {  //If not set serialNO ,used default
            r = JLINKARM_EMU_SelectByUSBSN(paras.SerialNo);
            if (r < 0) {
                return JLINK_CONERR_USB; // Error: Specific serial number not found on USB
            }
        }
    }

    sError = JLINKARM_Open(); //If there is no USB device ,defalut select ip device
    if (sError) { // Error occurred while connecting to J-Link?
        //printf("Error: %s", sError);
        return JLINK_CONERR_OPEN;
    }
    //
    // Select settings file
    // Used by the control panel to store its settings and can be used by the user to
    // enable features like flash breakpoints in external CFI flash, if not selectable
    // by the debugger. There should be different settings files for different debug
    // configurations, for example different settings files for LEDSample_DebugFlash
    // and LEDSample_DebugRAM. If this file does not exist, the DLL will create one
    // with default settings. If the file is already present, it will be used to load
    // the control panel settings
    //
    if (paras.sSettingsFile) {
        strcpy_s(acIn, "ProjectFile = ");
        strcat_s(acIn, paras.sSettingsFile);
        JLINKARM_ExecCommand(acIn, acOut, sizeof(acOut));
        if (acOut[0]) {
            return JLINK_CONERR_FILE;
        }
     }


    if (paras.sDevice) {
        strcpy_s(acIn, "device = ");
        strcat_s(acIn, paras.sDevice);
        JLINKARM_ExecCommand(acIn, &acOut[0], sizeof(acOut));
        if (acOut[0]) {
            return JLINK_CONERR_DEVICE;
        }
     }

    if (paras.sScriptFile) {
        strcpy_s(acIn, "ScriptFile = ");
        strcat_s(acIn, paras.sScriptFile);
        JLINKARM_ExecCommand(acIn, nullptr, 0);
    }

    JLINKARM_TIF_Select(paras.TargetIF);
    JLINKARM_SetSpeed(paras.Speed);

    r = JLINKARM_Connect();
    if (r) {
        return JLINK_CONERR_CONNECT;
    }

    return JLINK_CONERR_NO;
}

void jlinkAPI::connectClose()
{
    JLINKARM_Close();
}











