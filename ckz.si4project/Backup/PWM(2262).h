#ifndef PWM_H_
#define PWM_H_

typedef struct
{
	unsigned short cAxisNo;			//
	unsigned short cSetSpeed;
	unsigned short cCurSpeed;	
	unsigned short cCalSpeed;	
	unsigned short cSetAccVal;
	unsigned short cSetDecVal;
	unsigned short cCalAccPulse;
	unsigned short cCalDecPulse;
	unsigned short cSetStartSpeed;
	unsigned short cAccDecVal;
	unsigned short cMinSpeed;
	unsigned short cMaxSpeed;
	unsigned short cRstStep;
	unsigned short cZeroFilter;
	unsigned short State;	
	unsigned short bAuto_Mov;	
	long cRealPosi;
	long cDestPosi;
	unsigned long cPmovPulse;	
	long cMinLen;	
	long cMaxLen;	
} StepperDataInfo;

#define X_AXIS				0x0
#define Y_AXIS				0x1
#define Z_AXIS				0x2
#define A_AXIS				0x3

extern StepperDataInfo tXAxisStepper,tYAxisStepper,tZAxisStepper,tAAxisStepper;

#define AXIS_NUM  2
#if 0
//#define X_AXIS_PULSE_ON()	MTUB.TSTR.BIT.CST1 = 1
//#define Y_AXIS_PULSE_ON()	MTUB.TSTR.BIT.CST2= 1
#define Z_AXIS_PULSE_ON()	MTUB.TSTR.BIT.CST3= 1
#define U_AXIS_PULSE_ON()	MTUB.TSTR.BIT.CST4= 1

//#define X_AXIS_PULSE_OFF()	MTUB.TSTR.BIT.CST1 = 0
//#define Y_AXIS_PULSE_OFF()	MTUB.TSTR.BIT.CST2 = 0
#define Z_AXIS_PULSE_OFF()	MTUB.TSTR.BIT.CST3 = 0
#define U_AXIS_PULSE_OFF()	MTUB.TSTR.BIT.CST4 = 0

#define bXDirction 			PORTB.PORT.BIT.B1
#define bYDirction 			PORTB.PORT.BIT.B5
//#define bZDirction 			PORTB.PORT.BIT.B1
//#define bADirction 			PORTB.PORT.BIT.B5

//#define	DR_CW_X()			PORTA.DR.BIT.B5 = 0// 0
//#define	DR_CCW_X()			PORTA.DR.BIT.B5 = 1// 1
//#define	DR_CW_Y()			PORTA.DR.BIT.B7 = 0
//#define	DR_CCW_Y()			PORTA.DR.BIT.B7 = 1
//#define	DR_CW_Z()			PORTB.DR.BIT.B1 = 0
//#define	DR_CCW_Z()			PORTB.DR.BIT.B1 = 1
//#define	DR_CW_U()			PORTB.DR.BIT.B5 = 0
//#define	DR_CCW_U()			PORTB.DR.BIT.B5 = 1

#define X_AXIS_PULSE_ON()	MTUB.TSTR.BIT.CST3 = 1
#define Y_AXIS_PULSE_ON()	MTUB.TSTR.BIT.CST4= 1

#define X_AXIS_PULSE_OFF()	MTUB.TSTR.BIT.CST3 = 0
#define Y_AXIS_PULSE_OFF()	MTUB.TSTR.BIT.CST4 = 0

#define	DR_CW_X()			PORTB.DR.BIT.B1 = 0
#define	DR_CCW_X()			PORTB.DR.BIT.B1 = 1
#define	DR_CW_Y()			PORTB.DR.BIT.B5 = 0
#define	DR_CCW_Y()			PORTB.DR.BIT.B5 = 1

#define Y_PWM_STATE		    MTUB.TSTR.BIT.CST4
#define X_PWM_STATE		    MTUB.TSTR.BIT.CST3
#else
#define X_AXIS_PULSE_ON()	MTUB.TSTR.BIT.CST4 = 1
#define Y_AXIS_PULSE_ON()	MTUB.TSTR.BIT.CST3 = 1
#define Z_AXIS_PULSE_ON()	MTUB.TSTR.BIT.CST3 = 1
#define D_AXIS_PULSE_ON()	MTUB.TSTR.BIT.CST4 = 1

#define X_AXIS_PULSE_OFF()	MTUB.TSTR.BIT.CST4 = 0
#define Y_AXIS_PULSE_OFF()	MTUB.TSTR.BIT.CST3 = 0
#define Z_AXIS_PULSE_OFF()	MTUB.TSTR.BIT.CST3 = 0
#define D_AXIS_PULSE_OFF()	MTUB.TSTR.BIT.CST4 = 0

#define	DR_CW_X()			PORTB.DR.BIT.B5 = 0
#define	DR_CCW_X()			PORTB.DR.BIT.B5 = 1
#define	DR_CW_Y()			PORTB.DR.BIT.B1 = 0
#define	DR_CCW_Y()			PORTB.DR.BIT.B1 = 1
#define	DR_CW_Z()			PORTB.DR.BIT.B1 = 0
#define	DR_CCW_Z()			PORTB.DR.BIT.B1 = 1
#define	DR_CW_D()			PORTB.DR.BIT.B5 = 0
#define	DR_CCW_D()			PORTB.DR.BIT.B5 = 1

#define Y_PWM_STATE		    MTUB.TSTR.BIT.CST3
#define X_PWM_STATE		    MTUB.TSTR.BIT.CST4

#define bXDirction 			PORTB.PORT.BIT.B5
#define bYDirction 			PORTB.PORT.BIT.B1
#endif

extern  WORD_BITS cRunState;
#define bInit			cRunState.bits.b1
#define X_DRV			cRunState.bits.b2
#define Y_DRV			cRunState.bits.b3
#define Z_DRV			cRunState.bits.b4
#define A_DRV			cRunState.bits.b5
#define bLimitP_X		cRunState.bits.b6
#define bLimitP_Y		cRunState.bits.b7
#define bLimitN_X		cRunState.bits.b8
#define bLimitN_Y		cRunState.bits.b9
#define bLimitP_Z		cRunState.bits.b10
#define bLimitP_A		cRunState.bits.b11
#define bLimitN_Z		cRunState.bits.b12
#define bLimitN_A		cRunState.bits.b13
#define b100ms			cRunState.bits.b14

extern void MTU7_PWMInit(void);
extern void MTU8_PWMInit(void);
extern void MTU9_PWMInit(void);
extern void MTU10_PWMInit(void);

extern void CalRunPulse(StepperDataInfo *tCurSetStpper);
extern void LMV_Set_Speed(unsigned short cAxisNo,unsigned short cSpeed);
extern long LMV_Get_Command_Pos(unsigned short cAxisNo);
extern void LMV_Set_Limit_P(unsigned short cAxisNo,unsigned long dwMaxLen);
extern void LStartPWM(unsigned short cAxisNo);
extern void ReadAxisStatus(void);
extern void LMV_Suddent_Stop(unsigned short cAxisNo);
extern void LMV_Dec_Stop(unsigned short cAxisNo);
extern void LMV_Move_To_Position(unsigned short axis, long position);
extern void LMV_Pmove(unsigned short cAxisNo, unsigned long dwPosi);
extern void LMV_Pmove2(unsigned short cAxisNo, unsigned long dwPosi);
extern void MV_AccDec_Set(unsigned short cAxisNo, unsigned short cSetAcc, unsigned short cSetDec);
extern void LMV_Const_Move(unsigned short cAxisNo, unsigned short cSpeed, unsigned short dir);
extern void LMV_Const_Move2(unsigned short cAxisNo, unsigned short cSpeed, unsigned short dir);
extern void MV_Set_Limit_P(unsigned short cAxisNo, unsigned long dwMaxLen) ;
extern void ReadAxisStatus(void);

#endif
