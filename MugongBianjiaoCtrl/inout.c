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

