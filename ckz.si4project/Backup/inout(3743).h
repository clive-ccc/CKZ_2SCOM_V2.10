/*
 * inout.h
 *
 *  Created on: 2011-9-24
 *      Author: lenovo
 */

#ifndef INOUT_H_
#define INOUT_H_

typedef union
{
    unsigned char Val;
	struct
    {
        unsigned char b0:1;
        unsigned char b1:1;
        unsigned char b2:1;
        unsigned char b3:1;
        unsigned char b4:1;
        unsigned char b5:1;
        unsigned char b6:1;
        unsigned char b7:1;
    } bits;

} BYTE_BITS;

typedef union
{
    unsigned char Val;
	struct
    {
        unsigned char Tb0:2;
        unsigned char Tb1:2;
        unsigned char Tb2:2;
        unsigned char Tb3:2;
    } tobits;

} BYTE_TOBITS;

typedef union
{
	unsigned short Val;
	unsigned char v[2];
    struct
    {
		unsigned char LB;
		unsigned char HB;
    } byte;
    struct
    {
        unsigned char b0:1;
        unsigned char b1:1;
        unsigned char b2:1;
        unsigned char b3:1;
        unsigned char b4:1;
        unsigned char b5:1;
        unsigned char b6:1;
        unsigned char b7:1;
        unsigned char b8:1;
        unsigned char b9:1;
        unsigned char b10:1;
        unsigned char b11:1;
        unsigned char b12:1;
        unsigned char b13:1;
        unsigned char b14:1;
        unsigned char b15:1;
    } bits;
} WORD_BITS;

typedef union
{
	unsigned short Val;
 	unsigned char v[2];
    struct
    {
 		unsigned char LB;
 		unsigned char HB;
    } byte;
} WORD_VAL;


#define		I000	   	PORT4.PORT.BIT.B7
#define		I001		PORT4.PORT.BIT.B6
#define		I002		PORT0.PORT.BIT.B5
#define		I003		PORT0.PORT.BIT.B3
#define		I004		PORT0.PORT.BIT.B2
#define		I005		PORT0.PORT.BIT.B1
#define		I006		PORT0.PORT.BIT.B0
#define		I007		PORT0.PORT.BIT.B7

#define		I010		PORT4.PORT.BIT.B0
#define		I011		PORT4.PORT.BIT.B1
#define		I012		PORT4.PORT.BIT.B2
#define		I013		PORT4.PORT.BIT.B3
#define		I014		PORT9.PORT.BIT.B0
#define		I015		PORT9.PORT.BIT.B1
#define		I016		PORT9.PORT.BIT.B2
#define		I017		PORT9.PORT.BIT.B3

extern  WORD_BITS cInputVal1;
extern  WORD_BITS cOldInputVal1;
#define cInputX00_X17	cInputVal1.Val
#define X00	cInputVal1.bits.b0
#define X01	cInputVal1.bits.b1
#define X02	cInputVal1.bits.b2
#define X03	cInputVal1.bits.b3
#define X04	cInputVal1.bits.b4
#define X05	cInputVal1.bits.b5
#define X06	cInputVal1.bits.b6
#define X07	cInputVal1.bits.b7
#define X10	cInputVal1.bits.b8
#define X11	cInputVal1.bits.b9
#define X12	cInputVal1.bits.b10
#define X13	cInputVal1.bits.b11
#define X14	cInputVal1.bits.b12
#define X15	cInputVal1.bits.b13
#define X16	cInputVal1.bits.b14
#define X17	cInputVal1.bits.b15

#define bOldX00	cOldInputVal1.bits.b0
#define bOldX01	cOldInputVal1.bits.b1
#define bOldX02	cOldInputVal1.bits.b2
#define bOldX03	cOldInputVal1.bits.b3
#define bOldX04	cOldInputVal1.bits.b4
#define bOldX05	cOldInputVal1.bits.b5
#define bOldX06	cOldInputVal1.bits.b6
#define bOldX07	cOldInputVal1.bits.b7
#define bOldX10	cOldInputVal1.bits.b8
#define bOldX11	cOldInputVal1.bits.b9
#define bOldX12	cOldInputVal1.bits.b10
#define bOldX13	cOldInputVal1.bits.b11
#define bOldX14	cOldInputVal1.bits.b12
#define bOldX15	cOldInputVal1.bits.b13
#define bOldX16	cOldInputVal1.bits.b14
#define bOldX17	cOldInputVal1.bits.b15

#define		OUT00	   	PORT6.DR.BIT.B2
#define		OUT01		PORT6.DR.BIT.B5
#define		OUT02		PORT6.DR.BIT.B6
#define		OUT03		PORT6.DR.BIT.B7
#define		OUT04		PORT7.DR.BIT.B0
#define		OUT05		PORTA.DR.BIT.B4
#define		OUT06		PORTA.DR.BIT.B5


extern WORD_BITS cOutputVal;
#define Y00		cOutputVal.bits.b0
#define Y01		cOutputVal.bits.b1
#define Y02		cOutputVal.bits.b2
#define Y03		cOutputVal.bits.b3
#define Y04		cOutputVal.bits.b4
#define Y05		cOutputVal.bits.b5
#define Y06		cOutputVal.bits.b6
#define Y07		cOutputVal.bits.b7
#define Y10		cOutputVal.bits.b8
#define Y11		cOutputVal.bits.b9
#define Y12		cOutputVal.bits.b10
#define Y13		cOutputVal.bits.b11
#define Y14		cOutputVal.bits.b12
#define Y15		cOutputVal.bits.b13
#define Y16		cOutputVal.bits.b14
#define Y17		cOutputVal.bits.b15

extern unsigned char InputFiltering[40];		//输入滤波寄存器
#define FILTERTIME_P		8			//正滤波次数
#define FILTERTIME_N		2			//负滤波次数
extern void ReadInput(void);
extern void TestOutput(void);
extern void WriteOutput(void);
extern void GpioInit(void);
#endif /* INOUT_H_ */
