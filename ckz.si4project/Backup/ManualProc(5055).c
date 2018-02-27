#include "Global.h"

long dwRealPosi,cSetPulse;
unsigned char cAutoCheckSwitchstep = 0;
unsigned char CheckSwitchflag = 0;

extern void VariableInit_Int(void);
/************************************************/
/*
Y轴手动

*/
/************************************************/
void ManualProc_YAxis(void)
{
	unsigned long	Speedtmp = 0;
	long Distancetmp = 0;
//	long temp = 0;

	Speedtmp = SpeedParam->YManualSpeed;
	 if((bDotMVST))
	{
		if(!(Y_Dotting || Y_DRV))//&&(!(bXRst || bYRst )))
		{
			if(bMVYn)
			{
				Distancetmp = PositionToPluse(Y_AXIS, CustomPlug->DotDis);
			
				Y_Dotting = 1;	
				MV_AccDec_Set(Y_AXIS, 80, 80);
                tYAxisStepper.cSetStartSpeed = 1;
                tYAxisStepper.State = 0;
                tYAxisStepper.cAccDecVal = 0;
                LMV_Set_Speed(Y_AXIS, Speedtmp);
                Distancetmp = tYAxisStepper.cRealPosi - Distancetmp;
                LMV_Pmove(Y_AXIS,Distancetmp);
                if (tYAxisStepper.cPmovPulse) 
                {
                   tYAxisStepper.bAuto_Mov = 1;
                   LStartPWM(Y_AXIS);
                }
				bMVYn=0;
			}
			else if(bMVYp)
			{
				Distancetmp = PositionToPluse(Y_AXIS, CustomPlug->DotDis);
			
				Y_Dotting = 1;					
                MV_AccDec_Set(Y_AXIS, 80, 80);
				tYAxisStepper.cSetStartSpeed = 1;
				tYAxisStepper.State = 0;
				tYAxisStepper.cAccDecVal = 0;
				LMV_Set_Speed(Y_AXIS, Speedtmp);
                Distancetmp = tYAxisStepper.cRealPosi + Distancetmp;
				LMV_Pmove(Y_AXIS,Distancetmp);
				if (tYAxisStepper.cPmovPulse) 
				{
				   tYAxisStepper.bAuto_Mov = 1;
				   LStartPWM(Y_AXIS);
                }
				bMVYp=0;
			}
		}
		if(!Y_DRV )
		{
			Y_Dotting = 0;
		}
	}
	else
	{
		if(!(Y_DRV || Y_Dotting))//&&!(bXRst || bYRst ))
		{
			if(bMVYn &&!Y_ZERO)
			{			
				Y_Dotting = 1;	
               	MV_AccDec_Set(Y_AXIS, 80, 80);
               	LMV_Const_Move(Y_AXIS, Speedtmp, 1);
			}
			else if(bMVYp &&!bLimitP_Y && 
			    (tYAxisStepper.cRealPosi < tYAxisStepper.cMaxLen))
			{				
                Y_Dotting = 1;
                MV_AccDec_Set(Y_AXIS, 80, 80);
                LMV_Const_Move(Y_AXIS, Speedtmp, 0);
			}		

        }
		if(Y_Dotting && 
		    (!(bMVYn ||bMVYp)||(bMVYn &&Y_ZERO)||bMVYp&&
		        (tYAxisStepper.cRealPosi > tYAxisStepper.cMaxLen)))		
		{
			bMVYn =0;
			bMVYp=0;
			Y_Dotting = 0;
			LMV_Dec_Stop(Y_AXIS);
		}
	}
}
/************************************************/
/*
X轴手动

*/
/************************************************/
void ManualProc_XAxis(void)
{
	 unsigned long	 Speedtmp = 0;
	 long Distancetmp = 0;
//	 long temp = 0;

     Speedtmp = SpeedParam->ManualSpeed;
	 if((bDotMVST))
	{
		if(!(X_Dotting || X_DRV))//&&(!(bXRst || bYRst )))
		{
			if(bMVXn && !X_ZERO)//&& (bDrillBackSgn))
			{
			    if((bSystemMode && bDrillBackSgn) || (!bSystemMode && bQiGangBackSgn))
			    {
    				X_Dotting = 1;	
    				
					Distancetmp = PositionToPluse(X_AXIS, CustomPlug->DotDis);
					Distancetmp = tXAxisStepper.cRealPosi - Distancetmp;
                    MoveAction_Pulse2(Distancetmp, Speedtmp);
    				bMVXn=0;
				}
			}
			else if(bMVXp && !bLimitP_X)//&&bDrillBackSgn)
			{
			    if((bSystemMode && bDrillBackSgn) || (!bSystemMode && bQiGangBackSgn))
			    {
    				X_Dotting = 1;		
					Distancetmp = PositionToPluse(X_AXIS, CustomPlug->DotDis);
    				Distancetmp = tXAxisStepper.cRealPosi + Distancetmp;
                    MoveAction_Pulse2(Distancetmp, Speedtmp);
    				bMVXp=0;
				}
			}			
		}
		if(!X_DRV )
		{
			X_Dotting = 0;
		}
	}
	else
	{
		if(!(X_DRV || X_Dotting))//&&!(bXRst || bYRst ))
		{
			if(bMVXn && !X_ZERO)
			{			
				X_Dotting = 1;				      
                MV_AccDec_Set(X_AXIS, 150, 150);
                LMV_Const_Move(X_AXIS, Speedtmp, 1);
			}
			else if(bMVXp && !bLimitP_X)
			{				
				X_Dotting = 1;
                MV_AccDec_Set(X_AXIS, 150, 150);
                LMV_Const_Move(X_AXIS, Speedtmp, 0);
			}		
        }
		if(X_Dotting && 
		    (!(bMVXn ||bMVXp)||(bMVXn &&X_ZERO)||
		    	(bMVXp&&(tXAxisStepper.cRealPosi > tXAxisStepper.cMaxLen))))		
		{
			bMVXn =0;
			bMVXp=0;
			X_Dotting = 0;
			LMV_Dec_Stop(X_AXIS);
		}
	}
}
/************************************************/
/*
IO手动

*/
/************************************************/
void ManualProc_IO(void)
{
    if (M20) 
    {
        M20 = 0;
	    Y00=~Y00;
    }
    if (M21) 
    {
        M21 = 0;
	    Y01=~Y01;
    }
    if (M22)
    {
        Y02=~Y02;
        M22 = 0;
    }
    if (M23)
    {
        Y03=~Y03;
        M23 = 0;
    }
    if (M24) 
    {
        M24 = 0;
	    Y04=~Y04;
    }
    if (M25)
    {
        M25 = 0;
	    Y05=~Y05;
    }

    if(bCuiQi_ST)
    {
        bCuiQi_ST = 0;
        bCuiQi = ~bCuiQi;
    }

    if(!bSystemMode)
    {// 单伺服模式
        if(bMVYn)
        {
            bDrillValve = 0;
        }

        if(bMVYp)
        {
            bDrillValve = 1;
        }
    }
}

void Hole_MuBanSelect(void)
{
    if(SpeedParam->Hole_DrillOffsetMode == 0)
    {
        TimeParam->DWDrillDist = DrillOffset->Hole_DrillOffset1;
    }
    else if(SpeedParam->Hole_DrillOffsetMode == 1)
    {
        TimeParam->DWDrillDist = DrillOffset->Hole_DrillOffset2;
    }
    else if(SpeedParam->Hole_DrillOffsetMode == 2)
    {
        TimeParam->DWDrillDist = DrillOffset->Hole_DrillOffset3;
    }
    else if(SpeedParam->Hole_DrillOffsetMode == 3)
    {
        TimeParam->DWDrillDist = DrillOffset->Hole_DrillOffset4;
    }
    else if(SpeedParam->Hole_DrillOffsetMode == 4)
    {
        TimeParam->DWDrillDist = DrillOffset->Hole_DrillOffset5;
    }
}
void Cao_MuBanSelect(void)
{
    if(SpeedParam->Slot_DrillOffsetMode == 0)
    {
        TimeParam->DrillOffset_Cao = DrillOffset->Slot_DrillOffset1;
    }
    else if(SpeedParam->Slot_DrillOffsetMode == 1)
    {
        TimeParam->DrillOffset_Cao = DrillOffset->Slot_DrillOffset2;
    }
    else if(SpeedParam->Slot_DrillOffsetMode == 2)
    {
        TimeParam->DrillOffset_Cao = DrillOffset->Slot_DrillOffset3;
    }
    else if(SpeedParam->Slot_DrillOffsetMode == 3)
    {
        TimeParam->DrillOffset_Cao = DrillOffset->Slot_DrillOffset4;
    }
    else if(SpeedParam->Slot_DrillOffsetMode == 4)
    {
        TimeParam->DrillOffset_Cao = DrillOffset->Slot_DrillOffset5;
    }
}
void Scan_MuBanSelect(void)
{
    if(ScanMode < 1)
    {
        if(SpeedParam->Scan_DrillOffsetMode == 0)
        {
            TimeParam->DrillDist = DrillOffset->Scan_DrillOffset1;
        }
        else if(SpeedParam->Scan_DrillOffsetMode == 1)
        {
            TimeParam->DrillDist = DrillOffset->Scan_DrillOffset2;
        }
        else if(SpeedParam->Scan_DrillOffsetMode == 2)
        {
            TimeParam->DrillDist = DrillOffset->Scan_DrillOffset3;
        }
        else if(SpeedParam->Scan_DrillOffsetMode == 3)
        {
            TimeParam->DrillDist = DrillOffset->Scan_DrillOffset4;
        }
        else if(SpeedParam->Scan_DrillOffsetMode == 4)
        {
            TimeParam->DrillDist = DrillOffset->Scan_DrillOffset5;
        }
    }
    else
    {
        if(SpeedParam->Scan_DrillOffsetMode == 0)
        {
            TimeParam->DrillDist = DrillOffset2->Scan_DrillOffset1;
        }
        else if(SpeedParam->Scan_DrillOffsetMode == 1)
        {
            TimeParam->DrillDist = DrillOffset2->Scan_DrillOffset2;
        }
        else if(SpeedParam->Scan_DrillOffsetMode == 2)
        {
            TimeParam->DrillDist = DrillOffset2->Scan_DrillOffset3;
        }
        else if(SpeedParam->Scan_DrillOffsetMode == 3)
        {
            TimeParam->DrillDist = DrillOffset2->Scan_DrillOffset4;
        }
        else if(SpeedParam->Scan_DrillOffsetMode == 4)
        {
            TimeParam->DrillDist = DrillOffset2->Scan_DrillOffset5;
        }

    }
}

/************************************************/
/*
Y轴回原点  深度轴复位

*/
/************************************************/
void RebackYaxisOrign(void)
{  
    if(tYAxisStepper.cRstStep == 1)
    {
        if(Y_ZERO)
        {
            tYAxisStepper.cRstStep = 8;
            MV_AccDec_Set(Y_AXIS,100,100);
            LMV_Const_Move(Y_AXIS,3,XCW);// 0);// 远离原点
        }
        else
        {
            tYAxisStepper.cRstStep = 2;
        }
    }
    else if(tYAxisStepper.cRstStep == 8)// && !X_DRV)
    {
        if(!Y_ZERO)
        {
            tYAxisStepper.cRstStep = 4;
            LMV_Dec_Stop(Y_AXIS);
        }
    }
    else if(tYAxisStepper.cRstStep == 2)
    {
        if(!Y_DRV)
        {
            tYAxisStepper.cRstStep = 3;
            MV_AccDec_Set(Y_AXIS,100,100);
            LMV_Const_Move(Y_AXIS,3,YCCW);// 1);// 向原点走
        }
    }
    else if(tYAxisStepper.cRstStep == 3)
    {
        if(Y_ZERO)
        {
            LMV_Dec_Stop(Y_AXIS);	
            tYAxisStepper.cRstStep = 7; 
        }
    }
    else if(tYAxisStepper.cRstStep == 7)
    {
        if(!Y_DRV)
        {
            tYAxisStepper.cRstStep = 8;
            MV_AccDec_Set(Y_AXIS,100,100);
            LMV_Const_Move(Y_AXIS,3,YCW);// 0);// 远离原点
        } 
    }
    else if(tYAxisStepper.cRstStep == 4)
    {    
        if(!Y_DRV)
        {
            tYAxisStepper.cRstStep = 5;    
        }
    }
	else if(tYAxisStepper.cRstStep == 5)// && !X_DRV)
    {
        tYAxisStepper.cRstStep = 6;

        DR_CCW_Y();
        tYAxisStepper.bAuto_Mov = 0;	  
        tYAxisStepper.State = 3;
        tYAxisStepper.cSetSpeed = 1;
        tYAxisStepper.cSetStartSpeed = 1;
        tYAxisStepper.cAccDecVal = 0;
        LStartPWM(Y_AXIS);
    }
    else if(tYAxisStepper.cRstStep == 6)
    {
        if(!Y_DRV)
        {
            tYAxisStepper.cRstStep = 0;
            bYRst=0;	
        }
    }
}/************************************************/
/*
X轴回原点

*/
/************************************************/
static unsigned long ResetEncodeDelay = 0;
void RebackXaxisOrign(void)
{       

    unsigned short speed = SpeedParam->XResetSpeed;//10;

    if(tXAxisStepper.cRstStep == 1)
    {
        if(X_ZERO)
        {
            tXAxisStepper.cRstStep = 8;
            MV_AccDec_Set(X_AXIS,50,50);
            LMV_Const_Move(X_AXIS,1,XCW);// 0);// 远离原点
        }
        else
        {
            tXAxisStepper.cRstStep = 2;
        }
    }
    else if(tXAxisStepper.cRstStep == 2 && !X_DRV)
    {
        tXAxisStepper.cRstStep = 3;
        MV_AccDec_Set(X_AXIS,150,150);
        LMV_Const_Move(X_AXIS,speed,XCCW);// 1);// 向原点走
    }
    else if(tXAxisStepper.cRstStep == 3 && X_ZERO)
    {
        LMV_Dec_Stop(X_AXIS);	
        tXAxisStepper.cRstStep = 7;    
    }
    else if(tXAxisStepper.cRstStep == 4 &&!X_DRV)
    {    
        tXAxisStepper.cRstStep = 5;    
    }
    else if(tXAxisStepper.cRstStep == 5 && !X_DRV)
    {
        tXAxisStepper.cRstStep = 6;

        DR_CCW_X();
        tXAxisStepper.bAuto_Mov = 0;	  
        tXAxisStepper.State = 3;
        tXAxisStepper.cSetSpeed = 3;
        tXAxisStepper.cSetStartSpeed = 1;
        tXAxisStepper.cAccDecVal = 0;
        LStartPWM(X_AXIS);
    }
    else if(tXAxisStepper.cRstStep == 6)
    {
        if(!X_DRV)
        {
            tXAxisStepper.cRstStep = 9;
            ResetEncodeDelay = dwTickCount + 500;
        }
    }
    else if(tXAxisStepper.cRstStep == 7 && !X_DRV)
    {
        tXAxisStepper.cRstStep = 8;
        MV_AccDec_Set(X_AXIS,50,50);
        LMV_Const_Move(X_AXIS,1,XCW);// 0);
    }
    else if(tXAxisStepper.cRstStep == 8)
    {
        if(!X_ZERO)
        {
            tXAxisStepper.cRstStep = 4;
            LMV_Dec_Stop(X_AXIS);
        }
    }
    else if(tXAxisStepper.cRstStep == 9)
    {
        if(ResetEncodeDelay < dwTickCount)
        {
            tXAxisStepper.cRstStep = 0;
            ulEncoderCount = 0;
            bXRst=0;
        }
    }
}

#if 1
void AutoCheckSwitch(void)
{
	static unsigned long delaytick = 0;
	static unsigned long delaytick_manual = 0;
	static unsigned long delaytick_manual2 = 0;

	if(cAutoCheckSwitchstep == 0)
	{
		if(bManualCheckSwitch_ST)
		{
			bManualCheckSwitch_ST = 0;
			if(CheckSwitchflag < 1)
			{
				bManualCheckSwitch = 1;
				bCheckSwitchOut = 1;
				delaytick = dwTickCount + 100;
				delaytick_manual2 = dwTickCount + 10000;
				CheckSwitchflag = 1;
			}
		}
		else if(CheckSwitchflag == 1)
		{
			if(delaytick < dwTickCount)
			{
				bCheckSwitchOut = 0;
				delaytick_manual = dwTickCount + 3000;
				CheckSwitchflag = 2;
			}
		}
		else if(CheckSwitchflag == 2)
		{
			if(delaytick_manual < dwTickCount)
			{
				CheckSwitchflag = 0;
				bManualCheckSwitch = 0;
			}
		}
		if(delaytick_manual2 < dwTickCount)
		{
			CheckSwitchflag = 0;
			bManualCheckSwitch = 0;
			bCheckSwitchOut = 0;
		}
	}

	if(bAutoCheckSwitch_ST)
	{
		bAutoCheckSwitch_ST = 0;
		if(CheckSwitchflag == 0)
			bAutoCheckSwitch = 1;
	}
	
	if(bAutoCheckSwitch)
	{
		if(cAutoCheckSwitchstep == 0)
			cAutoCheckSwitchstep = 1;
	}
	else
	{
		cAutoCheckSwitchstep = 0;
	}

	if(cAutoCheckSwitchstep == 1)
	{// 关闭所有输出
		bPress1_Vavle =0;	
		bPress2_Vavle =0;
		bPress3_Vavle =0;
		bDrillValve=0;			
		bCuiQi =0;				
		bDrillMotor =0;
		bCheckSwitchOut = 0;

		if(!X_DRV) LMV_Dec_Stop(X_AXIS);
		if(!X_DRV) LMV_Dec_Stop(Y_AXIS);
		
		delaytick = dwTickCount + 100;
		cAutoCheckSwitchstep = 2;
	}
	else if(cAutoCheckSwitchstep == 2)
	{
		if(delaytick < dwTickCount)
			cAutoCheckSwitchstep = 3;
	}
	else if(cAutoCheckSwitchstep == 3)
	{
		if(X_ZERO && (Y_ZERO || bQiGangBackSgn))
		{
			cAutoCheckSwitchstep = 6;
		}
		else
		{
			cAutoCheckSwitchstep = 4;
		}
	}
	else if(cAutoCheckSwitchstep == 4)
	{
		if(CurBoardData->SystemMode1 > 0)
		{
			if(!Y_ZERO)
			{
				if(!Y_DRV)
					MoveAction_Pulse(Y_AXIS, 0, 5);
					//LMV_Const_Move(Y_AXIS, 5, YCCW);
			}
			else
			{
				cAutoCheckSwitchstep = 5;
			}
		}
		else
		{
			if(!bQiGangBackSgn)
			{
				bDrillValve = 0;
			}
			else
			{
				cAutoCheckSwitchstep = 5;
			}
		}
	}
	else if(cAutoCheckSwitchstep == 5)
	{
		if(!X_ZERO)
		{
			if(!X_DRV)
				MoveAction_Pulse(X_AXIS, 0, 10);
		}
		else
		{
			if(!X_DRV)
				cAutoCheckSwitchstep = 6;
		}
	}
	else if(cAutoCheckSwitchstep == 6)
	{
		VariableInit_Int();
		StartFlag = 1;
		MV_AccDec_Set(X_AXIS,80, 80);
		LMV_Const_Move2(X_AXIS, 5 , XCW);
		cAutoCheckSwitchstep = 7;
	}
	else if(cAutoCheckSwitchstep == 7)
	{
		if(flgSgn > 0)
			cAutoCheckSwitchstep = 8;
	}
	else if(cAutoCheckSwitchstep == 8)
	{
		if(flgSgn < 1)
		{
			cAutoCheckSwitchstep = 9;
		}
	}
	else if(cAutoCheckSwitchstep == 9)
	{
		unsigned long temp = 0;
		
		if(flgSgn > 0)
		{
			temp = HoleinfoB[CheckHoleNum[0] - 1].hole_point;
			temp -= HoleinfoF[CheckHoleNum[1] - 1].hole_point;
			temp = temp / XEncodeDivide;

			if(temp > TempDist.MinHoleR  && temp < TempDist.MaxHoleR)
			{
				StartFlag = 0;
				LMV_Dec_Stop(X_AXIS);
				cAutoCheckSwitchstep = 10;
			}
			else
			{
				cAutoCheckSwitchstep = 7;
			}
		}
	}
	else if(cAutoCheckSwitchstep == 10)
	{
		if(!X_DRV)
		{
			if(delaytick < dwTickCount)
			{
				unsigned long temp = 0;

				temp = HoleinfoB[CheckHoleNum[0] - 1].hole_point;
				temp += HoleinfoB[CheckHoleNum[0] - 2].hole_point;
				temp /= 2;
				temp /= XEncodeDivide;

				MoveAction_Pulse(X_AXIS, temp, 10);
				
				cAutoCheckSwitchstep = 11;
			}
		}
		else
		{
			delaytick =  dwTickCount + 1000;
		}
	}
	else if(cAutoCheckSwitchstep == 11)
	{
		if(!X_DRV)
		{
			delaytick =  dwTickCount + 1000;
			cAutoCheckSwitchstep = 12;
		}
	}
	else if(cAutoCheckSwitchstep == 12)
	{
		//if(!X_DRV)
		if(delaytick < dwTickCount)
		{
			bCheckSwitchOut = 1;
			delaytick =  dwTickCount + 100;
			cAutoCheckSwitchstep = 13;
		}
	}
	else if(cAutoCheckSwitchstep == 13)
	{
		if(delaytick < dwTickCount)
		{
			bCheckSwitchOut = 0;
			delaytick = dwTickCount + 1000;
			cAutoCheckSwitchstep = 14;
		}
	}
	else if(cAutoCheckSwitchstep == 14)
	{
		if(delaytick < dwTickCount)
		{
			unsigned long temp = 0;

			temp = HoleinfoB[CheckHoleNum[0] - 1].hole_point;
			temp += HoleinfoF[CheckHoleNum[1] - 1].hole_point;
			temp /= XEncodeDivide;
			temp /= 2;

			MoveAction_Pulse(X_AXIS, temp , 10);
			cAutoCheckSwitchstep = 15;
			delaytick = dwTickCount + 2000;
		}
	}
	else if(cAutoCheckSwitchstep == 15)
	{
		if(!X_DRV)
		{
			cAutoCheckSwitchstep = 16;
		}
	}
	else if(cAutoCheckSwitchstep == 16)
	{
		if(delaytick < dwTickCount)
		{
			bCheckSwitchOut = 1;
			delaytick =  dwTickCount + 100;
			cAutoCheckSwitchstep = 17;		
		}
	}
	else if(cAutoCheckSwitchstep == 17)
	{
		if(delaytick < dwTickCount)
		{
			bCheckSwitchOut = 0;
			cAutoCheckSwitchstep = 18;
		}
	}
	else if(cAutoCheckSwitchstep == 18)
	{
		bAutoCheckSwitch = 0;
		cAutoCheckSwitchstep = 0;
	}
}

#else
void AutoCheckSwitch(void)

{
	static unsigned long delaytick = 0;
	
	if(bAutoCheckSwitch_ST)
	{
		bAutoCheckSwitch_ST = 0;
		bAutoCheckSwitch = 1;
	}

	#if 0
	if(bAutoCheckSwitch)
	{
		bAutoCheckSwitch = 0;
		bAutoCheckSwitchOut = 1;
		delaytick = dwTickCount + 50;
	}
	else
	{
		if(delaytick < dwTickCount)
			bAutoCheckSwitchOut = 0;
	}
	#endif
	
	if(bAutoCheckSwitch)
	{
		if(cAutoCheckSwitchstep == 0)
			cAutoCheckSwitchstep = 1;
	}
	else
	{
		cAutoCheckSwitchstep = 0;
	}

	if(cAutoCheckSwitchstep == 1)
	{// 关闭所有输出
		bPress1_Vavle =0;	
		bPress2_Vavle =0;
		bPress3_Vavle =0;
		bDrillValve=0;			
		bCuiQi =0;				
		bDrillMotor =0;
		bCheckSwitchOut = 0;

		delaytick = dwTickCount + 100;
		cAutoCheckSwitchstep = 2;
	}
	else if(cAutoCheckSwitchstep == 2)
	{
		if(delaytick < dwTickCount)
			cAutoCheckSwitchstep = 3;
	}
	else if(cAutoCheckSwitchstep == 3)
	{
		if(X_ZERO && (Y_ZERO || bQiGangBackSgn))
		{
			cAutoCheckSwitchstep = 6;
		}
		else
		{
			cAutoCheckSwitchstep = 4;
		}
	}
	else if(cAutoCheckSwitchstep == 4)
	{
		if(CurBoardData->SystemMode1 > 0)
		{
			if(!Y_ZERO)
			{
				if(!Y_DRV)
					MoveAction_Pulse(Y_AXIS, 0, 5);
					//LMV_Const_Move(Y_AXIS, 5, YCCW);
			}
			else
			{
				cAutoCheckSwitchstep = 5;
			}
		}
		else
		{
			if(!bQiGangBackSgn)
			{
				bDrillValve = 0;
			}
			else
			{
				cAutoCheckSwitchstep = 5;
			}
		}
	}
	else if(cAutoCheckSwitchstep == 5)
	{
		if(!X_ZERO)
		{
			if(!X_DRV)
				MoveAction_Pulse(X_AXIS, 0, 10);
		}
		else
		{
			if(!X_DRV)
				cAutoCheckSwitchstep = 6;
		}
	}
	else if(cAutoCheckSwitchstep == 6)
	{
		VariableInit_Int();
		StartFlag = 1;
		MV_AccDec_Set(X_AXIS,80, 80);
		LMV_Const_Move(X_AXIS, 5 , XCW);
		cAutoCheckSwitchstep = 7;
	}
	else if(cAutoCheckSwitchstep == 7)
	{
		if(flgSgn > 0)
			cAutoCheckSwitchstep = 8;
	}
	else if(cAutoCheckSwitchstep == 8)
	{
		if(flgSgn < 1)
		{
			cAutoCheckSwitchstep = 9;
		}
	}
	else if(cAutoCheckSwitchstep == 9)
	{
		unsigned long temp = 0;
		
		if(flgSgn > 0)
		{
			temp = HoleinfoB[CheckHoleNum[0]].hole_point;
			temp -= HoleinfoF[CheckHoleNum[1]].hole_point;
			temp = temp / XEncodeDivide;

			if(temp > TempDist.MinHoleR  && temp < TempDist.MaxHoleR)
			{
				StartFlag = 0;
				LMV_Dec_Stop(X_AXIS);
				cAutoCheckSwitchstep = 10;
			}
			else
			{
				cAutoCheckSwitchstep = 7;
			}
		}
	}
	else if(cAutoCheckSwitchstep == 10)
	{
		if(!X_DRV)
		{
			cAutoCheckSwitchstep = 11;
		}
	}
	else if(cAutoCheckSwitchstep == 11)
	{
		unsigned long temp = 0;

		temp = HoleinfoB[CheckHoleNum[0]].hole_point;
		temp += HoleinfoB[CheckHoleNum[0] - 1].hole_point;
		temp = temp / XEncodeDivide;

		MoveAction_Pulse(X_AXIS, temp, 10);
		cAutoCheckSwitchstep = 12;
	}
	else if(cAutoCheckSwitchstep == 12)
	{
		if(!X_DRV)
		{
			bCheckSwitchOut = 1;
			delaytick =  dwTickCount + 100;
			cAutoCheckSwitchstep = 13;
		}
	}
	else if(cAutoCheckSwitchstep == 13)
	{
		if(delaytick < dwTickCount)
		{
			bCheckSwitchOut = 0;
			delaytick = dwTickCount + 3000;
			cAutoCheckSwitchstep = 14;
		}
	}
	else if(cAutoCheckSwitchstep == 14)
	{
		unsigned long temp = 0;

		temp = HoleinfoB[CheckHoleNum[0]].hole_point;
		temp += HoleinfoF[CheckHoleNum[1]].hole_point;
		temp /= XEncodeDivide;

		MoveAction_Pulse(X_AXIS, temp , 10);
		cAutoCheckSwitchstep = 15;
	}
	else if(cAutoCheckSwitchstep == 15)
	{
		if(!X_DRV)
		{
			cAutoCheckSwitchstep = 16;
		}
	}
	else if(cAutoCheckSwitchstep == 16)
	{
		if(delaytick < dwTickCount)
		{
			bCheckSwitchOut = 1;
			delaytick =  dwTickCount + 100;
			cAutoCheckSwitchstep = 17;		
		}
	}
	else if(cAutoCheckSwitchstep == 17)
	{
		if(delaytick < dwTickCount)
		{
			bCheckSwitchOut = 0;
			cAutoCheckSwitchstep = 18;
		}
	}
	else if(cAutoCheckSwitchstep == 18)
	{
		bAutoCheckSwitch = 0;
		cAutoCheckSwitchstep = 0;
	}
}
#endif

