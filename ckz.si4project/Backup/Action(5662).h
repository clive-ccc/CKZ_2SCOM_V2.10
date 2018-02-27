#ifndef ACTION_H
#define ACTION_H

// ����100 - 180 ��λϳ�� 180 - 237 Ϊ��λ���
#define CurAxisDistStr_Offset 16
#define SpeedParamStr_Offset 30
#define DrillOffset2str_offset 55
#define TimeParamStr_Offset 60
#define DrillOffsetstr_offset 240
#define TestData_offset 300

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
    unsigned short TongCaoDepth; // ������� һ��λ����һ��������λ�ã��ܹ�16�� 6
    unsigned short DrillStsrtDelay;
    unsigned long	CurBoardPos; 		// 8
    unsigned short HSevoAlarmState1;  //10 		//Բ���ٶ�
    unsigned short  VSevoAlarmState1;
    unsigned short  InverterAlarmState1;
    unsigned short  SystemMode1; // 13
    unsigned short  ScanMode1;
    unsigned short  ChaXiaoDist1;

}CurSystemData_str;


typedef struct 
{
    long	YDistance; 		//X�����	16
    long	XDistance;   		//Բ���ٶ�
    unsigned short  XCurSpeed;
    unsigned short  YCurSpeed;
	long WoodLength;
	long DrillNum;
	long EndLength;
	long DrillRadius;
}CurAxisDistStr;

typedef struct 
{
    unsigned short ManualSpeed; // 30
    unsigned short ScanSpeed2;
    unsigned short IdleSpeed;
    unsigned short XIdleSpeed;
    unsigned short DrillSpeed;
    unsigned short SlotSpeed;
    unsigned short MoveSpeed;
    unsigned short DrillRaidus;
    unsigned short res1;
    unsigned short res2;
    unsigned short ChaXiaoMode;// 40
    unsigned short ChaXiaoIndex; // �ڼ����״����
    unsigned short LocationAddrArray; // ��λ��׵ڼ���λ�ò���
    unsigned short Scan_DrillOffsetMode; //ɨ����
    unsigned short Slot_DrillOffsetMode; // ��
    unsigned short Hole_DrillOffsetMode; // ��λ��  45
    unsigned short ChaXiaoDepth; // ��λ��
    unsigned short CheckHoleSpeed2; // ��ɨ������ٶ�  47
    unsigned short YManualSpeed; // ��ɨ������ٶ�  48
    unsigned short XResetSpeed; // X�Ḵλ�ٶ�  49
    unsigned short XCalculate; // ɨ����λ�ü��㷽ʽ  50
}SpeedParam_Str;
#define CHAXIAO_MODE                cDataArea[40] 
#define CHAXIAO_Index               cDataArea[41]// �ڼ����״����
#define LocationAddrArray		cDataArea[42]// ��λ��׵ڼ���λ�ò���

typedef struct
{ // ��ɨ�����
    unsigned short Scan_DrillOffset1;// 55
    unsigned short Scan_DrillOffset2;
    unsigned short Scan_DrillOffset3;
    unsigned short Scan_DrillOffset4;
    unsigned short Scan_DrillOffset5;

}DrillOffset2_str;

typedef struct
{   
    unsigned short  DrillCycleTime;         //���Բ��ʱ��  60
    unsigned short StartDelay;
    unsigned short DotDis;
    unsigned short DrillDist;
    unsigned short LastDrillLength;
    unsigned short cyclelength;               //ָ���������
    unsigned short  NoWorktime;         //���Բ��ʱ��
    unsigned short  WorkMode;         //����ģʽ
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
    unsigned short Xcyclelength;               //ָ���������
    unsigned short Depth;               //ָ���������
    unsigned short XPulsTotal;               //ָ���������
    unsigned short CaoPicDepth;//89
    unsigned short DrillOffset_Cao;// ���� ����ͷƫ�� 90
    unsigned short res2;// ���� 32��ಹ��
    unsigned short res3;// ���� 64��ಹ��
    unsigned short SlowMotorBuchang;// ���ٻ������϶���� 
    unsigned long CaoOrign; // �����
    unsigned long CaoLength; //���۳���  
    unsigned short CaoMode; // ϳ��ģʽ
    unsigned short CaoDepth; // 99 ���������ڼ�
}TimeParam_Str;
/// ���30��short
typedef struct
{   
    unsigned short CustomPlug1;         //�������  150 -- 179
    unsigned short CustomPlug2;
    unsigned short OneHolePlug; // 152
    unsigned short QiGangReBackDelay; // 153
    unsigned short XBackDelayTime;
    unsigned short CyclePulse;//  155  
    unsigned short WorkMode;   //156  ����ģʽ          
    unsigned short BuChangRange;//157         
    long BuChangBase;        
    unsigned long PreDecDistance;//160
    unsigned long DotDis;//162
    /*
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
    unsigned long PosF[20];// 300
    unsigned long res1[5];// 
    unsigned long PosB[20];// 350
    unsigned long res2[5];//
    unsigned long Pos_Up[20];// 400 �����ؼ��
    unsigned long res3[5];//
    unsigned long Pos_Hole[20];// 450 �׾�

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

extern WORD_BITS cAxisRunState;

#define  X_Dotting		cAxisRunState.bits.b0
#define  Y_Dotting		cAxisRunState.bits.b1
#define	 bRBOrignY		cAxisRunState.bits.b5
#define	 bRBOrignXY		cAxisRunState.bits.b6
#define  bXRst			cAxisRunState.bits.b8
#define  bYRst			cAxisRunState.bits.b9
#define  bContinue		cAxisRunState.bits.b11	
#define  PrevbUpDrillLmt	cAxisRunState.bits.b12	
#define  PrevbDownDrillLmt	cAxisRunState.bits.b13	

#define bPress1_Vavle	Y00
#define bPress2_Vavle	Y01
#define bPress3_Vavle	Y02
#define bDrillValve     Y03
#define bCuiQi        	Y04
#define bDrillMotor     Y05
#define bAlarmOut	    Y06

#define  bStop			(!X00)
#define  bOldStop       (!bOldX00)
#define	 X_ZERO	        X01
#define	 X_LIMIT_P		X02
#define	 Y_ZERO	        X03
#define  bDrillBackSgn  X03  //��ԭ����λ
#define  CheckObjectSgn I004
#define  bDrillMotorAlarm   X05  //��Ƶ������
#define  bServoAlarm		X06
#define  bYServoAlarm	X07

#define  bQiGangComeSgn   X10//X16 // ����ǰ����λ
#define  bQiGangBackSgn   X14//X17  // ���׺��˵�λ

#define  bPress1_IN		X11
#define  bPress2_IN		X12
#define  bPress3_IN		X13

#define  bPrePress1_IN		bOldX11
#define  bPrePress2_IN		bOldX12
#define  bPrePress3_IN		bOldX13

#define  bRunning		M00 //���в���
#define  bRunning1		M01
#define  bRunning2		M02   //����׾�״̬
#define  bRunning3		M03  //��������
#define  bResume		M04	//��λ��	
#define  bWriteFactory	M05  //����׾�ѡ��
#define  bReadFactory	M06  //����׾�ѡ��
#define  bReadParam     M07 //��ȡ����
#define  bWriteParam    M08 //

#define  bSelServoST	M09 //����ѡ��
#define  bSelModeST		M10 //����ѡ��

#define  bAlarmFlag     M12 //����ѡ��
#define  bKeyEnable 	M13 

#define  bClearH		M14 //
#define  bClearC	    M15 //

#define  bOneHolePlug_ST    M16 //
#define  bOneHolePlug	    M17 //
#define  bMVXpp		M18 //����ѡ��
#define  bMVXnn	    M19 //

#define  bLongBoard_ST  M26
#define  bLongBoard  M27

#define  bCuiQi_ST  M29
//#define  bSelMoto6		M20 //����ѡ��
//#define  bSelMoto6ST	M21 //

#define  bMVXp			M32 //X-
#define  bMVXn			M33//X+
#define  bMVYp			M30 //X-
#define  bMVYn			M31//X+
#define  bMVZp			M35
#define  bMVZn			M34

//#define  Y_Alarm_In     X03

#define  bDotMVST		M36  //�ֶ�����
#define  bRest	       	M37 //X��Y�����״̬
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

#define bNOChaXiao_ST  M92
#define bNOChaXiao     M93



//#define bTWChaXiao     M95
//#define bALLChaXiao    M97
//#define bNeiWaiChaXiao M99

//#define bZhongJianChaXiao       M103
#define bLocationHole_ST   	    M105
#define bLocationHole   	    M106
#define bLocationCao_ST	    	M107
#define bLocationCao	        M108

#define bMiddleChaXiao_ST   M109
#define bMiddleChaXiao  	M110

#define bTWNei_ST	   M111
#define bTWNei	       M112// M113

#define bTWWai_ST	   M113// M115
#define bTWWai	       M114// M115

#define bScanHole_ST    M115
#define bScanHole       M116

#define bSystemMode_ST  M117 // 0���ŷ� 1˫�ŷ� ѡ��
#define bSystemMode     M118 // 0���ŷ� 1˫�ŷ� ѡ��

#define bDuanCao_ST     M119
#define bDuanCao        M120

#define bDisplayChaXiaoDepth  M121

#define bCustomPlugBeforHole_ST   M122
#define bCustomPlugBeforHole   M123
#define bCustomPlugafterHole_ST M124
#define bCustomPlugafterHole   M125
#define bCustomPlugAll_ST   M126
#define bCustomPlugAll   M127
#define bCustomPlugState_ST M128
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

#define WorkMode_Scan    0  // ɨ����
#define WorkMode_Slot    1  // ��λϳ��
#define WorkMode_Hole    2  // ��λ���

#define DuanCao 1
#define TongCao 0
#define DanCao  2

#define PlugforTwoHole   8
#define PlugforOneHole   7
#define PlugBeforHole 6
#define PlugafterHole 5
#define PlugAll 4
#define PlugCustom 3
#define PlugTWWai 2
#define PlugTWNei 1
#define NoPlug 0

#define OneServer 0
#define TwoServer 1

//
#define  SERVORDLY	      2000;   

typedef struct
{
    unsigned long Alarm3;
    unsigned long Alarm4;
    unsigned long Alarm5;
    unsigned long Alarm6;
    unsigned long Alarm12;
}AlarmTime_str;

//extern unsigned long    Secondtime;
//extern unsigned long    Mintuetime;
extern unsigned char _gcXZero; //
extern unsigned char _gcYZero;  //
extern unsigned char _gcLocalHole;
extern unsigned char _gcLocalSlot;

extern unsigned long Press1time;
extern unsigned long Press2time;
extern unsigned long Press3time;
extern unsigned short XEncodeDivide;
extern unsigned char cChaXiaoBackup;	
extern unsigned char RebackXYFLG;//��һ���ϵ��־
extern unsigned short	ServorDlay;
extern unsigned short   RstPphlDlay; //��λ������ʱ
extern CurSystemData_str *CurBoardData;;

extern void Action(void);
extern void ResetPeripheral(void);
extern void ServoInit(void);
extern void CalGearRatio(void);
extern void StopAxis(void);
extern long PositionToPluse(unsigned short cAxisNo,long dist);
extern long PluseToPosition(unsigned short cAxisNo,long pulse);

#endif 

