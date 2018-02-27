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
#define DrillModel		2

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
    
    unsigned long XMaxLength;
    unsigned long HoleDepth;
    unsigned long ChaXiaoDepth;
    unsigned long XOrignDist;
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
    unsigned long AllLength;
    unsigned long R; // °ë¾¶
    unsigned long D; // °ë¾¶
    unsigned long StopPos; // Í£Ö¹Î»ÖÃ
    long Remainder;
}CaoMode0_str;

typedef struct
{
    //long hole_site1;
    //long hole_site2;
    long hole_point;
	//unsigned short LastFlag;
    //unsigned short Done;
} HoleInfomation;

extern CaoMode0_str CaoMode0Data;

extern unsigned char cRunStep;
extern unsigned char cDrillStep;
extern unsigned char cChaXiaoStep;

extern unsigned short NextDlay;
extern unsigned short NextStepDlay;
extern unsigned char  VerDrillNum,HorDrillNum;
extern long WoodHeadDist;
extern long WoodEndDist;
extern long WoodLength;
extern long LastDrillDist;
extern char currentHoleNum;
extern  long CurDrillRaidus;
extern long CurWoodLength;
extern unsigned long NoworkStopdelay;
extern unsigned char TouChaxiaoFlag;
extern unsigned short WorkingFlag;

extern CheckHoleData_str CheckHoleData;
extern TempDist_str TempDist;
extern unsigned char cScanDrillRunStep;
//extern long tempdata;
//extern  HoleInfomation infos[100];
//extern HoleInfomation Winfos[100];
//extern char currentWorkHoleNum;
extern volatile char flgSgn1;
extern volatile char flgSgnOld1;
extern volatile char CheckHoleNum[2];
extern unsigned char StartFlag;
extern HoleInfomation HoleinfoF[50];
extern HoleInfomation HoleinfoB[50];

extern unsigned char cDrillRunStep;
extern unsigned long DrillRunStepDelay;
extern void AutoRun(void);
extern unsigned char DeterWaitState(void);
extern long   ReadAxisPos(unsigned short axis);
extern unsigned long  AcrDecPos(unsigned short Radius,unsigned short Radians
,unsigned short VSpeed,unsigned short VStart);

extern void MoveAction_Pulse(unsigned short cAxisNo,
                unsigned long dwPosi,unsigned short cSpeed);
extern void MoveAction_Pulse2(unsigned long dwPosi,unsigned short cSpeed);
extern void MoveAction_Dist(unsigned short cAxisNo,
                unsigned long dwPosi,unsigned short cSpeed);
extern void StopKeyPro(void);
extern void StopKeyPro2(void);
#endif

