#include "Global.h"

unsigned char cRunStep ,TongCaoEndFlag = 0;
unsigned char  HoleEndFlag = 0;// 定位打孔结束标示符
unsigned char  OneHolePlugFlag = 0;// 单孔自动判断插销位置标示符
unsigned char  TwoHolePlugFlag = 0;// 
unsigned long  DelayTime;
unsigned long  DelayTime1;
unsigned long  DelayTime2;
unsigned short WorkingFlag=0;
unsigned long NoworkStopdelay;
unsigned long XBackDelay = 0;
long XWorkOrignPos = 0;

unsigned short DingDrillNum;
unsigned short DingCaoNum;
long CurDrillRaidus;
long CurWoodLength;
long DirllRaidusDist;
long CurDrillingDist;
long CurWoodLastDist;
long CurDingDist;
unsigned char TouChaxiaoFlag=0;
unsigned short CurDepth;
unsigned short CurDepthNum;
unsigned short DrillOnceFlag=0;

static unsigned long DingweiDist[30];
static unsigned long DingWeiCao[30];
static unsigned long DingCaoWitch[30];
static unsigned short DingCaoDepth[30];
static unsigned short ScanSGCmpFlag=0;

//unsigned short NextDlay;
//unsigned short NextStepDlay;
//unsigned char  VerDrillNum,HorDrillNum,cYDrillCount;
//long tempdata = 0;


 static HoleInfomation infos[100];
 static HoleInfomation Winfos[100];

 HoleInfomation HoleinfoF[100];
 HoleInfomation HoleinfoB[100];
volatile char CheckHoleNum[2] = {0};
unsigned char StartFlag = 0;

unsigned char cDrillRunStep;
unsigned long DrillRunStepDelay;
unsigned char cScanDrillRunStep;
static unsigned char cCaoMoveDir = 0;
/************************************************/
/*
自动运行

*/
/************************************************/
char currentHoleNum = 0;
char currentWorkHoleNum = 0;
unsigned char cDrillStep = 0;
static unsigned long qigangbackdelay = 0;

//#define CheckObjectSgn I004

static volatile char CheckSgnFilter = 0;
static volatile char CheckSgnFilterOff = 0;
//static volatile long curSite1;
static volatile long nextPluse = 0;
volatile char flgSgn = 1;
volatile char flgSgnOld = 1;
volatile char flgSgn1 = 1;
volatile char flgSgnOld1= 1;
volatile char secondHoleFlg = 0;
long WoodLength;

static unsigned char DrillFlag;
static unsigned char PlugIndex = 0;

CaoMode0_str CaoMode0Data;
//volatile char nextCheckPluse = 0;
static volatile char NoObjectFlag = 0;
static volatile long endposition;
TempDist_str TempDist;

CheckHoleData_str CheckHoleData;

int DrillPageProc(void);
void DrillWeiChaXiaoProc(void) ;
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

void AutoCheckHole(void) 
{
    if (CheckObjectSgn) 
    {
        //NoObjectFlag = 0;
        if (++CheckSgnFilter > 2)
        {
        	flgSgnOld = flgSgn;
            flgSgn = 1;
            flgSgn1 = 1;
            CheckSgnFilterOff = 0;
        }
    }
    else
    {
        if (++CheckSgnFilterOff > 2)
        {
            flgSgnOld = flgSgn;
            flgSgn = 0;
            flgSgn1 = 0;
            CheckSgnFilter = 0;
        }
    }

    if (nextPluse > 0) 
    {
        nextPluse--;
    }

    if(_gcLocalHole)//bLocationHole)
    {
        DINGWEIHoleProc();
    }
    else if(_gcLocalSlot)// M108)
    {
        if(TimeParam->CaoMode != DuanCao)
        {
            TongCaoProc();
        }
        else
        {
            DINGWEIHoleProc();
        }
    }
    else
    {
        if(ScanMode != 1)
    	{
       	    CheckHoleProc();
        }
        else
        {
        	ScanCheckHoleProc();
        }
    }
}

void Work1StopCheck(void)
{
    bRunning1=0;
    bPress1_Vavle=0;
    
    if(bRunning2)
    {
        if(ScanMode == 1)
        {
            WorkingFlag=1;
            cRunStep=1; 
        }
        else
        {
            WorkingFlag=1;
            cRunStep=11;  
            memset(infos, 0, sizeof(infos));
            memset(Winfos, 0, sizeof(Winfos));
            currentHoleNum = 0;
            currentWorkHoleNum = 0;				           
            CheckSgnFilter = 0;				               
            nextPluse = 0;
            TouChaxiaoFlag=0;
        }
    }
    else if(bRunning3)
    {
        WorkingFlag=3;
        cRunStep=1; 
    }
    else
    {
        bCuiQi=0;
        WorkingFlag=0;
        cRunStep=1; 
        if((ScanMode == 1) || bLocationCao)
        {
         	MoveAction_Pulse2(TempDist.Work1Dist,SpeedParam->IdleSpeed);
        }
    }
}

void Work2StopCheck(void)
{
    bRunning2=0;
    bPress2_Vavle=0;
    if(bRunning3)
    {
        if(ScanMode == 1)
        {
        WorkingFlag=2;
        cRunStep=1;  
        }
        else
        {
        WorkingFlag=2;
        cRunStep=11;  
        memset(infos, 0, sizeof(infos));
        memset(Winfos, 0, sizeof(Winfos));
        currentHoleNum = 0;
        currentWorkHoleNum = 0;				           
        CheckSgnFilter = 0;				               
        nextPluse = 0;
        TouChaxiaoFlag=0;
        }
    }
    else if(bRunning1)
    {
        WorkingFlag=0;
        cRunStep=1; 
    }
    else
    {
        bCuiQi=0;
        WorkingFlag=0;
        cRunStep=1; 
        if((ScanMode == 1) || bLocationCao)
        {
         	MoveAction_Pulse2(TempDist.Work2Dist,SpeedParam->IdleSpeed);
        }
    }
}

void Work3StopCheck(void)
{
    bRunning3=0;
    bPress3_Vavle=0;
    if(bRunning1)
    {
        WorkingFlag=0;
        cRunStep=1;  						
    }
    else if(bRunning2)
    {
        WorkingFlag=1;
        cRunStep=1;  
    }
    else
    {
        bCuiQi=0;
        cRunStep=1;
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

// 走绝对位置脉冲数的
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

// 走绝对位置的
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

void RunInit(void)
{
    char i = 0;
    unsigned long temp = 0;
    
    memset(infos, 0, sizeof(infos));
	memset(Winfos, 0, sizeof(Winfos));
	memset(HoleinfoF, 0, sizeof(HoleinfoF));
	memset(HoleinfoB, 0, sizeof(HoleinfoB));

	//定位孔
	if(bLocationHole)
	{
    	memset(DingweiDist,0,sizeof(DingweiDist));
    	memcpy(DingweiDist,&cDataArea[180],120);
    	for(i = 0; i < 30;i++)
    	{
    	     DingDrillNum=i;
    	   	 if(DingweiDist[i]==0)break; 			                                       
    	}
    	for(i = 0; i < DingDrillNum;i++)
    	{
    	    temp = DingweiDist[i] + cDataArea[80];
	        DingweiDist[i] = PositionToPluse(X_AXIS, temp);
    	}
	}
	//定位槽
	//else if(bLocationCao)
	else if(TimeParam->CaoMode == DuanCao)
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
    	   	if(DingWeiCao[i]==0)
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
    	}
    }

	CheckHoleNum[0] = 0;
	CheckHoleNum[1] = 0;

    HoleEndFlag = 0;
	TongCaoEndFlag = 0;
    DrillFlag=0;
    currentHoleNum = 0;
    currentWorkHoleNum = 0;
    CurWoodLength = 0;
    cDrillStep = 0;
    CaoMode0Data.StartFlag = 0;
    CheckSgnFilter = 0;
    
    nextPluse = 0;
    TouChaxiaoFlag=0;  

    if(!bDrillMotor)
    {
        bDrillMotor = 1;
        DelayTime2 = dwTickCount + 100UL * TimeParam->StartDelay;
    }
}

void RunCheck(void)
{
    if(bRunning1)
	{
	    if(Press1time < dwTickCount)
	    {
    		bCuiQi=1;
    		WorkingFlag=0;
    	    cRunStep=11;
 
    	    MoveAction_Pulse2(TempDist.Work1Dist,SpeedParam->IdleSpeed);
	    }
	}
    else if(bRunning2)
	{
	    if(Press2time < dwTickCount)
	    {
    	 	bCuiQi=1;
    	 	WorkingFlag=1;
    		cRunStep=11;
    		
    		MoveAction_Pulse2(TempDist.Work2Dist,SpeedParam->IdleSpeed);
	    }  
	}
    else if(bRunning3)
    {
        if(Press3time < dwTickCount)
        {
        	bCuiQi=1;
        	WorkingFlag=2;
        	cRunStep=11;
        	
        	MoveAction_Pulse2(TempDist.Work3Dist,SpeedParam->IdleSpeed);
        }
    }
    else
    {
    	if(NoworkStopdelay==0)
    	    M43=1;
    }
}

void  RunFuncSelect(void)
{
    if(bLocationCao || bLocationHole)
    {
        cRunStep=2;
        cDrillStep = 100;
        DrillOnceFlag=1;
    }
    else
    {
        if(ScanMode==1)
        {
    	    cRunStep=2;
            cScanDrillRunStep = 1;
        }
    	else
    	{
            cRunStep=2;
            cDrillStep = 100;
            DrillOnceFlag=1;
        }
    }
}

void RunOverCheck(void)
{
    if(bLocationHole|M108)
    {
        cRunStep=22;  
        currentHoleNum=0;
    }
    else
    {
    	if(ScanMode != 1)
    	{
            CurWoodLastDist=LMV_Get_Command_Pos(X_AXIS);
            //if(bNOChaXiao||(currentHoleNum < 2))
            if((currentHoleNum < 2))
            {
                cRunStep=22;  
                currentHoleNum=0;	
            }
            else
            {
                if((SpeedParam->ChaXiaoMode == PlugTWNei) ||
                    (SpeedParam->ChaXiaoMode == PlugTWWai))
                {
                    cDrillStep=210;
                    cRunStep=200;
                }
                else
                {
                    cRunStep=22;  
                    currentHoleNum=0;
                }
            }
        }
        else
        {
    		OneHolePlugFlag = 0;
    		TwoHolePlugFlag = 0;
    		if(currentHoleNum > 0)
    		{    		    
    		    if(currentHoleNum == 1)
    		    {
                    //if(CustomPlug->OneHolePlug > 0)
                    if(SpeedParam->ChaXiaoMode == PlugforOneHole)
                    {
                        long temp = 0;
                        temp = endposition + CurWoodLength;
                        temp = temp / 2;
                        OneHolePlugFlag = 1;// 在孔前边
                        if(Winfos[currentHoleNum - 1].hole_point > temp)
                        {
                            OneHolePlugFlag = 2;// 在孔后边
                        }
                    }
    		    }
    		    else if(currentHoleNum == 2)
    		    {
    		        if(SpeedParam->ChaXiaoMode == PlugforTwoHole)
                    {
                        TwoHolePlugFlag = 1;
                    }
                }
                
    		    cScanDrillRunStep=2;
    			cRunStep=200;
    		}
    		else
    		{
    			cRunStep=22;  
    		}
        }
    }
}

void RunControl(void)
{
	static unsigned long  xbackdelay = 0;
	
    switch (cRunStep) 
    {
    case 1:
    	if(!X_DRV)
    	{
	        RunInit();
	        cRunStep=10;    
	        if(bSystemMode)
	        {
	            MoveAction_Pulse(Y_AXIS,0,SpeedParam->XIdleSpeed);
	        }
	        else
	        {
	            bDrillValve = 0;
	        }
        }
        break;
    case 10:    
        if(SystemMode > 0)// 1 双私服
        {
            if(bDrillBackSgn && !Y_DRV)
            {
                RunCheck(); 
            }
        }
        else
        {
            if(bQiGangBackSgn)
            {
                 RunCheck(); 
            }
        }
    	break;	        	 
    case 11:
    	if(!X_DRV)
    	{
    		cRunStep=100;
			xbackdelay = dwTickCount + 10 * CustomPlug->XBackDelayTime;
	    }
     	break;
    case 100:
    	if(xbackdelay < dwTickCount)
    	{
	    	if(!CheckObjectSgn)
			{
			    RunFuncSelect();
			    XWorkOrignPos = ulEncoderCount;
			}
			else
			{// 有板信号
				cRunStep=12;
			}
		}
    	break;
    case 12:
        MV_AccDec_Set(X_AXIS, XIDEACC, XIDEACC);
        LMV_Const_Move(X_AXIS, SpeedParam->IdleSpeed, XCCW);// 1);
        DelayTime1 = dwTickCount +50;
        cRunStep=13;
     	break;
     case 13:
        if(!X_ZERO)
        {
            if(!CheckObjectSgn)
            {// 检测到有信号       		
            	if(DelayTime1 < dwTickCount )
            	    cRunStep=14; // 认为有孔       		
            }
            else
            {
            	DelayTime1 = dwTickCount + 200;
            	if(WorkingFlag==1)
				{
					if((CurAxisDistance->YDistance <TimeParam->Work2Dist-20000))
					{
					 	AlarmNum = 14;
					 	//bAlarmFlag2 = 1;
						cRunStep=17;
					    LMV_Dec_Stop(X_AXIS);
					}
				}
				else if(WorkingFlag==2)
				{
					if((CurAxisDistance->YDistance <TimeParam->Work3Dist-20000))
					{
						AlarmNum = 14;
						//bAlarmFlag2 = 1;
						cRunStep=17;
						LMV_Dec_Stop(X_AXIS);
					}
				}	
            }
        }
     	else
     	{
     		cRunStep=17;
		    LMV_Dec_Stop(X_AXIS);
     	}
     	break;
     case 14:
		if(DelayTime1 < dwTickCount )
		{
         	cRunStep=15;
			LMV_Dec_Stop(X_AXIS);
		}         	
     	break;
     case 15:
     	if(!X_DRV)
     	{
     		cRunStep=16;
		    DelayTime1 = dwTickCount + 500;
     	}
     	break;
    case 17:
     	if(!X_DRV)
     	{
     		cRunStep=0;
     		cDrillStep=0;
     		
     	    WorkOver();
     	}
     	break;
     case 16:
     	if(DelayTime2 < dwTickCount )
     	{
     	    cRunStep=11;
     	}
     	break;
     case 20:
    	if(bLocationCao)
    	{
    		//if(bCaoMode)
    		if(TimeParam->CaoMode == DuanCao)
    		{
           	    if(CurDepth >= DingCaoDepth[currentWorkHoleNum]
                    && TongCaoEndFlag == 1)
            	{
                	TongCaoEndFlag = 0;
	     			LMV_Dec_Stop(X_AXIS);
	     			cRunStep=21;  
	     			cDrillStep=0;
            	}
        	}
        	else
        	{
            	if(CaoMode0Data.step == 0)
            	{
	     			cRunStep=21;  
	     			cDrillStep=0;
            	}
           	 }
        }
        else
        {
 		    LMV_Dec_Stop(X_AXIS);
 		    cRunStep=21;  
 		    cDrillStep=0;     
     	}
     	break;
     case 21:
        if(!X_DRV)
        {
            RunOverCheck();
        }     		   
     	break;
     case 22:
        if(!X_DRV)
        {
 			NoworkStopdelay=100*TimeParam->NoWorktime+1000;
 			if(!M27)
 			{
 				if(WorkingFlag==0)
 				{
 				    Work1StopCheck();
 				}
 				else if(WorkingFlag==1)
 				{
 				    Work2StopCheck();
 				}
 				 else if(WorkingFlag==2)
 				{
 				    Work3StopCheck();
 				}
 			} 
			else
 			{
 				bRunning1=0;
 				bPress1_Vavle=0;
 				bPress2_Vavle=0;
 				bPress3_Vavle=0;
 				bCuiQi=0;
 				cRunStep=23; 
 			}     			
 		}
     	break;
    case 23:
        cRunStep=24; 
        CurWoodLength = 0;
        MoveAction_Pulse2(TempDist.Work1Dist,SpeedParam->IdleSpeed);
        break;
    case 24:
        if(!X_DRV) 
            cRunStep=1; 
        break;
    default:
        break;
    }
}

void DrillCheck(void)
{
    if (currentWorkHoleNum < currentHoleNum) 
    {  
        cDrillStep=101;  
        LMV_Dec_Stop(X_AXIS);
    }
    if(_gcLocalSlot)//M108)
    {
        //if(!bCaoMode && CaoMode0Data.StartFlag > 0)
        if(TimeParam->CaoMode != DuanCao && CaoMode0Data.StartFlag > 0)
        {
            cDrillStep=101;
            LMV_Dec_Stop(X_AXIS);
            CaoMode0Data.StartFlag = 0;
        }
    }
    if (!X_DRV )//&& bDrillBackSgn)
    {
        //if(DrillOnceFlag&&(M106||M108))
        if(DrillOnceFlag &&
        	((_gcLocalHole > 0) || (_gcLocalSlot > 0)))
        {
            DrillOnceFlag = 0;
            StartFlag = 1;
            MV_AccDec_Set(X_AXIS,XIDEACC3, XIDEACC);
            LMV_Const_Move(X_AXIS, SpeedParam->WorkSpeed, XCW);// 0);
        }
        //else if(M106 && HoleEndFlag == 1)
        else if(_gcLocalHole && HoleEndFlag == 1)
        {
            HoleEndFlag = 0;
            cRunStep = 20; 
        }
        else
        {
            MV_AccDec_Set(X_AXIS,XIDEACC3, XIDEACC);
            LMV_Const_Move(X_AXIS, SpeedParam->WorkSpeed, XCW);//0);	            
        }
    }
}

long CalTmp1(void)
{
    long OverLcttmp;//LengthLcttmp,Datatmp;
    //unsigned short speed = 0;
/*
    //if(currentWorkHoleNum>0)
    //{
        //LengthLcttmp=Winfos[currentWorkHoleNum].hole_point-Winfos[currentWorkHoleNum-1].hole_point;
        //if(LengthLcttmp <= TempDist.Dist50)//(10000UL*5000/TimeParam->cyclelength ))
        //    Datatmp=1;
        //else if(LengthLcttmp <= TempDist.Dist65)//(10000UL*6500/TimeParam->cyclelength ))
        //    Datatmp=2;
        //else 
            Datatmp=0;
    //}
    else
    {
        Datatmp=0;
    }
    
    if(Datatmp==1)
    {
        OverLcttmp = Winfos[currentWorkHoleNum-1].hole_point + TempDist.DrillDist + TempDist.cDataArea91;
        //10000UL * (TimeParam->DrillDist+TimeParam->DrillCenCmp+cDataArea[91]) / TimeParam->cyclelength ;
        infos[currentWorkHoleNum].hole_point = infos[currentWorkHoleNum-1].hole_point+TempDist.cDataArea91;//10000UL*cDataArea[91]/TimeParam->cyclelength;
    }
    else*/
    {
        OverLcttmp = Winfos[currentWorkHoleNum].hole_point +  TempDist.DrillDist - TempDist.Dist15;
        //10000UL * (TimeParam->DrillDist+TimeParam->DrillCenCmp) / TimeParam->cyclelength ;
    }

    return OverLcttmp;
}

long CalTmp2(void)
{
    long OverLcttmp,LengthLcttmp,Datatmp;
    
   if(currentWorkHoleNum>0)
    {
        LengthLcttmp=Winfos[currentWorkHoleNum].hole_point-Winfos[currentWorkHoleNum-1].hole_point;
        if(LengthLcttmp <= TempDist.Dist50)
            Datatmp=1;
        else 
            Datatmp=0;
    }
    else
    {
        Datatmp=0;
    }
            
    if(Datatmp==1)
    {
        OverLcttmp = Winfos[currentWorkHoleNum-1].hole_point + TempDist.DrillDist - TempDist.Dist15+ TempDist.cDataArea91;
        Winfos[currentWorkHoleNum].hole_point= Winfos[currentWorkHoleNum-1].hole_point + TempDist.cDataArea91;
    }
    else
    {
        OverLcttmp = Winfos[currentWorkHoleNum].hole_point +  TempDist.DrillDist - TempDist.Dist15;
     }
    return OverLcttmp;
}

typedef struct
{
    char step;

}DrillHoleData_str;
static DrillHoleData_str DrillHoleData;

void DrillHoleProc(long depth)
{
    long OverLcttmp = 0;
    switch(DrillHoleData.step)
    {
    case 1:
        DrillHoleData.step = 2;
        break;
    case 2:
        if (bSystemMode)
        {
            if(!Y_DRV)
            {
                //开始钻孔
                OverLcttmp = TempDist.XOrignDist + depth;
                MoveAction_Pulse(Y_AXIS, OverLcttmp, SpeedParam->DrillSpeed);
                DrillHoleData.step = 3;
            }
        }
        else
        {
            bDrillValve = 1;
            DrillHoleData.step = 3;
        }
        break;
    case 3:
        if (bSystemMode)
        {
            if (!Y_DRV)
            {
                DelayTime = dwTickCount + 10UL * TimeParam->DrillCycleTime;
                DrillHoleData.step = 4;	                
            }
        }
        else
        {
            if(bQiGangComeSgn)
            {
                DelayTime = dwTickCount + 10UL * TimeParam->DrillCycleTime;
                DrillHoleData.step = 4;
            }
        }
        break;
    case 4:
        if (DelayTime < dwTickCount)
        {
            DrillHoleData.step = 5;
            CurDrillingDist=LMV_Get_Command_Pos(Y_AXIS);
            if(bSystemMode)
            {
                OverLcttmp =  0 ;	
                MoveAction_Pulse(Y_AXIS, OverLcttmp, SpeedParam->XIdleSpeed);
            }
            else
            {
                bDrillValve = 0;
                qigangbackdelay = 10UL * CustomPlug->QiGangReBackDelay;
                qigangbackdelay = dwTickCount + qigangbackdelay;
            }
        }
        break;
    case 5:
        if(bSystemMode)
        {
            if (bDrillBackSgn&&!Y_DRV) 
            {
                DrillHoleData.step = 6;
            }
        }
        else
        {
            if (bQiGangBackSgn) 
            {
                if(qigangbackdelay < dwTickCount)
                    DrillHoleData.step = 6;
            }
        }
        break;
    case 6:
        DrillHoleData.step = 0;
        break;
    default:
        break;
    }
}

void TongCaoXMoveAction(void)
{
    long OverLcttmp = 0;

    if(TimeParam->CaoMode == TongCao)
    {
        MV_AccDec_Set(X_AXIS, XIDEACC3, XIDEACC3);
        LMV_Const_Move(X_AXIS, SpeedParam->SlotSpeed, cCaoMoveDir);// cCaoMoveDir = 0远离原点  cCaoMoveDir = 1 接近原点
        if(cCaoMoveDir < 1)
        {          
            cCaoMoveDir = 1;
        }
        else
        {
            cCaoMoveDir = 0;
        }
    }
    else
    {
        if(TimeParam->CaoLength > SpeedParam->DrillRaidus)
        {
            if(CaoMode0Data.Flag == 0)
            {
                if(cCaoMoveDir < 1)
                {
                    cCaoMoveDir = 1;
                    OverLcttmp = CaoMode0Data.AllLength;// - CaoMode0Data.R;
                }
                else
                {
                    cCaoMoveDir = 0;
                    OverLcttmp = CurDingDist + CaoMode0Data.Offset + 
                            CaoMode0Data.Orign + CaoMode0Data.R;
                }
                MV_AccDec_Set(X_AXIS, XIDEACC3, XIDEACC3);
                MoveAction_Pulse(X_AXIS, OverLcttmp, SpeedParam->SlotSpeed);
                //CaoMode0Data.step = 8;
            }
        }
    }
}

void CaoOption(void)
{
    long OverLcttmp = 0;
    
    switch(CaoMode0Data.step)
    {
    case 1:
        CaoMode0Data.Delay = 0;
        cCaoMoveDir = 0;
        CaoMode0Data.R = PositionToPluse(X_AXIS,(SpeedParam->DrillRaidus / 2));//(SpeedParam->DrillRaidus / 2) * 10000 / TimeParam->XCycleLength;
        CaoMode0Data.D = PositionToPluse(X_AXIS,(SpeedParam->DrillRaidus));// * 10000 / TimeParam->XCycleLength;
        CaoMode0Data.StopPos = 0;
        CaoMode0Data.Remainder = 0;
        CaoMode0Data.Offset = PositionToPluse(X_AXIS,TimeParam->DrillOffset_Cao);//10000 * TimeParam->DrillOffset_Cao / TimeParam->XCycleLength;
        CaoMode0Data.Orign = PositionToPluse(X_AXIS,TimeParam->CaoOrign);// * 10000 / TimeParam->XCycleLength;
        CaoMode0Data.Depth = PositionToPluse(Y_AXIS,CurBoardData->TongCaoDepth);// * TimeParam->YPulsTotal / TimeParam->YCycleLength;
        CaoMode0Data.Length = PositionToPluse(X_AXIS,TimeParam->CaoLength);// * 10000 / TimeParam->XCycleLength;
        CaoMode0Data.AllLength = CurDingDist + CaoMode0Data.Offset + 
                                CaoMode0Data.Orign + CaoMode0Data.Length
                                - CaoMode0Data.R;
        if(CaoMode0Data.Orign == 0 && TimeParam->CaoLength == 0)
            CaoMode0Data.Flag = 1;
        else
            CaoMode0Data.Flag = 0;

        if(CaoMode0Data.Flag > 0)  
        {
            CaoMode0Data.step = 2;
        }
        else
        {
            if(CaoMode0Data.Depth > 0)  
                CaoMode0Data.step = 2;
            else
                CaoMode0Data.step = 0;
        }
        break;
    case 2:
        if(bSystemMode)
        {
            if(!X_DRV && !Y_DRV && bDrillBackSgn)
                CaoMode0Data.step = 3;
        }
        else
        {
            if(!X_DRV && bQiGangBackSgn)
                CaoMode0Data.step = 3;
        }
        break;
    case 3:
        if(TimeParam->CaoMode == TongCao)
        {
            OverLcttmp = CurDingDist + CaoMode0Data.Offset - CaoMode0Data.D;
        }
        else if(TimeParam->CaoOrign > 0)
        {
            OverLcttmp = CurDingDist + CaoMode0Data.Offset + CaoMode0Data.Orign;//DingWeiCao[currentWorkHoleNum];
            OverLcttmp = OverLcttmp + CaoMode0Data.R;
        }
        else
        {
            OverLcttmp = CurDingDist + CaoMode0Data.Offset - CaoMode0Data.D;//TempDist.Dist5;//3000;//
        }
        
        MoveAction_Pulse2(OverLcttmp, SpeedParam->WorkSpeed);
        CaoMode0Data.step = 4;
        break;
    case 4:
        if(!X_DRV)
        {
            if(bSystemMode)
            {
                CurDepth = 0;
                CurDepthNum = 0;

                CaoMode0Data.step = 5;
            }
            else
            {
                bDrillValve = 1;
                DelayTime = dwTickCount + 1500;
                CaoMode0Data.step = 6;
            }
        }
        break;
    case 5:
        if(CurDepth < CaoMode0Data.Depth)
        {
            OverLcttmp = CaoMode0Data.Depth - CurDepth;
            if(OverLcttmp > TempDist.CaoPicDepth)
            {
                CurDepth = CurDepth + TempDist.CaoPicDepth;
            }
            else
            {
                CurDepth = CurDepth + OverLcttmp;
            }
            OverLcttmp = TempDist.XOrignDist + CurDepth;
            CaoMode0Data.step = 6;
        }
        else
        {
            OverLcttmp = 0;
            CaoMode0Data.step = 9;
        }
        
        MoveAction_Pulse(Y_AXIS, OverLcttmp, SpeedParam->DrillSpeed);
        break;
    case 6:
        if(SystemMode == 1)
        {
            if(!Y_DRV)
            {
                DelayTime = dwTickCount + 50;
                CaoMode0Data.step = 7;
            }
        }
        else
        {
            if(bQiGangComeSgn)
            {
                DelayTime = dwTickCount + 50;
                CaoMode0Data.step = 7;
            }
        }
        break;
    case 7:
        if(DelayTime < dwTickCount)
        {
            TongCaoXMoveAction();
            CaoMode0Data.step = 8;
        }
        break;
    case 8:
        if(!X_DRV)
        {
            if(bSystemMode)
            {
                CaoMode0Data.step = 5;
            }
            else
            {
                bDrillValve = 0;
                CaoMode0Data.step = 9;
            }
        }

        if(TimeParam->CaoMode == 0)
        {// 通槽
            if(NoObjectFlag == 1)
            {
                NoObjectFlag = 0;
                CaoMode0Data.step = 10;
                CaoMode0Data.Flag = 0; 
                LMV_Dec_Stop(X_AXIS);
            }
        }
        break;
        
    case 9:
        if(SystemMode == 1)
        {
            if(!Y_DRV)
                CaoMode0Data.step = 12;
        }
        else
        {
            if(bQiGangBackSgn)
            {
                CaoMode0Data.step = 12;
            }
        }
        break;
        
    case 10:
        if(!X_DRV)
        {// 加上之前减速的距离肯定会拉出去的
            if(cCaoMoveDir > 0)
                CaoMode0Data.Remainder = tXAxisStepper.cRealPosi + CaoMode0Data.Offset + CaoMode0Data.D;
            else
                CaoMode0Data.Remainder = tXAxisStepper.cRealPosi - CaoMode0Data.Offset - CaoMode0Data.D;
            MoveAction_Pulse3(CaoMode0Data.Remainder, 2);
            CaoMode0Data.step = 8; 
        }
        break;
    case 11:
        if(!X_DRV)
        {
            CaoMode0Data.step = 10;
        }
        break;
    case 12:
        CaoMode0Data.step = 0;
        cRunStep = 20;
        break;
    default:
        CaoMode0Data.step = 0;
        break;
    }
}

typedef struct
{
    char step;

}DrillCao2Data_str;
static DrillCao2Data_str DrillCao2Data;
// 老的铣断槽
void CaoOption2(void)
{
    long OverLcttmp;
    
    switch(DrillCao2Data.step)
    {
    case 1:
        DrillCao2Data.step = 2;
        break;
    case 2:
        if(SystemMode == 1)
        {
            if (!Y_DRV && bDrillBackSgn)
            {
                DrillCao2Data.step = 3;
            } 
        }
        else
        {
            if (!Y_DRV && bQiGangBackSgn)
            {
                DrillCao2Data.step = 3;
            } 
        }        
        break;
    case 3:
        OverLcttmp = CurDingDist+DingWeiCao[currentWorkHoleNum];
        MoveAction_Pulse2(OverLcttmp, SpeedParam->WorkSpeed);

        DrillCao2Data.step = 4;     
        break;
    case 4:// 41
        if (!X_DRV)
        {
            if(bSystemMode)
            {
                DrillCao2Data.step = 5;  
            }
            else
            {
                bDrillValve = 1;
                CurDepth = DingCaoDepth[currentWorkHoleNum];
                DrillCao2Data.step = 6; 
            }
        }
        break;
    case 5:
        OverLcttmp = TempDist.XOrignDist;
        MoveAction_Pulse(Y_AXIS, OverLcttmp, SpeedParam->XIdleSpeed);
        CurDepth=0;
        CurDepthNum=0;

        if(DingCaoDepth[currentWorkHoleNum]>(CurDepthNum+1)*TimeParam->CaoPicDepth)
        {
            CurDepth += TimeParam->CaoPicDepth;
            CurDepthNum++;
            DrillCao2Data.step = 6;     
        }
        else if(DingCaoDepth[currentWorkHoleNum]==CurDepth)
        {
            DrillCao2Data.step = 14;
        }
        else
        {
            CurDepth=DingCaoDepth[currentWorkHoleNum];
            DrillCao2Data.step = 6;    
        }
        break;
    case 6:// 42
        if(SystemMode == 1)
        {
            if (!Y_DRV)
            {//开始钻孔
                OverLcttmp =TimeParam->TMk_XOrigin_Dist+CurDepth;	
                MoveAction_Dist(Y_AXIS, OverLcttmp, SpeedParam->DrillSpeed);

                DrillCao2Data.step = 7;
            }
        }
        else
        {
            bDrillValve = 1;
            DrillCao2Data.step = 7;
        }
        break;            
    case 7://43:
        if(SystemMode == 1)
        {
            if (!Y_DRV)
            {
                DrillCao2Data.step = 8;
            }
        }
        else
        {
            if(bQiGangComeSgn)
            {
                DrillCao2Data.step = 8;
            }
        }
        break;
    case 8:
        if(DingCaoWitch[currentWorkHoleNum] > TempDist.DrillRaidus)//SpeedParam->DrillRaidus)
        {
            OverLcttmp = DingCaoWitch[currentWorkHoleNum] - TempDist.DrillRaidus;//SpeedParam->DrillRaidus;
        }
        else
        {
            OverLcttmp = 0;
        }	
        OverLcttmp=OverLcttmp+CurDingDist+DingWeiCao[currentWorkHoleNum];
        MoveAction_Pulse2(OverLcttmp, SpeedParam->SlotSpeed);
        DrillCao2Data.step = 9;
        break;
    case 9://44:
        if (!X_DRV)
        {
            if(SystemMode == 1)
            {
                DrillCao2Data.step = 10;
            }
            else
            {
                //cDrillStep=50;
                DrillCao2Data.step = 14;
            }
        }
        break;	
    case 10:
        if(DingCaoDepth[currentWorkHoleNum]>(CurDepthNum+1)*TimeParam->CaoPicDepth)
        {
            CurDepth+=TimeParam->CaoPicDepth;
            CurDepthNum++;
            DrillCao2Data.step = 11;
        }
        else if(DingCaoDepth[currentWorkHoleNum]==CurDepth)
        {
            DrillCao2Data.step = 14;
        }
        else
        {
            CurDepth = DingCaoDepth[currentWorkHoleNum];
            DrillCao2Data.step = 11;
        }
        break;
    case 11://45:
        if (!X_DRV)
        {
            OverLcttmp =TimeParam->TMk_XOrigin_Dist+CurDepth;	
            MoveAction_Dist(Y_AXIS, OverLcttmp, SpeedParam->DrillSpeed);
            DrillCao2Data.step = 12;
        }
        break;	            
    case 12://46:
        if (!Y_DRV)
        {
            dwRealPosi = LMV_Get_Command_Pos(X_AXIS);
            OverLcttmp = CurDingDist+DingWeiCao[currentWorkHoleNum];
            MoveAction_Pulse2(OverLcttmp, SpeedParam->SlotSpeed);
            DrillCao2Data.step = 13;            
        }
        break;  
    case 13://47:
        if (!X_DRV)
        {
            if(DingCaoDepth[currentWorkHoleNum]>(CurDepthNum+1)*TimeParam->CaoPicDepth)
            {
                CurDepth+=TimeParam->CaoPicDepth;
                CurDepthNum++;
                DrillCao2Data.step = 6;
            }
            else if(DingCaoDepth[currentWorkHoleNum]==CurDepth)
            {
                DrillCao2Data.step = 14;  
            }
            else
            {
                CurDepth=DingCaoDepth[currentWorkHoleNum]; 
                DrillCao2Data.step = 6;
            }
        }
        break;
    case 14://50:
        if (!X_DRV)
        {
            DrillCao2Data.step = 15;
            if(SystemMode == 1)
            {
                OverLcttmp =  0 ;	
                MoveAction_Pulse(Y_AXIS, OverLcttmp, SpeedParam->XIdleSpeed);
 
            }
            else
            {
                bDrillValve = 0;
            }
        }
        break;  
    case 15://51:
        if(SystemMode == 1)
        {
            if (bDrillBackSgn&&!Y_DRV) 
            {
                DrillCao2Data.step = 16;
            }
        }
        else
        {
            if(bQiGangBackSgn)
            {
                DrillCao2Data.step = 16;
            }
        }
        break;
    case 16:
        DrillCao2Data.step = 0;
        break;
    default:
        break;
    }
}

typedef struct
{
    char step;

}DrillWeiData_str;
static DrillWeiData_str DrillWeiData;
// 边扫边打时的尾处理函数,应该是尾插销的处理
void DrillWeiProc(void)
{
    long OverLcttmp = 0;
    
    switch(DrillWeiData.step)
    {
    case 1:
        DrillWeiData.step = 2;
        break;
    case 2:
        if(bSystemMode)
        {
            if (bDrillBackSgn)
            {
                DrillWeiData.step = 3;
            }
        }
        else
        {
            if (bQiGangBackSgn)
            {
                DrillWeiData.step = 3;
            }
        }
        break;
    case 3:
        //cDrillStep = 211;
        DrillWeiData.step = 4;
        DelayTime = dwTickCount + 20;        
        break;
    case 4://211:
        if((DelayTime < dwTickCount))
        {
            if(bSystemMode)
            {
                if (bDrillBackSgn) 
                {
                    DrillWeiData.step = 5;
                }
            }
            else
            {
                if (bQiGangBackSgn) 
                {
                    DrillWeiData.step = 5;
                }
            }
        }
        break;
    case 5:
        if(currentHoleNum > 1)
        {
            DrillWeiChaXiaoProc();
            DrillWeiData.step = 6;
            //cDrillStep=212;
        }
        else
        {
            DrillWeiData.step = 8;

        }   
        break;
    case 6://212:
        if(!X_DRV)
        {
	        DrillHoleData.step = 1;
	        DrillWeiData.step = 7;
        }
        break;
    case 7://220:
        DrillHoleProc(TempDist.ChaXiaoDepth);//SpeedParam->ChaXiaoDepth);
        if(DrillHoleData.step == 0)
        {
            OverLcttmp = CurWoodLastDist;
            MoveAction_Pulse2(OverLcttmp, SpeedParam->WorkSpeed);
            DrillWeiData.step = 8;
        }
        break;    
    case 8://213:
        DrillWeiData.step = 0;
        break;
    }
}
void DrillAction(void)
{
    long OverLcttmp;//,LengthLcttmp,Datatmp;
    unsigned short speed = 0;

    switch (cDrillStep)
    {	
    case 100:
        if(DelayTime2 < dwTickCount )
        {
            DrillCheck();
        }
        break;
    case 101:
        if (!X_DRV && 
            ((bSystemMode && bDrillBackSgn) || (!bSystemMode && bQiGangBackSgn)))
        {
            if(bLocationHole)
            {
                cDrillStep=60; 
                CurDingDist = HoleinfoB[0].hole_point / XEncodeDivide;
            }
            else  if(bLocationCao)
            {
            	//_gCaoDrillDist = HoleinfoB[0].hole_point / 2;
            	CurDingDist = HoleinfoB[0].hole_point / XEncodeDivide;
                if(TimeParam->CaoMode == DuanCao)
                {
                    //LMV_Dec_Stop(Y_AXIS);
                    cDrillStep=40;
                }
                else
                {
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
    //设置伺服电机停的位置	                
        if (!X_DRV && 
            ((bSystemMode && bDrillBackSgn) || (!bSystemMode && bQiGangBackSgn)))
        {
            cDrillStep++;
            //dwRealPosi = LMV_Get_Command_Pos(Y_AXIS);

            if(_gcLocalHole)//M106)
            {
                speed = SpeedParam->MoveSpeed;//WorkSpeed;
                OverLcttmp = CurDingDist + DingweiDist[currentWorkHoleNum];
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
        {// 打孔//开始钻孔
            cDrillStep=3;
            DrillHoleData.step = 1;            
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
        DrillFlag=0;
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
        { // 打插销
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
    case 40://铣槽
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
    case 60:// 定位打孔
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
        OverLcttmp = CurDingDist + DingweiDist[currentWorkHoleNum];

        MoveAction_Pulse2(OverLcttmp,SpeedParam->MoveSpeed);
        cDrillStep=2;   
        break;
    default:
        break;
    }
}

void CalTempDist(void)
{
#if 1
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
        TempDist.XOrignDist = PositionToPluse(Y_AXIS,TimeParam->TMk_XOrigin_Dist);//TimeParam->TMk_XOrigin_Dist * TimeParam->XPulsTotal / TimeParam->Xcyclelength;
        TempDist.HoleDepth = PositionToPluse(Y_AXIS,TimeParam->Depth);//TimeParam->Depth * TimeParam->XPulsTotal / TimeParam->Xcyclelength;
        TempDist.ChaXiaoDepth = PositionToPluse(Y_AXIS,SpeedParam->ChaXiaoDepth);//SpeedParam->ChaXiaoDepth * TimeParam->XPulsTotal / TimeParam->Xcyclelength;
    }
#endif
}

void AutoRun(void)
{
    if (bRunning && (!bResume) && (!(bXRst || bYRst)) && (RstPphlDlay == 0) && !bStop) 
    {
    	RunControl(); 
    	//if(ScanMode == 1 && (!(bLocationCao || bLocationHole)) 
    	if(ScanMode > 0 && (!((_gcLocalSlot > 0) || (_gcLocalHole > 0))))
    	    ScanDrillHoleRun();
    	DrillAction();
    }
    else
    {
        if(!X_DRV && !Y_DRV)
        {
            CalGearRatio();
            CalTempDist();
        }
    }
     if (SystemMode > 0)
     {
     		bDrillValve=bCuiQi;
     }
}
int DrillPageProc(void) 
{
	long OverLcttmp = 0;
	int flag = 0;

    if(SpeedParam->ChaXiaoMode == PlugTWNei)
    {
        if (currentWorkHoleNum == 0)
        {
            OverLcttmp = CurDrillingDist + TempDist.Dist32;
            MoveAction_Pulse2(OverLcttmp, SpeedParam->MoveSpeed);
            flag = 1;
            TouChaxiaoFlag = 0;
            PlugIndex = 0;
        }
        else if (currentWorkHoleNum == (CHAXIAO_Index - 1))
        {
            if (bMiddleChaXiao)
            {
                OverLcttmp = CurDrillingDist + TempDist.Dist32;
                MoveAction_Pulse2(OverLcttmp, SpeedParam->MoveSpeed);
                flag = 1;
                TouChaxiaoFlag = 0;
                PlugIndex = 0;
            } 
            else
            {
                OverLcttmp = CurDrillingDist -TempDist.Dist32;
                MoveAction_Pulse2(OverLcttmp, SpeedParam->MoveSpeed);
                flag = 1;
                TouChaxiaoFlag = 1;
                PlugIndex = 0;
            }
        }
        else
        {
        	flag = 2;
        	//PlugIndex = 0;
        	TouChaxiaoFlag=0;
        }
    }
    else if(SpeedParam->ChaXiaoMode == PlugTWWai)
    {
        if (currentWorkHoleNum == 0)
        {
            OverLcttmp = CurDrillingDist -TempDist.Dist32;
            MoveAction_Pulse2(OverLcttmp, SpeedParam->MoveSpeed);
            flag = 1;
            TouChaxiaoFlag = 1;
            //PlugIndex = 0;
        } 
        else if (currentWorkHoleNum == (CHAXIAO_Index - 1))
        {
            if (bMiddleChaXiao)
            {
                OverLcttmp = CurDrillingDist + TempDist.Dist32;
                MoveAction_Pulse2(OverLcttmp, SpeedParam->MoveSpeed);
                flag = 1;
                TouChaxiaoFlag = 0;
                //PlugIndex = 0;
            } 
            else
            {
                OverLcttmp = CurDrillingDist -TempDist.Dist32;
                MoveAction_Pulse2(OverLcttmp, SpeedParam->MoveSpeed);
                flag = 1;
                TouChaxiaoFlag = 1;
                //PlugIndex = 0;
            }
        }
        else
        {
        	flag = 2;
        	//PlugIndex = 0;
        	TouChaxiaoFlag=0;
        }
    }  
    
    else if(SpeedParam->ChaXiaoMode == PlugBeforHole)
    {
        OverLcttmp = CurDrillingDist + TempDist.Dist32;
        MoveAction_Pulse2(OverLcttmp, SpeedParam->MoveSpeed);
        flag = 1;
        TouChaxiaoFlag=0;
        //PlugIndex = 0;
    }
    else if(SpeedParam->ChaXiaoMode == PlugafterHole)
    {
        OverLcttmp = CurDrillingDist -TempDist.Dist32;
        MoveAction_Pulse2(OverLcttmp, SpeedParam->MoveSpeed);
        flag = 1;
        TouChaxiaoFlag=1;
        //PlugIndex = 0;
    }
    
    return flag;
}

void DrillWeiChaXiaoProc(void) 
{
    long OverLcttmp = 0;

    if (bTongXiangNeiChaXiao)
    {
        OverLcttmp = CurDrillingDist - TempDist.Dist32;
        MoveAction_Pulse2(OverLcttmp, SpeedParam->MoveSpeed);
    } 
    else if (bTongXiangWaiChaXiao) 
    {
        OverLcttmp = CurDrillingDist + TempDist.Dist32;//+ TempDist.ChaXiaoPos;
        MoveAction_Pulse2(OverLcttmp, SpeedParam->MoveSpeed);
    }
      
}
void CheckHoleProc(void) 
{
    switch (cRunStep) 
    {           
    case 2:
        if (flgSgn == 1 && flgSgnOld == 0)
        {
            cRunStep = 3;  
            CurWoodLength= tXAxisStepper.cRealPosi;
            nextPluse = 10;
        }
        break;
    case 3: 				//下降沿跳变
        if (((flgSgn == 0 && flgSgnOld == 1) ||(flgSgn == 0))&&(nextPluse==0)&&(DrillFlag==0))
        {
        	DrillFlag=1;
        	infos[currentHoleNum].hole_point = tXAxisStepper.cRealPosi;
        	DirllRaidusDist= tXAxisStepper.cRealPosi;
            nextPluse =50;
        	cRunStep = 4;
        }
        break;
    case 4:
		CurDrillRaidus= tXAxisStepper.cRealPosi-DirllRaidusDist;
		WoodLength =tXAxisStepper.cRealPosi-CurWoodLength;
 //检测有孔信息
        if (((flgSgn == 1 && flgSgnOld == 0)||(flgSgn == 1))&&(nextPluse==0))
        {
            cRunStep = 5;
            CheckSgnFilter = 0;
            //secondHoleFlg = 1;
            //secondHoleFlg = 0;
            Winfos[currentHoleNum].hole_point = tXAxisStepper.cRealPosi;
            nextPluse =10;                    
        } 
        else if(CurDrillRaidus > TempDist.MaxHoleR)
		{
			cRunStep=6;
			DrillFlag=0;
		}

       break;
    case 5:
        CheckHoleData.R = Winfos[currentHoleNum].hole_point-infos[currentHoleNum].hole_point;

        if((CheckHoleData.R >= TempDist.MinHoleR)&&(TouChaxiaoFlag==0))
        {
            currentHoleNum++;
            if (currentHoleNum >= 100)
                currentHoleNum = 0;             
        }
        else
        {
        	DrillFlag=0;
        }
        if(TouChaxiaoFlag)
        {
        	DrillFlag=0;
        	cRunStep = 8;
        	nextPluse =10000;    
        }
        else
        {
        	cRunStep = 3;
        }
	    
        break;
    case 6:
		WoodLength =tXAxisStepper.cRealPosi-CurWoodLength;
		cRunStep = 7;
        break;
    case 7:
        cRunStep = 20;
        break;
    case 8:
        if(nextPluse==0)
        {
        	TouChaxiaoFlag=0;
        	cRunStep = 3;
        }
        break;
    }
}
void DINGWEIHoleProc(void) 
{
    unsigned char i;
    switch (cRunStep) 
    {           
    case 2:
        if (flgSgn == 1 && flgSgnOld == 0)
        {
            cRunStep = 3;  
            CurWoodLength= tXAxisStepper.cRealPosi;
            CurDingDist=tXAxisStepper.cRealPosi;
            nextPluse = 10;//1000;   
        	//for(i==0;i<currentHoleNum+1;i++)
        	//{
            //	infos[i].hole_point =tXAxisStepper.cRealPosi;
        	//}
        }
        break;
    case 3: 				//下降沿跳变
        if((nextPluse==0))
        {
        	if(_gcLocalHole)//M106)
        	    currentHoleNum=DingDrillNum;
        	else 
        	{
                if(TimeParam->CaoMode == DuanCao)
        	        currentHoleNum=DingCaoNum;
                else
                {
                    CaoMode0Data.StartFlag = 1;
                }
            }
        }

        if ((flgSgn == 0 && flgSgnOld == 1||flgSgn == 0)&& (nextPluse==0))
        {
            if(_gcLocalHole)//bLocationHole)
            {
                CurDrillRaidus = tYAxisStepper.cRealPosi;
                cRunStep = 8;
                nextPluse = 10;//1000;  
            }
            else
            {
                cRunStep=6;
                nextPluse = 10;//10000;  
            }
        }
        WoodLength =tYAxisStepper.cRealPosi-CurWoodLength;
        break;
    case 6:
		if(nextPluse==0)
		{			
			cRunStep = 7;
		}
        break;
    case 7:
        cRunStep = 20;        
        break;
    case 8:
        CheckHoleData.R = tYAxisStepper.cRealPosi - CurDrillRaidus;
        if (((flgSgn == 1 && flgSgnOld == 0) ||(flgSgn == 1))&&(nextPluse==0))
        //if ((flgSgn == 1) && (nextPluse==0))
        {
            cRunStep = 3;
            nextPluse = 10;//1000;             
        }
        else if(CheckHoleData.R > TempDist.MaxHoleR)
        {
            cRunStep = 7;
            //nextPluse = 10000;  
        }
        break;
    default:
        break;
    }
}

void TongCaoProc(void) 
{
    unsigned char i;
    switch (cRunStep) 
    {           
    case 2:
        if (flgSgn == 1 && flgSgnOld == 0)
        {
            cRunStep = 3;  
            CurWoodLength= tXAxisStepper.cRealPosi;
            CurDingDist=tXAxisStepper.cRealPosi;
            nextPluse = 1000;   
        	for(i==0;i<currentHoleNum+1;i++)
        	{
            	infos[i].hole_point =tXAxisStepper.cRealPosi;
        	}
        }
        break;
    case 3: 				//下降沿跳变
        if((nextPluse==0))
        {
            if(TimeParam->CaoMode == DuanCao)
    	        currentHoleNum=DingCaoNum;
            else
            {
                CaoMode0Data.StartFlag = 1;
            }
        }

        if ((flgSgn == 0 && flgSgnOld == 1||flgSgn == 0)&& (nextPluse==0))
        {
            {
                cRunStep=6;
                NoObjectFlag = 1;
                nextPluse = 10000;  
            }
        }
        WoodLength =tXAxisStepper.cRealPosi - CurWoodLength;
        break;
    case 6:
        if ((flgSgn == 1) && (nextPluse==0))
        {
            cRunStep = 3;
            nextPluse = 1000;             
        }
        break;
    case 7:
        cRunStep = 20;        
        break;
    case 8:
        break;
    default:
        break;
    }
}

/************************************************/
/*
先扫孔扫孔模式
*/
/************************************************/
void ScanCheckHoleProc(void) 
{
    switch (cRunStep) 
    {           
    case 2:        
        if (flgSgn == 1 && flgSgnOld == 0)
        {
            cRunStep = 3;  
            CurWoodLength= tXAxisStepper.cRealPosi;
            nextPluse = 10;
        }
        break;
    case 3: 				//下降沿跳变
        WoodLength =tXAxisStepper.cRealPosi - CurWoodLength;
        if (((flgSgn == 0 && flgSgnOld == 1) ||(flgSgn == 0))&&(nextPluse==0))
        {
        	infos[currentHoleNum].hole_point = tXAxisStepper.cRealPosi;
        	DirllRaidusDist= tXAxisStepper.cRealPosi;
            nextPluse =50;
        	cRunStep = 4;
        	endposition = tXAxisStepper.cRealPosi;
        }
        break;
    case 4:
		CurDrillRaidus= tXAxisStepper.cRealPosi - DirllRaidusDist;
		WoodLength =tXAxisStepper.cRealPosi - CurWoodLength;
 //检测有孔信息
        if (((flgSgn == 1 && flgSgnOld == 0)||(flgSgn == 1))&&(nextPluse==0))
        {
            cRunStep = 5;
            CheckSgnFilter = 0;
            //secondHoleFlg = 1;
            //secondHoleFlg = 0;
            Winfos[currentHoleNum].hole_point = tXAxisStepper.cRealPosi;
            nextPluse = 10;                    
        } 
        else if(CurDrillRaidus > TempDist.MaxHoleR)
    	{
    		cRunStep=6;
    	}
       break;
    case 5:
        CheckHoleData.R = Winfos[currentHoleNum].hole_point - infos[currentHoleNum].hole_point;
        if((CheckHoleData.R >= TempDist.MinHoleR)&&(TouChaxiaoFlag==0))
        {
            currentHoleNum++;
            if (currentHoleNum >= 100)
                currentHoleNum = 0;             
        }
        cRunStep = 3;	    
        break;
    case 6:
		WoodLength =tXAxisStepper.cRealPosi - CurWoodLength;
		cRunStep = 7;
        break;
    case 7:
        cRunStep = 20;
        break;
    default:
        break;
    }
}

/************************************************/
/*
扫描打孔处理
*/
/************************************************/
char PlugBeforHolePro(void)
{
    char flag = 0;
    
    //if(SpeedParam->ChaXiaoMode > 0)
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
            if(OneHolePlugFlag == 1)
            {
                flag = 1;
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
            {
                flag = 1;
            }
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
            if(OneHolePlugFlag == 2)
            {
                flag = 1;
            }
        }
        else if(SpeedParam->ChaXiaoMode == PlugforTwoHole)
        {
            if(TwoHolePlugFlag == 1)
            {
                if(currentWorkHoleNum == 1)
                {
                    flag = 1;
                }
            }
        }
    }

    return flag;
}


unsigned long DrillHoleLocation = 0;// 打孔位置
unsigned long DrillPlugLocation = 0;// 打插销位置
void CalDrillLocation(void)
{
	long OverLcttmp;//,Datatmp;
//	long disttemp = 0;
    if(SpeedParam->XCalculate > 0)
    {// 孔中心
        OverLcttmp = Winfos[currentWorkHoleNum].hole_point;
        OverLcttmp = OverLcttmp + infos[currentWorkHoleNum].hole_point;
        OverLcttmp = OverLcttmp / 2;
        DrillHoleLocation = OverLcttmp + TempDist.DrillDist;
    }
    else
    {// 孔边延
        OverLcttmp = Winfos[currentWorkHoleNum].hole_point;
        OverLcttmp = OverLcttmp + TempDist.DrillDist;
        DrillHoleLocation = OverLcttmp - TempDist.Dist15;
    }
}

void DataCompare(void)
{
    uint16 i = 0;
//    uint16 TempData = 0;
    uint16 j = 0;
    if(currentHoleNum >= 6)
        j = 6;
    else
        j = currentHoleNum;
    for(i = 0;i < 12;i++)
    {
        HolePos->PosB[i] = 0;
        HolePos->PosF[i] = 0;
    }
    for(i = 0;i < (j);i++)
    {
        HolePos->PosF[i] = infos[i].hole_point;//infos[i].hole_point;
        HolePos->PosF[6 + i] = Winfos[i].hole_point;
        
        HolePos->PosB[i] = Winfos[i].hole_point - infos[i].hole_point;//Winfos[i].hole_point;
		if(i < (j - 1))
		{
        	HolePos->PosB[6 + i] = Winfos[i + 1].hole_point - Winfos[i].hole_point;
        }
    }
}

unsigned char CheckHoleInfo(void)
{
    unsigned char HoleNumber = 0;
    unsigned char WorkHoleNumber = 0;
    unsigned char i = 0;
    long _lHoleData[4] = {0,0,0,0};
    long base = 0;
    
    if(CheckHoleNum[0] == CheckHoleNum[1])
    {
        HoleNumber = CheckHoleNum[0];
    }

    if(HoleNumber > 1)
    {// 第一个上升沿板头 最后一个下降延板尾
        for(i = 1;i < HoleNumber;i++)
        {
        	#if 1
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
            #endif
        }
    }
    return WorkHoleNumber;
}


void ScanDrillHoleRun(void) 
{
	long OverLcttmp,Datatmp;
	long disttemp = 0;
	
	if(cScanDrillRunStep==1)
	{	
        ScanSGCmpFlag=1;
        StartFlag = 1;
        MV_AccDec_Set(X_AXIS,120, 80);
        LMV_Const_Move(X_AXIS, SpeedParam->CheckHoleSpeed2, 0);	
        cScanDrillRunStep=100;

	}
	else  if((cScanDrillRunStep==2))
	{   
	    if(DelayTime2 < dwTickCount)
        {
            bCuiQi=0;
            StartFlag = 0;
			
            currentHoleNum = CheckHoleInfo();
            DataCompare();
            if(currentHoleNum > 0)
            {
            	currentWorkHoleNum = currentHoleNum - 1;
            	cScanDrillRunStep = 11;
            	Datatmp = TempDist.SlowMotorBuchang;
            	tXAxisStepper.cRealPosi=tXAxisStepper.cRealPosi + Datatmp;
			}
			else
			{
				cScanDrillRunStep = 10;
			}	
        }
	}
	else  if((cScanDrillRunStep == 3))//
	{// 头尾向外插销
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
	{// 孔后
		if(SpeedParam->ChaXiaoMode == PlugforTwoHole)
		//if(TwoHolePlugFlag == 1)
		{// 只有两个孔同时是第二个孔时才执行这里的
		    if(SpeedParam->XCalculate > 0)
		    {// 孔中心
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
		{
		    disttemp = TempDist.Dist32;
		}
		OverLcttmp = DrillHoleLocation - disttemp;
		MoveAction_Pulse2(OverLcttmp,SpeedParam->MoveSpeed);
		cDrillRunStep=1;
		cScanDrillRunStep=8;
	}
	else  if((cScanDrillRunStep==8)&&(!Y_DRV))
	{// 这是打孔后插销的，所以这里不应该出现反向补偿相关的
		DrillingHoleProc(TempDist.ChaXiaoDepth);
		if(cDrillRunStep==0)
		    cScanDrillRunStep=9;
	}
	else  if((cScanDrillRunStep==9) )
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
		cRunStep = 22;
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
钻孔程序
*/
/************************************************/

void DrillingHoleProc(long depth)
{
	unsigned long OverLcttmp = 0;
	static unsigned long BackDelay = 0;
	
	if((cDrillRunStep==1)&&!X_DRV&&!Y_DRV)
	{
	    CheckHoleData.R = Winfos[currentWorkHoleNum].hole_point-infos[currentWorkHoleNum].hole_point;
       	BackDelay = dwTickCount + 10 * cDataArea[154];// X轴移动延时
       	cDrillRunStep=2;
	}
	else if((cDrillRunStep==2))
	{	//开始钻孔
	    if(BackDelay < dwTickCount)
	    {
    	    if(SystemMode == 1)
    	    {
                OverLcttmp = TempDist.XOrignDist + depth;
                MoveAction_Pulse(Y_AXIS,OverLcttmp,SpeedParam->DrillSpeed);
            }
            else
            {
                bDrillValve = 1;
            }
            cDrillRunStep=3;
	    }
	}
	else if((cDrillRunStep==3))
    {
        if(SystemMode == 1)
        {
            if(!Y_DRV)//(!X_DRV&&!Y_DRV)
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
    else if((cDrillRunStep == 4)&&(DrillRunStepDelay < dwTickCount))
    {
        if(SystemMode == 1)
        {
            OverLcttmp =0;
            MoveAction_Pulse(Y_AXIS,OverLcttmp,SpeedParam->DrillSpeed);
        }
        else
        {
            bDrillValve = 0;
            qigangbackdelay = 10UL * CustomPlug->QiGangReBackDelay;
            qigangbackdelay = dwTickCount + qigangbackdelay;
        }
        cDrillRunStep=5;
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
            // 这里是处理磁性气缸后退没到位而采取的延时
            if(bQiGangBackSgn)
            {
                if(qigangbackdelay < dwTickCount)
                    cDrillRunStep=0;
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
        {// 报警
            if(BackDelay < dwTickCount)
            {
                AlarmNum = 15;
                bAlarmFlag = 1;
            } 
        }
    }
}

void StopKeyPro(void)
{
	bPress1_Vavle=0;	
	bPress2_Vavle	=0;
	bPress3_Vavle	=0;
	bDrillValve=0;        	
	bCuiQi =0;       		
	bDrillMotor =0;       
    M43 = 0;
    M38 = 0;
    M39 = 0;
    //M05 = 0;
    //bStop = 0;
    bRBOrignXY = 0;
    bXRst = 0;
    bYRst = 0;
    bResume = 0;
    bRunning = 0;
    bRunning1 = 0;
    bRunning2 = 0;
    bRunning3 = 0;
    cRunStep = 0;
    AlarmNum = 0;
    ResetPeripheral();
    cDrillRunStep = 0;
    cScanDrillRunStep = 0;
    cDrillStep = 0;
    if (X_DRV) LMV_Dec_Stop(X_AXIS);
    if (Y_DRV) LMV_Dec_Stop(Y_AXIS);
}

void StopKeyPro2(void)
{
	bPress1_Vavle=0;	
	bPress2_Vavle	=0;
	bPress3_Vavle	=0;
	bDrillValve=0;        	
	bCuiQi =0;       		
	bDrillMotor =0;       
    M43 = 0;
    M38 = 0;
    M39 = 0;
    bAlarmFlag = 0;
    //M05 = 0;
    //bStop = 0;
    bRBOrignXY = 0;
    bXRst = 0;
    bYRst = 0;
    bResume = 0;
    bRunning = 0;
    bRunning1 = 0;
    bRunning2 = 0;
    bRunning3 = 0;
    cRunStep = 0;
    //AlarmNum = 0;
    ResetPeripheral();
    cDrillRunStep = 0;
    cScanDrillRunStep = 0;
    cDrillStep = 0;
    if (X_DRV) LMV_Dec_Stop(X_AXIS);
    if (Y_DRV) LMV_Dec_Stop(Y_AXIS);
}
