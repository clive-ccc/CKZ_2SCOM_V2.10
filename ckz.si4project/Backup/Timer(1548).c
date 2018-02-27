/*
 * Timer.c
 *
 *  Created on: 2011-9-21
 *      Author: YWQ
 */
#include "Global.h"
//#include "Action.h"


#define TIMER0_INTERRUPT_PRIORITY	1
#define TICK_RATE_HZ		1000		// 1000hz 1ms
#define TICK_RATE_100KHZ	100000

unsigned long dwTickCount = 0x0;
unsigned short cEnableRxDelay;

void SetupTimer0Interrupt( void )
{
	/* Enable compare match timer 0. */
	MSTP( CMT0 ) = 0;

	/* Interrupt on compare match. */
	CMT0.CMCR.BIT.CMIE = 1;

	/* Set the compare match value. */
	CMT0.CMCOR = ( unsigned short ) ( ( ( PCLK_FREQUENCY / TICK_RATE_HZ ) -1 ) / 8 );

	/* Divide the PCLK by 8. */
	CMT0.CMCR.BIT.CKS = 0;

	/* Enable the interrupt... */
	IEN( CMT0,CMI0 ) = 1;

	/* ...and set its priority to the application defined kernel priority. */
	IPR( CMT0,CMI0 ) = TIMER0_INTERRUPT_PRIORITY;

	/* Start the timer. */
	CMT.CMSTR0.BIT.STR0 = 1;
}

void SetupTimer2Interrupt( void )
{
	/* Enable compare match timer 0. */
	MSTP( CMT2 ) = 0;

	/* Interrupt on compare match. */
	CMT2.CMCR.BIT.CMIE = 1;

	/* Set the compare match value. */
	CMT2.CMCOR = ( unsigned short ) ( ( ( PCLK_FREQUENCY / TICK_RATE_100KHZ ) -1 ) / 8 );

	/* Divide the PCLK by 8. */
	CMT2.CMCR.BIT.CKS = 0;

	/* Enable the interrupt... */
	IEN( CMT2,CMI2 ) = 1;

	/* ...and set its priority to the application defined kernel priority. */
	IPR( CMT2,CMI2 ) = TIMER0_INTERRUPT_PRIORITY;

	/* Start the timer. */
	CMT.CMSTR1.BIT.STR2 = 1;
}


/*-----------------------------------------------------------*/
extern void TestSend(void);
#pragma interrupt ( Timer0TickISR( vect = VECT( CMT0,CMI0 ), enable ) )
void Timer0TickISR( void )
{
	dwTickCount++;

	if(cEnableRxDelay) cEnableRxDelay--;
	if(bTxEnd == 1)
	{
		if(SCI0.SSR.BIT.TEND == 1)
		{
			bTxEnd = 0;
			Enableb485Rx();
//			SCI0_RxIntEn();
		}
	}
	
	if(ServorDlay) ServorDlay--;
	if(RstPphlDlay)RstPphlDlay--;
	if(NoworkStopdelay)NoworkStopdelay--;
}
// 扫描的定时器
// 通过定时器进行滤波
#pragma interrupt ( Timer2TickISR( vect = VECT( CMT2,CMI2 ), enable ) )
void Timer2TickISR( void )
{
	extern void AutoCheckHole(void);

	AutoCheckHole();
}


#define RunLED	PORT6.DR.BIT.B0 //^= 0x01
#define BLINKTIME 100

void BlinkRunLED(void)
{
	static unsigned long dwBlinkCount = 0;
	
	if(dwBlinkCount < dwTickCount)
	{
		dwBlinkCount = dwTickCount + BLINKTIME;
		//CPLRunLED();
		RunLED = ~RunLED;

	}
}
