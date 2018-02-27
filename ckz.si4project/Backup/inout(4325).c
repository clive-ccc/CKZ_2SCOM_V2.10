/*
 * inout.c
 *
 *  Created on: 2011-9-24
 *      Author: lenovo
 */
#include "Global.h"
WORD_BITS cInputVal1;
WORD_BITS cOldInputVal1;
WORD_BITS cOutputVal;
unsigned char InputFiltering[40];				//ÊäÈëÂË²¨¼Ä´æÆ÷

void GpioInit(void)
{
#ifdef BOARD_12007
	//if(cDataArea[400] == 12007 || cDataArea[400] == 12008)
	{
		PORT0.DDR.BYTE = 0x00;		
		PORT0.DR.BYTE = 0x00;		
		PORT0.ICR.BYTE = 0xAF;		

		PORT1.DDR.BYTE = 0x0c;		
		PORT1.DR.BYTE = 0x0c;		
		PORT1.ICR.BYTE = 0xf0;
		PORT1.ODR.BYTE=0x0c;

		PORT2.DDR.BYTE = 0x04;
		PORT2.DR.BYTE = 0x00;	
		PORT2.ICR.BYTE = 0x3B;	

		PORT3.DDR.BYTE = 0x00;	
		PORT3.DR.BYTE = 0x00;	
		PORT3.ICR.BYTE = 0x00;	
	        
		PORT4.DDR.BYTE = 0x0;	
		PORT4.DR.BYTE = 0x0;	
		PORT4.ICR.BYTE = 0xff;	

		PORT5.DDR.BYTE = 0x05;		
		PORT5.DR.BYTE = 0x05;		
		PORT5.ICR.BYTE = 0x7A;		

		PORT6.DDR.BYTE = 0xFF;		
		PORT6.DR.BYTE = 0xFF;		
		PORT6.ICR.BYTE = 0x00;		

		PORT7.DDR.BYTE = 0x07;		
		PORT7.DR.BYTE = 0x07;		
		PORT7.ICR.BYTE = 0x00;		

		PORT8.DDR.BYTE = 0x07;		
		PORT8.DR.BYTE = 0x07;		
		PORT8.ICR.BYTE = 0x00;		

		PORT9.DDR.BYTE = 0x00;		
		PORT9.DR.BYTE = 0x00;		
		PORT9.ICR.BYTE = 0x0F;		

		PORTA.DDR.BYTE = 0xff;		
		PORTA.DR.BYTE = 0xf1;		
		PORTA.ICR.BYTE = 0x00;		
	//	PORTA.PCR.BYTE = 0xff;		

		PORTB.DDR.BYTE = 0x03;		
		PORTB.DR.BYTE = 0x03;		
		PORTB.ICR.BYTE = 0x00;		
	//	PORTB.PCR.BYTE = 0xff;		

		PORTC.DDR.BYTE = 0x70;		
		PORTC.DR.BYTE = 0x10;		
		PORTC.ICR.BYTE = 0x80;		
	//	PORTC.PCR.BYTE = 0xff;		

		PORTD.DDR.BYTE = 0x00;		
		PORTD.DR.BYTE = 0x00;		
		PORTD.ICR.BYTE = 0x00;		
	//	PORTD.PCR.BYTE = 0xff;		

		PORTE.DDR.BYTE = 0x00;		
		PORTE.DR.BYTE = 0x00;		
		PORTE.ICR.BYTE = 0x00;		
	//	PORTE.PCR.BYTE = 0xff;	
	}
#endif
#ifdef BOARD_14007
	{
		PORT0.DDR.BYTE = 0x01;		
		PORT0.DR.BYTE = 0x01;		
		PORT0.ICR.BYTE = 0xae;		

		PORT1.DDR.BYTE = 0xfc;		
		PORT1.DR.BYTE = 0xfc;		
		PORT1.ICR.BYTE = 0x00;
		PORT1.ODR.BYTE=0x0c;

		PORT2.DDR.BYTE = 0x3f;
		PORT2.DR.BYTE = 0x1f;	
		PORT2.ICR.BYTE = 0x00;	

		PORT3.DDR.BYTE = 0x00;	
		PORT3.DR.BYTE = 0x00;	
		PORT3.ICR.BYTE = 0x00;	
	        
		PORT4.DDR.BYTE = 0x0;	
		PORT4.DR.BYTE = 0x0;	
		PORT4.ICR.BYTE = 0xff;	

		PORT5.DDR.BYTE = 0x77;		
		PORT5.DR.BYTE = 0x77;		
		PORT5.ICR.BYTE = 0x00;		

		PORT6.DDR.BYTE = 0x00;		
		PORT6.DR.BYTE = 0x0;		
		PORT6.ICR.BYTE = 0xff;		

		PORT7.DDR.BYTE = 0xe0;		
		PORT7.DR.BYTE = 0xe0;		
		PORT7.ICR.BYTE = 0x07;		

		PORT8.DDR.BYTE = 0x00;		
		PORT8.DR.BYTE = 0x00;		
		PORT8.ICR.BYTE = 0xff;		

		PORT9.DDR.BYTE = 0x00;		
		PORT9.DR.BYTE = 0x00;		
		PORT9.ICR.BYTE = 0x0f;		

		PORTA.DDR.BYTE = 0xf8;		
		PORTA.DR.BYTE = 0xf8;		
		PORTA.ICR.BYTE = 0x07;		
	//	PORTA.PCR.BYTE = 0xff;		

		PORTB.DDR.BYTE = 0x33;		
		PORTB.DR.BYTE = 0x33;		
		PORTB.ICR.BYTE = 0x00;		
	//	PORTB.PCR.BYTE = 0xff;		

		PORTC.DDR.BYTE = 0xdc;		
		PORTC.DR.BYTE = 0x0c;		
		PORTC.ICR.BYTE = 0x00;		
	//	PORTC.PCR.BYTE = 0xff;		

		PORTD.DDR.BYTE = 0x00;		
		PORTD.DR.BYTE = 0x00;		
		PORTD.ICR.BYTE = 0xff;		
	//	PORTD.PCR.BYTE = 0xff;		

		PORTE.DDR.BYTE = 0x00;		
		PORTE.DR.BYTE = 0x00;		
		PORTE.ICR.BYTE = 0xff;		
	//	PORTE.PCR.BYTE = 0xff;	
	}
#endif
}

void ReadInput(void)
{	

    cOldInputVal1.Val = cInputVal1.Val;
	if(I000)
	{
		InputFiltering[0]++;
		if(InputFiltering[0]>=FILTERTIME_P)
		{
			InputFiltering[0]=FILTERTIME_P;
			bOldX00=X00;
			X00=1;
		}
	}
	else
	{
		if(InputFiltering[0])
		{
			InputFiltering[0]--;
		}
		if(InputFiltering[0]<=FILTERTIME_N)
		{
			InputFiltering[0]=0;
			bOldX00=X00;
			X00=0;
		}
	}
	//
	if(I001)
	{
		InputFiltering[1]++;
		if(InputFiltering[1]>=FILTERTIME_P)
		{
			InputFiltering[1]=FILTERTIME_P;
			bOldX01=X01;
			X01=1;
		}
	}
	else
	{
		if(InputFiltering[1])
		{
			InputFiltering[1]--;
		}
		if(InputFiltering[1]<=FILTERTIME_N)
		{
			InputFiltering[1]=0;
			bOldX01=X01;
			X01=0;
		}
	}
	//
	if(I002)
	{
		InputFiltering[2]++;
		if(InputFiltering[2]>=FILTERTIME_P)
		{
			InputFiltering[2]=FILTERTIME_P;
			bOldX02=X02;
			X02=1;
		}
	}
	else
	{
		if(InputFiltering[2])
		{
			InputFiltering[2]--;
		}
		if(InputFiltering[2]<=FILTERTIME_N)
		{
			InputFiltering[2]=0;
			bOldX02=X02;
			X02=0;
		}
	}
	//
	if(I003)
	{
		InputFiltering[3]++;
		if(InputFiltering[3]>=FILTERTIME_P)
		{
			InputFiltering[3]=FILTERTIME_P;
			bOldX03=X03;
			X03=1;
			
		}
	}
	else
	{
		if(InputFiltering[3])
		{
			InputFiltering[3]--;
		}
		if(InputFiltering[3]<=FILTERTIME_N)
		{
			InputFiltering[3]=0;
			bOldX03=X03;
			X03=0;
		}
	}
	//
	if(I004)
	{
		InputFiltering[4]++;
		if(InputFiltering[4]>=FILTERTIME_P)
		{
			InputFiltering[4]=FILTERTIME_P;
			bOldX04=X04;
			X04=1;
		}
	}
	else
	{
		if(InputFiltering[4])
		{
			InputFiltering[4]--;
		}
		if(InputFiltering[4]<=FILTERTIME_N)
		{
			InputFiltering[4]=0;
			bOldX04=X04;
			X04=0;
		}
	}
	//
	if(I005)
	{
		InputFiltering[5]++;
		if(InputFiltering[5]>=FILTERTIME_P)
		{
			InputFiltering[5]=FILTERTIME_P;
			bOldX05=X05;
			X05=1;
		}
	}
	else
	{
		if(InputFiltering[5])
		{
			InputFiltering[5]--;
		}
		if(InputFiltering[5]<=FILTERTIME_N)
		{
			InputFiltering[5]=0;
			bOldX05=X05;
			X05=0;
		}
	}
	//
	if(I006)
	{
		InputFiltering[6]++;
		if(InputFiltering[6]>=FILTERTIME_P)
		{
			InputFiltering[6]=FILTERTIME_P;
			bOldX06=X06;
			X06=1;
		}
	}
	else
	{
		if(InputFiltering[6])
		{
			InputFiltering[6]--;
		}
		if(InputFiltering[6]<=FILTERTIME_N)
		{
			InputFiltering[6]=0;
			bOldX06=X06;
			X06=0;
		}
	}
	//
	if(I007)
	{
		InputFiltering[7]++;
		if(InputFiltering[7]>=FILTERTIME_P)
		{
			InputFiltering[7]=FILTERTIME_P;
			bOldX07=X07;
			X07=1;
		}
	}
	else
	{
		if(InputFiltering[7])
		{
			InputFiltering[7]--;
		}
		if(InputFiltering[7]<=FILTERTIME_N)
		{
			InputFiltering[7]=0;
			bOldX07=X07;
			X07=0;
		}
	}
	//
	if(I010)
	{
		InputFiltering[10]++;
		if(InputFiltering[10]>=FILTERTIME_P)
		{
			InputFiltering[10]=FILTERTIME_P;
			bOldX10=X10;
			X10=1;
		}
	}
	else
	{
		if(InputFiltering[10])
		{
			InputFiltering[10]--;
		}
		if(InputFiltering[10]<=FILTERTIME_N)
		{
			InputFiltering[10]=0;
			bOldX10=X10;
			X10=0;
		}
	}
	//
	if(I011)
	{
		InputFiltering[11]++;
		if(InputFiltering[11]>=FILTERTIME_P)
		{
			InputFiltering[11]=FILTERTIME_P;
			bOldX11=X11;
			X11=1;
		}
	}
	else
	{
		if(InputFiltering[11])
		{
			InputFiltering[11]--;
		}
		if(InputFiltering[11]<=FILTERTIME_N)
		{
			InputFiltering[11]=0;
			bOldX11=X11;
			X11=0;
		}
	}
	//
	if(I012)
	{
		InputFiltering[12]++;
		if(InputFiltering[12]>=FILTERTIME_P)
		{
			InputFiltering[12]=FILTERTIME_P;
			bOldX12=X12;
			X12=1;
		}
	}
	else
	{
		if(InputFiltering[12])
		{
			InputFiltering[12]--;
		}
		if(InputFiltering[12]<=FILTERTIME_N)
		{
			InputFiltering[12]=0;
			bOldX12=X12;
			X12=0;
		}
	}
	//
	if(I013)
	{
		InputFiltering[13]++;
		if(InputFiltering[13]>=FILTERTIME_P)
		{
			InputFiltering[13]=FILTERTIME_P;
			bOldX13=X13;
			X13=1;
		}
	}
	else
	{
		if(InputFiltering[13])
		{
			InputFiltering[13]--;
		}
		if(InputFiltering[13]<=FILTERTIME_N)
		{
			InputFiltering[13]=0;
			bOldX13=X13;
			X13=0;
		}
	}
	//
	if(I014)
	{
		InputFiltering[14]++;
		if(InputFiltering[14]>=FILTERTIME_P)
		{
			InputFiltering[14]=FILTERTIME_P;
			bOldX14=X14;
			X14=1;
		}
	}
	else
	{
		if(InputFiltering[14])
		{
			InputFiltering[14]--;
		}
		if(InputFiltering[14]<=FILTERTIME_N)
		{
			InputFiltering[14]=0;
			bOldX14=X14;
			X14=0;
		}
	}
	//
	if(I015)
	{
		InputFiltering[15]++;
		if(InputFiltering[15]>=FILTERTIME_P)
		{
			InputFiltering[15]=FILTERTIME_P;
			bOldX15=X15;
			X15=1;
		}
	}
	else
	{
		if(InputFiltering[15])
		{
			InputFiltering[15]--;
		}
		if(InputFiltering[15]<=FILTERTIME_N)
		{
			InputFiltering[15]=0;
			bOldX15=X15;
			X15=0;
		}
	}
	if(I016)
	{
		InputFiltering[16]++;
		if(InputFiltering[16]>=FILTERTIME_P)
		{
			InputFiltering[16]=FILTERTIME_P;
			bOldX16=X16;
			X16=1;
		}
	}
	else
	{
		if(InputFiltering[16])
		{
			InputFiltering[16]--;
		}
		if(InputFiltering[16]<=FILTERTIME_N)
		{
			InputFiltering[16]=0;
			bOldX16=X16;
			X16=0;
		}
	}
}


void TestOutput(void)
{
  static unsigned long dwOutTime = 0;
  static unsigned short temp = 0;

  if(dwTickCount > dwOutTime)
  {
    dwOutTime = dwTickCount + 500;
	cOutputVal.Val=0;
    if(temp++ > 16)
      temp = 0;
  }

  if((cInputX00_X17 == 0))
  {
    
    switch(temp)
    {
      case 0:
	cOutputVal.Val=0xffff;
	break;
      case 1:
	Y00=1;
	break;
      case 2:
	Y01 = 1;
	break;
      case 3:
	Y02 = 1;
	break;
      case 4:
	Y03 = 1;
	break;
      case 5:
	Y04 = 1;
	break;
      case 6:
	Y05 = 1;
	break;
      case 7:
	Y06 = 1;
        break;
      case 8:
        Y07 = 1;
	break;
      case 9:
	Y10 = 1;
	break;
      case 10:
	Y11 = 1;
	break;
      case 11:
	Y12 = 1;
        break;
      case 12:
	Y13 = 1;
        break;
      case 13:
	Y14 = 1;
	break;
      case 14:
	Y15 = 1;
	break;
      case 15:
	Y16 = 1;
	break;
      case 16:
	Y17 = 1;
	break;
      default:
	break;
    }
  }
  else
  {
    if(X00)
      Y00 = 1;
    if(X01)
      Y01 = 1;
    if(X02)
      Y02 = 1;
    if(X03)
      Y03 = 1;
    if(X04)
      Y04 = 1;
    if(X05)
      Y05 = 1;
    if(X06)
      Y06 = 1;
    if(X07)
      Y07 = 1;
    if(X10)
      Y10 = 1;
    if(X11)
      Y11 = 1;
    if(X12)
      Y12 = 1;
    if(X13)
      Y13 = 1;
    if(X14)
      Y14 = 1;
    if(X15)
      Y15 = 1;
    if(X16)
      Y16 = 1;
    if(X17)
      Y17 = 1;   
  }

}

void WriteOutput(void)
{
	OUT00 = !Y00;	
	OUT01 = !Y01;	
	OUT02 = !Y02;	
	OUT03 = !Y03;	
	OUT04 = !Y04;	
	OUT05 = !Y05;	
	OUT06 = !Y06;	
}

