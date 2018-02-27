#include "Global.h"

unsigned char cRunStep ,TongCaoEndFlag = 0;


static uint8 LookForBoardHeadstep = 0;

unsigned char  HoleEndFlag = 0;// ��λ��׽�����ʾ��
unsigned char  OneHolePlugFlag = 0;// �����Զ��жϲ���λ�ñ�ʾ��
unsigned char  TwoHolePlugFlag = 0;// 
unsigned long  DelayTime;
//unsigned long  DelayTime1;
unsigned long  DelayTime2;
unsigned short WorkingFlag=0;
unsigned long NoworkStopdelay;
//unsigned long XBackDelay = 0;
long XWorkOrignPos = 0;

unsigned short DingDrillNum;
unsigned short DingCaoNum;
unsigned long CurDrillRaidus;


long DirllRaidusDist;
long CurDrillingDist;
long CurWoodLastDist;
//long CurDingDist;

unsigned long CurDrillRaidus;

long _gDingHoleDist = 0;
long _gDingSlotDist = 0;

//unsigned char TouChaxiaoFlag=0;
unsigned short CurDepth;
unsigned short CurDepthNum;
unsigned short DrillOnceFlag=0;

static unsigned long DingweiDist[30];
static unsigned long DingWeiCao[30];
static unsigned long DingCaoWitch[30];
static unsigned short DingCaoDepth[30];
static unsigned short ScanSGCmpFlag=0;

unsigned long DrillRunStepDelay;

static unsigned char cCaoMoveDir = 0;

char currentHoleNum = 0;
char currentWorkHoleNum = 0;
unsigned char cDrillStep = 0;


static unsigned long qigangbackdelay = 0;

static HoleInfomation infos[100];
static HoleInfomation Winfos[100];

HoleInfomation HoleinfoF[100];
HoleInfomation HoleinfoB[100];

unsigned char cScanDrillRunStep = 0;
static unsigned char cScanHoleStep = 0;

unsigned char cDrillRunStep = 0;

static unsigned char cDingHoleWorkStep = 0;
static unsigned char cDingHoleScanStep = 0;

static unsigned char cTongCaoScanStep = 0;
static unsigned char cTongCaoRunStep = 0;
static unsigned char cTongCaoStartFlag = 0;

static unsigned char cDuanCaoRunStep = 0;
static unsigned char cDuanCaoScanStep = 0;

static unsigned long BoardHead = 0;// ��ͷ
static unsigned long BoardEnd = 0;// ��β

static long CurWoodLength = 0;// ��ͷ
static long endposition = 0; //��β
static unsigned long HoleR = 0;// �׾�

 
volatile char CheckHoleNum[2] = {0};
unsigned char StartFlag = 0;


static volatile char CheckSgnFilter = 0;
static volatile char CheckSgnFilterOff = 0;
//volatile long nextPluse = 0;
volatile char flgSgn = 0;
volatile char flgSgnOld = 0;

long WoodLength;

//static unsigned char DrillFlag;
//static unsigned char PlugIndex = 0;

static TongCao_str TongCaoData;
//volatile char nextCheckPluse = 0;
static volatile char NoObjectFlag = 0;

TempDist_str TempDist;

CheckHoleData_str CheckHoleData;
static unsigned long TempData = 0;

static void PreDecforScan(void);
static void DataCompare(void);

void DrillingHoleProc(long depth);
void CheckHoleProc(void) ;
void DINGWEIHoleProc(void) ;
char LocationDrillAction(void);
void CaoOption(void);
void ScanCheckHoleProc(void) ;
void ScanDrillHoleRun(void) ;
void DrillingHoleProc(long depth);
long CalTmp1(void);
long CalTmp2(void);
void TongCaoProc(void);
void OutSideStop(void);

void Work1StopCheck(void)
{
    bRunning1=0;
    bPress1_Vavle=0;
    
    if(bRunning2)
    {
        WorkingFlag=1;
    }
    else if(bRunning3)
    {
        WorkingFlag=3;
    }
    else
    {
        bCuiQi=0;
        WorkingFlag=0;
        MoveAction_Pulse2(TempDist.Work1Dist,SpeedParam->IdleSpeed);
    }
}

void Work2StopCheck(void)
{
    bRunning2=0;
    bPress2_Vavle=0;
    if(bRunning3)
    {
        WorkingFlag=2;
    }
    else if(bRunning1)
    {
        WorkingFlag=0;
    }
    else
    {
        bCuiQi=0;
        WorkingFlag=0;
        MoveAction_Pulse2(TempDist.Work2Dist,SpeedParam->IdleSpeed);
    }
}

void Work3StopCheck(void)
{
    bRunning3=0;
    bPress3_Vavle=0;
    
    if(bRunning1)
    {
        WorkingFlag=0;						
    }
    else if(bRunning2)
    {
        WorkingFlag=1;  
    }
    else
    {
        bCuiQi=0;
        WorkingFlag=0;
        if((ScanMode == 1) || bLocationCao)
        {
         	MoveAction_Pulse2(TempDist.Work3Dist,SpeedParam->IdleSpeed);
        }
    }
}

void WorkOver(void)
{   
    bRunning=0;
    bRunning1=0;
    bRunning2=0;
    bRunning3=0;
    bPress1_Vavle=0;
    bPress2_Vavle=0;
    bPress3_Vavle=0;
    
    bCuiQi=0;
    bDrillMotor = 0;
}

// �߾���λ����������
void MoveAction_Pulse(unsigned short cAxisNo,unsigned long dwPosi,unsigned short cSpeed)
{			   
    if(cAxisNo == X_AXIS)
    {
        MV_AccDec_Set(X_AXIS, XIDEACC, XIDEACC);	            	
        LMV_Set_Speed(X_AXIS, cSpeed);
        LMV_Pmove(X_AXIS, dwPosi);
        if (tXAxisStepper.cPmovPulse)
        {
            tXAxisStepper.bAuto_Mov = 1;
            LStartPWM(X_AXIS);
        }
    }
    else if(cAxisNo == Y_AXIS)
    {
        MV_AccDec_Set(Y_AXIS, XIDEACC, XIDEACC);	            	
        LMV_Set_Speed(Y_AXIS, cSpeed);
        LMV_Pmove(Y_AXIS, dwPosi);

        if (tYAxisStepper.cPmovPulse)
        {
            tYAxisStepper.bAuto_Mov = 1;
            LStartPWM(Y_AXIS);
        }       
    }
}

void MoveAction_Pulse2(unsigned long dwPosi,unsigned short cSpeed)
{			   
    MV_AccDec_Set(X_AXIS, XIDEACC3, XIDEACC3);	            	
    LMV_Set_Speed(X_AXIS, cSpeed);
    LMV_Pmove(X_AXIS, dwPosi);

    if (tXAxisStepper.cPmovPulse)
    {
        tXAxisStepper.bAuto_Mov = 1;
        LStartPWM(X_AXIS);
    }           
}

void MoveAction_Pulse3(unsigned long dwPosi,unsigned short cSpeed)
{			   
    MV_AccDec_Set(X_AXIS, XIDEACC, XIDEACC);	            	
    LMV_Set_Speed(X_AXIS, cSpeed);
    LMV_Pmove2(X_AXIS, dwPosi);

    if (tXAxisStepper.cPmovPulse)
    {
        tXAxisStepper.bAuto_Mov = 1;
        LStartPWM(X_AXIS);
    }           
}

// �߾���λ�õ�
void MoveAction_Dist(unsigned short cAxisNo,unsigned long dwPosi,unsigned short cSpeed)
{			
    long OverLcttmp = 0;
    
    if(cAxisNo == X_AXIS)
    {
	 	OverLcttmp = PositionToPluse(X_AXIS, dwPosi);
        MV_AccDec_Set(X_AXIS, XIDEACC3, XIDEACC3);
        LMV_Set_Speed(X_AXIS, cSpeed);
        LMV_Pmove(X_AXIS, OverLcttmp);
        if (tXAxisStepper.cPmovPulse)
        {
            tXAxisStepper.bAuto_Mov = 1;
            LStartPWM(X_AXIS);
        }
    }
    else if(cAxisNo == Y_AXIS)
    {
		OverLcttmp = PositionToPluse(Y_AXIS, dwPosi);
        MV_AccDec_Set(Y_AXIS, XIDEACC2, XIDEACC2);	     	            	
        LMV_Set_Speed(Y_AXIS, cSpeed);
        LMV_Pmove(Y_AXIS, OverLcttmp);
        if (tYAxisStepper.cPmovPulse)
        {
            tYAxisStepper.bAuto_Mov = 1;
            LStartPWM(Y_AXIS);
        }
    }
}
//ɨ���жϳ�ʼ��
void VariableInit_Int(void)
{
	memset(HoleinfoF, 0, sizeof(HoleinfoF));
	memset(HoleinfoB, 0, sizeof(HoleinfoB));
	CheckHoleNum[0] = 0;
	CheckHoleNum[1] = 0;
	StartFlag = 0;
	flgSgn = 0;
	flgSgnOld = 0;
}

void VariableInit_Hole(void)
{
    uint8 i = 0;
    unsigned long temp = 0;

	memset(DingweiDist,0,sizeof(DingweiDist));
	memcpy(DingweiDist,&cDataArea[180],120);
	for(i = 0; i < 30;i++)
	{
		 DingDrillNum=i;
		 if(DingweiDist[i]==0)
		 	break;
	}
	for(i = 0; i < DingDrillNum;i++)
	{
		temp = DingweiDist[i] + cDataArea[80];
		DingweiDist[i] = PositionToPluse(X_AXIS, temp);
	}
}

void VariableInit_Slot(void)
{
    uint8 i = 0;
    unsigned long temp = 0;

	if(TimeParam->CaoMode == DuanCao)
	{
		memset(DingWeiCao,0,sizeof(DingWeiCao));
		memset(DingCaoWitch,0,sizeof(DingCaoWitch));
		memset(DingCaoDepth,0,sizeof(DingCaoDepth));
		memcpy(DingWeiCao,&cDataArea[100],40);
		memcpy(DingCaoWitch,&cDataArea[120],40);
		memcpy(DingCaoDepth,&cDataArea[140],20);
		
		for(i = 0; i <10;i++)
		{
			DingCaoNum=i;
			if(DingWeiCao[i]==0 && DingCaoWitch[i] == 0)
				break;												   
		}
		for(i = 0; i < DingCaoNum;i++)
		{
			temp = DingWeiCao[i] + cDataArea[90] + SpeedParam->DrillRaidus / 2;
			DingWeiCao[i] = PositionToPluse(X_AXIS, temp);
			DingCaoWitch[i] = PositionToPluse(X_AXIS, DingCaoWitch[i]);
			
			if(SystemMode == 0)
			{
				DingCaoDepth[i] = TimeParam->CaoPicDepth; 
			}
			else
			{
				DingCaoDepth[i] = PositionToPluse(X_AXIS, DingCaoDepth[i]);
			}
		}
	}
}

static void RunInit(void)
{
//    uint8 i = 0;
//    unsigned long temp = 0;
    
    memset(infos, 0, sizeof(infos));
	memset(Winfos, 0, sizeof(Winfos));
	VariableInit_Int();
	//��λ��
	if(TimeParam->WorkMode == WorkMode_Hole)
	{
		VariableInit_Hole();
	}
	//��λ��
	else if(TimeParam->WorkMode == WorkMode_Slot)
	{
		VariableInit_Slot();
	}

    HoleEndFlag = 0;
	TongCaoEndFlag = 0;
    currentHoleNum = 0;
    currentWorkHoleNum = 0;
    CurWoodLength = 0;
    //CaoMode0Data.StartFlag = 0;
    
    nextPluse = 0; 

	if(CurBoardData->SystemMode1 == TwoServer)
	{
		MoveAction_Pulse(Y_AXIS,0,SpeedParam->XIdleSpeed);
	}
	else
	{
		bDrillValve = 0;
	}

    if(!bDrillMotor)
    {
        bDrillMotor = 1;
        DelayTime2 = dwTickCount + 100UL * TimeParam->StartDelay;
    }
}

static uint8  RunCheck(void)
{
	uint8 flag = 0;
	static uint8 flagold = 0;
	static uint32 delay = 0;
	
    if(bRunning1)
	{
	    if(Press1time < dwTickCount)
	    {
    		bCuiQi = 1;
    		WorkingFlag = 0;
    		if(flagold == 0)
    		{
	    	    flagold = 1;
	    	    MoveAction_Pulse2(TempDist.Work1Dist,SpeedParam->IdleSpeed);
	    	    delay = dwTickCount + 10 * CustomPlug->XBackDelayTime;
	    	}
	    }
	}
    else if(bRunning2)
	{
	    if(Press2time < dwTickCount)
	    {
    	 	bCuiQi=1;
    	 	WorkingFlag=1;
    	 	if(flagold == 0)
    	 	{
	    		flagold = 1;
	    		MoveAction_Pulse2(TempDist.Work2Dist,SpeedParam->IdleSpeed);
	    		delay = dwTickCount + 10 * CustomPlug->XBackDelayTime;
	    }   }
	}
    else if(bRunning3)
    {
        if(Press3time < dwTickCount)
        {
        	bCuiQi=1;
        	WorkingFlag=2;
        	if(flagold == 0)
        	{
	        	flagold = 1;
	        	MoveAction_Pulse2(TempDist.Work3Dist,SpeedParam->IdleSpeed);
	        	delay = dwTickCount + 10 * CustomPlug->XBackDelayTime;
        	}
        }
    }
    else
    {
    	if(NoworkStopdelay==0)
    	    M43=1;
    }

	if(flagold > 0)
    {
		if(delay < dwTickCount)
		{
			flagold = 0;
			flag = 1;
		}
    }
    return flag;
}

uint8  RunFuncSelect(void)
{
	uint8 flag = 0;
	
    if(bLocationCao || bLocationHole)
    {
        flag = 1;
        cDrillStep = 100;
        DrillOnceFlag=1;
    }
    else
    {
        if(ScanMode > 0)
        {
    	    //cRunStep=2;
    	    flag = 1;
            cScanDrillRunStep = 1;
        }
    	else
    	{
            //cRunStep=2;
            flag = 1;
            cDrillStep = 100;
            DrillOnceFlag=1;
        }
    }
    return flag;
}

// ͨ��ɨ��ͷ
void TongCaoScanProc(void) 
{
    if(cTongCaoScanStep == 1) 
    {           
        if (flgSgn > 0)
        {
            cTongCaoScanStep = 2;  
            TempData = HoleinfoB[0].hole_point / XEncodeDivide;
            BoardHead = TempData;
            _gDingSlotDist = TempData; 
            cTongCaoStartFlag = 1;
        }
    }
    else if(cTongCaoScanStep == 2)
    {
        if (flgSgn == 0)
        {
            cTongCaoScanStep = 3;
            NoObjectFlag = 1;
        }
        WoodLength = ulEncoderCount / XEncodeDivide;
        WoodLength -= BoardHead;
    }
    else if(cTongCaoScanStep == 3)
    {
        if (flgSgn == 1)
        {
            cTongCaoScanStep = 2;           
        }
    }
}

// ͨ��ʱ�ƶ�
void TongCaoXMoveAction(void)
{
    MV_AccDec_Set(X_AXIS, XIDEACC3, XIDEACC3);
    LMV_Const_Move2(X_AXIS, SpeedParam->SlotSpeed, cCaoMoveDir);
    // cCaoMoveDir = 0Զ��ԭ��  cCaoMoveDir = 1 �ӽ�ԭ��
    if(cCaoMoveDir < 1)
    {// ��һ�θ÷����ƶ��������ƶ�Ҫ���ǹ���ȵ���ͷ�ȳ�ȥ��
    //Ŀǰ����δ���ⲿ�ֵĴ���
        cCaoMoveDir = 1;
    }
    else
    {
        cCaoMoveDir = 0;
    }
}
// ͨ��
void TongCaoWorkProc(void)
{
    long OverLcttmp = 0;
    static unsigned long _lqigangfdelay = 0;
    
    if(cTongCaoRunStep == 1)
    {
		if(DrillOnceFlag > 0)
		{
			DrillOnceFlag = 0;
            VariableInit_Int();
            StartFlag = 1;
            MV_AccDec_Set(X_AXIS,XIDEACC2, XIDEACC);
            LMV_Const_Move(X_AXIS, SpeedParam->ScanSpeed2, XCW);
            cTongCaoRunStep = 2;
            cTongCaoScanStep = 1;
		}
    }
    else if(cTongCaoRunStep == 2)
    {
		if(cTongCaoStartFlag > 0)
		{
			cTongCaoStartFlag = 0;
			LMV_Dec_Stop(X_AXIS);
			cTongCaoRunStep = 3;
			
			TongCaoData.Delay = 0;
	        cCaoMoveDir = 0;
	        TongCaoData.StopPos = 0;
	        TongCaoData.Remainder = 0;
	        _lqigangfdelay = 0;
	        TongCaoData.R = PositionToPluse(X_AXIS,(SpeedParam->DrillRaidus / 2));//
	        TongCaoData.D = PositionToPluse(X_AXIS,(SpeedParam->DrillRaidus));// 
	        TongCaoData.Offset = PositionToPluse(X_AXIS,TimeParam->DrillOffset_Cao);//
	        TongCaoData.Orign = PositionToPluse(X_AXIS,TimeParam->CaoOrign);//
	        TongCaoData.Depth = PositionToPluse(Y_AXIS,CurBoardData->TongCaoDepth);//
	        TongCaoData.Length = PositionToPluse(X_AXIS,TimeParam->CaoLength);//
	        TongCaoData.AllLength1 = _gDingSlotDist + TongCaoData.Offset + 
	                                TongCaoData.Orign + TongCaoData.Length
	                                - TongCaoData.R;
	        TongCaoData.StartPos = _gDingSlotDist + TongCaoData.Offset - TongCaoData.D;
		}
    }
    else if(cTongCaoRunStep == 3)
    {// �ȴ�X��ͣ��
		if(!X_DRV)
		{
			cTongCaoRunStep = 4;
    	}
    }
    else if(cTongCaoRunStep == 4)
    {// �ȴ�X��ͣ��
        if(SystemMode > 0)
        {
            if((!(Y_DRV)) && bDrillBackSgn)
                cTongCaoRunStep = 5;
        }
        else
        {
            if(bQiGangBackSgn)
                cTongCaoRunStep = 5;
        }
    }
    else if(cTongCaoRunStep == 5)
    {
        OverLcttmp = TongCaoData.StartPos;
        MoveAction_Pulse2(OverLcttmp, SpeedParam->MoveSpeed);
        cTongCaoRunStep = 6;
    }
    else if(cTongCaoRunStep == 6)
    {
        if(!X_DRV)
        {
        	if(_lqigangfdelay < dwTickCount)
        	{
	            if(SystemMode > 0)
	            {
	                CurDepth = 0;
	                cTongCaoRunStep = 7;
	            }
	            else
	            {
	                bDrillValve = 1;
	                cTongCaoRunStep = 8;
	            }
            }
        }
        else
        {
			_lqigangfdelay = dwTickCount + 10 * CustomPlug->XBackDelayTime;
        }
    }
    else if(cTongCaoRunStep == 7)
    {// �����
        if(CurDepth < TongCaoData.Depth)
        {// ��ǰ���С�ڲ۵������
            OverLcttmp = TongCaoData.Depth - CurDepth;
            if(OverLcttmp > TempDist.CaoPicDepth)
            {// ��ʣ�����ȴ���ÿ�ν������
                CurDepth = CurDepth + TempDist.CaoPicDepth;
            }
            else
            {// ��ʣ������С�ڵ���ÿ�ν������
                CurDepth = CurDepth + OverLcttmp;
            }
            OverLcttmp = TempDist.YOrignDist + CurDepth;
            cTongCaoRunStep = 8;
        }
        else
        {// ��ǰ��ȴ��ڵ��ڲ۵������
            OverLcttmp = 0;
            cTongCaoRunStep = 13;
        }
        
        MoveAction_Pulse(Y_AXIS, OverLcttmp, SpeedParam->DrillSpeed);
    }
    else if(cTongCaoRunStep == 8)
    {// �ȴ���ǰ����λ
    	uint8 _lfalg = 0;
        if(SystemMode > 0)
        {
            if(!Y_DRV)
            {
            	_lfalg = 1;
            }
        }
        else
        {
            if(bQiGangComeSgn)
            {
				_lfalg = 1;
            }
        }

        if(_lfalg > 0)
        {
			if(_lqigangfdelay < dwTickCount)
			{
				DelayTime = dwTickCount + 50;
				cTongCaoRunStep = 9;
			}
        }
        else
        {
			_lqigangfdelay = dwTickCount + 10 * TimeParam->DrillCycleTime;
        }
    }
    else if(cTongCaoRunStep == 9)
    {// ��ʼ�ƶ�
        if(DelayTime < dwTickCount)
        {
            //TongCaoXMoveAction();
            MV_AccDec_Set(X_AXIS, XIDEACC3, XIDEACC3);
    		LMV_Const_Move2(X_AXIS, SpeedParam->SlotSpeed, cCaoMoveDir);
            cTongCaoRunStep = 10;
        }
    }
    else if(cTongCaoRunStep == 10)
    {// ͨ�������ǵȴ��½����źţ�����β		
        if(NoObjectFlag == 1)
        {// ϳ�۹�����ɨ���½���
            NoObjectFlag = 0;
            cTongCaoRunStep = 11;
            LMV_Dec_Stop(X_AXIS);
        }
    }  
    else if(cTongCaoRunStep == 11)
    {// ����������β
	    if(!X_DRV)
	    {// ����֮ǰ���ٵľ���϶�������ȥ��
	    	if(_lqigangfdelay < dwTickCount)
	    	{
	            if(cCaoMoveDir < 1)
	                TongCaoData.Remainder = tXAxisStepper.cRealPosi + TongCaoData.Offset + TongCaoData.D;
	            else
	                TongCaoData.Remainder = tXAxisStepper.cRealPosi - TongCaoData.Offset - TongCaoData.D;
				MoveAction_Pulse3(TongCaoData.Remainder, 1);

				cCaoMoveDir = 1 - cCaoMoveDir;// ��һ���ƶ���Ҫ��������
	            cTongCaoRunStep = 12; 
	    	}
	    }
	    else
		{
			_lqigangfdelay = dwTickCount + 10 * CustomPlug->XBackDelayTime;
		}
    }
    else if(cTongCaoRunStep == 12)
    {
        if(!X_DRV)
        {
        	if(_lqigangfdelay < dwTickCount)
        	{
	            if(SystemMode > 0)
	            {// ˫˽������Ҫ���ص���ȴ��������ж��Ƿ�ﵽ�������
	                cTongCaoRunStep = 7;
	            }
	            else
	            {// ���۵�����ͽ���
	                bDrillValve = 0;
	                cTongCaoRunStep = 13;
	            }
            }
        }
		else
		{
			_lqigangfdelay = dwTickCount + 10 * CustomPlug->XBackDelayTime;
		}
    }
    else if(cTongCaoRunStep == 13)
    {
        if(SystemMode > 0)
        {
            if(!Y_DRV)
                cTongCaoRunStep = 0;
        }
        else
        {
            if(bQiGangBackSgn)
            {
                cTongCaoRunStep = 0;
            }
        }
    }
}

void DuanCaoScanProc(void)
{
    switch (cDuanCaoScanStep) 
    {           
    case 1:
        if (flgSgn > 0)
        {
            cDuanCaoScanStep = 2;  
            TempData = HoleinfoB[0].hole_point / XEncodeDivide;
            BoardEnd = TempData;
            _gDingSlotDist = TempData;  
            currentHoleNum = DingCaoNum;
        }
        break;
    case 2: 
        WoodLength = ulEncoderCount / XEncodeDivide;
        WoodLength -= BoardEnd;
        StartFlag = 0;
        break;
    default:
        break;
    }
}

// ����ʱ�ƶ�
void DuanCaoXMoveAction(void)
{
    long OverLcttmp = 0;

    if(DingCaoWitch[currentWorkHoleNum] > TempDist.DrillRaidus)
    {
    	OverLcttmp = DingCaoWitch[currentWorkHoleNum] - TempDist.DrillRaidus;
        if(cCaoMoveDir < 1)
        {
            cCaoMoveDir = 1;
			OverLcttmp += _gDingSlotDist;
			OverLcttmp +=  DingWeiCao[currentWorkHoleNum];

        }
        else
        {
            cCaoMoveDir = 0;
            OverLcttmp += _gDingSlotDist;
        }
        MoveAction_Pulse3(OverLcttmp, SpeedParam->SlotSpeed);
    }
}

// �ϵ�ϳ�ϲ�
void DuanCaoWorkPro(void)
{
    long OverLcttmp;
    static unsigned long _lxbackdelay = 0;
    static unsigned long _lqigangfdelay = 0;
    static unsigned long _lalldepth = 0;
    
    if(cDuanCaoRunStep == 1)
    {
        if(DelayTime2 < dwTickCount )
        {
            if(DrillOnceFlag > 0)
        	{
	            DrillOnceFlag = 0;
	            _lxbackdelay = 0;
	            _lalldepth = 0;
	            cCaoMoveDir = 0;
	            VariableInit_Int();
	            StartFlag = 1;
	            MV_AccDec_Set(X_AXIS,XIDEACC2, XIDEACC);
	            LMV_Const_Move(X_AXIS, SpeedParam->ScanSpeed2, XCW);
	            cDuanCaoRunStep = 2;
	            cDuanCaoScanStep = 1;
	        }
        }
    }
    else if(cDuanCaoRunStep == 2)
    {// �ȴ�ɨ����ͷ
       	if (currentWorkHoleNum < currentHoleNum) 
    	{  
        	cDuanCaoRunStep = 3;  
        	LMV_Dec_Stop(X_AXIS);
    	}
   	}
   	else if(cDuanCaoRunStep == 3)
    {// �ȴ�X��ֹͣ
    	uint8 flag = 0;

    	if(SystemMode > 0)
    	{
			if(bDrillBackSgn && !(X_DRV || Y_DRV))
				flag = 1;
    	}
    	else 
    	{
			if(bQiGangBackSgn && !X_DRV)
				flag = 1;
    	}
        if (flag > 0)
        {
        	cDuanCaoRunStep = 4; 
        }
    }
    else if(cDuanCaoRunStep == 4)
    {
        OverLcttmp = _gDingSlotDist + DingWeiCao[currentWorkHoleNum];

        MoveAction_Pulse2(OverLcttmp,SpeedParam->MoveSpeed);
        cDuanCaoRunStep = 5;   
    }
    else if(cDuanCaoRunStep == 5)
    {
        if (!X_DRV)
        {
        	if(_lxbackdelay < dwTickCount)
        	{
	            if(SystemMode > 0)
	            {// ���ߵ����
                    OverLcttmp = TempDist.YOrignDist;
			        MoveAction_Pulse(Y_AXIS, OverLcttmp, SpeedParam->XIdleSpeed);
			        CurDepth = 0;
			        _lalldepth = PositionToPluse(Y_AXIS, DingCaoDepth[currentWorkHoleNum]);
	                cDuanCaoRunStep = 6;  
	            }
	            else
	            {// ���ŷ���ֱ�ӽ���
	                bDrillValve = 1;
	                _lalldepth = PositionToPluse(Y_AXIS, DingCaoDepth[currentWorkHoleNum]);
	                CurDepth = _lalldepth;
	                cDuanCaoRunStep = 7; 

	            }
            	//TempData = ulEncoderCount / XEncodeDivide;
                //TempData = TempData - _gDingSlotDist;
                //cDataArea[404] = PluseToPosition(X_AXIS, TempData);
            }
        }
        else
        {
			_lxbackdelay = dwTickCount + 10 * CustomPlug->XBackDelayTime;
        }
    }
    else if(cDuanCaoRunStep == 6)
    {// ˫˽�������
    	if(!Y_DRV)
    	{
			if(CurDepth < _lalldepth)
			{// ��ǰ���С�ڲ۵������
				 OverLcttmp = _lalldepth - CurDepth;
				 if(OverLcttmp > TempDist.CaoPicDepth)
				 {// ��ʣ�����ȴ���ÿ�ν������
					 CurDepth = CurDepth + TempDist.CaoPicDepth;
				 }
				 else
				 {// ��ʣ������С�ڵ���ÿ�ν������
					 CurDepth = CurDepth + OverLcttmp;
				 }
				 OverLcttmp = TempDist.YOrignDist + CurDepth;
				 cDuanCaoRunStep = 7;
			}
			else
			{// ��ǰ��ȴ��ڵ��ڲ۵������
				 OverLcttmp = 0;
				 cDuanCaoRunStep = 13;
			}
			
			MoveAction_Pulse(Y_AXIS, OverLcttmp, SpeedParam->DrillSpeed);
		}
    }           
   	else if(cDuanCaoRunStep == 7)
	{
			uint8 flag = 0;
			
	    if(SystemMode > 0)
	    {
	        if (!Y_DRV)
	        {
	        	flag = 1;
	        }
	    }
	    else
	    {   
	        if(bQiGangComeSgn)
	        {
	        	flag = 1;
	    	}
	    }

	    if(flag > 0)
	    {
			if(_lqigangfdelay < dwTickCount)
			{
				cDuanCaoRunStep = 8;
			}
	    }
	    else
	    {
			_lqigangfdelay = dwTickCount + 10 * TimeParam->DrillCycleTime;
	    }
	}
    else if(cDuanCaoRunStep == 8)
    {
    	DuanCaoXMoveAction();
        cDuanCaoRunStep = 9;
    }
    else if(cDuanCaoRunStep == 9)
    {
        if (!X_DRV)
        {
        	if(_lxbackdelay < dwTickCount)
        	{
	            if(SystemMode > 0)
	            {
	                cDuanCaoRunStep = 6;
	            }
	            else
	            {
	                cDuanCaoRunStep = 10;
	            }
            }
        }
        else
        {
			_lxbackdelay = dwTickCount + 10 * CustomPlug->XBackDelayTime;
        }
    }	
    else if(cDuanCaoRunStep == 10)
    {
        if (!X_DRV)
        {
            cDuanCaoRunStep = 11;
            if(SystemMode > 0)
            {
                OverLcttmp =  0 ;	
                MoveAction_Pulse(Y_AXIS, OverLcttmp, SpeedParam->XIdleSpeed);
            }
            else
            {
                bDrillValve = 0;
            }
        }
    } 
    else if(cDuanCaoRunStep == 11)
    {
        if(SystemMode > 0)
        {
            if (bDrillBackSgn && !Y_DRV) 
            {
                cDuanCaoRunStep = 12;
            }
        }
        else
        {
            if(bQiGangBackSgn)
            {
            	if(_lxbackdelay < dwTickCount)
                	cDuanCaoRunStep = 12;
            }
            else
            {
				_lxbackdelay = dwTickCount + 10 * CustomPlug->QiGangReBackDelay;
            }
        }
    }
    else if(cDuanCaoRunStep == 12)
    {
    	currentWorkHoleNum++;
    	if(currentWorkHoleNum >= currentHoleNum)
        	cDuanCaoRunStep = 3;
		else
			cDuanCaoRunStep = 0;
    }
}

void DingHoleScanProc(void) 
{
	unsigned long temp = 0;
	
    if(cDingHoleScanStep == 1) 
    {           
        if (flgSgn > 0)
        {
            cDingHoleScanStep = 2;  
            temp = HoleinfoB[0].hole_point / XEncodeDivide;
            BoardHead = temp;
            _gDingHoleDist = temp;
            currentHoleNum=DingDrillNum;
        }
    }
    else if(cDingHoleScanStep == 2)
    {
        if (flgSgn == 0)
        {
            CurDrillRaidus = HoleinfoF[CheckHoleNum[1]].hole_point / XEncodeDivide;
            cDingHoleScanStep = 3;  
        }
        WoodLength = ulEncoderCount / XEncodeDivide;
        WoodLength -= BoardHead;
    }
    else if(cDingHoleScanStep == 3)
    {
    	HoleR = ulEncoderCount / XEncodeDivide;
    	WoodLength = HoleR - BoardHead;
        HoleR -= CurDrillRaidus;

        if (flgSgn > 0)
        {
            cDingHoleScanStep = 2;            
        }
        else if(HoleR > TempDist.MaxHoleR)
        {
        	LMV_Dec_Stop(X_AXIS);
            cDingHoleScanStep = 4; 
        }
    }
    else if(cDingHoleScanStep == 4)
    {
    	if(!X_DRV)
			cDingHoleScanStep = 0; 
    }
}

void DingHoleWorkPro(void)
{
//	static unsigned long _lxbackdelay = 0;
	
    long OverLcttmp = 0;
//    unsigned short speed = 0;

	if(cDingHoleScanStep == 4)
	{// ɨ����β�����½����Ѿ��������׾�����
		cDingHoleWorkStep = 0;
    }

    if(cDingHoleWorkStep == 1)
    {	
        if(DelayTime2 < dwTickCount )
        {
            if(DrillOnceFlag > 0)
        	{
	            DrillOnceFlag = 0;
	            VariableInit_Int();
	            StartFlag = 1;
	            MV_AccDec_Set(X_AXIS,XIDEACC2, XIDEACC);
	            LMV_Const_Move(X_AXIS, SpeedParam->ScanSpeed2, XCW);
	            cDingHoleWorkStep = 2;
	            cDingHoleScanStep = 1;
	        }
        }
    }
    else if(cDingHoleWorkStep == 2)
    {// ɨ����ͷ�󣬸�ֵҪ��Ŀ���
    	if (currentWorkHoleNum < currentHoleNum) 
    	{  
        	cDingHoleWorkStep = 3;  
        	LMV_Dec_Stop(X_AXIS);
    	}
   	}
    else if(cDingHoleWorkStep == 3)
    {
    	uint8 flag = 0;

    	if(SystemMode > 0)
    	{
			if(bDrillBackSgn && !(X_DRV || Y_DRV))
				flag = 1;
    	}
    	else 
    	{
			if(bQiGangBackSgn && !X_DRV)
				flag = 1;
    	}
        if (flag > 0)
        {
        	cDingHoleWorkStep = 4; 
        }
    }
    else if(cDingHoleWorkStep == 4)
    {
        OverLcttmp = _gDingHoleDist + DingweiDist[currentWorkHoleNum];

        MoveAction_Pulse2(OverLcttmp,SpeedParam->MoveSpeed);
        cDingHoleWorkStep = 5;   
        cDrillRunStep = 1;
    }
    else if(cDingHoleWorkStep == 5)
    {
    	DrillingHoleProc(TempDist.HoleDepth);
        if(cDrillRunStep == 0)
        {// �ȴ���׽���
			cDingHoleWorkStep = 6;
        }
    }
    else if(cDingHoleWorkStep == 6)
    {
		currentWorkHoleNum++;
		if (currentWorkHoleNum >= currentHoleNum)
		{
			HoleEndFlag = 1;
			cDingHoleWorkStep = 0;
		} 
		else
		{
			cDingHoleWorkStep = 4;
		}
    }
}
/*
void DrillAction(void)
{
    long OverLcttmp;//,LengthLcttmp,Datatmp;
    static unsigned long _lxbackdelay = 0;
    unsigned short speed = 0;

    switch (cDrillStep)
    {	
    case 100:
        if(DelayTime2 < dwTickCount )
        {
            DrillCheck();
        }
        break;
    case 102:
        if(_gcLocalSlot)//M108)
	    {
	        if(TimeParam->CaoMode != DuanCao && CaoMode0Data.StartFlag > 0)
	        {
	            cDrillStep=101;
	            LMV_Dec_Stop(X_AXIS);
	            CaoMode0Data.StartFlag = 0;
	        }
	    }
    	if (currentWorkHoleNum < currentHoleNum) 
    	{  

        	cDrillStep=101;  
        	LMV_Dec_Stop(X_AXIS);
    	}
    	break;
    case 101:
        if (!X_DRV && 
            ((bSystemMode && bDrillBackSgn) || (!bSystemMode && bQiGangBackSgn)))
        {
            if(_gcLocalHole > 0)
            {
                cDrillStep=60; 
                //CurDingDist = HoleinfoB[0].hole_point / XEncodeDivide;
                _gDingHoleDist = HoleinfoB[0].hole_point / XEncodeDivide;
            }
            else  if(_gcLocalSlot > 0)//bLocationCao)
            {
                if(TimeParam->CaoMode == DuanCao)
                {
                    _gDingSlotDist = HoleinfoB[0].hole_point/XEncodeDivide;
                    cDataArea[402] = PluseToPosition(X_AXIS, _gDingSlotDist);// _gDingSlotDist
					cDataArea[403] = CheckHoleNum[0];
					//cDataArea[404] = PluseToPosition(X_AXIS, HoleinfoB[0].hole_point/2);
                    cDrillStep=40;
                }
                else
                {
                	_gDingSlotDist = HoleinfoB[0].hole_point / XEncodeDivide;
                    cDrillStep = 55;
                }
            }
            else
            {
                OverLcttmp = CalTmp2();
                MoveAction_Pulse2(OverLcttmp,SpeedParam->MoveSpeed);
                cDrillStep = 2; 
            } 
        }
        break;
    case 1:
    //�����ŷ����ͣ��λ��	                
        if (!X_DRV && 
            ((bSystemMode && bDrillBackSgn) || (!bSystemMode && bQiGangBackSgn)))
        {
            cDrillStep = 2;
            //dwRealPosi = LMV_Get_Command_Pos(Y_AXIS);

            if(_gcLocalHole > 0)//M106)
            {
                speed = SpeedParam->MoveSpeed;//WorkSpeed;
                OverLcttmp = _gDingHoleDist + DingweiDist[currentWorkHoleNum];
            }
            else
            {
                OverLcttmp = CalTmp1();
                speed = SpeedParam->MoveSpeed;//MV_SPEED;
            }

            MoveAction_Pulse2(OverLcttmp, speed);
        } 
        break;
    case 2:
    	if(currentWorkHoleNum>0)
    	{
    		CurBoardData->CurBoardPos=Winfos[currentWorkHoleNum].hole_point-Winfos[currentWorkHoleNum-1].hole_point;
    		CurBoardData->CurBoardPos=CurBoardData->CurBoardPos* TimeParam->cyclelength/10000;
    	}
        if (!X_DRV)
        {// ���//��ʼ���
        	if(_lxbackdelay < dwTickCount)
        	{
            	cDrillStep=3;
            	DrillHoleData.step = 1;            
        	}
        }
        else
        {
			_lxbackdelay = dwTickCount + 10 * CustomPlug->XBackDelayTime;
        }
        break;
     case 3:
        DrillHoleProc(TempDist.HoleDepth);//TimeParam->Depth);
        if(DrillHoleData.step == 0)
        {
            cDrillStep = 4;
        }
        break;
     case 4:
        if(SpeedParam->ChaXiaoMode > 0)
        {
            cDrillStep = 6;
        }
        else
        {
            cDrillStep = 5;
        }
        break;
     case 5:
        currentWorkHoleNum++;
        if (currentWorkHoleNum == currentHoleNum)
        {
            HoleEndFlag = 1;
            cDrillStep = 100;
        } 
        else
        {
            cDrillStep = 1;
        }
        //DrillFlag=0;
        DelayTime = dwTickCount + 20;
        break;  
    case 6:
        cDrillStep = 7;
        PlugIndex = 0;
        DelayTime = dwTickCount + 20;
        break;
    case 7:
        if(DelayTime < dwTickCount)
        {
            OverLcttmp = DrillPageProc(); 
            if(OverLcttmp == 1)
            {
                cDrillStep=8;
            }
            else if(OverLcttmp == 2)
            {
                cDrillStep=5;
            }
        }
        break;
    case 8:
        if (!X_DRV) 
        { // �����
            cDrillStep = 9;
            DrillHoleData.step = 1;
        }
        break;          
    case 9:
        DrillHoleProc(TempDist.ChaXiaoDepth);//SpeedParam->ChaXiaoDepth);
        if(DrillHoleData.step == 0)
        {
            if(PlugIndex < 1)
                cDrillStep = 10;
            else
                cDrillStep = 7;
        }
        break;
    case 10:
        cDrillStep = 5;
        break;
    case 210:
        DrillWeiData.step = 1;
        cDrillStep = 211;
        break;
    case 211:
        DrillWeiProc();
        if(DrillWeiData.step == 0)
        {
            cDrillStep = 212;
        }
        break;
    case 212:
        cDrillStep=0;
        cRunStep=22;   
        currentHoleNum=0;
        break;
    case 40://ϳ��
        cDrillStep=41;
        DrillCao2Data.step = 1;
        break;   
    case 41:
        CaoOption2();
        if(DrillCao2Data.step == 0)
        {
            cDrillStep=42;
        }
        break;
    case 42:
        currentWorkHoleNum++;
        if (currentWorkHoleNum == currentHoleNum)
        {
            cDrillStep = 100;
            TongCaoEndFlag = 1;
            HoleEndFlag = 1;
        } 
        else
        {
            cDrillStep = 40;
        }
        DelayTime = dwTickCount + 20;
        break;
    case 55:
        CaoMode0Data.step = 1;
        cDrillStep = 56;
        break;
    case 56:
        CaoOption();
        if(CaoMode0Data.step == 0)
        {
            cRunStep = 20;
            cDrillStep = 0;
        }
        break;
    case 60:// ��λ���
        if(SystemMode == 1)
        {
            if (!Y_DRV && bDrillBackSgn)
            {
                cDrillStep=61;
            } 
        }
        else
        {
            if (!X_DRV && bQiGangBackSgn)
            {
                cDrillStep=61;
            } 
        }
        break; 
    case 61:
        OverLcttmp = _gDingHoleDist + DingweiDist[currentWorkHoleNum];

        MoveAction_Pulse2(OverLcttmp,SpeedParam->MoveSpeed);
        cDrillStep=2;   
        break;
    default:
        break;
    }
}
*/
void CalTempDist(void)
{
    TempDist.Dist5 = PositionToPluse(X_AXIS,500);//(10000UL*500/TimeParam->cyclelength);
    TempDist.Dist10 = PositionToPluse(X_AXIS,1000);//(10000UL*1000/TimeParam->cyclelength);
    TempDist.Dist15 = PositionToPluse(X_AXIS,1500);//(10000UL*1500/TimeParam->cyclelength);
    TempDist.Dist20 = PositionToPluse(X_AXIS,2000);//(10000UL*2000/TimeParam->cyclelength);
    TempDist.Dist50 = PositionToPluse(X_AXIS,3200);//(10000UL*3200/TimeParam->cyclelength);
    TempDist.Dist70 = PositionToPluse(X_AXIS,7000);//(10000UL*7000/TimeParam->cyclelength);
    
    TempDist.Dist32 = PositionToPluse(X_AXIS,ChaXiaoDist);//(10000UL*ChaXiaoDist/TimeParam->cyclelength );
    TempDist.MinHoleR = PositionToPluse(X_AXIS,TimeParam->MinHoleRaidus);//10000UL * TimeParam->MinHoleRaidus / TimeParam->cyclelength;
    TempDist.MaxHoleR = PositionToPluse(X_AXIS,TimeParam->MaxHoleRaidus);//10000UL * TimeParam->MaxHoleRaidus / TimeParam->cyclelength;
    TempDist.DrillDist = PositionToPluse(X_AXIS,TimeParam->DrillDist);//10000UL * (TimeParam->DrillDist) / TimeParam->cyclelength;
    TempDist.cDataArea91 = PositionToPluse(X_AXIS,cDataArea[91]);//(10000UL * cDataArea[91]/TimeParam->cyclelength );
    TempDist.cDataArea92 = PositionToPluse(X_AXIS,cDataArea[92]);//(10000UL * cDataArea[92]/TimeParam->cyclelength );
    TempDist.MaxLength = PositionToPluse(X_AXIS,TimeParam->MaxLength);//1000UL * (TimeParam->MaxLength) / TimeParam->cyclelength * 10;
    TempDist.Work1Dist = PositionToPluse(X_AXIS,TimeParam->Work1Dist);//10000UL * (TimeParam->Work1Dist) / TimeParam->cyclelength;
    TempDist.Work2Dist = PositionToPluse(X_AXIS,TimeParam->Work2Dist);//10000UL * (TimeParam->Work2Dist) / TimeParam->cyclelength;
    TempDist.Work3Dist = PositionToPluse(X_AXIS,TimeParam->Work3Dist);//10000UL * (TimeParam->Work3Dist) / TimeParam->cyclelength;
    TempDist.ChaXiaoPos =  PositionToPluse(X_AXIS,TimeParam->SlowMotorBuchang);//10000UL * (TimeParam->SlowMotorBuchang) / TimeParam->cyclelength;
    TempDist.DrillRaidus = PositionToPluse(X_AXIS,SpeedParam->DrillRaidus);//10000UL * SpeedParam->DrillRaidus / TimeParam->cyclelength;
    TempDist.SlowMotorBuchang = PositionToPluse(X_AXIS,TimeParam->SlowMotorBuchang);//10000UL * (TimeParam->SlowMotorBuchang) / TimeParam->cyclelength;
	TempDist.XBuChangRange = PositionToPluse(X_AXIS,CustomPlug->BuChangRange);
	//TempDist.XBuChangTotal = PositionToPluse(X_AXIS,CustomPlug->BuChangDistance);
	//TempDist.XBuChangBase = PositionToPluse(X_AXIS,CustomPlug->BuChangBase);
	TempDist.XBuChangBase = CustomPlug->BuChangBase;
	
    if(SystemMode == 1)
    {
        TempDist.CaoPicDepth = PositionToPluse(Y_AXIS,TimeParam->CaoPicDepth);//TimeParam->CaoPicDepth * TimeParam->XPulsTotal / TimeParam->Xcyclelength;
        TempDist.XMaxLength = PositionToPluse(Y_AXIS,TimeParam->XMaxLength);//TimeParam->XMaxLength * TimeParam->XPulsTotal / TimeParam->Xcyclelength;	
        TempDist.YOrignDist = PositionToPluse(Y_AXIS,TimeParam->TMk_XOrigin_Dist);//TimeParam->TMk_XOrigin_Dist * TimeParam->XPulsTotal / TimeParam->Xcyclelength;
        TempDist.HoleDepth = PositionToPluse(Y_AXIS,TimeParam->Depth);//TimeParam->Depth * TimeParam->XPulsTotal / TimeParam->Xcyclelength;
        TempDist.ChaXiaoDepth = PositionToPluse(Y_AXIS,SpeedParam->ChaXiaoDepth);//SpeedParam->ChaXiaoDepth * TimeParam->XPulsTotal / TimeParam->Xcyclelength;
    }
}

uint8 GotoWorkOrign(void)
{
	uint8 flag = 0;
	
	if(CurBoardData->SystemMode1 > 0)// 1 ˫˽��
	{
		if(bDrillBackSgn && !Y_DRV)
		{
			flag = RunCheck(); 
		}
	}
	else
	{
		if(bQiGangBackSgn)
		{
			 flag = RunCheck(); 
		}
	}

	return flag;
}

uint8 LookForBoardHeadCheck(void)
{
	uint8 flag = 0;
	
	if(WorkingFlag == 0)
	{
		if(CurAxisDistance->XDistance < 0)
		{
			flag = 1;
		}
	}
	else if(WorkingFlag==1)
	{
		if((CurAxisDistance->XDistance <TimeParam->Work2Dist-20000))
		{
			flag = 1;
		}
	}
	else if(WorkingFlag==2)
	{
		if((CurAxisDistance->XDistance <TimeParam->Work3Dist-20000))
		{
			flag = 1;
		}
	}

	return flag;
}

void LookForBoardHead(void)
{
	static uint32 delay = 0;
	uint8 flag = 0;
	
	if(LookForBoardHeadstep == 1)
	{
		if(!CheckObjectSgn)
		{
			LookForBoardHeadstep = 0;
		}
		else
		{// �а��ź�
			LookForBoardHeadstep = 2;
		}		
	}
	else if(LookForBoardHeadstep == 2)
	{
		MV_AccDec_Set(X_AXIS, XIDEACC3, XIDEACC3);
		LMV_Const_Move(X_AXIS, SpeedParam->IdleSpeed, XCCW);// 1);
		delay = dwTickCount + 50;
		LookForBoardHeadstep = 3;
	}
	else if(LookForBoardHeadstep == 3)
	{
		if(_gcXZero == 0)
		{
			if(!CheckObjectSgn)
			{// ��⵽���ź�				
				if(delay < dwTickCount )
				{
					LookForBoardHeadstep = 4; // �ҵ���ͷ
					LMV_Dec_Stop(X_AXIS);
				}
			}
			else
			{
				delay = dwTickCount + 50;
				flag = LookForBoardHeadCheck();
				if(flag > 0)
				{// �����߽�
					AlarmNum = 14;
					LookForBoardHeadstep = 6;
					LMV_Dec_Stop(X_AXIS);
				}
			}
		}
		else
		{
			LookForBoardHeadstep = 6;
			LMV_Dec_Stop(X_AXIS);
		}
	}
	else if(LookForBoardHeadstep == 4)
	{
		if(!X_DRV )
		{
			if(delay < dwTickCount)
			{
				LookForBoardHeadstep = 1;
			}
		}
		else
		{
			delay = dwTickCount + 10 * CustomPlug->XBackDelayTime;
		}
	}
	else if(LookForBoardHeadstep == 6)
	{
		if(!X_DRV)
		{
			LookForBoardHeadstep = 100;
			//WorkOver();
		}
	}
}

/************************************************/
/*
�ռ�����ֹͣ

*/
/************************************************/
void OutSideStop(void)
{
	if(bRunning)
	{
		if(!bLongBoard)
		{
			if(WorkingFlag==0)
			{
				if((CurAxisDistance->XDistance >TimeParam->Work2Dist+20000)&&(cRunStep==2))
				cRunStep=20;
			}
			else if(WorkingFlag==1)
			{
				if((CurAxisDistance->XDistance >TimeParam->Work3Dist+20000)&&(cRunStep==2))
				cRunStep=20;
			}
		} 
	}
}
/*
if(currentWorkHoleNum > 1)
{
	OverLcttmp = Winfos[currentWorkHoleNum].hole_point-Winfos[currentWorkHoleNum-1].hole_point;
	CurBoardData->CurBoardPos = PluseToPosition(X_AXIS, OverLcttmp);
}*/


void AutoRun(void)
{
	uint8 flag = 0;
	
	if(cRunStep == 1)
	{// ��ʼ��
		RunInit();
		cRunStep = 2;
	}
	else if(cRunStep == 2)
	{// ���������ߵ�����ԭ��
		flag = GotoWorkOrign();
		if(flag > 0)
		{
			cRunStep = 3;
			LookForBoardHeadstep = 1;
		}
	}
	else if(cRunStep == 3)
	{// �Ұ�ͷ
		LookForBoardHead();
		if(LookForBoardHeadstep == 0)
		{// �ҵ���ͷ
			cRunStep = 4;
		}
		else if(LookForBoardHeadstep == 100)
		{
			M43 = 1;
		}
	}
	else if(cRunStep == 4)
	{// �ֹ��ܿ�ʼ
		long temp = 0;
		// �������������� ����¼�������
		temp = LMV_Get_Command_Pos(X_AXIS);
		ulEncoderCount = temp * XEncodeDivide;
		XWorkOrignPos = temp;
		
		if(TimeParam->WorkMode == WorkMode_Scan)
		{
			cScanDrillRunStep = 1;
			cRunStep = 5;
		}
		else if(TimeParam->WorkMode == WorkMode_Hole)
		{
			cDingHoleWorkStep = 1;
			cRunStep = 10;
		}
		else if(TimeParam->WorkMode == WorkMode_Slot)
		{
			cRunStep = 15;
		}
	}
	else if(cRunStep == 5)
	{
		ScanDrillHoleRun();
		if(cScanDrillRunStep == 0)
			cRunStep = 22;
	}
	else if(cRunStep == 10)
	{// ��λ���Ҫһֱɨ�裬�жϰ�β
		DingHoleScanProc();
		DingHoleWorkPro();
		if(cDingHoleWorkStep == 0 || cDingHoleScanStep == 0)
		{
			if(!X_DRV)
			{
				cRunStep = 22;
			}
		}
	}
	else if(cRunStep == 15)
	{// ��λϳ��Ҫһֱɨ�裬�жϰ�β
		if(TimeParam->CaoMode == DuanCao)
		{
			cDuanCaoRunStep = 1;
			cRunStep = 16;
		}
		else if(TimeParam->CaoMode == TongCao)
		{
			cTongCaoRunStep = 1;
			cRunStep = 18;
		}
	}
	else if(cRunStep == 16)
	{
		DuanCaoScanProc();
		DuanCaoWorkPro();
		if(cDuanCaoRunStep == 0)
		{
			if(!X_DRV)
			{
				cRunStep = 22;
			}
		}
	}
	else if(cRunStep == 18)
	{// ͨ��
		TongCaoScanProc();
		TongCaoProc();
		if(cTongCaoRunStep == 0)
		{
			if(!X_DRV)
			{
				cRunStep = 22;
			}
		}
	}
    else if(cRunStep == 22)
	{
		flgSgn = 0;
		HoleEndFlag = 0;
		cRunStep = 20; 
		StartFlag = 0;
		cDrillStep=0;
		currentHoleNum = 0;
	
		if(!X_DRV)
		{
			NoworkStopdelay=100*TimeParam->NoWorktime+1000;
			if(!bLongBoard)
			{
				if(WorkingFlag==0)
				{
					Work1StopCheck();
					cRunStep=1; 
				}
				else if(WorkingFlag==1)
				{
					Work2StopCheck();
					cRunStep=1; 
				}
				 else if(WorkingFlag==2)
				{
					Work3StopCheck();
					cRunStep=1;  
				}
			} 
			else
			{
				bRunning1 = 0;
				bPress1_Vavle = 0;
				bPress2_Vavle = 0;
				bPress3_Vavle = 0;
				bCuiQi = 0;
				MoveAction_Pulse2(TempDist.Work1Dist,SpeedParam->IdleSpeed);
				cRunStep = 1; 
			}				
		}
	}

    if (bRunning && (!bResume) && (RstPphlDlay == 0) && !bStop) 

	if((!(X_DRV || Y_DRV)) && (!(bRunning1 || bRunning2 || bRunning3)))
	{
		CalGearRatio();
		CalTempDist();
	}

	OutSideStop();
    
     if (SystemMode > 0)
     {
     		bDrillValve=bCuiQi;
     }
}

/************************************************/
/*
��ɨ��ɨ��ģʽ
*/
/************************************************/
void AutoCheckHole(void) 
{// �ж�Ϊ10usһ�εĶ�ʱ���ն�
	if(StartFlag > 0)
	{
	    if (CheckObjectSgn) 
	    {
	        //NoObjectFlag = 0;
	        CheckSgnFilter++;
	        if (CheckSgnFilter > 2)
	        {
	            flgSgn = 1;
	            CheckSgnFilterOff = 0;
	        }
	    }
	    else
	    {
	        CheckSgnFilterOff++;
	        if (CheckSgnFilterOff > 2)
	        {
	            flgSgn = 0;
	            CheckSgnFilter = 0;
	        }
	    }

		if(flgSgn != flgSgnOld)
		{
			flgSgnOld = flgSgn;
		    if(flgSgn > 0)
			{// ������
				HoleinfoB[CheckHoleNum[0]].hole_point = ulEncoderCount;
				CheckHoleNum[0]++;
				flgSgn = 1;
			}
			else
			{// �½���
				HoleinfoF[CheckHoleNum[1]].hole_point = ulEncoderCount;
				CheckHoleNum[1]++;
				flgSgn = 0;
			}
		}
    }
}

// ���жϰ�ͷ�����жϰ�β
void ScanCheckHoleProc(void) 
{
	unsigned long temp = 0;
	static unsigned long _lxstopdelay = 0;
	
	if(cScanHoleStep == 1)
	{
		if(flgSgn > 0)
		{// ��⵽��ͷ
			BoardHead = HoleinfoB[0].hole_point;
			cScanHoleStep = 2;
		}
	}
	else if(cScanHoleStep == 2)
	{
		if(flgSgn < 1)
		{// ��⵽��β
			BoardEnd = HoleinfoF[CheckHoleNum[1]].hole_point;
			temp = ulEncoderCount / XEncodeDivide;
			temp = temp - BoardEnd;
			if(temp > TempDist.MaxHoleR)
			{
				LMV_Dec_Stop(X_AXIS);
				cScanHoleStep = 3;
			}
		}
		if(CurAxisDistance->XDistance >= CustomPlug->PreDecDistance)
		{
			LMV_Dec_Stop(X_AXIS);
		}		
		if(currentHoleNum > 0)
		{
			if(!X_DRV)
			{
				if(_lxstopdelay < dwTickCount)
				{
					AlarmNum = 17;
				}
			}
			else
			{
				_lxstopdelay = dwTickCount + 10 * CustomPlug->XBackDelayTime;
			}
		}
	}
	else if(cScanHoleStep == 3)
	{
		if(!X_DRV)
		{// ����ɨ�����
			cScanHoleStep = 0;
		}
	}
}

/************************************************/
/*
����ѡ��
*/
/************************************************/
void CheckForOnePlug(void)
{
	long temp1 = 0;

	OneHolePlugFlag = 0;
	
	temp1 = endposition + CurWoodLength;
	temp1 = temp1 / 2;
	if(temp1 > Winfos[0].hole_point)
	{
		OneHolePlugFlag = 1;// �ڿ�ǰ��
	}
	else
	{
		OneHolePlugFlag = 2;// �ڿ׺��
	}
}
char PlugBeforHolePro(void)
{
    char flag = 0;
    
    if(SpeedParam->ChaXiaoMode > 0)
	{	
	    if(SpeedParam->ChaXiaoMode == PlugTWNei)
	    {
		    if(currentWorkHoleNum == 0)
    	    {
	        	flag = 1;
    	    }
    	    else if(currentWorkHoleNum == (CHAXIAO_Index - 1))
            {
                if(CHAXIAO_Index != currentHoleNum && CHAXIAO_Index > 1)
                {
        		    if(bMiddleChaXiao)
        		    {
        		        flag = 1;
    	        	}
	        	}
    		}
	    }
	    else if(SpeedParam->ChaXiaoMode == PlugTWWai)
	    {
            if(currentWorkHoleNum == (currentHoleNum-1))
            {
				if(currentHoleNum >= 2)
				{
					flag = 1;
				}
            }
            else if(currentWorkHoleNum == (CHAXIAO_Index - 1))
            {
                if(CHAXIAO_Index != currentHoleNum && CHAXIAO_Index > 1)
                {
        		    if(bMiddleChaXiao)
        		    {
                        flag = 1;
    	        	}
	        	}
    		}
        }
        else if(SpeedParam->ChaXiaoMode == PlugBeforHole)
        {
            flag = 1;
        }
        else if(SpeedParam->ChaXiaoMode == PlugAll)
        {
            flag = 1;
        }
        else if(SpeedParam->ChaXiaoMode == PlugCustom)
        {
            if((cMidleCoil[3].Val) & (1 << currentWorkHoleNum))
            {
                flag = 1;
            }
        }
        else if(SpeedParam->ChaXiaoMode == PlugforOneHole)
        {
        	if(currentHoleNum == 1)
        	{
				CheckForOnePlug();
				if(OneHolePlugFlag == 1)
	            {
	                flag = 1;
	            }
            }
        }
        else if(SpeedParam->ChaXiaoMode == PlugforTwoHole)
        {
        
        }
	}

	return flag; 
}

char  PlugAfterHolePro(void)
{
    char flag = 0;
    //if(!bNOChaXiao)
    {
        if(SpeedParam->ChaXiaoMode == PlugTWWai)
        {
            if(currentWorkHoleNum == 0)
            {
                flag = 1;
            }
            else if(currentWorkHoleNum == (CHAXIAO_Index - 1))
            {
                if(CHAXIAO_Index != currentHoleNum && CHAXIAO_Index > 1)
                {
        		    if(!bMiddleChaXiao)
        		    {
        		        flag = 1;
        		    }
    		    }
    	    }
        }
        else if(SpeedParam->ChaXiaoMode == PlugTWNei)
        {
            if(currentWorkHoleNum == (currentHoleNum-1))
            {
                if(currentHoleNum >= 2)
                {
        	        {
        	            flag = 1;
        	        }
    	        }
            }
            else if(currentWorkHoleNum == (CHAXIAO_Index - 1))
            {
                if(CHAXIAO_Index != currentHoleNum && CHAXIAO_Index > 1)
                {
        		    if(!bMiddleChaXiao)
        		    {
        		        flag = 1;
        		    }
    		    }
    	    }
        }
        else if(SpeedParam->ChaXiaoMode == PlugafterHole)
        {
            flag = 1;
        }
        else if(SpeedParam->ChaXiaoMode == PlugAll)
        {
            flag = 1;
        }
        else if(SpeedParam->ChaXiaoMode == PlugCustom)
        {
            if(cMidleCoil[4].Val & (1 << currentWorkHoleNum))
            {
                flag = 1;
            }
        }
        else if(SpeedParam->ChaXiaoMode == PlugforOneHole)
        {
        	if(currentHoleNum == 1)
        	{
	            //CheckForOnePlug(); 
	            // ���ǰ�Ѿ������һ����������Բ�����
	            if(OneHolePlugFlag == 2)
	            {
	                flag = 1;
	            }
            }
        }
        else if(SpeedParam->ChaXiaoMode == PlugforTwoHole)
        {
            if(currentHoleNum == 2)
            {// ֻ�п�����������ʱ��Ŵ�����ʱ�򲻴�
                if(currentWorkHoleNum == 1)
                {
                    flag = 1;
                }
            }
        }
    }

    return flag;
}


unsigned long DrillHoleLocation = 0;// ���λ��
unsigned long DrillPlugLocation = 0;// �����λ��
void CalDrillLocation(void)
{
	long OverLcttmp;//,Datatmp;
//	long disttemp = 0;
    if(SpeedParam->XCalculate > 0)
    {// ������
        OverLcttmp = Winfos[currentWorkHoleNum].hole_point;
        OverLcttmp = OverLcttmp + infos[currentWorkHoleNum].hole_point;
        OverLcttmp = OverLcttmp / 2;
        DrillHoleLocation = OverLcttmp + TempDist.DrillDist;
    }
    else
    {// �ױ���
        OverLcttmp = Winfos[currentWorkHoleNum].hole_point;
        OverLcttmp = OverLcttmp + TempDist.DrillDist;
        DrillHoleLocation = OverLcttmp - TempDist.Dist15;
    }
}

void DataCompare(void)
{// ���ɨ�����ݵ�����
    uint16 i = 0;
    uint16 j = 0;
    long temp = 0;
    

    for(i = 0;i < 20;i++)
    {
        HolePos->PosB[i] = 0;
        HolePos->PosF[i] = 0;
        HolePos->Pos_Up[i] = 0;
        HolePos->Pos_Hole[i] = 0;
    }

	cDataArea[500] = CheckHoleNum[0];
	cDataArea[501] = CheckHoleNum[1];
    
    if(CheckHoleNum[0] > 20)
        j = 20;
    else
        j = CheckHoleNum[0];
    for(i = 0;i < j;i++)
    {
    	temp = HoleinfoB[i].hole_point / XEncodeDivide;
        HolePos->PosB[i] = PluseToPosition(X_AXIS, temp);
        if(i > 0)
    	{
    		Winfos[i - 1].hole_point = temp;
        }
    }  
    
    if(CheckHoleNum[1] > 20)
        j = 20;
    else
        j = CheckHoleNum[1];
	for(i = 0;i < (j);i++)
	{
    	infos[i].hole_point = HoleinfoF[i].hole_point / XEncodeDivide;
		HolePos->PosF[i] = PluseToPosition(X_AXIS, infos[i].hole_point);
    }

    for(i = 1;i < (j);i++)
    {
    	HolePos->Pos_Up[i] = HolePos->PosB[i] - HolePos->PosB[i - 1];
    	HolePos->Pos_Hole[i] = HolePos->PosB[i] - HolePos->PosF[i - 1];
    } 
}

unsigned char CheckHoleInfo(void)
{
    unsigned char HoleNumber = 0;
    unsigned char WorkHoleNumber = 0;
    unsigned char i = 0;
    long _lHoleData[4] = {0,0,0,0};
    long base = 0;
    
    //if(CheckHoleNum[0] == CheckHoleNum[1])
    {
        HoleNumber = CheckHoleNum[0];
    }
	//HoleNumber = currentHoleNum;
	CurWoodLength = HoleinfoB[0].hole_point/XEncodeDivide; 
	endposition = HoleinfoF[CheckHoleNum[1]].hole_point/XEncodeDivide; 

    if(HoleNumber > 1)
    {// ��һ�������ذ�ͷ ���һ���½��Ӱ�β
        for(i = 1;i < HoleNumber;i++)
        {
        	_lHoleData[0] = HoleinfoB[i].hole_point / XEncodeDivide;
        	_lHoleData[1] = HoleinfoF[i - 1].hole_point / XEncodeDivide;
            _lHoleData[2] = _lHoleData[0] - _lHoleData[1];
            if(_lHoleData[2] >= TempDist.MinHoleR && _lHoleData[2] <= TempDist.MaxHoleR)
            {
            	_lHoleData[3] = _lHoleData[0] - XWorkOrignPos;
            	if(_lHoleData[3] < TempDist.XBuChangRange)
            	{
            		if(TempDist.XBuChangBase > 0)
            		{
            			base = TempDist.XBuChangBase;
						_lHoleData[3] = TempDist.XBuChangRange - _lHoleData[3];
						_lHoleData[3] = _lHoleData[3] * base;
						_lHoleData[3] = _lHoleData[3] / TempDist.XBuChangRange;
						_lHoleData[0] = _lHoleData[0] + _lHoleData[3];
						_lHoleData[0] = _lHoleData[0] + base;
            		}
            		else
            		{
						base = 0 - TempDist.XBuChangBase;
						_lHoleData[3] = TempDist.XBuChangRange - _lHoleData[3];
						_lHoleData[3] = _lHoleData[3] * base;
						_lHoleData[3] = _lHoleData[3] / TempDist.XBuChangRange;
						_lHoleData[0] = _lHoleData[0] + _lHoleData[3];
						_lHoleData[0] = _lHoleData[0] - base;
            		}
            	}
                Winfos[WorkHoleNumber].hole_point = _lHoleData[0];
                infos[WorkHoleNumber].hole_point = _lHoleData[1];
                WorkHoleNumber++;
            }
        }
    }
    
    return WorkHoleNumber;
}

void ScanDrillHoleRun(void) 
{
	long OverLcttmp,Datatmp;
	long disttemp = 0;
	static unsigned long _lxstopdelay = 0;
	
	if(cScanDrillRunStep==1)
	{	
        VariableInit_Int();
        StartFlag = 1;
        ScanSGCmpFlag=1;
        cScanHoleStep = 1;
        MV_AccDec_Set(X_AXIS,120, 60);
        LMV_Const_Move(X_AXIS, SpeedParam->CheckHoleSpeed2, 0);	
        cScanDrillRunStep=200;
	}
	else if(cScanDrillRunStep==200)
	{// ��ʱ���ж����ȡ��������
		ScanCheckHoleProc();
		if(cScanHoleStep == 0)
		{			
			bCuiQi=0;
			StartFlag = 0;
			flgSgn = 0;
			flgSgnOld = 0;
			DataCompare();
			cScanDrillRunStep = 2;
		}
	}
	else  if((cScanDrillRunStep==2))
	{   
	    //if(DelayTime2 < dwTickCount)
        {
            currentHoleNum = CheckHoleInfo();
            CheckHoleNum[0] = 0;
            CheckHoleNum[1] = 0;
           
            if(currentHoleNum > 0)
            {
            	currentWorkHoleNum = currentHoleNum - 1;
            	cScanDrillRunStep = 11;
            	tXAxisStepper.cRealPosi += TempDist.SlowMotorBuchang;
			}
			else
			{
				cScanDrillRunStep = 10;
			}	
        }
	}
	else  if((cScanDrillRunStep == 3))//
	{// ͷβ�������
        if(!X_DRV)
        { 
            CalDrillLocation();
            OverLcttmp = DrillHoleLocation + TempDist.Dist32;
    		MoveAction_Pulse2(OverLcttmp,SpeedParam->MoveSpeed);
    		cDrillRunStep=1;
    		cScanDrillRunStep=4;
		}
	}
	else  if((cScanDrillRunStep==4))
	{
	    if(!X_DRV)
	    {
    		if((ScanSGCmpFlag==1)&&!bNOChaXiao)
    		{
    			ScanSGCmpFlag=0; 
    	        Datatmp = TempDist.SlowMotorBuchang;
    	        tXAxisStepper.cRealPosi=tXAxisStepper.cRealPosi-Datatmp;
    		}
    		DrillingHoleProc(TempDist.ChaXiaoDepth);
    		if(cDrillRunStep==0)
    		    cScanDrillRunStep=5;
		}
	}
	else  if((cScanDrillRunStep==5))
	{   	
	    if(!Y_DRV)
	    {       
            CalDrillLocation();
    		MoveAction_Pulse2(DrillHoleLocation,SpeedParam->MoveSpeed);
    		cDrillRunStep=1;
    		cScanDrillRunStep=6;
		}
	}
	else  if((cScanDrillRunStep==6)&&(!Y_DRV))
	{
		if((ScanSGCmpFlag==1)&&bNOChaXiao)
		{// fanxiang
			ScanSGCmpFlag=0;
        	Datatmp = TempDist.SlowMotorBuchang;
            tYAxisStepper.cRealPosi=tYAxisStepper.cRealPosi-Datatmp;
		}
		if(currentWorkHoleNum>0)
		{
			CurBoardData->CurBoardPos=Winfos[currentWorkHoleNum].hole_point-Winfos[currentWorkHoleNum-1].hole_point;
			CurBoardData->CurBoardPos=CurBoardData->CurBoardPos* TimeParam->cyclelength/10000;
		}
		DrillingHoleProc(TempDist.HoleDepth);
		if(cDrillRunStep==0)
		{
		    cScanDrillRunStep = 12;
		}
	}
	else  if((cScanDrillRunStep==7)&&(!Y_DRV))
	{// �׺�
		if(SpeedParam->ChaXiaoMode == PlugforTwoHole)
		{// ֻ��������ͬʱ�ǵڶ�����ʱ��ִ�������,
		//�����˫���м������п״���2���򲻻�ִ�иò�
		    if(SpeedParam->XCalculate > 0)
		    {// ������
                OverLcttmp = Winfos[1].hole_point + infos[1].hole_point;
                disttemp = Winfos[0].hole_point + infos[0].hole_point;
                disttemp = OverLcttmp - disttemp;
                disttemp = disttemp / 4;
		    }
		    else
		    {
                disttemp = Winfos[1].hole_point - Winfos[0].hole_point;
                disttemp = disttemp / 2;
		    }
		}
		else
		{// 
		    disttemp = TempDist.Dist32;
		}
		OverLcttmp = DrillHoleLocation - disttemp;
		MoveAction_Pulse2(OverLcttmp,SpeedParam->MoveSpeed);
		cDrillRunStep=1;
		cScanDrillRunStep=8;
	}
	else  if((cScanDrillRunStep==8))
	{// ���Ǵ�׺�����ģ��������ﲻӦ�ó��ַ��򲹳���ص�
		if((!Y_DRV))
		{
			DrillingHoleProc(TempDist.ChaXiaoDepth);
			if(cDrillRunStep==0)
			    cScanDrillRunStep=9;
		}
	}
	else  if((cScanDrillRunStep==9))
	{
		if(currentWorkHoleNum>0)
		{
			currentWorkHoleNum--;
			cScanDrillRunStep=11;
		}
		else
		{
			cScanDrillRunStep=10;
		}
	}
	else  if(cScanDrillRunStep==10)
	{
		cScanDrillRunStep=0;
	}
	else if(cScanDrillRunStep == 11)
	{
		cScanDrillRunStep = 5;
		if(SpeedParam->ChaXiaoMode > 0)
		{	
            char flag = 0;
		    flag = PlugBeforHolePro();
            if(flag > 0)
            {
                cScanDrillRunStep = 3;
            }
		}
	}
    else if(cScanDrillRunStep == 12)
	{
        cScanDrillRunStep = 9;
        if(CustomPlug->OneHolePlug > 0)
		{
            if(OneHolePlugFlag == 2)
            {
                cScanDrillRunStep = 7;
            }
		}
		else
		{
    	    if(SpeedParam->ChaXiaoMode > 0)
    	    {
    	        char flag = 0;
    	        flag = PlugAfterHolePro();
                if(flag > 0)
                {
                    cScanDrillRunStep = 7;
                }            
    	    }
	    }
	}
	else if(cScanDrillRunStep == 13)
	{

	}
}
/************************************************/
/*
��׳���
�������ǰ�ĵ�λ��ʱ���Լ���׺���˵�λ��ʱ
*/
/************************************************/
void DrillingHoleProc(long depth)
{
	unsigned long OverLcttmp = 0;
	static unsigned long BackDelay = 0;
	
	if((cDrillRunStep==1))
	{
		if(!(X_DRV || Y_DRV))
		{
		    CheckHoleData.R = Winfos[currentWorkHoleNum].hole_point-infos[currentWorkHoleNum].hole_point;
	       	BackDelay = dwTickCount + 10 * cDataArea[154];// X���ƶ���ʱ
	       	cDrillRunStep=2;
		}
	}
	else if((cDrillRunStep==2))
	{	//��ʼ���
	    if(BackDelay < dwTickCount)
	    {
	    	if(DelayTime2 < dwTickCount)
	    	{
	    	    if(SystemMode == 1)
	    	    {
	                OverLcttmp = TempDist.YOrignDist + depth;
	                MoveAction_Pulse(Y_AXIS,OverLcttmp,SpeedParam->DrillSpeed);
	            }
	            else
	            {
	                bDrillValve = 1;
	            }
	            cDrillRunStep=3;
            }
	    }
	}
	else if((cDrillRunStep==3))
    {
        if(SystemMode == 1)
        {
            if(!Y_DRV)
            {
                DrillRunStepDelay =  10UL * TimeParam->DrillCycleTime;
                DrillRunStepDelay = dwTickCount + DrillRunStepDelay;
                cDrillRunStep=4;
            }
        }
        else
        {
            if(bQiGangComeSgn)
            {
                DrillRunStepDelay =  10UL * TimeParam->DrillCycleTime;
                DrillRunStepDelay = dwTickCount + DrillRunStepDelay;
                cDrillRunStep=4;
            }
        }
    }
    else if((cDrillRunStep == 4))
    {
    	if(DrillRunStepDelay < dwTickCount)
    	{
	        if(SystemMode == 1)
	        {
	            OverLcttmp =0;
	            MoveAction_Pulse(Y_AXIS,OverLcttmp,SpeedParam->DrillSpeed);
	        }
	        else
	        {
	            bDrillValve = 0;
	        }
	        cDrillRunStep=5;
        }
    }
 	else if ((cDrillRunStep==5))
    {
        if(SystemMode == 1)
        {
            if(!Y_DRV)
            {
                cDrillRunStep=6;
                BackDelay = dwTickCount + 5000;
            }
        }
        else
        {
            // �����Ǵ���������׺���û��λ����ȡ����ʱ
            if(bQiGangBackSgn)
            {
            	qigangbackdelay = 10UL * CustomPlug->QiGangReBackDelay;
	            qigangbackdelay = dwTickCount + qigangbackdelay;
				cDrillRunStep = 7;
            }
        }  
    }
    else if ((cDrillRunStep==6))
    {
        if(bDrillBackSgn)
        {
            cDrillRunStep=0;
        }
        else
        {// ����
            if(BackDelay < dwTickCount)
            {
                AlarmNum = 15;
                bAlarmFlag = 1;
            } 
        }
    }
    else if (cDrillRunStep==7)
    {// ���������Ѿ������׺��Ʋ���λ�ı�����
		if(qigangbackdelay < dwTickCount)
			cDrillRunStep=0;
    }
}

void StopKeyPro(void)
{
	bPress1_Vavle =0;	
	bPress2_Vavle =0;
	bPress3_Vavle =0;
	bDrillValve=0;        	
	bCuiQi =0;       		
	bDrillMotor =0;       
    M43 = 0;
    M38 = 0;
    M39 = 0;
    AlarmNum = 0;

    if(bResume)
    {
	    bRBOrignXY = 0;
	    bXRst = 0;
	    bYRst = 0;
	    bResume = 0;
	    if (X_DRV) LMV_Dec_Stop(X_AXIS);
    	if (Y_DRV) LMV_Dec_Stop(Y_AXIS);
    }
    if(bRunning)
    {
	    bRunning = 0;
	    bRunning1 = 0;
	    bRunning2 = 0;
	    bRunning3 = 0;

	    ResetPeripheral();
	    //cDrillRunStep = 0;
	    //cScanDrillRunStep = 0;
	    //cDrillStep = 0;
	    cRunStep = 0;
	    flgSgn = 0;
	    StartFlag = 0;
    }
    if (X_DRV) LMV_Dec_Stop(X_AXIS);
    if (Y_DRV) LMV_Dec_Stop(Y_AXIS);
}

void StopKeyPro2(void)
{
	bPress1_Vavle =0;	
	bPress2_Vavle =0;
	bPress3_Vavle =0;
	bDrillValve=0;        	
	bCuiQi =0;       		
	bDrillMotor =0;       
    M43 = 0;
    M38 = 0;
    M39 = 0;
    bAlarmFlag = 0;

	if(bRunning)
	{
	    bRunning = 0;
	    bRunning1 = 0;
	    bRunning2 = 0;
	    bRunning3 = 0;
	    
	    ResetPeripheral();
	    
	    cRunStep = 0;
	    //cDrillRunStep = 0;
	    //cScanDrillRunStep = 0;
	    //cDrillStep = 0;
	    StartFlag = 0;
	    flgSgn = 0;
	    
	    if (X_DRV) LMV_Dec_Stop(X_AXIS);
	    if (Y_DRV) LMV_Dec_Stop(Y_AXIS);
    }
}

// �ж��Ƿ�쵽����г̣���ǰ����
void PreDecforScan(void)
{
	unsigned long temp = 0;

	HolePos->PosF[11] = cRunStep;
	HolePos->PosF[10] = CheckHoleNum[1];
	HolePos->PosF[9] = currentHoleNum;
	HolePos->PosF[8] = flgSgn;
	HolePos->PosF[7] = PluseToPosition(X_AXIS,CurDrillRaidus);
	if(cScanDrillRunStep == 100)
	{
		temp = CheckHoleNum[1] - 1;
		HolePos->PosF[0]  = HoleinfoF[temp].hole_point / XEncodeDivide;
		HolePos->PosF[1]  = DirllRaidusDist;
		if(CurAxisDistance->XDistance >= CustomPlug->PreDecDistance)
		{
			LMV_Dec_Stop(X_AXIS);
		}
		ScanCheckHoleProc();
	}
}

