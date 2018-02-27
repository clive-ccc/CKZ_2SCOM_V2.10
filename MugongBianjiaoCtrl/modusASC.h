/*
 * modusASC.h
 *
 *  Created on: 2011-10-14
 *      Author: lenovo
 */

#ifndef MODUSASC_H_
#define MODUSASC_H_

typedef unsigned char  uint8;
typedef unsigned short uint16;
typedef unsigned long  uint32;
//typedef unsigned int   BOOL;

extern unsigned short cDataArea[512];
extern WORD_BITS cMidleCoil[50];


#define MB_NO_ERR					0x00
#define ILLEGAL_FUNCTION			0x01
#define ILLEGAL_DATA_ADDR			0x02
#define ILLEGAL_DATA_VALUE			0x03
#define SLAVE_DEVICE_FAILURE		0x04
#define ACKNOWLEDGE					0x05
#define SLAVE_DEVICE_BUSY			0x06
#define MEMORY_PARITY_ERR			0x08
#define GATEWAY_PATH_UNABLE			0x0A
#define GATEWAY_TARGET_DEV_FAIL		0x0B

#define MB_FUN_NOT_DEF				0x80
#define MB_MASTER_BUSY				0x81
#define MB_MASTER_IDLE				0x82
#define MB_RESPONSE_TIME_OUT		0x82
#define MB_PARAMETER_ERR			0x83

//extern uint8 Modbus_CRC16(uint8 *Buff_addr,uint16 len);
extern uint8  CheckReceiveSum(void);
extern void CheckReceData(void);
extern void ReadCoil(void);
extern void ReadRegister(void);
extern void WriteCoil(void);
extern void WriteRegister(void);
extern void WriteMulRegister(void);
extern void TouchCommOK(void);

#define	M00		cMidleCoil[0].bits.b0
#define	M01		cMidleCoil[0].bits.b1
#define	M02		cMidleCoil[0].bits.b2
#define	M03		cMidleCoil[0].bits.b3
#define	M04		cMidleCoil[0].bits.b4
#define	M05		cMidleCoil[0].bits.b5
#define	M06		cMidleCoil[0].bits.b6
#define	M07		cMidleCoil[0].bits.b7
#define	M08		cMidleCoil[0].bits.b8
#define	M09		cMidleCoil[0].bits.b9
#define	M10		cMidleCoil[0].bits.b10
#define	M11		cMidleCoil[0].bits.b11
#define	M12		cMidleCoil[0].bits.b12
#define	M13		cMidleCoil[0].bits.b13
#define	M14		cMidleCoil[0].bits.b14
#define	M15		cMidleCoil[0].bits.b15

#define	M16		cMidleCoil[1].bits.b0
#define	M17		cMidleCoil[1].bits.b1
#define	M18		cMidleCoil[1].bits.b2
#define	M19		cMidleCoil[1].bits.b3
#define	M20		cMidleCoil[1].bits.b4
#define	M21		cMidleCoil[1].bits.b5
#define	M22		cMidleCoil[1].bits.b6
#define	M23		cMidleCoil[1].bits.b7
#define	M24		cMidleCoil[1].bits.b8
#define	M25		cMidleCoil[1].bits.b9
#define	M26		cMidleCoil[1].bits.b10
#define	M27		cMidleCoil[1].bits.b11
#define	M28		cMidleCoil[1].bits.b12
#define	M29		cMidleCoil[1].bits.b13
#define	M30		cMidleCoil[1].bits.b14
#define	M31		cMidleCoil[1].bits.b15

#define	M32		cMidleCoil[2].bits.b0
#define	M33		cMidleCoil[2].bits.b1
#define	M34		cMidleCoil[2].bits.b2
#define	M35		cMidleCoil[2].bits.b3
#define	M36		cMidleCoil[2].bits.b4
#define	M37		cMidleCoil[2].bits.b5
#define	M38		cMidleCoil[2].bits.b6
#define	M39		cMidleCoil[2].bits.b7
#define	M40		cMidleCoil[2].bits.b8
#define	M41		cMidleCoil[2].bits.b9
#define	M42		cMidleCoil[2].bits.b10
#define	M43		cMidleCoil[2].bits.b11
#define	M44		cMidleCoil[2].bits.b12
#define	M45		cMidleCoil[2].bits.b13
#define	M46		cMidleCoil[2].bits.b14
#define	M47		cMidleCoil[2].bits.b15

#define	M48		cMidleCoil[3].bits.b0
#define	M49		cMidleCoil[3].bits.b1
#define	M50		cMidleCoil[3].bits.b2
#define	M51		cMidleCoil[3].bits.b3
#define	M52		cMidleCoil[3].bits.b4
#define	M53		cMidleCoil[3].bits.b5
#define	M54		cMidleCoil[3].bits.b6
#define	M55		cMidleCoil[3].bits.b7
#define	M56		cMidleCoil[3].bits.b8
#define	M57		cMidleCoil[3].bits.b9
#define	M58		cMidleCoil[3].bits.b10
#define	M59		cMidleCoil[3].bits.b11
#define	M60		cMidleCoil[3].bits.b12
#define	M61		cMidleCoil[3].bits.b13
#define	M62		cMidleCoil[3].bits.b14
#define	M63		cMidleCoil[3].bits.b15

#define	M64		cMidleCoil[4].bits.b0
#define	M65		cMidleCoil[4].bits.b1
#define	M66		cMidleCoil[4].bits.b2
#define	M67		cMidleCoil[4].bits.b3
#define	M68		cMidleCoil[4].bits.b4
#define	M69		cMidleCoil[4].bits.b5
#define	M70		cMidleCoil[4].bits.b6
#define	M71		cMidleCoil[4].bits.b7
#define	M72		cMidleCoil[4].bits.b8
#define	M73		cMidleCoil[4].bits.b9
#define	M74		cMidleCoil[4].bits.b10
#define	M75		cMidleCoil[4].bits.b11
#define	M76		cMidleCoil[4].bits.b12
#define	M77		cMidleCoil[4].bits.b13
#define	M78		cMidleCoil[4].bits.b14
#define	M79		cMidleCoil[4].bits.b15

#define	M80		cMidleCoil[5].bits.b0
#define	M81		cMidleCoil[5].bits.b1
#define	M82		cMidleCoil[5].bits.b2
#define	M83		cMidleCoil[5].bits.b3
#define	M84		cMidleCoil[5].bits.b4
#define	M85		cMidleCoil[5].bits.b5
#define	M86		cMidleCoil[5].bits.b6
#define	M87		cMidleCoil[5].bits.b7
#define	M88		cMidleCoil[5].bits.b8
#define	M89		cMidleCoil[5].bits.b9
#define	M90		cMidleCoil[5].bits.b10
#define	M91		cMidleCoil[5].bits.b11
#define	M92		cMidleCoil[5].bits.b12
#define	M93		cMidleCoil[5].bits.b13
#define	M94		cMidleCoil[5].bits.b14
#define	M95		cMidleCoil[5].bits.b15

#define	M96			cMidleCoil[6].bits.b0
#define	M97			cMidleCoil[6].bits.b1
#define	M98			cMidleCoil[6].bits.b2
#define	M99			cMidleCoil[6].bits.b3
#define	M100		cMidleCoil[6].bits.b4
#define	M101		cMidleCoil[6].bits.b5
#define	M102		cMidleCoil[6].bits.b6
#define	M103		cMidleCoil[6].bits.b7
#define	M104		cMidleCoil[6].bits.b8
#define	M105		cMidleCoil[6].bits.b9
#define	M106		cMidleCoil[6].bits.b10
#define	M107		cMidleCoil[6].bits.b11
#define	M108		cMidleCoil[6].bits.b12
#define	M109		cMidleCoil[6].bits.b13
#define	M110		cMidleCoil[6].bits.b14
#define	M111		cMidleCoil[6].bits.b15

#define	M112		cMidleCoil[7].bits.b0
#define	M113		cMidleCoil[7].bits.b1
#define	M114		cMidleCoil[7].bits.b2
#define	M115		cMidleCoil[7].bits.b3
#define	M116		cMidleCoil[7].bits.b4
#define	M117		cMidleCoil[7].bits.b5
#define	M118		cMidleCoil[7].bits.b6
#define	M119		cMidleCoil[7].bits.b7
#define	M120		cMidleCoil[7].bits.b8
#define	M121		cMidleCoil[7].bits.b9
#define	M122		cMidleCoil[7].bits.b10
#define	M123		cMidleCoil[7].bits.b11
#define	M124		cMidleCoil[7].bits.b12
#define	M125		cMidleCoil[7].bits.b13
#define	M126		cMidleCoil[7].bits.b14
#define	M127		cMidleCoil[7].bits.b15


#define	M128		cMidleCoil[8].bits.b0
#define	M129		cMidleCoil[8].bits.b1
#define	M130		cMidleCoil[8].bits.b2
#define	M131		cMidleCoil[8].bits.b3
#define	M132		cMidleCoil[8].bits.b4
#define	M133		cMidleCoil[8].bits.b5
#define	M134		cMidleCoil[8].bits.b6
#define	M135		cMidleCoil[8].bits.b7
#define	M136		cMidleCoil[8].bits.b8
#define	M137		cMidleCoil[8].bits.b9
#define	M138		cMidleCoil[8].bits.b10
#define	M139		cMidleCoil[8].bits.b11
#define	M140		cMidleCoil[8].bits.b12
#define	M141		cMidleCoil[8].bits.b13
#define	M142		cMidleCoil[8].bits.b14
#define	M143		cMidleCoil[8].bits.b15

#define	M144		cMidleCoil[9].bits.b0
#define	M145		cMidleCoil[9].bits.b1
#define	M146		cMidleCoil[9].bits.b2
#define	M147		cMidleCoil[9].bits.b3
#define	M148		cMidleCoil[9].bits.b4
#define	M149		cMidleCoil[9].bits.b5
#define	M150		cMidleCoil[9].bits.b6
#define	M151		cMidleCoil[9].bits.b7
#define	M152		cMidleCoil[9].bits.b8
#define	M153		cMidleCoil[9].bits.b9
#define	M154		cMidleCoil[9].bits.b10
#define	M155		cMidleCoil[9].bits.b11
#define	M156		cMidleCoil[9].bits.b12
#define	M157		cMidleCoil[9].bits.b13
#define	M158		cMidleCoil[9].bits.b14
#define	M159		cMidleCoil[9].bits.b15

#define	M160		cMidleCoil[10].bits.b0
#define	M161		cMidleCoil[10].bits.b1
#define	M162		cMidleCoil[10].bits.b2
#define	M163		cMidleCoil[10].bits.b3
#define	M164		cMidleCoil[10].bits.b4
#define	M165		cMidleCoil[10].bits.b5
#define	M166		cMidleCoil[10].bits.b6
#define	M167		cMidleCoil[10].bits.b7
#define	M168		cMidleCoil[10].bits.b8
#define	M169		cMidleCoil[10].bits.b9
#define	M170		cMidleCoil[10].bits.b10
#define	M171		cMidleCoil[10].bits.b11
#define	M172		cMidleCoil[10].bits.b12
#define	M173		cMidleCoil[10].bits.b13
#define	M174		cMidleCoil[10].bits.b14
#define	M175		cMidleCoil[10].bits.b15

#define	M192		cMidleCoil[12].bits.b0
#define	M193		cMidleCoil[12].bits.b1
#define	M194		cMidleCoil[12].bits.b2
#define	M195		cMidleCoil[12].bits.b3
#define	M196		cMidleCoil[12].bits.b4
#define	M197		cMidleCoil[12].bits.b5
#define	M198		cMidleCoil[12].bits.b6
#define	M199		cMidleCoil[12].bits.b7
#define	M200		cMidleCoil[12].bits.b8
#define	M201		cMidleCoil[12].bits.b9
#define	M202		cMidleCoil[12].bits.b10
#define	M203		cMidleCoil[12].bits.b11
#define	M204		cMidleCoil[12].bits.b12
#define	M205		cMidleCoil[12].bits.b13
#define	M206		cMidleCoil[12].bits.b14
#define	M207		cMidleCoil[12].bits.b15

#define	M208		cMidleCoil[13].bits.b0
#define	M209		cMidleCoil[13].bits.b1
#define	M210		cMidleCoil[13].bits.b2
#define	M211		cMidleCoil[13].bits.b3
#define	M212		cMidleCoil[13].bits.b4
#define	M213		cMidleCoil[13].bits.b5
#define	M214		cMidleCoil[13].bits.b6
#define	M215		cMidleCoil[13].bits.b7
#define	M216		cMidleCoil[13].bits.b8
#define	M217		cMidleCoil[13].bits.b9
#define	M218		cMidleCoil[13].bits.b10
#define	M219		cMidleCoil[13].bits.b11
#define	M220		cMidleCoil[13].bits.b12
#define	M221		cMidleCoil[13].bits.b13
#define	M222		cMidleCoil[13].bits.b14
#define	M223		cMidleCoil[13].bits.b15

#define	M224		cMidleCoil[13].bits.b0
#define	M225		cMidleCoil[13].bits.b1
#define	M226		cMidleCoil[13].bits.b2
#define	M227		cMidleCoil[13].bits.b3
#define	M228		cMidleCoil[13].bits.b4
#define	M229		cMidleCoil[13].bits.b5
#define	M230		cMidleCoil[13].bits.b6
#define	M231		cMidleCoil[13].bits.b7
#define	M232		cMidleCoil[13].bits.b8
#define	M233		cMidleCoil[13].bits.b9
#define	M234		cMidleCoil[13].bits.b10
#define	M235		cMidleCoil[13].bits.b11
#define	M236		cMidleCoil[13].bits.b12
#define	M237		cMidleCoil[13].bits.b13
#define	M238		cMidleCoil[13].bits.b14
#define	M239		cMidleCoil[13].bits.b15

#endif /* MODUSASC_H_ */
