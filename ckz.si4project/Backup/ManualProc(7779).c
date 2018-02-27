#include "Global.h"

long dwRealPosi,cSetPulse;

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

	 if((bDotMVST))
	{
		if((!Y_Dotting)&&(!Y_DRV)&&(!(bXRst || bYRst )))
		{
			if(bMVYn)
			{
				Speedtmp = SpeedParam->YManualSpeed;
				Distancetmp = PositionToPluse(Y_AXIS, TimeParam->DotDis);
			
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
				Speedtmp = SpeedParam->YManualSpeed;
				Distancetmp = PositionToPluse(Y_AXIS, TimeParam->DotDis);
			
				Y_Dotting = 1;					
                MV_AccDec_Set(Y_AXIS, 80, 80);
				tYAxisStepper.cSetStartSpeed = 1;
				tYAxisStepper.State = 0;
				tYAxisStepper.cAccDecVal = 0;
				LMV_Set_Speed(Y_AXIS, 5);
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
		if(!Y_DRV && !Y_Dotting&&!(bXRst || bYRst ))
		{
			if(bMVYn &&!Y_ZERO)
			{			
				Y_Dotting = 1;	
               	MV_AccDec_Set(Y_AXIS, 80, 80);
               	LMV_Const_Move(Y_AXIS, SpeedParam->YManualSpeed, 1);
			}
			else if(bMVYp &&!bLimitP_Y && 
			    (tYAxisStepper.cRealPosi < tYAxisStepper.cMaxLen))
			{				
                Y_Dotting = 1;
                MV_AccDec_Set(Y_AXIS, 80, 80);
                LMV_Const_Move(Y_AXIS, SpeedParam->YManualSpeed, 0);
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
	 long temp = 0;

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
    				
					Distancetmp = PositionToPluse(X_AXIS, TimeParam->DotDis);
					Distancetmp = tXAxisStepper.cRealPosi - Distancetmp;
                    MoveAction_Pulse2(Distancetmp, SpeedParam->ManualSpeed);
    				bMVXn=0;
				}
			}
			else if(bMVXp && !bLimitP_X)//&&bDrillBackSgn)
			{
			    if((bSystemMode && bDrillBackSgn) || (!bSystemMode && bQiGangBackSgn))
			    {
    				X_Dotting = 1;		
    				Speedtmp = SpeedParam->ManualSpeed;
					Distancetmp = PositionToPluse(X_AXIS, TimeParam->DotDis);
    				Distancetmp = tXAxisStepper.cRealPosi + Distancetmp;
                    MoveAction_Pulse2(Distancetmp, SpeedParam->ManualSpeed);
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
                LMV_Const_Move(X_AXIS, SpeedParam->ManualSpeed, 1);
			}
			else if(bMVXp && !bLimitP_X)
			{				
				X_Dotting = 1;
                MV_AccDec_Set(X_AXIS, 150, 150);
                LMV_Const_Move(X_AXIS, SpeedParam->ManualSpeed, 0);
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
    
	if(!bNOChaXiao)
    {
        if(M110)
        {
            M110 = 0;
            bMiddleChaXiao = ~bMiddleChaXiao;
        }
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
