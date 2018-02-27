#include "Global.h"

double XGearRatio,YGearRatio;
unsigned short ZGearRatio,UGearRatio;	//电子此轮比

WORD_BITS cAxisRunState;

unsigned char cResumeStep;  //复归步骤
unsigned char cChaXiaoBackup = 0;  //
unsigned char OnceOnpowerFLG,NOAirFLG;  //第一次上电标志
unsigned char RebackXYFLG;
unsigned short ServorDlay;
unsigned short   RstPphlDlay; //复位外设延时
unsigned long    Secondtime;
unsigned long    Mintuetime;
unsigned long cLimitShowCount = 0;
unsigned long Press1time;
unsigned long Press2time;
unsigned long Press3time;

CurSystemData *CurBoardData;
//long Xdistance;
//long Ydistance;
//long XResume;
//long YResume;
//short YAxisPLimit;
//short YAxisNLimit;
//unsigned long    PrevMintuetime;
//unsigned short   PrevShouqian1;
//unsigned short   PrevShouqian2;
//unsigned short   Shouqian1Flag;
//unsigned long    ComeInPlaceCount = 0;
//unsigned short   PreWorkMode;
//unsigned short   PreDrillkMode;
//unsigned short   PreDrillDist;
//unsigned short    PreMenuSel;

void ReadRobotCurrent(void);
void KeyBoardProc(void);
void RebackXYOrign(void);
void ManualAction(void);
//void ParamSaveProc(void);
void ResumeLocation(void);
//void SaveResumeLct(void);
void OncePowerResume(void);
void AlarmProtect(void);
void SaveAndLoadParams(void);
//void  MotorDrillSel(void);
void OutSideStop(void);
/************************************************/
/*
动作运行

*/
/************************************************/
void Action(void) 
{ 
    OncePowerResume();
    //ParamSaveProc();
    ReadRobotCurrent();
    KeyBoardProc();
    RebackXYOrign();
    ManualAction();
    AutoRun();
    ResumeLocation();
    AlarmProtect();
    SaveAndLoadParams();
    OutSideStop();
}

void Save16BitDate(unsigned short data,unsigned short addr)
{
    unsigned char byteh = 0;
    unsigned char bytel = 0;
    unsigned short temp = 0;
    
    byteh = (data >> 8)& 0x0ff;
    bytel = data & 0x0ff;
    temp = addr * 2;

    Write24C04(temp,bytel);
    Write24C04((temp + 1),byteh);
}

void Save32BitDate(unsigned long data,unsigned short addr)
{
    unsigned char bytehh = 0;
    unsigned char bytehl = 0;
    unsigned char bytelh = 0;
    unsigned char bytell = 0;
    unsigned short temp = 0;
    
    bytehh = (data >> 24)& 0x0ff;
    bytehl = (data >> 16)& 0x0ff;
    bytelh = (data >> 8)& 0x0ff;
    bytell = data & 0x0ff;
    temp = addr * 2;

    Write24C04(temp,bytell);
    Write24C04((temp + 1),bytelh);
    Write24C04((temp + 2),bytehl);
    Write24C04((temp + 3),bytehh);
}


/************************************************/
/*
当前两轴状态

*/
/************************************************/
void ReadRobotCurrent(void) 
{
    long datatmp;
    
    ReadAxisStatus();
    
    datatmp=TimeParam->cyclelength;
    //CurAxisDistance->XDistance = CaoMode0Data.step;
    CurAxisDistance->YDistance=tYAxisStepper.cRealPosi*TimeParam->Xcyclelength/TimeParam->XPulsTotal ;
	CurAxisDistance->XDistance =(tXAxisStepper.cRealPosi /100)*datatmp / 100;
	//CurAxisDistance->Ydistance = tempdata;
	if(WoodLength > 0)
	{
	    datatmp = WoodLength -TempDist.MaxHoleR;
        CurAxisDistance->WoodLength = datatmp * TimeParam->cyclelength / 10000;
	}
    if(CheckHoleData.R > 0)
	    CurAxisDistance->DrillRadius=CheckHoleData.R * TimeParam->cyclelength / 10000;
	CurAxisDistance->DrillNum=currentHoleNum;
	//CurAxisDistance->EndLength=LastDrillDist * TimeParam->cyclelength / 10000;

    if(CurWoodLength > 0)
    {
        if(M106)
        {
            datatmp = tXAxisStepper.cRealPosi - CurWoodLength;
            //CurBoardPos = (datatmp/100)*TimeParam->cyclelength / 100;
            //CurBoardPos  = CurBoardPos - TimeParam->DWDrillDist;
        }
        else if(M108)
        {
            datatmp = tXAxisStepper.cRealPosi - CurWoodLength;
            //CurBoardPos = (datatmp / 100)*TimeParam->cyclelength / 100;
            //CurBoardPos  = CurBoardPos - TimeParam->DrillOffset_Cao;
        }
        else
        {
            datatmp = tXAxisStepper.cRealPosi - CurWoodLength - TempDist.DrillDist;
            //CurBoardPos = (datatmp/100)*TimeParam->cyclelength / 100;
        }
    }
    cMidleCoil[10].Val = cInputVal1.Val;
    cMidleCoil[15].Val = cOutputVal.Val;

}
/************************************************/
/*
按键处理

*/
/************************************************/
// 外部压料输入
void KeyPressIn(void)
{
    if(bPress1_IN && !bPrePress1_IN && (Press1time< dwTickCount))
    {
		Press1time = dwTickCount + CurBoardData->DrillStsrtDelay * 10;
		if(!M27)
		{
			if(!bPress1_Vavle)
			{
			    bPress1_Vavle=1;
			    bRunning1=1;
			}
			if(bPress1_Vavle && !bRunning1)
			{
			    bPress1_Vavle=0;
			}
		}
		else
		{
			if(!bPress1_Vavle)
			{
				bPress1_Vavle=1;
				bPress2_Vavle=1;
				bPress3_Vavle=1;
				bRunning1=1;    				
			}
			if(bPress1_Vavle&&!bRunning1)
			{
				bPress1_Vavle=0;
				bPress2_Vavle=0;
				bPress3_Vavle=0;
			}
		}
    }
    if(bPress2_IN && !bPrePress2_IN 
        && (Press2time< dwTickCount))
    {
		Press2time = dwTickCount + CurBoardData->DrillStsrtDelay * 10;
		if(!M27)
		{
    		if(!bPress2_Vavle){bPress2_Vavle=1;bRunning2=1;}
    		if(bPress2_Vavle&&!bRunning2){bPress2_Vavle=0;}
		}
		else
		{
			if(!bPress1_Vavle)
			{
				bPress1_Vavle=1;
				bPress2_Vavle=1;
				bPress3_Vavle=1;
				bRunning1=1;    				
			}
			if(bPress1_Vavle&&!bRunning1)
			{
				bPress1_Vavle=0;
				bPress2_Vavle=0;
				bPress3_Vavle=0;
			}
		}
    }
    if(bPress3_IN&&!bPrePress3_IN&&(Press3time< dwTickCount))
    {
        Press3time = dwTickCount + CurBoardData->DrillStsrtDelay * 10;
        if(!M27)
		{
    		if(!bPress3_Vavle){bPress3_Vavle=1;bRunning3=1;}
    		if(bPress3_Vavle&&!bRunning3){bPress3_Vavle=0;}
		}
    	else
		{
			if(!bPress1_Vavle)
			{
				bPress1_Vavle=1;
				bPress2_Vavle=1;
				bPress3_Vavle=1;
				bRunning1=1;    				
			}
			if(bPress1_Vavle&&!bRunning1)
			{
				bPress1_Vavle=0;
				bPress2_Vavle=0;
				bPress3_Vavle=0;
			}
		}
    }
}

#define SETBIT()

void CustomPlugSelect(void)
{
    unsigned char i = 0;
    unsigned short temp1 = 0;
    unsigned short temp2 = 0;
    unsigned short temp3 = 0;

    if(M112)
    {
        M112 = 0;
        SpeedParam->ChaXiaoMode = PlugTWNei;
        bNOChaXiao = 0;
        bTWNei = 1;
        bTWWai = 0;
        bCustomPlugAll = 0;
        bCustomPlugafterHole = 0;
        bCustomPlugBeforHole = 0;
        bCustomPlugState = 0;
        bPlugforOneHole = 0;
        bPlugforTwoHole = 0;
        //CustomPlug->OneHolePlug = 0;
    }
    else if(M114)
    {
        M114 = 0;
        SpeedParam->ChaXiaoMode = PlugTWWai;
        bNOChaXiao = 0;
        bTWNei = 0;
        bTWWai = 1;
        bCustomPlugAll = 0;
        bCustomPlugafterHole = 0;
        bCustomPlugBeforHole = 0;
        bCustomPlugState = 0;
        bPlugforOneHole = 0;
        bPlugforTwoHole = 0;
        //CustomPlug->OneHolePlug = 0;
    }
    else if (M92)
    {
        M92 = 0;
        SpeedParam->ChaXiaoMode = 0;
        bNOChaXiao = 1;
        bTWNei = 0;
        bTWWai = 0;        
        bCustomPlugAll = 0;
        bCustomPlugafterHole = 0;
        bCustomPlugBeforHole = 0;
        bCustomPlugState = 0;
        bPlugforOneHole = 0;
        bPlugforTwoHole = 0;
        //CustomPlug->OneHolePlug = 0;
    } 
    else if(M128)
    {
        M128 = 0;
        bCustomPlugState = 1;//~bCustomPlugState;
        bNOChaXiao = 0;
        bTWNei = 0;
        bTWWai = 0;        
        bCustomPlugAll = 0;
        bCustomPlugafterHole = 0;
        bCustomPlugBeforHole = 0;
        SpeedParam->ChaXiaoMode = PlugCustom;
        bPlugforOneHole = 0;
        bPlugforTwoHole = 0;
        //CustomPlug->OneHolePlug = 0;
    }
    else if(M126)
    {
        M126 = 0;
        bCustomPlugAll = 1;
        bCustomPlugafterHole = 0;
        bCustomPlugBeforHole = 0;
        bNOChaXiao = 0;
        bTWNei = 0;
        bTWWai = 0;
        bCustomPlugState = 0;
        SpeedParam->ChaXiaoMode = PlugAll;
        bPlugforOneHole = 0;
        bPlugforTwoHole = 0;
        //CustomPlug->OneHolePlug = 0;
    }
    else if(M124)
    {
        M124 = 0;
        bCustomPlugafterHole = 1;
        bCustomPlugAll = 0;
        bCustomPlugBeforHole = 0;
        bNOChaXiao = 0;
        bTWNei = 0;
        bTWWai = 0;
        bCustomPlugState = 0;
        SpeedParam->ChaXiaoMode = PlugafterHole;
        bPlugforOneHole = 0;
        bPlugforTwoHole = 0;
        //CustomPlug->OneHolePlug = 0;
    }
    else if(M122)
    {
        M122 = 0;
        bCustomPlugBeforHole = 1;
        bCustomPlugAll = 0;
        bCustomPlugafterHole = 0;
        bNOChaXiao = 0;
        bTWNei = 0;
        bTWWai = 0;
        bCustomPlugState = 0;
        SpeedParam->ChaXiaoMode = PlugBeforHole;
        bPlugforOneHole = 0;
        bPlugforTwoHole = 0;
        //CustomPlug->OneHolePlug = 0;
    }
    //else if(CustomPlug->OneHolePlug == 1)
    else if(bPlugforOneHoleST)
    {
        bPlugforOneHoleST = 0;
        bCustomPlugBeforHole = 0;
        bCustomPlugAll = 0;
        bCustomPlugafterHole = 0;
        bNOChaXiao = 0;
        bTWNei = 0;
        bTWWai = 0;
        bCustomPlugState = 0;
        bPlugforOneHole = 1;
        bPlugforTwoHole = 0;
        SpeedParam->ChaXiaoMode = PlugforOneHole;
    }
    else if(bPlugforTwoHoleST)
    {
        bPlugforTwoHoleST = 0;
        bCustomPlugBeforHole = 0;
        bCustomPlugAll = 0;
        bCustomPlugafterHole = 0;
        bNOChaXiao = 0;
        bTWNei = 0;
        bTWWai = 0;
        bCustomPlugState = 0;
        bPlugforOneHole = 0;
        bPlugforTwoHole = 1;
        SpeedParam->ChaXiaoMode = PlugforTwoHole;
    }
    
    if(bNOChaXiao)
    {
        SpeedParam->ChaXiaoMode = 0;
        bTWNei = 0;
        bTWWai = 0;        
        bCustomPlugAll = 0;
        bCustomPlugafterHole = 0;
        bCustomPlugBeforHole = 0;
        bCustomPlugState = 0;
        bPlugforOneHole = 0;
        bPlugforTwoHole = 0;
    }
    
    if(cChaXiaoBackup != SpeedParam->ChaXiaoMode)
    {
        cChaXiaoBackup = SpeedParam->ChaXiaoMode;
        Save16BitDate(SpeedParam->ChaXiaoMode,(40));
    }

    if(bCustomPlugState)
    {
        for(i = 0;i < 16;i++)
        {
            temp1 = (1 << i);
            temp2 = cMidleCoil[12].Val & temp1;
            if(temp2 > 0)
            {
                cMidleCoil[12].Val &= ~temp1; 
                if((cMidleCoil[3].Val & temp2) > 0)
                    cMidleCoil[3].Val &= ~temp1;
                else
                    cMidleCoil[3].Val |= temp2;  
            }

            temp3 = cMidleCoil[13].Val & temp1;
            if(temp3 > 0)
            {
                cMidleCoil[13].Val &= ~temp1; 
                if((cMidleCoil[4].Val & temp3) > 0)
                    cMidleCoil[4].Val &= ~temp1;
                else
                    cMidleCoil[4].Val |= temp3;
            }
        }

        if(bChaXiaoSave)
        {
            bChaXiaoSave = 0;
            //if(cMidleCoil[3].Val != cDataArea[150])
            {
                cDataArea[150] = cMidleCoil[3].Val;
                Save16BitDate(cDataArea[150],(150));
            }

            //if(cMidleCoil[4].Val != cDataArea[151])
            {
                cDataArea[151] = cMidleCoil[4].Val;
                Save16BitDate(cDataArea[151],(151));
            }
        }
    }
}

void KeyBoardProc(void) 
{
    if (( bRunning1||bRunning2||bRunning3) 
        && (!bRunning) && (!bRBOrignXY) 
        && (!bResume)  && (!bStop) && !(X_DRV || Y_DRV)
        && (RebackXYFLG==0)) 
    {  
        bRunning = 1;
        cRunStep = 1;        
        ResetPeripheral();
		NoworkStopdelay = 100*TimeParam->NoWorktime+1000;
    }

    if ( M43 ||(bStop))// && !bOldStop)) 
    {
        StopKeyPro();
    }
    if(bAlarmFlag)
    {
        StopKeyPro2();
    }
    if ((bRest) && (NOAirFLG == 0) && !bRunning) 
    { //复位
        bRest = 0;
        bRunning = 0;
        AlarmNum = 0;
        bResume = 1;
        cResumeStep = 1;
    }

    if (bRunning)
    {
        if(AlarmNum == 0)
            AlarmNum = 10;
    } 
    else 
    {
        if (AlarmNum == 10) 
            AlarmNum = 0;
    }    
    if (bResume)
    {
        AlarmNum = 7;
    }
    else
    {
        if (AlarmNum == 7) 
            AlarmNum = 0;
    }

    if(AlarmNum == 0 || AlarmNum == 10)
        KeyPressIn();
    
    if(ScanMode > 1)
    {
        ScanMode = 0;
    }
    if(ScanMode)
        bSelModeST=1;
    else 
        bSelModeST=0;
    //if(!bRunning)
    {
        if(SystemMode >= 1)
        {
            bSystemMode = 1;
            bSelServoST=1;
        }
        else
        {
            bSystemMode = 0;
            bSelServoST=0;
        }
/*
        if(M10)
        {
            M10 = 0;
            bTongCao = ~bTongCao;
        }
        */
        if(M119)
        {
            M119 = 0;
            bTongcao = 0;
            bDanCao = 0;
            bCaoMode = 1;
            TimeParam->CaoMode = DuanCao;
            //bCaoMode = ~bCaoMode;
        }
        else if(bTongcaoST)
        {
            bTongcaoST = 0;
            bTongcao = 1;
            bDanCao = 0;
            bCaoMode = 0;
            TimeParam->CaoMode = TongCao;
        }
        else if (bDanCaoST)
        {
            bDanCaoST = 0;
            bTongcao = 0;
            bDanCao = 1;
            bCaoMode = 0;
            TimeParam->CaoMode = DanCao;
        }

        if(TimeParam->CaoMode == DuanCao)
        {
            bTongcao = 0;
            bDanCao = 0;
            bCaoMode = 1;
        }
        else if(TimeParam->CaoMode == TongCao)
        {
            bTongcao = 1;
            bDanCao = 0;
            bCaoMode = 0;
        }
        else if (TimeParam->CaoMode == DanCao)
        {
            bTongcao = 0;
            bDanCao = 1;
            bCaoMode = 0;
        }
        
        if(bLocationHole)
        {
            Hole_MuBanSelect();
        }
        else if(bLocationCao)
        {
            Cao_MuBanSelect();
        }
        else//if(SpeedParam->Scan_DrillOffsetMode)
        {
            Scan_MuBanSelect();
        }    
    }

    if(bRunning1 || bRunning2 || bRunning3)
    {
        bKeyEnable = 1;
    }
    else
    {
        if(bClearH)
        {
            short i = 0;
            for(i = 0;i < 24;i++)
            {
                cDataArea[180 + i * 2] = 0;
                cDataArea[180 + i * 2 + 1] = 0;
            }
            bClearH = 0;
        }
        if(bClearC)
        {
            short i = 0;
            for(i = 0;i < 10;i++)
            {
                cDataArea[100 + i * 2] = 0;
                cDataArea[100 + i * 2 + 1] = 0;
                cDataArea[120 + i * 2] = 0;
                cDataArea[120 + i * 2 + 1] = 0;
                cDataArea[140 + i] = 0;
            }
            bClearC = 0;
        }    
        if(bLocationHole|M108)
        {
        	//M109=1;
            //bTWChaXiao = 0;
            //bNOChaXiao = 1;
            //bTongXiangNeiChaXiao = 0;
            //bTongXiangWaiChaXiao = 0;

            SpeedParam->ChaXiaoMode = 0;
            bNOChaXiao = 1;
            bTWNei = 0;
            bTWWai = 0;        
            bCustomPlugAll = 0;
            bCustomPlugafterHole = 0;
            bCustomPlugBeforHole = 0;
            bCustomPlugState = 0;
            bPlugforOneHole = 0;
            bPlugforTwoHole = 0;
            //CustomPlug->OneHolePlug = 0;
        }
        if(!bNOChaXiao && SystemMode > 0)
        {
            bDisplayChaXiaoDepth = 1;
        }
        else
        {
            bDisplayChaXiaoDepth = 0;
        }
        if(!bNOChaXiao)
        {
            if(M110)
            {
                M110 = 0;
                bMiddleChaXiao = ~bMiddleChaXiao;
            }
        }
        bKeyEnable = 0;

        CustomPlugSelect();
    }
}

void QiGangReBack(void)
{
    if(bQiGangComeSgn || bDrillValve)
    {
        bDrillValve = 0;
        bDrillMotor = 0;
    }
    
    if(bQiGangBackSgn )
    {
        bXRst = 0;
        X_DRV = 0;
    }
}
/************************************************/
/*
XY轴回零

*/
/************************************************/
void RebackXYOrign(void)
{
    if (bRBOrignXY ) 
    {
        if(bSystemMode)
        {
            RebackYaxisOrign();
        }
        else
        {
            QiGangReBack();
        }
        if (!bXRst && !(X_DRV))
        {
            RebackXaxisOrign();
        }
        if(!bXRst&&!bYRst&&!X_DRV&&!Y_DRV)
            bRBOrignXY=0;
    }
}
/************************************************/
/*
手动操作

*/
/************************************************/
void ManualAction(void) 
{
    if ((!bRunning) && (!bRBOrignXY) && (!bResume) && !bStop)
    {
        if(bSystemMode)
        {
            ManualProc_YAxis();
        }
        
        ManualProc_XAxis();
        
        ManualProc_IO();

    }
}

/************************************************/
/*
参数保存

*/
/************************************************/
//void ParamSaveProc(void) {



//}
/************************************************/
/*
复归

*/
/************************************************/
void ResumeLocation(void) 
{
    if (bResume && (NOAirFLG == 0)) 
    {

        if (cResumeStep == 1)
        {
            LMV_Dec_Stop(Y_AXIS);
            ResetPeripheral();
            cResumeStep = 2;
        } 
    	else if ((cResumeStep == 2) && (RstPphlDlay == 0)) 
        {
            bRBOrignXY = 1;
            bXRst = 1;
            bYRst = 1;
            tYAxisStepper.cRstStep=1;
            tXAxisStepper.cRstStep=1;
            cResumeStep = 4;
        } 
	    else if (cResumeStep == 3)
        {
            cResumeStep = 4;
        } 
	    else if ((cResumeStep == 4) &&!bRBOrignXY)
        {
            OnceOnpowerFLG = 0;
            cResumeStep = 0;
            bResume = 0;
            RebackXYFLG=0;

        }
    }
}
/************************************************/
/*
保存复归

*/
/************************************************/
//void SaveResumeLct(void) {

//}
/************************************************/
/*
上电较刀

*/
/************************************************/
void OncePowerResume(void)
{

    if (RebackXYFLG) 
    {
        AlarmNum = 1;

    } else 
    {
        if (AlarmNum == 1) AlarmNum = 0;
    }
	
}
/************************************************/
/*
告警保护

*/
/************************************************/
//static unsigned long drillComeAlarmTime = 0;
//static unsigned long drillBackAlarmTime = 0;
typedef struct
{
    unsigned long Alarm3;
    unsigned long Alarm4;
    unsigned long Alarm5;
    unsigned long Alarm6;
    unsigned long Alarm12;
}AlarmTime_str;
void AlarmProtect(void)
{
	unsigned char flag = 0;
    
    static AlarmTime_str AlarmTime[2];
    
    if(!bSystemMode)
    {
        if (bDrillValve != bQiGangComeSgn) 
        {
            if (AlarmTime[0].Alarm5 < dwTickCount)
            {
                AlarmTime[1].Alarm5 = dwTickCount + 5000;
                AlarmNum = 5;
            }
        }
        else 
        {
            AlarmTime[0].Alarm5 = dwTickCount + 5000; 
            if (AlarmNum == 5) 
            {
                if (AlarmTime[1].Alarm5 < dwTickCount)
                    AlarmNum = 0;
            }
        }

        if (bDrillValve ==  bQiGangBackSgn) 
        {
            if (AlarmTime[0].Alarm6 < dwTickCount)
            {
                AlarmTime[1].Alarm6 = dwTickCount + 5000;
                AlarmNum = 6;
            }
        }
        else 
        {
            AlarmTime[0].Alarm6 = dwTickCount + 5000;
            if (AlarmNum == 6) 
            {
                if (AlarmTime[1].Alarm6 < dwTickCount)
                    AlarmNum = 0;
            }  
        }
    }
    
    if (bStop) 
    {
        AlarmNum = 9;
    }
    else if (AlarmNum == 9) 
    {
        AlarmNum = 0;
    }
    if((!bDrillMotorAlarm && (cDataArea[12]==1))|| 
        (bDrillMotorAlarm&&(cDataArea[12]==0)))
    {
    		AlarmNum = 4;
    		AlarmTime[1].Alarm4 = dwTickCount + 5000;
    		if(bRunning)M43=1;
    }
    else if (AlarmNum == 4) 
    {
        if(dwTickCount > AlarmTime[1].Alarm4)
            AlarmNum = 0;
    }
     if( !bServoAlarm&&(cDataArea[10]==1)|| bServoAlarm&&(cDataArea[10]==0))
    {
		AlarmNum = 3;
		RebackXYFLG= 1;
		AlarmTime[1].Alarm3 = dwTickCount + 5000;
		if(bRunning)M43=1;
    }
    else if (AlarmNum == 3) 
    {
        if(dwTickCount > AlarmTime[1].Alarm3)
            AlarmNum = 0;
    }
    if(bSystemMode)
    {
        if( ! bYServoAlarm&&(cDataArea[11]==1)|| bYServoAlarm&&(cDataArea[11]==0))
        {
    		AlarmNum = 12;
    		RebackXYFLG = 1;
    		AlarmTime[1].Alarm12 = dwTickCount + 5000;
    		if(bRunning)M43=1;
        }
        else if (AlarmNum == 12) 
        {
            if(dwTickCount > AlarmTime[1].Alarm12)
                AlarmNum = 0;
        }
    }
    else
    {
        if (AlarmNum == 12) 
        {
            AlarmNum = 0;
        }
    }
    //MV_Set_Limit_P(Y_AXIS,10000UL * (TimeParam->MaxLength) / TimeParam->cyclelength);	
    MV_Set_Limit_P(X_AXIS,TempDist.MaxLength);
    if(X_DRV && !bXDirction && 
        ((!bLimitP_X && (tXAxisStepper.cRealPosi > tXAxisStepper.cMaxLen))|| 
            X_LIMIT_P))
	{		
		bLimitP_X = 1;
		LMV_Dec_Stop(X_AXIS);
		bRunning=0;
		//M43=1;
		bAlarmFlag = 1;
		cDrillStep=0;
		cRunStep=0;
		cScanDrillRunStep = 0;
		AlarmNum=2;
	}
	if(X_DRV && bXDirction && (tXAxisStepper.cRealPosi < tXAxisStepper.cMaxLen))
	{
		bLimitP_X = 0;
		if(AlarmNum==2)
		    AlarmNum=0;
	}
	/*if(Y_DRV && bYDirction && Y_SWITCH_ZERO && bRunning)
	{
		LMV_Dec_Stop(Y_AXIS);
		bRunning=0;
		M43=1;
		cDrillStep=0;
		cRunStep=0;
		cScanDrillRunStep = 0;
		AlarmNum=2;
	}
	else
	{
        if(AlarmNum == 2)
            AlarmNum = 0;
	}*/
	//
	if(bSystemMode)
	{
    	//datatmp=TimeParam->XMaxLength;
    	//datatmp=datatmp*TimeParam->XPulsTotal/TimeParam->Xcyclelength;	
    	//MV_Set_Limit_P(X_AXIS,datatmp);	
	    MV_Set_Limit_P(Y_AXIS,TempDist.XMaxLength);	
    	if(Y_DRV && !bYDirction && !bLimitP_Y && 
    	    (tYAxisStepper.cRealPosi > tYAxisStepper.cMaxLen))
    	{		
    		bLimitP_Y = 1;
    		LMV_Dec_Stop(Y_AXIS);
    		bRunning=0;
    		//M43=1;
    		bAlarmFlag = 1;
    		cDrillStep=0;
    		cRunStep=0;
    		AlarmNum=13;
    	}
    	if(Y_DRV && bYDirction && (tYAxisStepper.cRealPosi < tYAxisStepper.cMaxLen))
    	{
    		bLimitP_Y = 0;
    		if(AlarmNum==13)
    		    AlarmNum=0;
    	}
    }
    else
    {
        if(AlarmNum == 13)
            AlarmNum=0;
    }
	if(AlarmNum == 10 || AlarmNum == 0 || AlarmNum == 7 
	    || AlarmNum == 11 || AlarmNum == 1)
	{
        flag = 0;// 
	}
	else
	{
        flag = 1;
	}

	if(flag)
	{
        Y00 = 0;
        Y01 = 0;
        Y02 = 0;
        Y03 = 0;
        Y04 = 0;
        Y05 = 0;
        Y06 = 1;
        LMV_Dec_Stop(X_AXIS);
	}
	else
	{
        Y06 = 0;
	}
}
/************************************************/
/*
保存参数集，读取参数集

*/
/************************************************/

void SaveAndLoadParams(void) 
{	
    unsigned short buffertmp[2048];
    unsigned char buffer[512];
    unsigned short i = 0,j = 0;//k = 0;
    unsigned long addr = 0;
    
    if (!bRunning) 
    {
        if(LocationAddrArray > 99)
            LocationAddrArray = 99;
        if (bWriteParam) 
        {
            //if(LocationAddrArray > 99)
                
            addr = 4096 * LocationAddrArray;
            j = 256;
    		memcpy(buffertmp, &cDataArea[100], j);

    		EraseSector(addr);		
    		ProgramPage(addr, buffertmp);
    		bWriteParam = 0;
    	}
        if(bReadParam)
    	{		
    	    addr = 4096 * LocationAddrArray;
            j = 256;
    		
    		ReadPage (addr,buffertmp);
    		memcpy(&cDataArea[100],buffertmp,j);
            memcpy(buffer,buffertmp,j);
    		for(i=0;i<j;i++)
    		{
    			Write24C04(200+i, buffer[i]);
    		}
    		bReadParam = 0;
    	}
		if(bWriteFactory)
        {
            addr = 4096 * 100;
            j = 512;
            memcpy(buffertmp, &cDataArea[0], j);

    		EraseSector(100*4096);
    		ProgramPage(100*4096, buffertmp);
            bWriteFactory = 0;            
        }
        if(bReadFactory)
        {
            addr = 4096 * 100;
            j = 512;
            
    		ReadPage (addr,buffertmp);
    		memcpy(&cDataArea[0], buffertmp, j);
            memcpy(buffer,buffertmp,j);
    		for(i=0;i<j;i++)
    		{
    			Write24C04(i, buffer[i]);
    		}
    		bReadFactory = 0;
        }
	}	
}

/************************************************/
/*
在停止位时，外设复位


*/
/************************************************/
void ResetPeripheral(void) {

    RstPphlDlay = 500;
}
/************************************************/
/*
钻排选择


*/
/************************************************/
//void  MotorDrillSel(void) {

//}

/************************************************/
/*
在停止位时，轴停止


*/
/************************************************/
void StopAxis(void)
{
    /*
	if(AxisNum>0)
	{
		MV_Dec_Stop(X_AXIS);
	}
	if(AxisNum>1)
	{
		MV_Dec_Stop(Y_AXIS);
	}
	if(AxisNum>2)
	{
		MV_Dec_Stop(Z_AXIS);
	}
	if(AxisNum>3)
	{
		MV_Dec_Stop(U_AXIS);
	}
    */
    LMV_Dec_Stop(Y_AXIS);
}
/************************************************/
/*
计算齿轮比及速度比率
*/
/************************************************/
void CalGearRatio(void)
{	
	//unsigned long speedratiotmp,gearratiotmp;
    /*
	if((MotroParam->XMotorSpeed==0)||(MotroParam->XMotorSpeed>5000))
		MotroParam->XMotorSpeed=1200;
	if((MotroParam->YMotorSpeed==0)||(MotroParam->YMotorSpeed>5000))
		MotroParam->YMotorSpeed=1200;
	if((MotroParam->ZMotorSpeed==0)||(MotroParam->ZMotorSpeed>5000))
		MotroParam->ZMotorSpeed=1200;
	if((MotroParam->UMotorSpeed==0)||(MotroParam->UMotorSpeed>5000))
		MotroParam->UMotorSpeed=1200;
	
	if(MotroParam->XCirclingPulse==0)
		MotroParam->XCirclingPulse=10000;
	if(MotroParam->YCirclingPulse==0)
		MotroParam->YCirclingPulse=10000;
	if(MotroParam->ZCirclingPulse==0)
		MotroParam->ZCirclingPulse=10000;
	if(MotroParam->UCirclingPulse==0)
		MotroParam->UCirclingPulse=10000;
	
	if(MotroParam->XMoveDist==0)
		MotroParam->XMoveDist=500;
	if(MotroParam->YMoveDist==0)
		MotroParam->YMoveDist=500;
	if(MotroParam->ZMoveDist==0)
		MotroParam->ZMoveDist=500;
	if(MotroParam->UMoveDist==0)
		MotroParam->UMoveDist=500;
      */
    //MotroParam->YMotorSpeed=2000;
    //MotroParam->YCirclingPulse=10000;
    //MotroParam->YMoveDist = 1400;
    /*
	speedratiotmp=(MotroParam->XMotorSpeed/60);
	speedratiotmp*=MotroParam->XCirclingPulse;
	speedratiotmp/=1000;
	XSpeedRatio=speedratiotmp;

	speedratiotmp=(MotroParam->YMotorSpeed/60);
	speedratiotmp*=MotroParam->YCirclingPulse;
	speedratiotmp/=1000;
	YSpeedRatio=speedratiotmp;

	speedratiotmp=(MotroParam->ZMotorSpeed/60);
	speedratiotmp*=MotroParam->ZCirclingPulse;
	speedratiotmp/=1000;
	ZSpeedRatio=speedratiotmp;

	speedratiotmp=(MotroParam->UMotorSpeed/60);
	speedratiotmp*=MotroParam->UCirclingPulse;
	speedratiotmp/=1000;
	USpeedRatio=speedratiotmp;	
	  */
    //speedratiotmp=(MotroParam->YMotorSpeed/60);
	//speedratiotmp*=MotroParam->YCirclingPulse;
	//speedratiotmp/=1000;
//	YSpeedRatio=speedratiotmp;
	//if(XSpeedRatio==0)XSpeedRatio=100;
//	if(YSpeedRatio==0)YSpeedRatio=100;	
	  /*
	gearratiotmp=MotroParam->XMoveDist;
	gearratiotmp=MotroParam->XCirclingPulse/gearratiotmp;
	XGearRatio=gearratiotmp;
	    
	gearratiotmp=MotroParam->YMoveDist;
	gearratiotmp=MotroParam->YCirclingPulse/gearratiotmp;
	YGearRatio=gearratiotmp;
	*/
	//if(XGearRatio==0)XGearRatio=10;	
	//if(YGearRatio==0)YGearRatio=10;	
}
/************************************************/
/*
上电使能伺服驱动器

*/
/************************************************/
void ServoInit(void)
{	
#ifdef BOARD_12007
	MTU10_PWMInit();
	MTU9_PWMInit();
#endif
#ifdef BOARD_14007
	MTU7_PWMInit();// xzhou
	MTU8_PWMInit();// Yzhou
#endif
	ServorDlay=3000;//延时500ms
	while (ServorDlay)ReadInput();

    tYAxisStepper.cAxisNo = Y_AXIS;
    tYAxisStepper.cDestPosi = 0;
    tYAxisStepper.cSetStartSpeed = 0;
    tYAxisStepper.cSetSpeed = 20;
    tYAxisStepper.cSetAccVal = 50;
    tYAxisStepper.cSetDecVal = 50;
    tYAxisStepper.State = 0;
    tYAxisStepper.cAccDecVal = 0;

    tXAxisStepper.cAxisNo = X_AXIS;
    tXAxisStepper.cDestPosi = 0;
    tXAxisStepper.cSetStartSpeed = 0;
    tXAxisStepper.cSetSpeed = 20;
    tXAxisStepper.cSetAccVal = 50;
    tXAxisStepper.cSetDecVal = 50;
    tXAxisStepper.State = 0;
    tXAxisStepper.cAccDecVal = 0;
}
/************************************************/
/*
空间区域停止

*/
/************************************************/
void OutSideStop(void)
{
	if(bRunning)
	{
		if(!M27)
		{
			if(WorkingFlag==0)
			{
				if((CurAxisDistance->YDistance >TimeParam->Work2Dist+20000)&&(cRunStep==2))
				cRunStep=20;
			}
			else if(WorkingFlag==1)
			{
				if((CurAxisDistance->YDistance >TimeParam->Work3Dist+20000)&&(cRunStep==2))
				cRunStep=20;
			}
		} 
	}

}
