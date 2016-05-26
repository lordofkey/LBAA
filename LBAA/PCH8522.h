#ifndef _PCH8522_DEVICE_
#define _PCH8522_DEVICE_
//***********************************************************
// 用于AD采样的实际硬件参数
typedef struct _PCH8522_PARA_AD     
{
	LONG bChannelArray[2]; // 采样通道选择阵列，分别控制2个通道，=TRUE表示该通道采样，否则不采样
	LONG Frequency;         // 采集频率,单位为Hz, [10, 80000000]
	LONG InputRange[2];		// 模拟量输入量程选择
	LONG CouplingType;		// 耦合类型(直流耦合, 交流耦合)
	LONG M_Length;          // M段长度(字), 总的取值范围1-32M, 但是M加N长度不能大于32M
	LONG N_Length;          // N段长度(字), 总的取值范围1-32M, 但是M加N长度不能大于32M
	LONG TriggerMode;		// 触发模式选择
	LONG ATRTriggerChannel;	// ATR通道选择
	LONG TriggerSource;		// 触发源选择
	LONG TriggerDir;		// 触发方向选择(正向/负向触发)
	LONG TrigLevelVolt;		// 触发电平(0--10000mV)
	LONG ClockSource;		// 时钟源选择(内/外时钟源)
	LONG OutClockSource;	// 时钟输入输出源
	LONG bClockSourceDir;   // 是否将时钟信号输出到PXI总线,=TRUE:允许输出, =FALSE:允许输入
} PCH8522_PARA_AD, *PPCH8522_PARA_AD;

//***********************************************************
// AD参数PCH8522_PARA_AD中的InputRange[X]使用的硬件增益选项
const long PCH8522_INPUT_N1000_P1000mV		= 0x00; // ±1000mV
const long PCH8522_INPUT_N5000_P5000mV		= 0x01; // ±5000mV

// AD参数PCH8522_PARA_AD中的CouplingType耦合类型所使用的选项
const long PCH8522_COUPLING_AC				= 0x00;	// 交流耦合
const long PCH8522_COUPLING_DC				= 0x01;	// 直流耦合

//***********************************************************
// AD硬件参数PCH8522_PARA_AD中的TriggerMode成员变量所使用触发模式选项
const long PCH8522_TRIGMODE_MIDL			= 0x00; // 中间触发
const long PCH8522_TRIGMODE_POST			= 0x01; // 后触发
const long PCH8522_TRIGMODE_PRE				= 0x02; // 预触发
const long PCH8522_TRIGMODE_DELAY			= 0x03; // 硬件延时触发

//***********************************************************
// AD硬件参数PCH8522_PARA_AD中的ATRTriggerChannel成员变量所使用ATR触发通道选项
const long PCH8522_ATRTRIG_CH0				= 0x00; // 通道0信号作为ATR输入
const long PCH8522_ATRTRIG_CH1				= 0x01; // 通道1信号作为ATR输入

//***********************************************************
// AD硬件参数PCH8522_PARA_AD中的TriggerSource触发源信号所使用的选项
const long PCH8522_TRIGMODE_SOFT			= 0x00; // 软件触发
const long PCH8522_TRIGSRC_DTR				= 0x01; // 选择DTR作为触发源
const long PCH8522_TRIGSRC_ATR				= 0x02; // 选择ATR作为触发源
const long PCH8522_TRIGSRC_TRIGGER		    = 0x03; // Trigger信号触发（用于多卡同步）

//***********************************************************
// AD硬件参数PCH8522_PARA_AD中的TriggerDir触发方向所使用的选项
const long PCH8522_TRIGDIR_NEGATIVE			= 0x00; // 下降沿触发
const long PCH8522_TRIGDIR_POSITIVE			= 0x01; // 上升沿触发
const long PCH8522_TRIGDIR_NEGAT_POSIT		= 0x02; // 上下边沿均触发

//***********************************************************
// AD硬件参数PCH8522_PARA_AD中的ClockSource时钟源所使用的选项
const long PCH8522_CLOCKSRC_IN				= 0x00; // 使用内部时钟
const long PCH8522_CLOCKSRC_OUT				= 0x01; // 使用外部时钟

//***********************************************************
// AD硬件参数PCH8522_PARA_AD中的OutClockSource时钟源所使用的选项
const long PCH8522_OUTCLOCKSRC_TRIGGER0		= 0x00; // 选择PXI总线上的TRIG0输入/输出时钟
const long PCH8522_OUTCLOCKSRC_TRIGGER1		= 0x01; // 选择PXI总线上的TRIG1输入/输出时钟
const long PCH8522_OUTCLOCKSRC_TRIGGER2		= 0x02; // 选择PXI总线上的TRIG2输入/输出时钟
const long PCH8522_OUTCLOCKSRC_TRIGGER3		= 0x03; // 选择PXI总线上的TRIG3输入/输出时钟
const long PCH8522_OUTCLOCKSRC_TRIGGER4		= 0x04; // 选择PXI总线上的TRIG4输入/输出时钟
const long PCH8522_OUTCLOCKSRC_TRIGGER5		= 0x05; // 选择PXI总线上的TRIG5输入/输出时钟
const long PCH8522_OUTCLOCKSRC_TRIGGER6		= 0x06; // 选择PXI总线上的TRIG6输入/输出时钟
const long PCH8522_OUTCLOCKSRC_TRIGGER7		= 0x07; // 选择PXI总线上的TRIG7输入/输出时钟
const long PCH8522_OUTCLOCKSRC_PXISTAR		= 0x08; // PXI_STAR信号触发

//*************************************************************************************
// 用于AD采样的实际硬件参数
typedef struct _PCH8522_STATUS_AD     
{
	LONG bADEanble;	// AD是否已经使能，=TRUE:表示已使能，=FALSE:表示未使能
	LONG bTrigger;  // AD是否被触发，=TRUE:表示已被触发，=FALSE:表示未被触发
	LONG bComplete; // AD是否整个转换过程是否结束，=TRUE:表示已结束，=FALSE:表示未结束
	LONG bAheadTrig;// AD触发点是否提前，=TRUE:表示触发点提前，=FALSE:表示触发点未提前
	LONG lEndAddr;	// 数据完成的结束地址
} PCH8522_STATUS_AD, *PPCH8522_STATUS_AD;

//######################## 常量定义 #################################
// CreateFileObject所用的文件操作方式控制字(可通过或指令实现多种方式并操作)
#define PCH8522_modeRead          0x0000			// 只读文件方式
#define PCH8522_modeWrite         0x0001			// 只写文件方式
#define	PCH8522_modeReadWrite     0x0002			// 既读又写文件方式
#define PCH8522_modeCreate        0x1000			// 如果文件不存可以创建该文件，如果存在，则重建此文件，并清0
#define PCH8522_typeText          0x4000			// 以文本方式操作文件

//***********************************************************
// 用户函数接口
#ifndef _PCH8522_DRIVER_
#define DEVAPI __declspec(dllimport)
#else
#define DEVAPI __declspec(dllexport)
#endif

#ifdef __cplusplus
extern "C" {
#endif
	//######################## 常规通用函数 #################################
	// 适用于本设备的最基本操作
	HANDLE DEVAPI FAR PASCAL PCH8522_CreateDevice(int DeviceID = 0);		// 创建设备对象
	int DEVAPI FAR PASCAL PCH8522_GetDeviceCount(HANDLE hDevice);			// 取得设备总台数
	BOOL DEVAPI FAR PASCAL PCH8522_GetDeviceCurrentID(HANDLE hDevice,		// 取得当前设备相应的ID号
													PLONG DeviceLgcID);
	BOOL DEVAPI FAR PASCAL PCH8522_ListDeviceDlg(HANDLE hDevice);			// 列表系统当中的所有的该PCI设备
    BOOL DEVAPI FAR PASCAL PCH8522_ReleaseDevice(HANDLE hDevice);			// 关闭设备,禁止传输,且释放资源

	//####################### AD数据读取函数 #################################
	// 适于大多数普通用户，这些接口最简单、最快捷、最可靠，让用户不必知道设备
	// 低层复杂的硬件控制协议和繁多的软件控制编程，仅用下面的初始化设备和读取
	// AD数据两个函数便能轻松高效地实现高速、连续的数据采集
	BOOL DEVAPI FAR PASCAL PCH8522_GetDDR2Length(								// 获取DDR2的存储长度
												HANDLE hDevice,					// 设备对象
												PULONG pulDDR2Length);			// DDR2的长度(单位：MB)
	BOOL DEVAPI FAR PASCAL PCH8522_ADCalibration(								// AD校准
												HANDLE hDevice);				// 设备对象				
    BOOL DEVAPI FAR PASCAL PCH8522_InitDeviceAD(								// 初始化设备，当返回TRUE后,设备即刻开始传输.
												HANDLE hDevice,					// 设备对象
												PPCH8522_PARA_AD pADPara);		// 硬件参数, 它仅在此函数中决定硬件状态

    BOOL DEVAPI FAR PASCAL PCH8522_StartDeviceAD(								// 在初始化之后，启动设备
												HANDLE hDevice);				// 设备对象句柄

	BOOL DEVAPI FAR PASCAL PCH8522_SetDeviceTrigAD(								// 当设备使能允许后，产生软件触发事件（只有触发源为软件触发时有效）
												HANDLE hDevice);				// 设备对象句柄
	
	BOOL DEVAPI FAR PASCAL PCH8522_GetDevStatusAD(
												HANDLE hDevice,					// 设备句柄
												PPCH8522_STATUS_AD pADStatus);	// AD状态

	BOOL DEVAPI FAR PASCAL PCH8522_ReadDeviceAD(								// DMA方式读AD数据
												HANDLE hDevice,					// 设备句柄
												PUSHORT pADBuffer,				// 将用于接受数据的用户缓冲区(该区必须开辟大于M加N个字的空间)
												ULONG nReadSizeWords,			// 必须等于M加N的长度
												ULONG ulStartAddr,				// 数据在RAM中的起始地址
												PLONG nRetSizeWords);			// 返回实际读取的数据长度
								
    BOOL DEVAPI FAR PASCAL PCH8522_StopDeviceAD(								// 在启动设备之后，暂停设备
												HANDLE hDevice);				// 设备对象句柄

	BOOL DEVAPI FAR PASCAL PCH8522_ReleaseDeviceAD(								// 关闭AD设备,禁止传输,且释放资源
												HANDLE hDevice);				// 设备句柄

	//################# AD的硬件参数操作函数 ########################
	BOOL DEVAPI FAR PASCAL PCH8522_SaveParaAD(HANDLE hDevice, PPCH8522_PARA_AD pADPara);  
    BOOL DEVAPI FAR PASCAL PCH8522_LoadParaAD(HANDLE hDevice, PPCH8522_PARA_AD pADPara);
	BOOL DEVAPI FAR PASCAL PCH8522_ResetParaAD(HANDLE hDevice, PPCH8522_PARA_AD pADPara);

	//################# 内存映射寄存器直接操作及读写函数 ########################
	// 适用于用户对本设备更直接、更特殊、更低层、更复杂的控制。比如根据特殊的
	// 控制对象需要特殊的控制流程和控制效率时，则用户可以使用这些接口予以实现。
	BOOL DEVAPI FAR PASCAL PCH8522_GetDeviceBar(					// 取得指定的指定设备寄存器组BAR地址
											HANDLE hDevice,			// 设备对象句柄,它由CreateDevice函数创建
											PUCHAR pbPCIBar[6]);	// 返回PCI BAR所有地址,具体PCI BAR中有多少可用地址请看硬件说明书
	BOOL DEVAPI FAR PASCAL PCH8522_GetDevVersion(					// 获取设备固件及程序版本
											HANDLE hDevice,			// 设备对象句柄,它由CreateDevice函数创建
											PULONG pulFmwVersion,	// 固件版本
											PULONG pulDriverVersion);// 驱动版本
	BOOL DEVAPI FAR PASCAL PCH8522_WriteRegisterByte(				// 往设备的映射寄存器空间指定端口写入单节字数据
											HANDLE hDevice,			// 设备对象
											PUCHAR pbLinearAddr,		// 指定映射寄存器的线性基地址
											ULONG OffsetBytes,		// 相对于基地址的偏移位置
											BYTE Value);			// 往指定地址写入单字节数据（其地址由线性基地址和偏移位置决定）
	BOOL DEVAPI FAR PASCAL PCH8522_WriteRegisterWord(				// 写双字节数据（其余同上）
											HANDLE hDevice, 
											PUCHAR pbLinearAddr, 
											ULONG OffsetBytes,  
											WORD Value);
	BOOL DEVAPI FAR PASCAL PCH8522_WriteRegisterULong(				// 写四节字数据（其余同上）
											HANDLE hDevice, 
											PUCHAR pbLinearAddr, 
											ULONG OffsetBytes,  
											ULONG Value);
	BYTE DEVAPI FAR PASCAL PCH8522_ReadRegisterByte(				// 读入单字节数据（其余同上）
											HANDLE hDevice, 
											PUCHAR pbLinearAddr, 
											ULONG OffsetBytes);
	WORD DEVAPI FAR PASCAL PCH8522_ReadRegisterWord(				// 读入双字节数据（其余同上）
											HANDLE hDevice, 
											PUCHAR pbLinearAddr, 
											ULONG OffsetBytes);
	ULONG DEVAPI FAR PASCAL PCH8522_ReadRegisterULong(				// 读入四字节数据（其余同上）
											HANDLE hDevice, 
											PUCHAR pbLinearAddr, 
											ULONG OffsetBytes);

	//################# I/O端口直接操作及读写函数 ########################
	// 适用于用户对本设备更直接、更特殊、更低层、更复杂的控制。比如根据特殊的
	// 控制对象需要特殊的控制流程和控制效率时，则用户可以使用这些接口予以实现。
	// 但这些函数主要适用于传统设备，如ISA总线、并口、串口等设备，不能用于本PCI设备
    BOOL DEVAPI FAR PASCAL PCH8522_WritePortByte(HANDLE hDevice, PUCHAR pbPort, ULONG offserBytes, BYTE Value);
    BOOL DEVAPI FAR PASCAL PCH8522_WritePortWord(HANDLE hDevice, PUCHAR pbPort, ULONG offserBytes, WORD Value);
    BOOL DEVAPI FAR PASCAL PCH8522_WritePortULong(HANDLE hDevice, PUCHAR pbPort, ULONG offserBytes, ULONG Value);

    BYTE DEVAPI FAR PASCAL PCH8522_ReadPortByte(HANDLE hDevice, PUCHAR pbPort, ULONG offserBytes);
    WORD DEVAPI FAR PASCAL PCH8522_ReadPortWord(HANDLE hDevice, PUCHAR pbPort, ULONG offserBytes);
    ULONG DEVAPI FAR PASCAL PCH8522_ReadPortULong(HANDLE hDevice, PUCHAR pbPort, ULONG offserBytes);

	//########################### 线程操作函数 ######################################
	HANDLE DEVAPI FAR PASCAL PCH8522_CreateSystemEvent(void);			// 创建内核事件对象，供InitDeviceInt和VB子线程等函数使用
	BOOL DEVAPI FAR PASCAL PCH8522_ReleaseSystemEvent(HANDLE hEvent);	// 释放内核事件对象

#ifdef __cplusplus
}
#endif

// 自动包含驱动函数导入库
#ifndef _PCH8522_DRIVER_
	#ifndef _WIN64
		#pragma comment(lib, "PCH8522_32.lib")
		#pragma message("======== Welcome to use our art company's products!")
		#pragma message("======== Automatically linking with PCH8522_32.dll...")
		#pragma message("======== Successfully linked with PCH8522_32.dll")
	#else
		#pragma comment(lib, "PCH8522_64.lib")
		#pragma message("======== Welcome to use our art company's products!")
		#pragma message("======== Automatically linking with PCH8522_64.dll...")
		#pragma message("======== Successfully linked with PCH8522_64.dll")
	#endif	
#endif

#endif // _PCH8522_DEVICE_
