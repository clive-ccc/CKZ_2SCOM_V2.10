#include "Global.h"

StepperDataInfo tXAxisStepper,tYAxisStepper;
WORD_BITS cRunState;

static StepperDataInfo *StepInfo[] = {
    &tXAxisStepper, &tYAxisStepper,
};


unsigned short FreqTab_X[] = {22118, 11059, 6530, 4686, 2765, 2212, 1843, 1580, 1382,
    1229, 1106, 1005, 922, 851, 790, 737, 691, 651, 614,
    582, 553, 527, 503, 481, 461, 442, 425, 410, 395, 381,
    369, 357, 346, 335, 325, 316, 307, 299, 291, 284, 276,
    270, 263, 257, 251, 246, 240, 235, 230, 226, 221, 217,
    213, 209, 205, 201, 197, 194, 191, 187, 184, 181, 178,
    176, 173, 170, 168, 165, 163, 160, 158, 156, 154, 151,
    149, 147, 146, 144, 142, 140, 138, 137, 135, 133, 132,
    130, 129, 127, 126, 124, 123, 122, 120, 119, 118, 116,
    115, 114, 113, 112, 111, 110.109, 108, 107, 106, 105,
    104, 103, 102, 101, 100
};

unsigned short FreqTab_Y[] = {44000,33000,22000,11059,4424,3791,3318,2949,2654,2413,2212,2042,1896,
							  1769,1659,1561,1475,1397,1327,1264,1206,1154,
							  1106,1062,1021,983,948,915,885,856,829,804,781,
							  758,737,717,698,681,664,657,651,644,638,632,626,
							  620,614,609,603,598,592,587,582,577,572,567,562,
							  558,553,548,544,539,535,531,527,522,518,514,510,
							  507,503,499,495,492,488,484,481,477,474,471,467,
							  464,461,458,454,451,448,445,442,439,437,434,431,
							  428,425,423,420,417,415,412,410,407,405,402,400,
							  397,395
};

extern volatile char secondHoleFlg;

void MTU7_PWMProc(void);
void MTU8_PWMProc(void);


/********************************************************************
MTU7脉冲输出初始化
 *******************************************************************/
void MTU7_PWMInit(void) {
    MSTP(MTU7) = 0;

    /* TCNT3 stop */
    MTUB.TSTR.BIT.CST1 = 0;

    MTU7.TCR.BYTE = 0x20;

    /* TCNT4 synchronization */
    MTUB.TSYR.BIT.SYNC1 = 0;

    /* TCNT10 initialize */
    MTU7.TCNT = 0x0000;
    /* Synchronous Clear */

    /* MTIOC3A output initial output is 1 */
    MTU7.TIOR.BYTE = 0x61;

    /* Set TGRA3 */
    MTU7.TGRA = 11059;
    /* Set TGRB3 */
    MTU7.TGRB = 5529;
    /* Set TGRC3 */
//	MTU8.TGRC = 11059;

//	MTU8.TBTM.BYTE = 0x01;

    /* ---- Timer interrupt enable register setting ---- */
    MTU7.TIER.BYTE = 0x01;         /* The interrupt request (TGIA) by TGFA bit is enabled */
    /* initial interrupt */
    IEN(MTU7, TGIA7) = 0;
    IR(MTU7, TGIA7) = 0;
    IPR(MTU7, TGIA7) = 18;
    IEN(MTU7, TGIA7) = 1;

    /* MTU7 pwm mode1 */
    MTU7.TMDR.BYTE = 0x02;

    /* MTU7 Start */
    MTUB.TSTR.BIT.CST1 = 0;

}
/********************************************************************
MTU8脉冲输出初始化
 *******************************************************************/
void MTU8_PWMInit(void) {
    MSTP(MTU8) = 0;

    /* TCNT3 stop */
    MTUB.TSTR.BIT.CST2 = 0;
//	MTUA.TSTR.BIT.CST3 =0;

    MTU8.TCR.BYTE = 0x20;

    /* TCNT4 synchronization */
    MTUB.TSYR.BIT.SYNC2 = 0;

    /* TCNT10 initialize */
    MTU8.TCNT = 0x0000;
    /* Synchronous Clear */

    /* MTIOC3A output initial output is 1 */
    MTU8.TIOR.BYTE = 0x61;

    /* Set TGRA3 */
    MTU8.TGRA = 11059;
    /* Set TGRB3 */
    MTU8.TGRB = 5529;
    /* Set TGRC3 */

    /* ---- Timer interrupt enable register setting ---- */
    MTU8.TIER.BYTE = 0x01;         /* The interrupt request (TGIA) by TGFA bit is enabled */
    /* initial interrupt */
    IEN(MTU8, TGIA8) = 0;
    IR(MTU8, TGIA8) = 0;
    IPR(MTU8, TGIA8) = 17;
    IEN(MTU8, TGIA8) = 1;

    /* MTU8 pwm mode1 */
    MTU8.TMDR.BYTE = 0x02;

    /* MTU3 Start */
    MTUB.TSTR.BIT.CST2 = 0;

}
/********************************************************************
MTU4脉冲输出初始化
 *******************************************************************/
void MTU10_PWMInit(void) {
    MSTP(MTU10) = 0;

    /* TCNT3 stop */
    MTUB.TSTR.BIT.CST4 = 0;

    MTU10.TCR.BYTE = 0x20;

    /* TCNT4 synchronization */
    MTUB.TSYR.BIT.SYNC4 = 0;

    /* TCNT10 initialize */
    MTU10.TCNT = 0x0000;
    /* Synchronous Clear */

    /* MTIOC10A output initial output is 1 */
    MTU10.TIORH.BYTE = 0x61;

    /* Set TGRA3 */
    MTU10.TGRA = 11059;
    /* Set TGRB3 */
    MTU10.TGRB = 5529;
    /* Set TGRC3 */
    MTU10.TGRC = 11059;

    MTU10.TBTM.BYTE = 0x01;

    /* ---- Timer interrupt enable register setting ---- */
    MTU10.TIER.BYTE = 0x01;         /* The interrupt request (TGIA) by TGFA bit is enabled */
    /* initial interrupt */
    IEN(MTU10, TGIA10) = 0;
    IR(MTU10, TGIA10) = 0;
    IPR(MTU10, TGIA10) = 10;
    IEN(MTU10, TGIA10) = 1;

    /* MTU3 pwm mode1 */
    MTU10.TMDR.BYTE = 0x12;

    MTUB.TOER.BIT.OE4A = 1;
    MTUB.TSTR.BIT.CST4 = 0;

}
/********************************************************************
MTU9脉冲输出初始化
 *******************************************************************/
void MTU9_PWMInit(void) {
//	IOPORT.PFCMTU.BIT.MTUS4 = 1;

    MSTP(MTU9) = 0;

    /* TCNT4 stop */
//	MTUB.TSTR.BYTE = 0;

    MTUB.TSTR.BIT.CST3 = 0;

    MTU9.TCR.BYTE = 0x20;

    /* TCNT3 synchronization */
    MTUB.TSYR.BIT.SYNC4 = 0;

    /* TCNT3 initialize */
    MTU9.TCNT = 0x0000;
    /* Synchronous Clear */

    /* MTIOC4A-b output initial output is 1 */
    /* Output=0 Compare Match(TGRB3),Output=1 Compare Match(TGRA3) */
    MTU9.TIORH.BYTE = 0x61;

    /* Set TGRA4 */
    MTU9.TGRA = 11059;
    /* Set TGRB4 */
    MTU9.TGRB = 5529;
    /* Set TGRC4 */
    MTU9.TGRC = 11059;

    MTU9.TBTM.BYTE = 0x01;

    /* ---- Timer interrupt enable register setting ---- */
    MTU9.TIER.BYTE = 0x01;         /* The interrupt request (TGIA) by TGFA bit is enabled */
    /* initial interrupt */
    IEN(MTU9, TGIA9) = 0;
    IR(MTU9, TGIA9) = 0;
    IPR(MTU9, TGIA9) = 12;
    IEN(MTU9, TGIA9) = 1;

    /* MTU9 pwm mode1 */
    MTU9.TMDR.BYTE = 0x12;

    /* MTU4 Start */
    MTUB.TSTR.BIT.CST3 = 0;

}
/********************************************

MTU7-PWM脉冲输出中断程序

*********************************************/

#pragma interrupt ( MTU7_TGRA( vect = VECT( MTU7,TGIA7), enable ) )
void  MTU7_TGRA(void)
{
    MTU7_PWMProc();

}
void MTU7_PWMProc(void)
{
    extern void AutoCheckHole(void) ;

    if (bXDirction) 
        tXAxisStepper.cRealPosi--;
    else 
        tXAxisStepper.cRealPosi++;
    AutoCheckHole();
    if (tXAxisStepper.bAuto_Mov == 1) 
    {
        if (--tXAxisStepper.cPmovPulse == 0)
        {
            tXAxisStepper.bAuto_Mov = 0;
            X_AXIS_PULSE_OFF();
            tXAxisStepper.cCurSpeed = 0;
        } 
        else if (secondHoleFlg > 0) 
        {
            tXAxisStepper.cCurSpeed = tXAxisStepper.cSetSpeed >> 1;
            MTU7.TGRA = FreqTab_X[tXAxisStepper.cCurSpeed];
            MTU7.TGRB = FreqTab_X[tXAxisStepper.cCurSpeed] >> 1;
        } 
        else if (tXAxisStepper.cPmovPulse > tXAxisStepper.cCalDecPulse)
        {
            if (tXAxisStepper.cCurSpeed < tXAxisStepper.cCalSpeed) 
            {
                if (++tXAxisStepper.cAccDecVal >= tXAxisStepper.cSetAccVal) 
                {
                    tXAxisStepper.cAccDecVal = 0;
                    tXAxisStepper.cCurSpeed++;
                    MTU7.TGRA = FreqTab_X[tXAxisStepper.cCurSpeed];
                    MTU7.TGRB = FreqTab_X[tXAxisStepper.cCurSpeed] >> 1;
                }
            }
        }	
        else 
        {
            if (tXAxisStepper.cCurSpeed > tXAxisStepper.cSetStartSpeed) 
            {
                if (++tXAxisStepper.cAccDecVal >= tXAxisStepper.cSetDecVal)
                {
                    tXAxisStepper.cAccDecVal = 0;
                    tXAxisStepper.cCurSpeed--;
                    MTU7.TGRA = FreqTab_X[tXAxisStepper.cCurSpeed];
                    MTU7.TGRB = FreqTab_X[tXAxisStepper.cCurSpeed] >> 1;
                }
            }
        }
    } 
  else
  {
  	if(tXAxisStepper.State == 3)
	{
		if(X_ZERO)
		{
			tXAxisStepper.cZeroFilter++;
			if(tXAxisStepper.cZeroFilter >= 3)
			{
				X_AXIS_PULSE_OFF();			
				tXAxisStepper.cRealPosi = 0;
				//tYAxisStepper.cRstStep = 0;
				tXAxisStepper.State = 0;
				tXAxisStepper.cCurSpeed = 0;
			}				
		}
		else
			tXAxisStepper.cZeroFilter = 0;			
	}
    else if (tXAxisStepper.State == 1) 
	{
            if (secondHoleFlg)
            {
                tXAxisStepper.cCurSpeed = tXAxisStepper.cSetSpeed >> 1;
                MTU7.TGRA = FreqTab_X[tXAxisStepper.cCurSpeed];
                MTU7.TGRB = FreqTab_X[tXAxisStepper.cCurSpeed] >> 1;
            }
            else if (tXAxisStepper.cCurSpeed < tXAxisStepper.cSetSpeed)
            {
                if (++tXAxisStepper.cAccDecVal >= tXAxisStepper.cSetAccVal)
                {
                    tXAxisStepper.cAccDecVal = 0;
                    tXAxisStepper.cCurSpeed++;
                    MTU7.TGRA = FreqTab_X[tXAxisStepper.cCurSpeed];
                    MTU7.TGRB = FreqTab_X[tXAxisStepper.cCurSpeed] >> 1;
                }
            } 
            else 
                tXAxisStepper.State = 0;
        }	
	else if (tXAxisStepper.State == 2) 
	{
            if (tXAxisStepper.cCurSpeed > tXAxisStepper.cSetStartSpeed)
            {
                if (++tXAxisStepper.cAccDecVal >= tXAxisStepper.cSetDecVal) 
                {
                    tXAxisStepper.cAccDecVal = 0;
                    tXAxisStepper.cCurSpeed--;
                    MTU7.TGRA = FreqTab_X[tXAxisStepper.cCurSpeed];
                    MTU7.TGRB = FreqTab_X[tXAxisStepper.cCurSpeed] >> 1;
                }
            } 
            else 
            {
                X_AXIS_PULSE_OFF();
                tXAxisStepper.State = 0;
                tXAxisStepper.cCurSpeed = 0;
            }
        }
    }
}
/********************************************

MTU8-PWM脉冲输出中断程序

*********************************************/
#pragma interrupt ( MTU8_TGRA( vect = VECT( MTU8,TGIA8), enable ) )
void  MTU8_TGRA(void)
{
    MTU8_PWMProc();
}

void MTU8_PWMProc(void)
{// y zhou
	if(bYDirction)
		tYAxisStepper.cRealPosi--;
	else
		tYAxisStepper.cRealPosi++;
		
	if(tYAxisStepper.bAuto_Mov)
	{
		if(--tYAxisStepper.cPmovPulse == 0)
		{
			tYAxisStepper.bAuto_Mov = 0;
			Y_AXIS_PULSE_OFF();			
			tYAxisStepper.cCurSpeed = 0;
		}
		else if(tYAxisStepper.cPmovPulse > tYAxisStepper.cCalDecPulse)
		{
			if(tYAxisStepper.cCurSpeed < tYAxisStepper.cCalSpeed)
			{
				if(++tYAxisStepper.cAccDecVal >= tYAxisStepper.cSetAccVal)
				{
					tYAxisStepper.cAccDecVal = 0;	
					tYAxisStepper.cCurSpeed++;
					MTU8.TGRA = FreqTab_Y[tYAxisStepper.cCurSpeed];
					MTU8.TGRB = FreqTab_Y[tYAxisStepper.cCurSpeed]>>1;
				}	
			}
		}	
		else
		{
			if(tYAxisStepper.cCurSpeed > tYAxisStepper.cSetStartSpeed)
			{
				if(++tYAxisStepper.cAccDecVal >= tYAxisStepper.cSetDecVal)
				{
					tYAxisStepper.cAccDecVal = 0;	
					tYAxisStepper.cCurSpeed--;
					MTU8.TGRA = FreqTab_Y[tYAxisStepper.cCurSpeed];
					MTU8.TGRB = FreqTab_Y[tYAxisStepper.cCurSpeed]>>1;
				}	
			}
		}		
	}
	else
	{	
		if(tYAxisStepper.State == 3)
		{
			if(Y_ZERO)
			{
				tYAxisStepper.cZeroFilter++;
				if(tYAxisStepper.cZeroFilter >= 50)
				{
					Y_AXIS_PULSE_OFF();						
					tYAxisStepper.cRealPosi = 0;
					//tXAxisStepper.cRstStep = 0;
					tYAxisStepper.State = 0;
					tYAxisStepper.cCurSpeed = 0;
				}				
			}
			else
				tYAxisStepper.cZeroFilter = 0;
		}
		else if(tYAxisStepper.State == 1)
		{
			if(tYAxisStepper.cCurSpeed < tYAxisStepper.cSetSpeed)
			{
				if(++tYAxisStepper.cAccDecVal >= tYAxisStepper.cSetAccVal)
				{
					tYAxisStepper.cAccDecVal = 0;	
					tYAxisStepper.cCurSpeed++;
					MTU8.TGRA = FreqTab_Y[tYAxisStepper.cCurSpeed];
					MTU8.TGRB = FreqTab_Y[tYAxisStepper.cCurSpeed]>>1;
				}	
			}
			else 
				tYAxisStepper.State = 0;
		}	
		else if(tYAxisStepper.State == 2)
		{
			if(tYAxisStepper.cCurSpeed > tYAxisStepper.cSetStartSpeed)
			{
				if(++tYAxisStepper.cAccDecVal >= tYAxisStepper.cSetDecVal)
				{
					tYAxisStepper.cAccDecVal = 0;	
					tYAxisStepper.cCurSpeed--;
					MTU8.TGRA= FreqTab_Y[tYAxisStepper.cCurSpeed];
					MTU8.TGRB= FreqTab_Y[tYAxisStepper.cCurSpeed]>>1;
				}	
			}
			else 
			{
				Y_AXIS_PULSE_OFF();			
				tYAxisStepper.State = 0;
				tYAxisStepper.cCurSpeed = 0;
			}	
		}
	}	
}
/********************************************

MTU10-PWM脉冲输出中断程序

*********************************************/
#pragma interrupt ( MTU10_TGRA( vect = VECT( MTU10,TGIA10), enable ) )
void  MTU10_TGRA(void)
{}
/********************************************

MTU9-PWM脉冲输出中断程序

*********************************************/
#pragma interrupt ( MTU9_TGRA( vect = VECT( MTU9,TGIA9), enable ) )
void  MTU9_TGRA(void)
{}







void LCalRunPulse(StepperDataInfo *tCurSetStpper) {

    if (tCurSetStpper->cDestPosi & 0x80000000) {
        tCurSetStpper->cPmovPulse = 0 - tCurSetStpper->cDestPosi;
        if (tCurSetStpper->cAxisNo == X_AXIS) DR_CCW_X();
        else if (tCurSetStpper->cAxisNo == Y_AXIS) DR_CCW_Y();
        else tCurSetStpper->cPmovPulse = 0;
    } else {
        tCurSetStpper->cPmovPulse = tCurSetStpper->cDestPosi;
        if (tCurSetStpper->cAxisNo == X_AXIS) DR_CW_X();
        else if (tCurSetStpper->cAxisNo == Y_AXIS) DR_CW_Y();
        else tCurSetStpper->cPmovPulse = 0;
    }
    tCurSetStpper->cCalAccPulse = tCurSetStpper->cSetAccVal * (tCurSetStpper->cSetSpeed - tCurSetStpper->cSetStartSpeed);
    tCurSetStpper->cCalDecPulse = tCurSetStpper->cSetDecVal * (tCurSetStpper->cSetSpeed - tCurSetStpper->cSetStartSpeed);
    tCurSetStpper->cCurSpeed = tCurSetStpper->cSetStartSpeed;
    tCurSetStpper->cAccDecVal = 0;

    if (tCurSetStpper->cPmovPulse < (tCurSetStpper->cCalAccPulse + tCurSetStpper->cCalDecPulse)) {
        tCurSetStpper->cCalSpeed = tCurSetStpper->cPmovPulse / (tCurSetStpper->cSetAccVal + tCurSetStpper->cSetDecVal);
        tCurSetStpper->cCalDecPulse = tCurSetStpper->cSetDecVal * (tCurSetStpper->cCalSpeed - tCurSetStpper->cSetStartSpeed);
    } else {
        tCurSetStpper->cCalSpeed = tCurSetStpper->cSetSpeed;
    }
}

void LMV_Set_Speed(unsigned short cAxisNo, unsigned short cSpeed) 
{
    if (cAxisNo < AXIS_NUM)
        StepInfo[cAxisNo]->cSetSpeed = cSpeed;
}

long LMV_Get_Command_Pos(unsigned short cAxisNo) {
    long cRealPosi;

    if (cAxisNo < AXIS_NUM) cRealPosi = StepInfo[cAxisNo]->cRealPosi;
    return cRealPosi;
}

void LMV_Pmove(unsigned short cAxisNo, unsigned long dwPosi)
{
    if (cAxisNo < AXIS_NUM)
    {
        StepInfo[cAxisNo]->cDestPosi = dwPosi - StepInfo[cAxisNo]->cRealPosi;
        StepInfo[cAxisNo]->cSetStartSpeed = 3;
        StepInfo[cAxisNo]->State = 0;
        StepInfo[cAxisNo]->cAccDecVal = 0;
        LCalRunPulse(StepInfo[cAxisNo]);
    }
}

void LMV_Pmove2(unsigned short cAxisNo, unsigned long dwPosi)
{
    if (cAxisNo < AXIS_NUM)
    {
        StepInfo[cAxisNo]->cDestPosi = dwPosi - StepInfo[cAxisNo]->cRealPosi;
        StepInfo[cAxisNo]->cSetStartSpeed = 0;
        StepInfo[cAxisNo]->State = 0;
        StepInfo[cAxisNo]->cAccDecVal = 0;
        LCalRunPulse(StepInfo[cAxisNo]);
    }
}

void MV_Set_Limit_P(unsigned short cAxisNo, unsigned long dwMaxLen) {
    if (cAxisNo < AXIS_NUM) StepInfo[cAxisNo]->cMaxLen = dwMaxLen;
}

void LStartPWM(unsigned short cAxisNo)
{
    if (cAxisNo == X_AXIS)
    {
        tXAxisStepper.cCurSpeed = tXAxisStepper.cSetStartSpeed;
        MTU7.TGRA = FreqTab_X[tXAxisStepper.cSetStartSpeed];
        //MTU7.TGRC = FreqTab_X[tXAxisStepper.cSetStartSpeed];
        MTU7.TGRB = FreqTab_X[tXAxisStepper.cSetStartSpeed] >> 1;
        X_AXIS_PULSE_ON();
        X_DRV = 1;
    }
    else if (cAxisNo == Y_AXIS)
    {
        tYAxisStepper.cCurSpeed = tYAxisStepper.cSetStartSpeed;
        MTU8.TGRA = FreqTab_Y[tYAxisStepper.cSetStartSpeed];
        //MTU8.TGRC = FreqTab_Y[tYAxisStepper.cSetStartSpeed];
        MTU8.TGRB = FreqTab_Y[tYAxisStepper.cSetStartSpeed] >> 1;
        Y_AXIS_PULSE_ON();
        Y_DRV = 1;
    }
}


void LMV_Suddent_Stop(unsigned short cAxisNo) 
{
    if (cAxisNo < AXIS_NUM)
    {
        switch (cAxisNo) 
        {
        case X_AXIS:
            X_AXIS_PULSE_OFF();
            tXAxisStepper.bAuto_Mov = 0;
            break;
        case Y_AXIS:
            Y_AXIS_PULSE_OFF();
            tYAxisStepper.bAuto_Mov = 0;
            break;
        }
        StepInfo[cAxisNo]->bAuto_Mov = 0;
        StepInfo[cAxisNo]->cRstStep = 0;
        StepInfo[cAxisNo]->State = 0;
        StepInfo[cAxisNo]->cCurSpeed = 0;
    }
}

void LMV_Dec_Stop(unsigned short cAxisNo) 
{
    if (cAxisNo < AXIS_NUM) 
    {
        StepInfo[cAxisNo]->bAuto_Mov = 0;
        StepInfo[cAxisNo]->State = 2;
    }
}

void LMV_Const_Move(unsigned short cAxisNo, unsigned short cSpeed, unsigned short dir)
{
    unsigned char sta;

    if (cAxisNo < AXIS_NUM)
    {
        StepInfo[cAxisNo]->bAuto_Mov = 0;
        switch (cAxisNo) 
        {
        case X_AXIS:
            sta = X_PWM_STATE;
            if (dir) DR_CCW_X();
            else DR_CW_X();
            break;
        case Y_AXIS:
            sta = Y_PWM_STATE;
            if (dir) DR_CCW_Y();
            else DR_CW_Y();
            break;
        }
        //如果电机是停止的，就需要启动一次
        if (sta == 0) 
        {
            StepInfo[cAxisNo]->State = 1;
            StepInfo[cAxisNo]->cSetSpeed = cSpeed;
            StepInfo[cAxisNo]->cSetStartSpeed = 3;
            StepInfo[cAxisNo]->cAccDecVal = 0;
            LStartPWM(cAxisNo);
        }
    }
}

/*
void DR_CW_X(void)
{
    PORTB.DR.BIT.B1 = 0;
}

void DR_CCW_X(void)
{
    PORTB.DR.BIT.B1 = 1;
}

void DR_CW_Y(void)
{

    PORTB.DR.BIT.B5 = 0;
}

void DR_CCW_Y(void)
{
    PORTB.DR.BIT.B5 = 1;
}
*/

void ReadAxisStatus(void)
{
    if (X_PWM_STATE)
        X_DRV = 1;
    else 
        X_DRV = 0;
    if (Y_PWM_STATE) 
        Y_DRV = 1;
    else 
        Y_DRV = 0;
    
/*
    if(Z_PWM_STATE)
        Z_DRV = 1;	
    else
        Z_DRV = 0;

    if(A_PWM_STATE)
        A_DRV = 1;	
    else
        A_DRV = 0;
*/
}

void MV_AccDec_Set(unsigned short cAxisNo, unsigned short cSetAcc, unsigned short cSetDec)
{
    if (cAxisNo < AXIS_NUM) 
    {
        StepInfo[cAxisNo]->cSetAccVal = cSetAcc;
        StepInfo[cAxisNo]->cSetDecVal = cSetDec;
        StepInfo[cAxisNo]->cAccDecVal = 0;
    }
}

void LMV_Move_To_Position(unsigned short cAxisNo, long dwPosi)
{
    if (cAxisNo < AXIS_NUM) 
    {
        if (dwPosi < StepInfo[cAxisNo]->cRealPosi) 
        {
            LMV_Dec_Stop(cAxisNo);
        } 
        else 
        {
            StepInfo[cAxisNo]->cDestPosi = dwPosi - StepInfo[cAxisNo]->cRealPosi;
            StepInfo[cAxisNo]->cPmovPulse = StepInfo[cAxisNo]->cDestPosi;
            //StepInfo[cAxisNo]->cSetStartSpeed = 0;
            StepInfo[cAxisNo]->State = 0;
            StepInfo[cAxisNo]->cAccDecVal = 0;
            StepInfo[cAxisNo]->cCalSpeed=  StepInfo[cAxisNo]->cSetSpeed;
            StepInfo[cAxisNo]->cCalDecPulse =50+ StepInfo[cAxisNo]->cSetDecVal * (StepInfo[cAxisNo]->cSetSpeed - StepInfo[cAxisNo]->cSetStartSpeed);
            StepInfo[cAxisNo]->bAuto_Mov = 1;
        }
    }
}

