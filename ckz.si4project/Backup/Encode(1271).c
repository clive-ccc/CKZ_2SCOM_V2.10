#include "Global.h"

#define PIN_7   0x80//(1 << 7)
#define PIN_6   0x40//(1 << 6)

//#define ENCODER_LINE_A PORTE.PORT.BIT.B7
//#define ENCODER_LINE_B PORTE.PORT.BIT.B6
#define ENCODER_LINE_A (PORTE.PORT.BYTE & PIN_7)
#define ENCODER_LINE_B (PORTE.PORT.BYTE & PIN_6)

//#define Extern_Interrupt PORT0.PORT.BIT.B7
#define Extern_Interrupt (PORT0.PORT.BYTE & PIN_7)

volatile long ulEncoderCount=0;

void EncoderInit(void)
{
	IEN(ICU,IRQ6)=0;
	IEN(ICU,IRQ7)=0;
	IOPORT.PF9IRQ.BIT.ITS6 = 0; // PE6
	IOPORT.PF9IRQ.BIT.ITS7 = 0;	// PE7
	ICU.IRQCR[6].BYTE= 0x0C;
	ICU.IRQCR[7].BYTE= 0x0C;
	IR(ICU,IRQ6)=0;
	IR(ICU,IRQ7)=0;
	IPR(ICU,IRQ6)=4;
	IPR(ICU,IRQ7)=5;
	IEN(ICU,IRQ6)=1;
	IEN(ICU,IRQ7)=1;
}

void IOInterruptInit(void)
{
	IEN(ICU,IRQ15)=0;
	IOPORT.PF8IRQ.BIT.ITS15 = 0; // P07
	ICU.IRQCR[15].BYTE= 0x0C;//双边延触发
	IR(ICU,IRQ15)=0;
	IPR(ICU,IRQ15)=6;
	IEN(ICU,IRQ15)=1;
}
static unsigned char PinStatus[3] = {0};

#pragma interrupt (EncodeA_IRQm(vect=70))
static void EncodeA_IRQm(void)
{
    PinStatus[0] = ENCODER_LINE_A;
    PinStatus[1] = ENCODER_LINE_B;
	if (PinStatus[0]) // == 1
	{
		//读B线状态
		if (!PinStatus[1]) 
		{
			//A上升沿的时候如果B是低电平，那就是正转
			ulEncoderCount++;		
		} 
		else 
		{
			//如果B是低则是反转
			ulEncoderCount--;
		}
	} 
	else
	{		
		//读B线状态
		if (PinStatus[1])
		{
			//A下降沿的时候如果B是高，那就是正转
			ulEncoderCount++;
		} 
		else
		{
			//如果B是高则是反转
			ulEncoderCount--;	
		}
		//#endif
	}
	 /*       	if (flgSgn1 != flgSgnOld1)
            {  
                flgSgnOld1 = flgSgn1;
                if(flgSgn1 > 0)// 上升沿
                {
                    HoleinfoB[CheckHoleNum[0]].hole_point = ulEncoderCount;
                    CheckHoleNum[0]++;
                }
                                if(flgSgn1 < 1)// 下降沿
                {
                    HoleinfoF[CheckHoleNum[1]].hole_point = ulEncoderCount;
                    CheckHoleNum[1]++;
                }
            }*/
}
// IRQ7
#pragma interrupt (EncodeB_IRQm(vect=71))
static void EncodeB_IRQm(void)
{
    PinStatus[0] = ENCODER_LINE_A;
    PinStatus[1] = ENCODER_LINE_B;

		//读B线状态
	if (PinStatus[1])
	{
		//读A线状态
		if (PinStatus[0])
		{
			//B上升沿的时候如果A是高电平，那就是正转
			ulEncoderCount++;		
		} 
		else
		{
			//如果A是低则是反转
			ulEncoderCount--;
		}
	} 
	else
	{
		//读A线状态
		if (!PinStatus[0])
		{
			//A下降沿的时候如果A是高电平，那就是正转
			ulEncoderCount++;
		} 
		else
		{
			//如果B是高则是反转
			ulEncoderCount--;	
		}
	}	
}
//irq15
#pragma interrupt (IOInterrupt(vect=79))
static void IOInterrupt(void)
{
    if(StartFlag > 0)
    {
        PinStatus[2] = Extern_Interrupt;
        if(PinStatus[2] > 0)
        {// 上升沿
            HoleinfoB[CheckHoleNum[0]].hole_point = ulEncoderCount;
            CheckHoleNum[0]++;
        }
        else
        {// 下降沿
            HoleinfoF[CheckHoleNum[1]].hole_point = ulEncoderCount;
            CheckHoleNum[1]++;
        }
    }
}