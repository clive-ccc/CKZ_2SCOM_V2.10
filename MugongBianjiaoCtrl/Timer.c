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
void BlinkRunLED(void);

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

void SetupTimer1Interrupt( void )
{
	unsigned long temp = 0;
/* Enable compare match timer 0. */
	MSTP( CMT1 ) = 0;
	/* Interrupt on compare match. */
	CMT1.CMCR.BIT.CMIE = 1;
	/* Set the compare match value. */
	temp = 11059200 * 4;
	temp = temp / 100000;
	temp = temp - 1;
	temp = temp / 8;
	//temp = (((PCLK_FREQUENCY / TICK_RATE_100KHZ) -1 ) / 8);
	CMT1.CMCOR = ( unsigned short ) temp;
	/* Divide the PCLK by 8. */
	CMT1.CMCR.BIT.CKS = 0;
	/* Enable the interrupt... */
	IEN( CMT1,CMI1 ) = 1;
	/* ...and set its priority to the application defined kernel priority. */
	IPR( CMT1,CMI1 ) = 6;

	/* Start the timer. */
	CMT.CMSTR0.BIT.STR1 = 1;
}


/*-----------------------------------------------------------*/

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
		}
	}
	
	//if(ServorDlay) ServorDlay--;
	if(RstPphlDlay)RstPphlDlay--;
	if(NoworkStopdelay)NoworkStopdelay--;
}
// 扫描的定时器
// 通过定时器进行滤波

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
