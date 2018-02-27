#ifndef ManualProc_H
#define ManualProc_H

#define XCW 0// 远离原点
#define XCCW 1// 靠近原点
#define YCW 0// 远离原点
#define YCCW 1// 靠近原点


extern unsigned char cXRstStep,cYRstStep;
extern long dwRealPosi,cSetPulse;
//extern unsigned short ManuSpeed;
extern unsigned short  DrllUpDownDly;
extern void ManualProc_XAxis(void);
extern void ManualProc_YAxis(void);
extern void ManualProc_IO(void);
extern void RebackXaxisOrign(void);
extern void RebackYaxisOrign(void);
extern void Hole_MuBanSelect(void);
extern void Cao_MuBanSelect(void);
extern void Scan_MuBanSelect(void);

#endif

