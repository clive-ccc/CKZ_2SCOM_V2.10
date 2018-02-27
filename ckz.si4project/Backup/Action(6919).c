#include "Global.h"

double XGearRatio,YGearRatio;
unsigned short XEncodeDivide;	//���Ӵ��ֱ�

WORD_BITS cAxisRunState;

unsigned char cResumeStep;  //���鲽��
unsigned char cChaXiaoBackup = 0;  //
unsigned char _gcXZero = 0,_gcYZero = 0;  //
unsigned char _gcLocalHole = 0,_gcLocalSlot = 0;

unsigned char RebackXYFLG;//��һ���ϵ��־
unsigned short ServorDlay;
unsigned short   RstPphlDlay; //��λ������ʱ

unsigned long Press1time;
unsigned long Press2time;
unsigned long Press3time;

CurSystemData_str *CurBoardData;

void ReadRobotCurrent(void);
void KeyBoardProc(void);
void RebackXYOrign(void);
void ManualAction(void);
void ResumeLocation(void);
void OncePowerResume(void);
void AlarmProtect(void);
void SaveAndLoadParams(void);

#if 1
// X����ݾ������������
long PositionToPluse(unsigned short cAxisNo,long dist)
{
	long PulseNumber = 0; 
	
	//if(dist > 0)
	{
		if(cAxisNo == Y_AXIS)
		{// ���
			PulseNumber = ((double)dist * YGearRatio);
		}
		else if(cAxisNo == X_AXIS)
		{// λ��
			PulseNumber = ((double)dist * XGearRatio);
		}
	}

	if (PulseNumber > 0)
	{
		PulseNumber += 0.5;
	}
	else
	{
		PulseNumber -= 0.5;
	}

	return (long)PulseNumber;
}

// X������������������
long PluseToPosition(unsigned short cAxisNo,long pulse)
{
	long dist = 0; 
	
	//if(pulse > 0)
	{
		if(cAxisNo == Y_AXIS)
		{// ���
			dist = ((double)pulse / YGearRatio);
		}
		else if(cAxisNo == X_AXIS)
		{// λ��
			dist = ((double)pulse / XGearRatio);
		}
	}
	if (dist > 0)
	{
		dist += 0.5;
	}
	else
	{
		dist -= 0.5;
	}

	return (long)dist;
}
#else
long PluseToPosition(char axis, long pulse)
{
	double value = 0.0;
	double GearRatio = 0.0;
   
	if(axis == X_AXIS)
	{
		value = (double)TimeParam->cyclelength;// ��λС��
		value = value / 10;// һ���������붼����λС����
		value = 10000.0 / value;
		XGearRatio = value;// X Y �Ե���
	}
   else if(axis == Y_AXIS)
   {
	  value = (double)TimeParam->Xcyclelength;
	  value = TimeParam->XPulsTotal / value;
	  YGearRatio = value;// X Y �Ե���
   }

   value = (double)pulse;
   value = value / GearRatio;

   if (value > 0)
   {
      value += 0.5;
   }
   else
   {
      value -= 0.5;
   }
   
   return (long)value;
}

long PositionToPluse(char axis, long pos)
{
   	double value = 0.0;
	double GearRatio = 0.0;
   
   if(axis == X_AXIS)
	{
		value = (double)TimeParam->cyclelength;// ��λС��
		value = value / 10;// һ���������붼����λС����
		value = 10000.0 / value;
		GearRatio = value;// X Y �Ե���
	}
   else if(axis == Y_AXIS)
   {
	  value = (double)TimeParam->Xcyclelength;
	  value = TimeParam->XPulsTotal / value;
	  GearRatio = value;// X Y �Ե���
   }

   value = (double)pos;
   value = value * GearRatio;
   if (value > 0)
   {
      value += 0.5;
   }
   else
   {
      value -= 0.5;
   }
   return (long)value;
}

#endif
/************************************************/
/*
��������

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

void ReadIOState(void)
{
    cMidleCoil[10].Val = cInputVal1.Val;
    cMidleCoil[15].Val = cOutputVal.Val;

	_gcXZero = X_ZERO;
	_gcYZero = Y_ZERO;
	_gcLocalHole = M106;
	_gcLocalSlot = M108;
}

/************************************************/
/*
��ǰ����״̬

*/
/************************************************/
void ReadRobotCurrent(void) 
{
    long datatmp;
    
    ReadAxisStatus();
    ReadIOState();
    
	CurAxisDistance->XDistance = PluseToPosition(X_AXIS,tXAxisStepper.cRealPosi);
	//CurAxisDistance->YDistance = ulEncoderCount/2;//cScanDrillRunStep;
	if(SystemMode > 0)
	{
		CurAxisDistance->YDistance = PluseToPosition(Y_AXIS,tYAxisStepper.cRealPosi);
	}
	if(WoodLength > 0)
	{
	    datatmp = WoodLength -TempDist.MaxHoleR;
        CurAxisDistance->WoodLength = PluseToPosition(X_AXIS, datatmp);
	}
    if(CheckHoleData.R > 0)
    {
	    CurAxisDistance->DrillRadius = PluseToPosition(X_AXIS, CheckHoleData.R);
	}
	CurAxisDistance->DrillNum = currentHoleNum;
}
/************************************************/
/*
��������

*/
/************************************************/
// �ⲿѹ������
void KeyPressIn(void)
{
    if(bPress1_IN && !bPrePress1_IN && (Press1time< dwTickCount))
    {
		Press1time = dwTickCount + CurBoardData->DrillStsrtDelay * 10;
		if(!bLongBoard)
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
		if(!bLongBoard)
		{
    		if(!bPress2_Vavle){bPress2_Vavle=1;bRunning2=1;}
    		if(bPress2_Vavle&&!bRunning2){bPress2_Vavle=0;}
		}
		else
		{
			Press1time = Press2time;
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
        if(!bLongBoard)
		{
    		if(!bPress3_Vavle){bPress3_Vavle=1;bRunning3=1;}
    		if(bPress3_Vavle&&!bRunning3){bPress3_Vavle=0;}
		}
    	else
		{
			Press1time = Press3time;
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
// ����ѡ��
void CustomPlugSelect(void)
{
    unsigned char i = 0;
    unsigned short temp1 = 0;
    unsigned short temp2 = 0;
    unsigned short temp3 = 0;

    if(TimeParam->WorkMode != WorkMode_Scan)
    {
    	if(SpeedParam->ChaXiaoMode != 0)
    	{
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
    	}
    }

    if(bTWNei_ST)
    {
        bTWNei_ST = 0;
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
    else if(bTWWai_ST)
    {
        bTWWai_ST = 0;
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
    else if (bNOChaXiao_ST)
    {
        bNOChaXiao_ST = 0;
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
    } 
    else if(bCustomPlugState_ST)
    {
        bCustomPlugState_ST = 0;
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
    else if(bCustomPlugAll_ST)
    {
        bCustomPlugAll_ST = 0;
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
    else if(bCustomPlugafterHole_ST)
    {
        bCustomPlugafterHole_ST = 0;
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
    else if(bCustomPlugBeforHole_ST)
    {
        bCustomPlugBeforHole_ST = 0;
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

    if(SpeedParam->ChaXiaoMode != NoPlug)
    {
        if(bMiddleChaXiao_ST)
        {
            bMiddleChaXiao_ST = 0;
            bMiddleChaXiao = ~bMiddleChaXiao;
        }
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

void WorkModeSelect(void)
{
	if(bScanHole_ST)
	{
		bScanHole_ST = 0;
		bScanHole = 1;
		bLocationHole = 0;
		bLocationCao = 0;
		TimeParam->WorkMode = WorkMode_Scan;
	}
	else if(bLocationCao_ST)
	{
		bLocationCao_ST = 0;
		bScanHole = 0;
		bLocationHole = 0;
		bLocationCao = 1;
		TimeParam->WorkMode = WorkMode_Slot;
	}
	else if(bLocationHole_ST)
	{
		bLocationHole_ST = 0;
		bScanHole = 0;
		bLocationHole = 1;
		bLocationCao = 0;
		TimeParam->WorkMode = WorkMode_Hole;
	}
}

void CaoModeSelect(void)
{
	if(bDuanCao_ST)
	{
		bDuanCao_ST = 0;
		bTongcao = 0;
		bDanCao = 0;
		bDuanCao = 1;
		TimeParam->CaoMode = DuanCao;
	}
	else if(bTongcaoST)
	{
		bTongcaoST = 0;
		bTongcao = 1;
		bDanCao = 0;
		bDuanCao = 0;
		TimeParam->CaoMode = TongCao;
	}
	else if (bDanCaoST)
	{
		bDanCaoST = 0;
		bTongcao = 0;
		bDanCao = 1;
		bDuanCao = 0;
		TimeParam->CaoMode = DanCao;
	}

	if(TimeParam->CaoMode == DuanCao)
    {
        bTongcao = 0;
        bDanCao = 0;
        bDuanCao = 1;
    }
    else if(TimeParam->CaoMode == TongCao)
    {
        bTongcao = 1;
        bDanCao = 0;
        bDuanCao = 0;
    }
    else if (TimeParam->CaoMode == DanCao)
    {
        bTongcao = 0;
        bDanCao = 1;
        bDuanCao = 0;
    }
}

void KeyBoardProc(void) 
{
    if ((bRunning1||bRunning2||bRunning3) 
        && (!(bRunning || bResume || bStop || X_DRV || Y_DRV))
        && (RebackXYFLG==0) &&(cAutoCheckSwitchstep == 0)
        && (CheckSwitchflag == 0)) 
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
    if (bRest && !bRunning) 
    { //��λ
        bRest = 0;
        if((cAutoCheckSwitchstep == 0)&& (CheckSwitchflag == 0))
        {
	        bRunning = 0;
	        AlarmNum = 0;
	        bResume = 1;
	        cResumeStep = 1;
    	}
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
    {
        KeyPressIn();
    }
    CurBoardData->ScanMode1 = 1;
    
    if(!bRunning)
    {
    	WorkModeSelect();
    	if(bSystemMode_ST)
    	{
			bSystemMode_ST = 0;
			bSystemMode = ~bSystemMode;
			bSelServoST = bSystemMode;
			CurBoardData->SystemMode1 = bSystemMode;
			Save16BitDate(CurBoardData->SystemMode1, 13);
    	}
		CaoModeSelect();  
    }

    if(bRunning1 || bRunning2 || bRunning3)
    {
        bKeyEnable = 1;
    }
    else
    {
    	if(bLongBoard_ST)
    	{
    		bLongBoard_ST = 0;
			bLongBoard = ~bLongBoard;
    	}
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

        bKeyEnable = 0;

        CustomPlugSelect();
        if(TimeParam->WorkMode == WorkMode_Hole)
        {
            Hole_MuBanSelect();
        }
        else if(TimeParam->WorkMode == WorkMode_Slot)
        {
            Cao_MuBanSelect();
        }
        else if(TimeParam->WorkMode == WorkMode_Scan)
        {
            Scan_MuBanSelect();
        }  
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
        bYRst = 0;
        Y_DRV = 0;
    }
}
/************************************************/
/*
XY�����

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
        if (!bYRst && !(Y_DRV))
        {
            RebackXaxisOrign();
        }
        if(!bXRst&&!bYRst&&!X_DRV&&!Y_DRV)
            bRBOrignXY=0;
    }
}
/************************************************/
/*
�ֶ�����

*/
/************************************************/
void ManualAction(void) 
{
    if ((!bRunning) && (!bRBOrignXY) && (!bResume) && !bStop)
    {
    	if(cAutoCheckSwitchstep == 0)
    	{
	        if(SystemMode > 0)
	        {
	            ManualProc_YAxis();
	        }
	        
	        ManualProc_XAxis();
	        
	        ManualProc_IO();
		}
		if(RebackXYFLG < 1)
		{
			AutoCheckSwitch();
		}
    }
}

/************************************************/
/*
��������

*/
/************************************************/
//void ParamSaveProc(void) {



//}
/************************************************/
/*
����

*/
/************************************************/
void ResumeLocation(void) 
{
    if (bResume)// && (NOAirFLG == 0)) 
    {

        if (cResumeStep == 1)
        {
            LMV_Dec_Stop(X_AXIS);
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
            cResumeStep = 0;
            bResume = 0;
            RebackXYFLG=0;

        }
    }
}
/************************************************/
/*
���渴��

*/
/************************************************/
//void SaveResumeLct(void) {

//}
/************************************************/
/*
�״��ϵ���Ҫ��λ

*/
/************************************************/
void OncePowerResume(void)
{

    if (RebackXYFLG) 
    {
        AlarmNum = 1;
    } 
    else 
    {
        if (AlarmNum == 1) 
        {
        	AlarmNum = 0;
    	}
    }
	
}
/************************************************/
/*
�澯����

*/
/************************************************/
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
    		if(bRunning)
    			bAlarmFlag = 1;//M43=1;
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
		if(bRunning)
			bAlarmFlag = 1;//M43=1;
    }
    else if (AlarmNum == 3) 
    {
        if(dwTickCount > AlarmTime[1].Alarm3)
            AlarmNum = 0;
    }
    if(bSystemMode)
    {
        if( !bYServoAlarm&&(cDataArea[11]==1)|| bYServoAlarm&&(cDataArea[11]==0))
        {
    		AlarmNum = 12;
    		RebackXYFLG = 1;
    		AlarmTime[1].Alarm12 = dwTickCount + 5000;
    		if(bRunning)
    			bAlarmFlag = 1;//M43=1;
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
    
    MV_Set_Limit_P(X_AXIS,TempDist.MaxLength);
    if(X_DRV && !bXDirction && 
        ((!bLimitP_X && (tXAxisStepper.cRealPosi > tXAxisStepper.cMaxLen))|| 
            X_LIMIT_P))
	{		
		bLimitP_X = 1;
		LMV_Dec_Stop(X_AXIS);
		bRunning=0;
		bAlarmFlag = 1;

		cRunStep=0;

		if(X_LIMIT_P)
			AlarmNum = 2;
		else
			AlarmNum=16;
	}
	if(X_DRV && bXDirction && (tXAxisStepper.cRealPosi < tXAxisStepper.cMaxLen))
	{
		bLimitP_X = 0;
		if(AlarmNum == 2)
		{
			AlarmNum = 0;
		}
		if(AlarmNum==16)
		    AlarmNum=0;
	}

	if(SystemMode > 0)
	{
	    MV_Set_Limit_P(Y_AXIS,TempDist.XMaxLength);	
    	if(Y_DRV && !bYDirction && !bLimitP_Y && 
    	    (tYAxisStepper.cRealPosi > tYAxisStepper.cMaxLen))
    	{		
    		bLimitP_Y = 1;
    		LMV_Dec_Stop(Y_AXIS);
    		bRunning=0;
    		bAlarmFlag = 1;

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
        //Y06 = 1;
        Y06 = 0;
        bAlarmFlag = 1;
	}
	else
	{
        //Y06 = 0;
	}
}
/************************************************/
/*
�������������ȡ������

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
��ֹͣλʱ�����踴λ


*/
/************************************************/
void ResetPeripheral(void) 
{
    RstPphlDlay = 500;
}
/************************************************/
/*
����ѡ��


*/
/************************************************/
//void  MotorDrillSel(void) {

//}

/************************************************/
/*
��ֹͣλʱ����ֹͣ


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
������ֱȼ��ٶȱ���
*/
/************************************************/
void CalGearRatio(void)
{	
	double gearratiotmp;
	  
	gearratiotmp = (double)TimeParam->cyclelength;// ��λС��
	gearratiotmp = gearratiotmp / 10;// һ���������붼����λС����
	//gearratiotmp = 10000.0 / gearratiotmp;
	gearratiotmp = CustomPlug->CyclePulse / gearratiotmp;
	XGearRatio = gearratiotmp;// X ��
	
	XEncodeDivide = 10000 / CustomPlug->CyclePulse;
	
	gearratiotmp = (double)TimeParam->Xcyclelength;
	gearratiotmp = TimeParam->XPulsTotal / gearratiotmp;
	YGearRatio = gearratiotmp;// Y��
	
	if(XGearRatio==0)XGearRatio=10;	
	if(YGearRatio==0)YGearRatio=10;	
}
/************************************************/
/*
�ϵ�ʹ���ŷ�������

*/
/************************************************/
void ServoInit(void)
{	
	MTU10_PWMInit();
	MTU9_PWMInit();

	ServorDlay= dwTickCount + 500;//��ʱ500ms
	while (ServorDlay < dwTickCount)
		ReadInput();

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


