/*
 * modbusASC.c
 *
 *  Created on: 2011-10-14
 *      Author: lenovo
 */

#include "Global.h"
#include "uart.h"
#include "modusASC.h"
#include "Action.h"

#define  CMD_READ_COIL			0x02
#define  CMD_READ_REGISTER		0x03
#define  CMD_WRITE_COIL			0x05
#define  CMD_WRITE_REGISTER		0x06
#define  CMD_WRITE_MULREGISTER	0x10

uint8 cRxASCToBinBuf[128];


const uint8 BinToAscTab[] = {'0','1','2','3','4','5','6','7','8','9',
							'A','B','C','D','E','F'};

WORD_BITS  cMidleCoil[50];
unsigned short cDataArea[512];
//对接收的数据进行和检�?
uint8  CheckReceiveSum(void)
{
	uint8 i,j,sum,temp;

	j = (SCI0Data.RxPosi >> 1) - 1;
	sum = 0;

	for(i = 0; i < j; i++)
	{
		temp = SCI0Data.RxBuf[2*i + 1] & 0x7f;

		if(temp >= 'A')
			cRxASCToBinBuf[i] = temp - 0x37;
		else
			cRxASCToBinBuf[i] = temp - 0x30;
		cRxASCToBinBuf[i] <<= 4;

		temp = SCI0Data.RxBuf[2*i + 2] & 0x7f;

		if(temp >= 'A')
			cRxASCToBinBuf[i] += (temp - 0x37);
		else
			cRxASCToBinBuf[i] += (temp - 0x30);
		sum = sum + cRxASCToBinBuf[i] ;
	}
	return(sum);
}// CheckReceiveSum(void)

extern unsigned short cEnableRxDelay;

void CheckReceData(void)
{
	if((SCI0Data.cRxOK == 1) && (cEnableRxDelay == 0))
	{
//		CheckReceiveSum();
		if(CheckReceiveSum() == 0)
		{
			switch(cRxASCToBinBuf[1])
			{
				case CMD_READ_COIL:
					ReadCoil();
					break;
				case CMD_READ_REGISTER:
					ReadRegister();
					break;
				case CMD_WRITE_COIL:
					WriteCoil();
					break;
				case CMD_WRITE_REGISTER:
					WriteRegister();
					break;
				case CMD_WRITE_MULREGISTER:
					WriteMulRegister();
					break;
				default :
					break;
			}
		}
		SCI0Data.cRxOK = 0;
		SCI0Data.RxPosi = 0;
	}
}// void CheckReceData(void)

void ReadCoil(void)
{
	unsigned short i,j,k,m,temp,temp1,sum = 0;
    unsigned char  * coils = (unsigned char *)(&cMidleCoil[0]);
	unsigned short cRXbuffertmp;
	if(cRxASCToBinBuf[2]>=8)
	{
		cRXbuffertmp=(cRxASCToBinBuf[2]-8)<<8;
	}
	cRXbuffertmp=cRXbuffertmp+	cRxASCToBinBuf[3];
	i = (cRXbuffertmp >> 3);
	if(i >= 100)
	{
		i=0;
	}
	temp = cRxASCToBinBuf[3] & 0x7;

	SCI0Data.TxBuf[3] = '0';
	SCI0Data.TxBuf[4] = '2';

	m = cRxASCToBinBuf[5]/8;
	
	if((cRxASCToBinBuf[5] - 8*m) > 0)
		m++;

	k = (m>>4) & 0xf;
	SCI0Data.TxBuf[5] = BinToAscTab[k];

	k = m &0xf;
	SCI0Data.TxBuf[6] = BinToAscTab[k];
		
	for(j = 0; j < m; j++)
	{		
		k= cRxASCToBinBuf[3] & 0x7;
		if(temp == 0)
			temp1 = coils[i];
		else
			temp1 = (coils[i] >> k) | (coils[i+1] << (8 - k));

		k = (temp1 >> 4) & 0xf;
		SCI0Data.TxBuf[7+j*2] = BinToAscTab[k];

		k = temp1 & 0xf;
		SCI0Data.TxBuf[8+j*2] = BinToAscTab[k];

		sum =sum+ temp1;
		i++;
	}
	sum = sum + 3 + m;
	sum = ((~sum)+1) & 0xff;
	j = 7 + 2*m;

	k = (sum >> 4) & 0xf;
	SCI0Data.TxBuf[j] = BinToAscTab[k];

	k = sum & 0xf;
	SCI0Data.TxBuf[j+1] = BinToAscTab[k];

	SCI0Data.TxBuf[j+2] = 0x0d;
	SCI0Data.TxBuf[j+3] = 0x0a;
	SCI0Data.TxLen = j+3;

	SCI0_TxStart();
}

void ReadRegister(void)
{
	unsigned short i,j,k,sum=0;

	i = ((cRxASCToBinBuf[2] &0xf) << 8) + cRxASCToBinBuf[3];

	SCI0Data.TxBuf[3] = '0';
	SCI0Data.TxBuf[4] = '3';
	j = (cRxASCToBinBuf[5] << 1);
	k = (j>>4) & 0xf;
	SCI0Data.TxBuf[5] = BinToAscTab[k];

	k = j &0xf;
	SCI0Data.TxBuf[6] = BinToAscTab[k];

	for(j = 0; j< cRxASCToBinBuf[5]; j++)
	{

		k = cDataArea[i] >> 12;
		SCI0Data.TxBuf[7+j*4] = BinToAscTab[k];

		k = (cDataArea[i] >> 8) & 0xf;
		SCI0Data.TxBuf[8+j*4] = BinToAscTab[k];

		k = (cDataArea[i] >> 4) & 0xf;
		SCI0Data.TxBuf[9+j*4] = BinToAscTab[k];

		k = cDataArea[i] & 0xf;
		SCI0Data.TxBuf[10+j*4] = BinToAscTab[k];

		sum =sum+ (cDataArea[i] >> 8) + (cDataArea[i] & 0xff);
		i++;
	}
	sum = sum + 4 + (cRxASCToBinBuf[5]<<1);
	sum = ((~sum)+1) & 0xff;
	j = 7 + 4*cRxASCToBinBuf[5];

	k = (sum >> 4) & 0xf;
	SCI0Data.TxBuf[j] = BinToAscTab[k];

	k = sum & 0xf;
	SCI0Data.TxBuf[j+1] = BinToAscTab[k];

	SCI0Data.TxBuf[j+2] = 0x0d;
	SCI0Data.TxBuf[j+3] = 0x0a;
	SCI0Data.TxLen = j+3;

	SCI0_TxStart();
}

void WriteCoil(void)
{
unsigned short j,k;
		unsigned short cRXbuffertmp;
        unsigned char  * coils = (unsigned char *)(&cMidleCoil[0]);
	if(cRxASCToBinBuf[2]>=8)
	{
		cRXbuffertmp=(cRxASCToBinBuf[2]-8)<<8;
	}
	cRXbuffertmp=cRXbuffertmp+	cRxASCToBinBuf[3];

    j = cRXbuffertmp >> 3;
	k = cRxASCToBinBuf[3] & 0x7;
	if(cRxASCToBinBuf[4] == 0xff)
		coils[j] |= (1 << k);
	else
		coils[j] = (coils[j] & (~(1 << k)));

	TouchCommOK();
}

void WriteRegister(void)
{
	unsigned short i;
	i = ((cRxASCToBinBuf[2]&0xf) << 8 ) + cRxASCToBinBuf[3];

	cDataArea[i] = ((unsigned short)cRxASCToBinBuf[4] << 8) + cRxASCToBinBuf[5];

	Write24C04(i*2,cRxASCToBinBuf[5]);
	Write24C04(i*2+1,cRxASCToBinBuf[4]);    
	TouchCommOK();
}

void WriteMulRegister(void)
{
	unsigned char j,sum = 0;
	unsigned short ch;
	unsigned short i;

	i = ((cRxASCToBinBuf[2] & 0xf) << 8 ) + cRxASCToBinBuf[3];
	for(j = 0;j < cRxASCToBinBuf[5];j++)
	{
		ch = i+j;
		cDataArea[ch] = ((unsigned short)cRxASCToBinBuf[2*j+7] << 8) + cRxASCToBinBuf[2*j+8];
		
		Write24C04(ch*2, cRxASCToBinBuf[2*j+8]);
        Write24C04(ch * 2 + 1, cRxASCToBinBuf[2 * j + 7]);
		
	}

//	SCI0Data.TxBuf[1] = '0';
//	SCI0Data.TxBuf[2] = '1';
	SCI0Data.TxBuf[3] = '1';
	SCI0Data.TxBuf[4] = '0';

	SCI0Data.TxBuf[5] = BinToAscTab[(cRxASCToBinBuf[2]>>4)];
	SCI0Data.TxBuf[6] = BinToAscTab[(cRxASCToBinBuf[2]&0xf)];
	SCI0Data.TxBuf[7] = BinToAscTab[(cRxASCToBinBuf[3]>>4)];
	SCI0Data.TxBuf[8] = BinToAscTab[(cRxASCToBinBuf[3]&0xf)];
	SCI0Data.TxBuf[9] = BinToAscTab[(cRxASCToBinBuf[4]>>4)];
	SCI0Data.TxBuf[10] = BinToAscTab[(cRxASCToBinBuf[4]&0xf)];
	SCI0Data.TxBuf[11] = BinToAscTab[(cRxASCToBinBuf[5]>>4)];
	SCI0Data.TxBuf[12] = BinToAscTab[(cRxASCToBinBuf[5]&0xf)];

	sum = 0x11 + cRxASCToBinBuf[2]+ cRxASCToBinBuf[3]+ cRxASCToBinBuf[4]+ cRxASCToBinBuf[5];
	sum = ((~sum)+1) & 0xff;
	j = (sum >> 4) & 0xf;
	SCI0Data.TxBuf[13] = BinToAscTab[j];

	j = sum & 0xf;
	SCI0Data.TxBuf[14] = BinToAscTab[j];

	SCI0Data.TxBuf[15] = 0x0d;
	SCI0Data.TxBuf[16] = 0x0a;
	SCI0Data.TxLen = 16;

	SCI0_TxStart();
}// void WriteMulRegister(void)

void TouchCommOK(void)
{
	SendBackRxBuf();
}// void TouchCommOK(void)
