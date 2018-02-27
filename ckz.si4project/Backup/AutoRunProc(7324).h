#ifndef AUTORUNPROC_H
#define AUTORUNPROC_H

#define STARTSPD	10
#define NSTEPDLAY  3
#define ACCTIME	10
#define XCONCMP	50

#define CCW 1
#define CW 0

#define ENCODE_USED 1

#define XIDEACC		80
#define XIDEACC2		120
#define XIDEACC3		150

typedef struct
{
    unsigned long R;
}CheckHoleData_str;

typedef struct
{
    unsigned long Dist5;
    unsigned long Dist10;
    unsigned long Dist15;
    unsigned long Dist20;
    unsigned long Dist32;
    unsigned long Dist50;
    unsigned long Dist65;
    unsigned long Dist70;
    unsigned long MinHoleR;
    unsigned long MaxHoleR;
    unsigned long DrillDist;
    unsigned long cDataArea91;
    unsigned long cDataArea92;
    unsigned long MaxLength;
    unsigned long Work1Dist;
    unsigned long Work2Dist;
    unsigned long Work3Dist;
    unsigned long ChaXiaoPos;
    unsigned long DrillRaidus;

    unsigned long SlowMotorBuchang;

	unsigned long XBuChangTotal;// 补偿脉冲总数
	unsigned long XBuChangRange;// 补偿范围，距工位原点距离
    long XBuChangBase;
    
    unsigned long XMaxLength;
    unsigned long HoleDepth;
    unsigned long ChaXiaoDepth;
    unsigned long YOrignDist;
    unsigned long CaoPicDepth;
}TempDist_str;

typedef struct
{
    char step;
    char Flag;
    char StartFlag;
    unsigned long Delay;
    unsigned long CurLength;
    unsigned long Length;
    unsigned long Depth;
    unsigned long Orign;
    unsigned long Offset;
    unsigned long AllLength1;// 正向走的长度
    unsigned long AllLength2;// 反向走的长度
    unsigned long R; // 半径
    unsigned long D; // 直径
    unsigned long StopPos; // 停止位置
    unsigned long StartPos; // 开始位置
    long Remainder;
}TongCao_str;

typedef struct
{
    long hole_point;
} HoleInfomation;

extern unsigned char cRunStep;
extern long WoodLength;
extern char currentHoleNum;

extern unsigned long NoworkStopdelay;
extern unsigned short WorkingFlag;

extern CheckHoleData_str CheckHoleData;
extern TempDist_str TempDist;

extern volatile char CheckHoleNum[2];
extern unsigned char StartFlag;

extern HoleInfomation HoleinfoF[100];
extern HoleInfomation HoleinfoB[100];

extern volatile char CheckSgnFilter;
extern volatile char CheckSgnFilterOff;

extern volatile unsigned char flgSgn;

extern void AutoRun(void);
extern void AutoCheckHole(void);

extern void MoveAction_Pulse(unsigned short cAxisNo,
                unsigned long dwPosi,unsigned short cSpeed);
extern void MoveAction_Pulse2(unsigned long dwPosi,unsigned short cSpeed);
extern void MoveAction_Dist(unsigned short cAxisNo,
                unsigned long dwPosi,unsigned short cSpeed);
extern void StopKeyPro(void);
extern void StopKeyPro2(void);
#endif

