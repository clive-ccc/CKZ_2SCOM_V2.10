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
	ICU.IRQCR[15].BYTE= 0x0C;//˫���Ӵ���
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
	 /*       	if (flgSgn1 != flgSgnOld1)
            {  
                flgSgnOld1 = flgSgn1;
                if(flgSgn1 > 0)// ������
                {
                    HoleinfoB[CheckHoleNum[0]].hole_point = ulEncoderCount;
                    CheckHoleNum[0]++;
                }
                                if(flgSgn1 < 1)// �½���
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
//irq15
#pragma interrupt (IOInterrupt(vect=79))
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