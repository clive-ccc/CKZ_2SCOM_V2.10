#include "Global.h"

#define PIN_7   0x80//(1 << 7)
#define PIN_6   0x40//(1 << 6)
#define PIN_5   0x20//(1 << 7)
#define PIN_4   0x10//(1 << 6)
#define PIN_3   0x08//(1 << 6)
#define PIN_2   0x04//(1 << 6)
//#define ENCODER_LINE_A PORTE.PORT.BIT.B7
//#define ENCODER_LINE_B PORTE.PORT.BIT.B6
#define ENCODER_LINE_A (PORT1.PORT.BYTE & PIN_5)
#define ENCODER_LINE_B (PORT1.PORT.BYTE & PIN_4)

//#define Extern_Interrupt PORT0.PORT.BIT.B7
#define Extern_Interrupt (PORT0.PORT.BYTE & PIN_2)

volatile long ulEncoderCount=0;

void EncoderInit(void)
{
	IEN(ICU,IRQ5)=0;
	IEN(ICU,IRQ4)=0;
	IOPORT.PF9IRQ.BIT.ITS5 = 1; // P15 irq5
	IOPORT.PF9IRQ.BIT.ITS4 = 1;	// P14 irq4
	ICU.IRQCR[5].BYTE= 0x0C;
	ICU.IRQCR[4].BYTE= 0x0C;
	IR(ICU,IRQ5)=0;
	IR(ICU,IRQ4)=0;
	IPR(ICU,IRQ5)=4;
	IPR(ICU,IRQ4)=5;
	IEN(ICU,IRQ5)=1;
	IEN(ICU,IRQ4)=1;
}

void IOInterruptInit(void)
{
	IEN(ICU,IRQ10)=0;
	IOPORT.PF8IRQ.BIT.ITS10 = 0; // P02 irq10
	ICU.IRQCR[10].BYTE= 0x0C;//˫���Ӵ���
	IR(ICU,IRQ10)=0;
	IPR(ICU,IRQ10)=6;
	IEN(ICU,IRQ10)=1;
}
static unsigned char PinStatus[3] = {0};

// IRQ4
#pragma interrupt (EncodeA_IRQm(vect=68))
static void EncodeA_IRQm(void)
{
    PinStatus[0] = ENCODER_LINE_A;
    PinStatus[1] = ENCODER_LINE_B;
	if (PinStatus[0]) // == 1
	{
		//��B��״̬
		if (!PinStatus[1]) 
		{
			//A�����ص�ʱ�����B�ǵ͵�ƽ���Ǿ�����ת
			ulEncoderCount++;		
		} 
		else 
		{
			//���B�ǵ����Ƿ�ת
			ulEncoderCount--;
		}
	} 
	else
	{		
		//��B��״̬
		if (PinStatus[1])
		{
			//A�½��ص�ʱ�����B�Ǹߣ��Ǿ�����ת
			ulEncoderCount++;
		} 
		else
		{
			//���B�Ǹ����Ƿ�ת
			ulEncoderCount--;	
		}
		//#endif
	}
}
// IRQ5
#pragma interrupt (EncodeB_IRQm(vect=69))
static void EncodeB_IRQm(void)
{
    PinStatus[0] = ENCODER_LINE_A;
    PinStatus[1] = ENCODER_LINE_B;

		//��B��״̬
	if (PinStatus[1])
	{
		//��A��״̬
		if (PinStatus[0])
		{
			//B�����ص�ʱ�����A�Ǹߵ�ƽ���Ǿ�����ת
			ulEncoderCount++;		
		} 
		else
		{
			//���A�ǵ����Ƿ�ת
			ulEncoderCount--;
		}
	} 
	else
	{
		//��A��״̬
		if (!PinStatus[0])
		{
			//A�½��ص�ʱ�����A�Ǹߵ�ƽ���Ǿ�����ת
			ulEncoderCount++;
		} 
		else
		{
			//���B�Ǹ����Ƿ�ת
			ulEncoderCount--;	
		}
	}	
}
//irq10
#pragma interrupt (IOInterrupt(vect=74))
static void IOInterrupt(void)
{
    if(StartFlag > 0)
    {
        PinStatus[2] = Extern_Interrupt;
        if(PinStatus[2] > 0)
        {// ������
            HoleinfoB[CheckHoleNum[0]].hole_point = ulEncoderCount;
            CheckHoleNum[0]++;
        }
        else
        {// �½���
            HoleinfoF[CheckHoleNum[1]].hole_point = ulEncoderCount;
            CheckHoleNum[1]++;
        }
    }
}