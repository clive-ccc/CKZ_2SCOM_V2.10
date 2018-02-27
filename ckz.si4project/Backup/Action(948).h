#ifndef ACTION_H
#define ACTION_H

// 其中100 - 180 定位铣槽 180 - 237 为定位打孔
#define CurAxisDistStr_Offset 16
#define SpeedParamStr_Offset 30
#define DrillOffset2str_offset 55
#define TimeParamStr_Offset 60
#define DrillOffsetstr_offset 240

#define  AlarmNum		cDataArea[1]
//#define CurBoardPos                 cDataArea[8]
#define HSevoAlarmState             cDataArea[10]
#define VSevoAlarmState             cDataArea[11]
#define InverterAlarmState          cDataArea[12]
#define SystemMode                  cDataArea[13]
#define ScanMode                    cDataArea[14]
#define ChaXiaoDist                 cDataArea[15]

#pragma pack
typedef struct 
{
    long	YDistance; 		//X轴距离	16
    long	XDistance;   		//圆弧速度
    unsigned short  XCurSpeed;
    unsigned short  YCurSpeed;
	long WoodLength;
	long DrillNum;
	long EndLength;
	long DrillRadius;
}CurAxisDistStr;

typedef struct 
{
    unsigned short TongCaoDepth; // 任意插销 一个位代表一个插销的位置，总共16个 6
    unsigned short DrillStsrtDelay;
    unsigned long	CurBoardPos; 		// 8
    unsigned short HSevoAlarmState1;   		//圆弧速度
    unsigned short  VSevoAlarmState1;
    unsigned short  InverterAlarmState1;
    unsigned short  SystemMode1; // 13
    unsigned short  ScanMode1;
    unsigned short  ChaXiaoDist1;

}CurSystemData;

typedef struct 
{
    unsigned short ManualSpeed; // 30
    unsigned short WorkSpeed;
    unsigned short IdleSpeed;
    unsigned short XIdleSpeed;
    unsigned short DrillSpeed;
    unsigned short SlotSpeed;
    unsigned short MoveSpeed;
    unsigned short DrillRaidus;
    unsigned short res1;
    unsigned short res2;
    unsigned short ChaXiaoMode;// 40
    unsigned short ChaXiaoIndex; // 第几个孔打插销
    unsigned short LocationAddrArray; // 定位打孔第几组位置参数
    unsigned short Scan_DrillOffsetMode; //扫描打孔
    unsigned short Slot_DrillOffsetMode; // 槽
    unsigned short Hole_DrillOffsetMode; // 定位孔  45
    unsigned short ChaXiaoDepth; // 定位孔
    unsigned short CheckHoleSpeed2; // 先扫后打检孔速度  47
    unsigned short YManualSpeed; // 先扫后打检孔速度  48
    unsigned short XResetSpeed; // X轴复位速度  49
    unsigned short XCalculate; // 扫描打孔位置计算方式  50
}SpeedParam_Str;
#define CHAXIAO_MODE                cDataArea[40] 
#define CHAXIAO_Index               cDataArea[41]// 第几个孔打插销
#define LocationAddrArray		cDataArea[42]// 定位打孔第几组位置参数

typedef struct
{ // 先扫描后打孔
    unsigned short Scan_DrillOffset1;// 55
    unsigned short Scan_DrillOffset2;
    unsigned short Scan_DrillOffset3;
    unsigned short Scan_DrillOffset4;
    unsigned short Scan_DrillOffset5;

}DrillOffset2_str;

typedef struct
{   
    unsigned short  DrillCycleTime;         //钻孔圆整时间  60
    unsigned short StartDelay;
    unsigned short DotDis;
    unsigned short DrillDist;
    unsigned short LastDrillLength;
    unsigned short cyclelength;               //指定脉冲距离
    unsigned short  NoWorktime;         //钻孔圆整时间
    unsigned short  DrillCenCmp;         //钻孔圆整时间
    unsigned short MinHoleRaidus;
    unsigned short MaxHoleRaidus;
    unsigned long CheckWoodLenght;
    unsigned long Work1Dist;
    unsigned long Work2Dist;
    unsigned long Work3Dist;
    unsigned long  MaxLength;
    unsigned short DWDrillDist;//80
    unsigned short TMk_XOrigin_Dist;
    unsigned long  XMaxLength;    
    long Work4Dist;
    unsigned short Xcyclelength;               //指定脉冲距离
    unsigned short Depth;               //指定脉冲距离
    unsigned short XPulsTotal;               //指定脉冲距离
    unsigned short CaoPicDepth;//89
    unsigned short DrillOffset_Cao;// 已用 槽钻头偏移 90
    unsigned short res2;// 已用 32间距补偿
    unsigned short res3;// 已用 64间距补偿
    unsigned short SlowMotorBuchang;// 减速机反向间隙补偿 
    unsigned long CaoOrign; // 槽起点
    unsigned long CaoLength; //　槽长度  
    unsigned short CaoMode; // 铣槽模式
    unsigned short CaoDepth; // 99 已满不能在加
}TimeParam_Str;
/// 最多30个short
typedef struct
{   
    unsigned short CustomPlug1;         //任意插销  150 -- 179
    unsigned short CustomPlug2;
    unsigned short OneHolePlug; // 152
    unsigned short QiGangReBackDelay; // 153
    /*unsigned short CustomPlug4;
    unsigned short CustomPlug5;
    unsigned short CustomPlug6;               
    unsigned short CustomPlug7;         
    unsigned short CustomPlug8;        
    unsigned short CustomPlug9;
    unsigned short CustomPlug10;
    unsigned short CustomPlug11;
    unsigned short CustomPlug12;             
    unsigned short CustomPlug13;              
    unsigned short CustomPlug14;//
    unsigned short CustomPlug15;// 
    unsigned short CustomPlug16;//*/
}CustomPlug_Str;

typedef struct
{
    unsigned long PosF[12];// 240
    unsigned long PosB[12];// 240

}HolePos_str;

typedef struct
{
    unsigned short Scan_DrillOffset1;// 240
    unsigned short Scan_DrillOffset2;
    unsigned short Scan_DrillOffset3;
    unsigned short Scan_DrillOffset4;
    unsigned short Scan_DrillOffset5;
    unsigned short Slot_DrillOffset1;
    unsigned short Slot_DrillOffset2;
    unsigned short Slot_DrillOffset3;
    unsigned short Slot_DrillOffset4;
    unsigned short Slot_DrillOffset5;
    unsigned short Hole_DrillOffset1;
    unsigned short Hole_DrillOffset2;
    unsigned short Hole_DrillOffset3;
    unsigned short Hole_DrillOffset4;
    unsigned short Hole_DrillOffset5;
}DrillOffset_str;
/*
#pragma packoption
typedef enum _EParaAddr_
{
    ECOUTPROC    = 0,    //生产计数(2个字节)
    ECOUTDRIL	= 1,
    EXAXISLCT    	= 16,    //X轴当前位置
    EYAXISLCT     	= 18,    //Y轴当前位置
    EXCURSPD	= 6,	   //X轴当前速度
    EYCURSPD	= 7,	   //Y轴当前速度
    ERARMNUM	= 150,  //故障代码
    EINPSTATE	= 9,  //当前输入状态   
    EOUTSTATE	=10,
    EMSPEEDST	= 11,  //手动速度设置值
    EDOTSPEED	= 12,  //寸动距离 
    EPARAMNUM	= 13,//参数号
    EMIDPOINT   = 36, //中点位置参数
    EDRILMODE	=20,//钻孔模式
    EDRILMODEST  =21,//钻孔模式状态
    EPOSIPARAM  = 84, //位置参数设置
    ETIMEPARAM	= 60,  //时间参数设置
    ESYSPARAM    = 40, //速度参数设置
    ESOFTLIMIT    = 44,  //软件限位设置

    ESTEPPARAM  = 120,  //单步设置    
    ECYCLEPARAM	= 101,  //钻孔参数设置
    MORTORADD     	= 240,    //Y轴当前位置
    EPABUTT
}EParaAddr;
*/
extern WORD_BITS cAxisRunState;

#define  X_Dotting		cAxisRunState.bits.b0
#define  Y_Dotting		cAxisRunState.bits.b1
#define	 bRBOrignY		cAxisRunState.bits.b5
#define	 bRBOrignXY		cAxisRunState.bits.b6
#define  bXRst			cAxisRunState.bits.b8
#define  bYRst			cAxisRunState.bits.b9
#define bContinue		cAxisRunState.bits.b11	
#define  PrevbUpDrillLmt		cAxisRunState.bits.b12	
#define  PrevbDownDrillLmt	cAxisRunState.bits.b13	
#define  TONGYOU
#ifdef   TONGYOU
#define bPress1_Vavle	Y00
#define bPress2_Vavle	Y01
#define bPress3_Vavle	Y02
#define bDrillValve     Y03
#define bCuiQi        	Y04
#define bDrillMotor     Y05
#define bAlarmOut	    Y06

//#define	Y_SWITCH_ZERO	X01
#define	X_ZERO	        X01
#define	X_LIMIT_P		X02
//#define	X_SWITCH_ZERO	X03
#define	Y_ZERO	        X03
//#define  bDrillComeSgn   X11 //钻前进限位
#define  bDrillBackSgn   X03  //钻原点限位
#define CheckObjectSgn I004
#define  bDrillMotorAlarm  X05  //变频器报警
#define bServoAlarm		X06
#define bYServoAlarm	X07

#define bPress1_IN		X11
#define bPress2_IN		X12
#define bPress3_IN		X13
#define  bStop			(!X00)//(!X14)
#define  bOldStop       (!bOldX00)// (!bOldX14)

#define  bQiGangComeSgn   X10//X16 // 气缸前进到位
#define  bQiGangBackSgn   X07//X17  // 气缸后退到位

#define bPrePress1_IN		bOldX11
#define bPrePress2_IN		bOldX12
#define bPrePress3_IN		bOldX13
#else

#define bPress1_Vavle	Y00
#define bPress2_Vavle	Y01
#define bPress3_Vavle	Y02
#define bDrillValve        	Y03
#define bCuiQi        		Y04
#define bDrillMotor        	Y05
#define bAlarmOut	    Y06


#define  bStop			(!X00)
#define  bOldStop       		(!bOldX00)

#define bPress1_IN		X01
#define bPress2_IN		X02
#define bPress3_IN		X03

#define bPrePress1_IN		bOldX01
#define bPrePress2_IN		bOldX02
#define bPrePress3_IN		bOldX03

//#define	Y_SWITCH_ZERO	X04
//#define	Y_LIMIT_P		X05
#define	X_ZERO	        X04
#define	X_LIMIT_P		X05

#define CheckObjectSgn I010
#define  bQiGangComeSgn   X11 // 气缸前进到位
#define  bQiGangBackSgn   X12  // 气缸后退到位

#define  bDrillComeSgn   X11 //钻前进限位
#define  bDrillBackSgn   X12  //钻原点限位

#define bServoAlarm		X07
#define  bDrillMotorAlarm  X06  //变频器报警

//#define	X_SWITCH_ZERO	X13
//#define bYServoAlarm	X14
#define	Y_ZERO	X13
#define bYServoAlarm	X14
#endif

#define  bRunning		M00 //运行参数
#define  bRunning1		M01
#define  bRunning2		M02   //横向孔距状态
#define  bRunning3		M03  //产量清零
#define  bResume		M04	//复位中	
#define  bWriteFactory	M05  //横向孔距选择
#define  bReadFactory	M06  //横向孔距选择
#define  bReadParam     M07 //读取参数
#define  bWriteParam    M08 //

#define  bSelServoST		M09 //钻排选择
#define  bSelModeST		M10 //钻排选择
//#define  bTongCao	M11 //

#define  bAlarmFlag     M12 //钻排选择
#define  bKeyEnable 	M13 

#define  bClearH		M14 //
#define  bClearC	    M15 //

#define  bOneHolePlug_ST    M16 //
#define  bOneHolePlug	    M17 //
#define  bMVXpp		M18 //钻排选择
#define  bMVXnn	    M19 //

#define  bCuiQi_ST  M29
//#define  bSelMoto6		M20 //钻排选择
//#define  bSelMoto6ST	M21 //

#define  bMVXp			M32 //X-
#define  bMVXn			M33//X+
#define  bMVYp			M30 //X-
#define  bMVYn			M31//X+
#define  bMVZp			M35
#define  bMVZn			M34

//#define  Y_Alarm_In     X03

#define  bDotMVST		M36  //手动推料
#define  bRest	       	M37 //X，Y轴回零状态
//#define bDotMVST        M44

#define  bCustomPlug1_1		M48 //
#define  bCustomPlug2_1		M49 //
#define  bCustomPlug3_1		M50 //
#define  bCustomPlug4_1		M51 //
#define  bCustomPlug5_1		M52 //
#define  bCustomPlug6_1		M53 //
#define  bCustomPlug7_1		M54 //
#define  bCustomPlug8_1		M55 //
#define  bCustomPlug9_1		M56 //
#define  bCustomPlug10_1		M57 //
#define  bCustomPlug11_1		M58 //
#define  bCustomPlug12_1		M59 //
#define  bCustomPlug13_1		M60 //
#define  bCustomPlug14_1		M61 //
#define  bCustomPlug15_1		M62 //
#define  bCustomPlug16_1		M63 //

#define  bCustomPlug1_2		M64 //
#define  bCustomPlug2_2		M65 //
#define  bCustomPlug3_2		M66 //
#define  bCustomPlug4_2		M67 //
#define  bCustomPlug5_2		M68 //
#define  bCustomPlug6_2		M69 //
#define  bCustomPlug7_2		M70 //
#define  bCustomPlug8_2		M71 //
#define  bCustomPlug9_2		M72 //
#define  bCustomPlug10_2		M73 //
#define  bCustomPlug11_2		M74 //
#define  bCustomPlug12_2		M75 //
#define  bCustomPlug13_2		M76 //
#define  bCustomPlug14_2		M77 //
#define  bCustomPlug15_2		M78 //
#define  bCustomPlug16_2		M79 //

#define PlugforTwoHole   8
#define PlugforOneHole   7
#define PlugBeforHole 6
#define PlugafterHole 5
#define PlugAll 4
#define PlugCustom 3
#define PlugTWWai 2
#define PlugTWNei 1
#define NoPlug 0
/*
#define  bCustomPlug1_1		M40 //
#define  bCustomPlug1_2		M41 //
#define  bCustomPlug1_3		M42 //
#define  bCustomPlug2_1		M43 //
#define  bCustomPlug2_2		M44 //
#define  bCustomPlug2_3		M45 //
#define  bCustomPlug3_1		M46 //
#define  bCustomPlug3_2		M47 //
#define  bCustomPlug3_3		M48 //
#define  bCustomPlug4_1		M49 //
#define  bCustomPlug4_2		M50 //
#define  bCustomPlug4_3		M51 //
#define  bCustomPlug5_1		M52 //
#define  bCustomPlug5_2		M53 //
#define  bCustomPlug5_3		M54 //
#define  bCustomPlug6_1		M55 //
#define  bCustomPlug6_2		M56 //
#define  bCustomPlug6_3		M57 //
#define  bCustomPlug7_1		M58 //
#define  bCustomPlug7_2		M59 //
#define  bCustomPlug7_3		M60 //
#define  bCustomPlug8_1		M61 //
#define  bCustomPlug8_2		M62 //
#define  bCustomPlug8_3		M63 //
#define  bCustomPlug9_1		M64 //
#define  bCustomPlug9_2		M65 //
#define  bCustomPlug9_3		M66 //
#define  bCustomPlug10_1		M67 //
#define  bCustomPlug10_2		M68 //
#define  bCustomPlug10_3		M69 //
#define  bCustomPlug11_1		M70 //
#define  bCustomPlug11_2		M71 //
#define  bCustomPlug11_3		M72 //
#define  bCustomPlug12_1		M73 //
#define  bCustomPlug12_2		M74 //
#define  bCustomPlug12_3		M75 //
#define  bCustomPlug13_1		M76 //
#define  bCustomPlug13_2		M77 //
#define  bCustomPlug13_3		M78 //
#define  bCustomPlug14_1		M79 //
#define  bCustomPlug14_2		M80 //
#define  bCustomPlug14_3		M81 //
#define  bCustomPlug15_1		M82 //
#define  bCustomPlug15_2		M83 //
#define  bCustomPlug15_3		M84 //
#define  bCustomPlug16_1		M85 //
#define  bCustomPlug16_2		M86 //
#define  bCustomPlug16_3		M87 //
*/
#define bNOChaXiao     M93
#define bTWChaXiao     M95
#define bALLChaXiao    M97
#define bNeiWaiChaXiao          M99
#define bTongXiangNeiChaXiao    M100
#define bTongXiangWaiChaXiao    M102
#define bTWNei	       M100// M113
#define bTWWai	       M102// M115
//#define bZhongJianChaXiao       M103
#define bLocationHole   	    M106
#define CheckObjectSgn_old	    M107
#define bLocationCao	        M108
#define bMiddleChaXiao  M111

#define bSystemMode     M117 // 0单伺服 1双伺服 选择
#define bScanHole       M118
#define bCaoMode        M120
#define bDisplayChaXiaoDepth  M121

#define bCustomPlugBeforHole   M123
#define bCustomPlugafterHole   M125
#define bCustomPlugAll   M127
#define bCustomPlugState M129

#define bTongcaoST  M130
#define bTongcao    M131
#define bDanCaoST   M132
#define bDanCao     M133
#define bChaXiaoSave       M134
#define bPlugforOneHoleST  M135
#define bPlugforOneHole    M136
#define bPlugforTwoHoleST  M137
#define bPlugforTwoHole    M138


#define DuanCao 1
#define TongCao 0
#define DanCao  2
//定义齿轮倍数
//#define  XAxisConst		100//461
//#define  YAxisConst		100//521
//#define  UseAxisNum		1   //运用轴数量
//
#define  SERVORDLY	      2000;   

extern unsigned long    Secondtime;
extern unsigned long    Mintuetime;

extern unsigned long Press1time;
extern unsigned long Press2time;
extern unsigned long Press3time;
extern unsigned char cChaXiaoBackup;
extern unsigned char OnceOnpowerFLG,NOAirFLG;	//第一次上电标志
extern unsigned char RebackXYFLG;
extern unsigned short	ServorDlay;
extern unsigned short   RstPphlDlay; //复位外设延时
extern CurSystemData *CurBoardData;;
extern void Action(void);
extern void ResetPeripheral(void);
extern void ServoInit(void);
extern void CalGearRatio(void);
extern void StopAxis(void);
extern unsigned long CalPulseNumber(unsigned short cAxisNo,unsigned long dist);
extern long CalDistance(unsigned short cAxisNo,long pulse);

#endif 

