/*
*********************************************************************************************************
* Filename      : uart.c
*********************************************************************************************************
*/

#include "Global.h"
/*
#ifdef BOARD_14007
#define UART6_USE (1)
#endif
#ifdef BOARD_12007
#define UART0_USE (1)
#endif
*/
#define UART6_USE (1)
/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static void SCI0_TxHandler(void);
static void SCI6_TxHandler(void);

extern unsigned short cEnableRxDelay;

SCI0DataInfo SCI0Data;
unsigned short cRxState;
unsigned short bTxEnd = 0;
unsigned short cTxTimer = 0,cTxOKTimer = 0;

//#pragma section DD_FRAM
//unsigned short cDataArea[512],cMidleCoil[50];
//#pragma section 
/*
inline void Enableb485Tx(void)
{
    PORT2.DR.BIT.B5 = 1;
}

inline void Enableb485Rx(void)
{
    PORT2.DR.BIT.B5 = 0;
}
*/
/*
*********************************************************************************************************
*                                      UartInit()
*
* Description : Initialize the UART for uC/Probe communication.
*
* Argument(s) : baud_rate   Intended baud rate of the RS-232.
*
* Return(s)   : none.
*********************************************************************************************************
*/

void  UartInit(unsigned int  baud_rate)
{
	unsigned int i;

#if UART0_USE

    MSTP(SCI0) = 0;

    PORT2.ICR.BIT.B0 = 1;                                       /* TxD0                                                 */
    PORT2.ICR.BIT.B1 = 1;                                       /* RxD0                                                 */

/* SCR-- b7  b6 b5 b4  b3   b2   b1-b0
 * 		TIE RIE TE RE MPIE TEIE CKE[1:0]
 * 	TIE		发送中断允许位		0 - Disable 1- enable
 * 	RIE		接收中断允许�? 		0 - Disable 1- enable
 * 	TE		发送允许位			0 - Disable 1- enable
 * 	RE		接收允许�?		0 - Disable 1- enable
 * 	MPIE	多处理器中断允许�?	0 - Disable 1- enable
 * 	TEIE	发送结束中断允许位		0 - Disable 1- enable
 * 	CKE[1:0]时钟允许�?		0-  内部波特率发生器,SCKn 引脚能用作输�? 输出端口
 */
    SCI0.SCR.BYTE = 0;

    while(!PORT2.ICR.BIT.B1);

    IR(SCI0, ERI0) = 0;          /* Clear any pending ISR.                               */
    IR(SCI0, RXI0) = 0;
    IR(SCI0, TXI0) = 0;
    IR(SCI0, TEI0) = 0;

    IPR(SCI0,) = 3;         /* Set interrupt priority.                              */
//    IPR(SCI0, ERI0) = 3;         /* Set interrupt priority.                              */
//    IPR(SCI0, RXI0) = 3;
//    IPR(SCI0, TXI0) = 3;
//    IPR(SCI0, TEI0) = 3;

    IEN(SCI0, ERI0) = 1;         /* Enable interrupt source.                             */
    IEN(SCI0, RXI0) = 1;
    IEN(SCI0, TXI0) = 1;
    IEN(SCI0, TEI0) = 0;

/* SCMR-- b7  b6 b5 b4  b3   b2   b1  b0
* 		 BCP2 -- -- -- SDIR SINV  --  SMIF
* 	BCP2	基本时钟脉冲�?
* 	--		保留�?			1 -- rw
* 	SDIR	位序选择�?		0-以LSB first 进行发送和接收, 1- 以MSB first 进行发送和接收
* 	SINV	智能卡数据反相位		0 - 不反相， 1- 反相
* 	MPIE	多处理器中断允许�?	0 - Disable 1- enable
* 	SMIF	智能卡接口模式选择�?0 - 串行通信接口模式,1 - 智能卡接口模�?
*/
    SCI0.SCMR.BYTE = 0xf2;

    SCI0.RDR;
    SCI0.SSR.BIT.TEND;
    SCI0.SSR.BIT.TEND = 0;
    SCI0.SSR.BIT.PER;
    SCI0.SSR.BIT.PER  = 0;
    SCI0.SSR.BIT.FER;
    SCI0.SSR.BIT.FER  = 0;
    SCI0.SSR.BIT.ORER;
    SCI0.SSR.BIT.ORER = 0;

    SCI0.SCR.BIT.CKE  = 0;                               /* Use internal clock, set SCK pin as input pin         */

/*SMR--	b7 	b6 	b5 	b4 	b3 	 b2 b1 	b0
 *		CM	CHR	PE	PM	STOP MP CKS[1:0]
 *	CM		通信模式�?	0：异步通信模式,1：时钟同步通信模式
 *	CHR		字符长位		0：数据长度为8 �?1：数据长度为7�?
 *	PE		奇偶校验允许�?0：不校验�?：检�?
 *	PM		奇偶校验允许�?0：偶校验�?：奇校验
 *	STOP	停止位长选择�?0�? 个停止位�?�? 个停止位
 *	MP		多处理器模式�?0：禁止多处理器通信功能�?：允许多处理器通信功能
 *	CKS�?�?】时钟选择�?	0：PCLK  1：PCLK/4  2:PCLK/16  3:PCLK/64
*/
    SCI0.SMR.BIT.CM   = 0;	    /* Set for asynchronous mode                            */
    SCI0.SMR.BIT.CHR  = 0;  	/* 8-bit data                                           */
    SCI0.SMR.BIT.PE   = 0;     	/* Parity mode disabled                                 */
    SCI0.SMR.BIT.PM   = 0;      /* No parity                                            */
    SCI0.SMR.BIT.STOP = 0;      /* 1 stop bit                                           */

    SCI0.SMR.BIT.CKS = 1;//baud_clk;
    SCI0.BRR         = 35;//brr

    for (i = 0; i < 20000; i++);                                /* Wait for at least 1 bit time                         */

    SCI0.SCR.BIT.TIE = 1;                                /* Enable Transmit Interrupts                           */
    SCI0.SCR.BIT.RIE = 1;                                /* Enable Receive Interrupts                            */
    SCI0.SCR.BYTE |= 0x30;            /* Enable Tx & Rx (cannot be done thru bit-fields).     */
//    SCI0.RDR;
#endif

#if UART1_USE
    MSTP(SCI1) = 0;

#ifndef PROBE_CFG_RS232_UART_ALT
    IOPORT.PFFSCI.BIT.SCI1S = 0;
    PORT2.ICR.BIT.B6 = 1;                                       /* TxD1-A                                               */
    PORT3.ICR.BIT.B0 = 1;                                       /* RxD1-A                                               */
#else
    IOPORT.PFFSCI.BIT.SCI1S = 1;
    PORTF.ICR.BIT.B0 = 1;                                       /* TxD1-B                                               */
    PORTF.ICR.BIT.B2 = 1;                                       /* RxD1-B                                               */
#endif

    IR(SCI1, ERI1) = 0;                                         /* Clear any pending ISR.                               */
    IR(SCI1, RXI1) = 0;
    IR(SCI1, TXI1) = 0;
    IR(SCI1, TEI1) = 0;

    IPR(SCI1, ERI1) = 3;                                        /* Set interrupt priority.                              */
    IPR(SCI1, RXI1) = 3;
    IPR(SCI1, TXI1) = 3;
    IPR(SCI1, TEI1) = 3;

    IEN(SCI1, ERI1) = 1;                                        /* Enable interrupt source.                             */
    IEN(SCI1, RXI1) = 1;
    IEN(SCI1, TXI1) = 1;
    IEN(SCI1, TEI1) = 1;


#endif

#if UART2_USE

    MSTP(SCI2) = 0;

#ifndef PROBE_CFG_RS232_UART_ALT
    IOPORT.PFFSCI.BIT.SCI2S = 0;
    PORT1.ICR.BIT.B3 = 1;                                       /* TxD2-A                                               */
    PORT1.ICR.BIT.B2 = 1;                                       /* RxD2-A                                               */
#else
    IOPORT.PFFSCI.BIT.SCI2S = 1;
    PORT5.ICR.BIT.B0 = 1;                                       /* TxD2-B                                               */
    PORT5.ICR.BIT.B2 = 1;                                       /* RxD2-B                                               */
#endif

    IR(SCI2, ERI2) = 0;                                         /* Clear any pending ISR.                               */
    IR(SCI2, RXI2) = 0;
    IR(SCI2, TXI2) = 0;
    IR(SCI2, TEI2) = 0;

    IPR(SCI2, ERI2) = 3;                                        /* Set interrupt priority.                              */
    IPR(SCI2, RXI2) = 3;
    IPR(SCI2, TXI2) = 3;
    IPR(SCI2, TEI2) = 3;

    IEN(SCI2, ERI2) = 1;                                        /* Enable interrupt source.                             */
    IEN(SCI2, RXI2) = 1;
    IEN(SCI2, TXI2) = 1;
    IEN(SCI2, TEI2) = 1;


#endif

#if UART3_USE

    MSTP(SCI3) = 0;

#ifndef PROBE_CFG_RS232_UART_ALT
    IOPORT.PFFSCI.BIT.SCI3S = 0;
    PORT1.ICR.BIT.B7 = 1;                                       /* TxD3-A                                               */
    PORT1.ICR.BIT.B6 = 1;                                       /* RxD3-A                                               */
#else
    IOPORT.PFFSCI.BIT.SCI3S = 1;
    PORT2.ICR.BIT.B3 = 1;                                       /* TxD3-B                                               */
    PORT2.ICR.BIT.B5 = 1;                                       /* RxD3-B                                               */
#endif

    IR(SCI3, ERI3) = 0;                                         /* Clear any pending ISR.                               */
    IR(SCI3, RXI3) = 0;
    IR(SCI3, TXI3) = 0;
    IR(SCI3, TEI3) = 0;

    IPR(SCI3, ERI3) = 3;                                        /* Set interrupt priority.                              */
    IPR(SCI3, RXI3) = 3;
    IPR(SCI3, TXI3) = 3;
    IPR(SCI3, TEI3) = 3;

    IEN(SCI3, ERI3) = 1;                                        /* Enable interrupt source.                             */
    IEN(SCI3, RXI3) = 1;
    IEN(SCI3, TXI3) = 1;
    IEN(SCI3, TEI3) = 1;

#endif


#if UART5_USE

    MSTP(SCI5) = 0;

    PORTC.ICR.BIT.B3 = 1;                                       /* TxD5                                                 */
    PORTC.ICR.BIT.B2 = 1;                                       /* RxD5                                                 */

    IR(SCI5, ERI5) = 0;                                         /* Clear any pending ISR.                               */
    IR(SCI5, RXI5) = 0;
    IR(SCI5, TXI5) = 0;
    IR(SCI5, TEI5) = 0;

    IPR(SCI5, ERI5) = 3;                                        /* Set interrupt priority.                              */
    IPR(SCI5, RXI5) = 3;
    IPR(SCI5, TXI5) = 3;
    IPR(SCI5, TEI5) = 3;

    IEN(SCI5, ERI5) = 1;                                        /* Enable interrupt source.                             */
    IEN(SCI5, RXI5) = 1;
    IEN(SCI5, TXI5) = 1;
    IEN(SCI5, TEI5) = 1;

#endif

#if UART6_USE

    MSTP(SCI6) = 0;

#ifndef PROBE_CFG_RS232_UART_ALT
    IOPORT.PFFSCI.BIT.SCI6S = 0;
    PORT0.ICR.BIT.B0 = 1;                                       /* TxD6-A                                               */
    PORT0.ICR.BIT.B1 = 1;                                       /* RxD6-A                                               */
#else
    IOPORT.PFFSCI.BIT.SCI6S = 1;
    PORT3.ICR.BIT.B2 = 1;                                       /* TxD6-B                                               */
    PORT3.ICR.BIT.B3 = 1;                                       /* RxD6-B                                               */
#endif
    SCI6.SCR.BYTE = 0;
    
    IR(SCI6, ERI6) = 0;                                         /* Clear any pending ISR.                               */
    IR(SCI6, RXI6) = 0;
    IR(SCI6, TXI6) = 0;
    IR(SCI6, TEI6) = 0;

    IPR(SCI6,) = 3;         /* Set interrupt priority */
//    IPR(SCI0, ERI0) = 3;   /* Set interrupt priority */
//    IPR(SCI0, RXI0) = 3;
//    IPR(SCI0, TXI0) = 3;
//    IPR(SCI0, TEI0) = 3;

    IEN(SCI6, ERI6) = 1;     /* Enable interrupt source */
    IEN(SCI6, RXI6) = 1;
    IEN(SCI6, TXI6) = 1;
    IEN(SCI6, TEI6) = 0;

    SCI6.SCMR.BYTE = 0xf2;

    SCI6.RDR;
    SCI6.SSR.BIT.TEND;
    SCI6.SSR.BIT.TEND = 0;
    SCI6.SSR.BIT.PER;
    SCI6.SSR.BIT.PER  = 0;
    SCI6.SSR.BIT.FER;
    SCI6.SSR.BIT.FER  = 0;
    SCI6.SSR.BIT.ORER;
    SCI6.SSR.BIT.ORER = 0;

    SCI6.SCR.BIT.CKE  = 0; /* Use internal clock, set SCK pin as input pin  */

    SCI6.SMR.BIT.CM   = 0;	    /* Set for asynchronous mode                            */
    SCI6.SMR.BIT.CHR  = 0;  	/* 8-bit data                                           */
    SCI6.SMR.BIT.PE   = 0;     	/* Parity mode disabled                                 */
    SCI6.SMR.BIT.PM   = 0;      /* No parity                                            */
    SCI6.SMR.BIT.STOP = 0;      /* 1 stop bit                                           */

    SCI6.SMR.BIT.CKS = 1;//baud_clk;
    SCI6.BRR         = 35;//brr

    for (i = 0; i < 20000; i++);    /* Wait for at least 1 bit time                         */

    SCI6.SCR.BIT.TIE = 1;      /* Enable Transmit Interrupts                           */
    SCI6.SCR.BIT.RIE = 1;      /* Enable Receive Interrupts                            */
    SCI6.SCR.BYTE |= 0x30;     /* Enable Tx & Rx (cannot be done thru bit-fields).     */
#endif
                               /* Read each flag before clearing  */
}



/*
*********************************************************************************************************
*                                    SCI0_RxISRHandler()
* Description : Handle Rx interrupts.
* Note(s)     : (1) This function may empty if Rx interrupts are handled by the combined Rx/Tx interrupt
*                   handler, SCI0_RxTxISRHandler()
*********************************************************************************************************
*/
#pragma interrupt ( SCI0_RxISRHandler( vect = VECT( SCI0,RXI0 ), enable ) )
void  SCI0_RxISRHandler (void)
{
    unsigned char  data;


    data = SCI0.RDR;
    SCI0Data.RxPosi++;
    switch(data)
    {
		case RX_STATE_SD1:
	    	SCI0Data.RxPosi = 0;
	    	cRxState = RX_START;
			break;
		case RX_STATE_END1:
	    	cRxState = RX_END1;
			break;
		case RX_STATE_END2:
			if(cRxState == RX_END1)
			{
				SCI0Data.cRxOK = 1;
				cRxState = RX_END2;
				cEnableRxDelay = 3;
			}
			break;
		default:
			SCI0Data.RxBuf[SCI0Data.RxPosi] = data;
			break;
    }
}


/*
*********************************************************************************************************
*                                      SCI0_RxIntDis()
* Description : Disable Rx interrupts.
*********************************************************************************************************
*/

void  SCI0_RxIntDis (void)
{
    SCI0.SCR.BIT.RIE = 0;
}


/*
*********************************************************************************************************
*                                      SCI0_RxIntEn()
* Description : Enable Rx interrupts.
*********************************************************************************************************
*/

void  SCI0_RxIntEn (void)
{
    SCI0.SCR.BIT.RIE = 1;
}


static void SCI0_TxHandler(void)
{
	if(SCI0Data.TxLen > 0 )
	{
		SCI0.TDR = SCI0Data.TxBuf[SCI0Data.TxPosi++];
		SCI0Data.TxLen--;
	}
	else
	{
//		SCI0_TxIntDis();
		bTxEnd = 1;
		cTxOKTimer++;
	}
}

/*
*********************************************************************************************************
*                                    SCI0_TxISRHandler()
* Description : Handle Tx interrupts.
* Note(s)     : (1) This function may empty if Tx interrupts are handled by the combined Rx/Tx interrupt
*                   handler, SCI0_RxTxISRHandler()
*********************************************************************************************************
*/
#pragma interrupt ( SCI0_TxISRHandler( vect = VECT( SCI0,TXI0 ), enable ) )
void  SCI0_TxISRHandler (void)
{
	SCI0_TxHandler();
}

/*
*********************************************************************************************************
*                                    SCI0_TxEndISRHandler()
* Description : Handle Tx end interrupts.
*********************************************************************************************************
*/
#pragma interrupt ( SCI0_TxEndISRHandler( vect = VECT( SCI0,TEI0 ), enable ) )
void  SCI0_TxEndISRHandler (void)
{
	SCI0_TxHandler();
}

/*
*********************************************************************************************************
*                                      SCI0_TxStart()
* Description : Transmit one byte.
*
* Argument(s) : data        The byte to transmit.
*********************************************************************************************************
*/

void  SCI0_TxStart (void)
{
	Enableb485Tx();
	SCI0Data.TxBuf[1] = '0';
	SCI0Data.TxBuf[2] = '1';
	SCI0Data.TxPosi = 1;
    SCI0.SCR.BIT.TEIE = 0;
//	SCI0_TxIntEn();
	SCI0.TDR = TX_STATE_SD1;
}

/*
*********************************************************************************************************
*                                      SCI0_TxIntDis()
* Description : Disable Tx interrupts.
*********************************************************************************************************
*/

void  SCI0_TxIntDis (void)
{
    do {
        SCI0.SCR.BIT.TIE = 0;                            /* Disable tx int.                                      */
    } while (SCI0.SCR.BIT.TIE > 0);

	/* Read transmission end flag until set.                */
//    while (SCI0.SSR.BIT.TEND == 0)
//    {
//        ;
//    }
}


/*
*********************************************************************************************************
*                                      SCI0_TxIntEn()
* Description : Enable Tx interrupts.
*
*********************************************************************************************************
*/

void  SCI0_TxIntEn (void)
{
	SCI0.SCR.BIT.TIE = 1;          /* Enable tx int.                                       */
}

/*
*********************************************************************************************************
*                                      SCI0_ErISRHandler()
* Description : Probe receive error handler.
*********************************************************************************************************
*/
#pragma interrupt ( SCI0_ErISRHandler( vect = VECT( SCI0,ERI0 ), enable ) )
void SCI0_ErISRHandler (void)
{
//	SCI0.SSR.BYTE = 0xc0;
#if 1
	if(SCI0.SSR.BIT.FER > 0)
    {
    	/* Framing Error Flag     */
        while(SCI0.SSR.BIT.FER > 0)
        {
        	SCI0.SSR.BIT.FER = 0;
        }
    }
    while (SCI0.SSR.BIT.PER > 0)
    {
    	/* Parity Error Flag       */
        SCI0.SSR.BIT.PER = 0;
    }
    if (SCI0.SSR.BIT.ORER > 0)
    {
    	/* Overrun Error Flag      */
        SCI0.RDR;
        while (SCI0.SSR.BIT.ORER > 0)
        {
        	SCI0.SSR.BIT.ORER = 0;
        }
   }
#endif
}

void  Uart6B_Init(unsigned int  baud_rate)
{
	unsigned int i;

    MSTP(SCI6) = 0;

#ifdef PROBE_CFG_RS232_UART_ALT
    IOPORT.PFFSCI.BIT.SCI6S = 0;
    PORT0.ICR.BIT.B0 = 1;                                       /* TxD6-A                                               */
    PORT0.ICR.BIT.B1 = 1;                                       /* RxD6-A                                               */
#else
    IOPORT.PFFSCI.BIT.SCI6S = 1;
    PORT3.ICR.BIT.B2 = 1;                                       /* TxD6-B                                               */
    PORT3.ICR.BIT.B3 = 1;                                       /* RxD6-B                                               */
#endif
    SCI6.SCR.BYTE = 0;

    IR(SCI6, ERI6) = 0;          /* Clear any pending ISR.                               */
    IR(SCI6, RXI6) = 0;
    IR(SCI6, TXI6) = 0;
    IR(SCI6, TEI6) = 0;

    IPR(SCI6,) = 3;         /* Set interrupt priority.                              */
//    IPR(SCI0, ERI0) = 3;         /* Set interrupt priority.                              */
//    IPR(SCI0, RXI0) = 3;
//    IPR(SCI0, TXI0) = 3;
//    IPR(SCI0, TEI0) = 3;

    IEN(SCI6, ERI6) = 1;         /* Enable interrupt source.                             */
    IEN(SCI6, RXI6) = 1;
    IEN(SCI6, TXI6) = 1;
    IEN(SCI6, TEI6) = 0;

    SCI6.SCMR.BYTE = 0xf2;

    SCI6.RDR;
    SCI6.SSR.BIT.TEND;
    SCI6.SSR.BIT.TEND = 0;
    SCI6.SSR.BIT.PER;
    SCI6.SSR.BIT.PER  = 0;
    SCI6.SSR.BIT.FER;
    SCI6.SSR.BIT.FER  = 0;
    SCI6.SSR.BIT.ORER;
    SCI6.SSR.BIT.ORER = 0;

    SCI6.SCR.BIT.CKE  = 0;                               /* Use internal clock, set SCK pin as input pin         */

    SCI6.SMR.BIT.CM   = 0;	    /* Set for asynchronous mode                            */
    SCI6.SMR.BIT.CHR  = 0;  	/* 8-bit data                                           */
    SCI6.SMR.BIT.PE   = 0;     	/* Parity mode disabled                                 */
    SCI6.SMR.BIT.PM   = 0;      /* No parity                                            */
    SCI6.SMR.BIT.STOP = 0;      /* 1 stop bit                                           */

    SCI6.SMR.BIT.CKS = 1;//baud_clk;
    SCI6.BRR         = 35;//brr

    for (i = 0; i < 20000; i++);                                /* Wait for at least 1 bit time                         */

    SCI6.SCR.BIT.TIE = 1;                                /* Enable Transmit Interrupts                           */
    SCI6.SCR.BIT.RIE = 1;                                /* Enable Receive Interrupts                            */
    SCI6.SCR.BYTE |= 0x30;            /* Enable Tx & Rx (cannot be done thru bit-fields).     */
                                                          /* Read each flag before clearing                       */
}


#pragma interrupt ( SCI6_RxISRHandler( vect = VECT( SCI6,RXI6 ), enable ) )
void  SCI6_RxISRHandler (void)
{
    unsigned char  data;

    data = SCI6.RDR;
    SCI0Data.RxPosi++;
	if(SCI0Data.RxPosi > 250)
		SCI0Data.RxPosi = 0;
    switch(data)
    {
		case RX_STATE_SD1:
	    	SCI0Data.RxPosi = 0;
	    	cRxState = RX_START;
			break;
		case RX_STATE_END1:
	    	cRxState = RX_END1;
			break;
		case RX_STATE_END2:
			if(cRxState == RX_END1)
			{
				SCI0Data.cRxOK = 1;
				cRxState = RX_END2;
				cEnableRxDelay = 3;
			}
			break;
		default:
			SCI0Data.RxBuf[SCI0Data.RxPosi] = data;
			break;
    }
}

void  SCI6_RxIntDis (void)
{
    SCI6.SCR.BIT.RIE = 0;
}

void  SCI6_RxIntEn (void)
{
    SCI6.SCR.BIT.RIE = 1;
}

static void SCI6_TxHandler(void)
{
	if(SCI0Data.TxLen > 0 )
	{
		SCI6.TDR = SCI0Data.TxBuf[SCI0Data.TxPosi++];
		SCI0Data.TxLen--;
	}
	else
	{
//		SCI0_TxIntDis();
		bTxEnd = 1;
	}
}

#pragma interrupt ( SCI6_TxISRHandler( vect = VECT( SCI6,TXI6 ), enable ) )
void  SCI6_TxISRHandler (void)
{
	SCI6_TxHandler();
}

#pragma interrupt ( SCI6_TxEndISRHandler( vect = VECT( SCI6,TEI6 ), enable ) )
void  SCI6_TxEndISRHandler (void)
{
	SCI6_TxHandler();
}

void  SCI6_TxStart (void)
{
	Enableb485Tx();
//	SCI0Data.TxBuf[0] = ':';
	SCI0Data.TxBuf[1] = '0';
	SCI0Data.TxBuf[2] = '1';
	SCI0Data.TxPosi = 1;
    SCI0.SCR.BIT.TEIE = 0;
//	SCI0Data.TxLen++;
//	SCI0_TxIntEn();
	SCI6.TDR = TX_STATE_SD1;
//	SCI0.TDR = 0xff;
}

void  SCI6_TxIntDis (void)
{
    do 
    {
        SCI6.SCR.BIT.TIE = 0;    /* Disable tx int */
    }
    while (SCI6.SCR.BIT.TIE > 0);
}

void  SCI6_TxIntEn (void)
{
	SCI6.SCR.BIT.TIE = 1;          /* Enable tx int
.                                       */
}

#pragma interrupt ( SCI6_ErISRHandler( vect = VECT( SCI6,ERI6 ), enable ) )
void SCI6_ErISRHandler (void)
{
//	SCI0.SSR.BYTE = 0xc0;
#if 1
	if(SCI6.SSR.BIT.FER > 0)
    {
    	/* Framing Error Flag     */
        while(SCI6.SSR.BIT.FER > 0)
        {
        	SCI6.SSR.BIT.FER = 0;
        }
    }
    while (SCI6.SSR.BIT.PER > 0)
    {
    	/* Parity Error Flag       */
        SCI6.SSR.BIT.PER = 0;
    }
    if (SCI6.SSR.BIT.ORER > 0)
    {
    	/* Overrun Error Flag      */
        SCI6.RDR;
        while (SCI6.SSR.BIT.ORER > 0)
        {
        	SCI6.SSR.BIT.ORER = 0;
        }
   }
#endif
}


extern unsigned short cEnableRxDelay;

void SendBackRxBuf(void)
{
	unsigned short i,j;

	SCI0Data.TxLen = SCI0Data.RxPosi;
	cTxTimer++;
	j = SCI0Data.TxLen - 1;
	for(i = 1; i < j;i++)
	{
		SCI0Data.TxBuf[i] = SCI0Data.RxBuf[i];
	}

	SCI0Data.TxBuf[i++]  = TX_STATE_END1;
	SCI0Data.TxBuf[i]  = TX_STATE_END2;
	
	SCI6_TxStart();
}


