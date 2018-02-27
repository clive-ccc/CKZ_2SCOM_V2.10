#include "Global.h"

CurAxisDistStr *CurAxisDistance;
SpeedParam_Str *SpeedParam;
TimeParam_Str  *TimeParam;
DrillOffset_str *DrillOffset;
DrillOffset2_str *DrillOffset2;
CustomPlug_Str *CustomPlug;
HolePos_str * HolePos;
VerData_str *VerData;

/************************************************/
/*
初始化系统变量

*/
/************************************************/
// 第一次上电时的初始化
void FirstInit(void)
{
    unsigned char *buf = (unsigned char *)&cDataArea[0];
	unsigned short i;

	memset(cDataArea,0,512*2);

	for (i = 0; i < 512;i++) 
    {
        buf[i]=Read24C04(i);
    } 
    if(cDataArea[0] != 0x55)
    {
        cDataArea[0] = 0x55;

        CurBoardData->SystemMode1 = 1;
        CurBoardData->ScanMode1 = 1;

        SpeedParam->XCalculate = 1;
        // 厂家参数
        cDataArea[2] = 0;
        cDataArea[30] = 10;
        cDataArea[47] = 50;
        cDataArea[31] = 30;
        cDataArea[32] = 75;
        cDataArea[36] = 50;
        cDataArea[65] = 2000;
        TimeParam->MaxLength = 240000;
        cDataArea[62] = 10000;
        cDataArea[35] = 5;
        cDataArea[33] = 30;
        cDataArea[34] = 20;
        cDataArea[86] = 500;
        cDataArea[88] = 1000;
        TimeParam->XMaxLength = 4500;
        cDataArea[68] = 1000;
        cDataArea[69] = 1800;
        cDataArea[61] = 50;
        cDataArea[7] = 50;
        cDataArea[11] = 1;
        cDataArea[12] = 1;
        cDataArea[10] = 1;
        cDataArea[49] = 20;
        CustomPlug->CyclePulse = 5000;
        // 扫描打孔
        cDataArea[15] = 3200;
        cDataArea[55] = 1000;
        cDataArea[56] = 1000;
        cDataArea[57] = 1000;
        cDataArea[58] = 1000;
        cDataArea[59] = 1000;
        cDataArea[43] = 0;
        cDataArea[41] = 0;
        cDataArea[87] = 1000;
        cDataArea[46] = 1000;
        
        cDataArea[240] = 1000;
        cDataArea[241] = 1000;
        cDataArea[242] = 1000;
        cDataArea[243] = 1000;
        cDataArea[244] = 1000;
        // 定位打孔
        cDataArea[45] = 0;
        
        cDataArea[250] = 1000;
        cDataArea[251] = 1000;
        cDataArea[252] = 1000;
        cDataArea[253] = 1000;
        cDataArea[254] = 1000;
        // 定位铣槽
        cDataArea[44] = 0;
        cDataArea[37] = 800;
        cDataArea[89] = 500;
        cDataArea[245] = 1000;
        cDataArea[246] = 1000;
        cDataArea[247] = 1000;
        cDataArea[248] = 1000;
        cDataArea[249] = 1000;
        // 客户参数
        cDataArea[60] = 10;
        cDataArea[66] = 150;
        cDataArea[81] = 1000;
        TimeParam->Work1Dist = 0;
        TimeParam->Work2Dist = 80000;
        TimeParam->Work3Dist = 160000;
        cDataArea[153] = 30;
        cDataArea[154] = 30;

        for(i=0;i<512;i++)
		{
			Write24C04(i, buf[i]);
		}
    }

}
// 版本管理
void VerControl(void)
{
	VerData->Ver1 = 3;
	VerData->Ver2 = 0;
	VerData->Ver3 = 0;
}

void WorkModeInit(void)
{
	if(TimeParam->WorkMode == WorkMode_Scan)
	{
		bScanHole = 1;
		bLocationHole = 0;
		bLocationCao = 0;
	}
	else if(TimeParam->WorkMode == WorkMode_Slot)
	{
		bScanHole = 0;
		bLocationHole = 0;
		bLocationCao = 1;		
	}
	else if(TimeParam->WorkMode == WorkMode_Hole)
	{
		bScanHole = 0;
		bLocationHole = 1;
		bLocationCao = 0;		
	}
}

void PlugInit(void)
{
    cChaXiaoBackup = SpeedParam->ChaXiaoMode;
	if(SpeedParam->ChaXiaoMode == PlugTWWai)
	{
        bNOChaXiao = 0;
        bTWNei = 0;
        bTWWai = 1;
        bCustomPlugBeforHole = 0;
        bCustomPlugAll = 0;
        bCustomPlugafterHole = 0;
        bCustomPlugState = 0;
        CustomPlug->OneHolePlug = 0;
	}
	else if(SpeedParam->ChaXiaoMode == PlugTWNei)
	{
        bNOChaXiao = 0;
        bTWNei = 1;
        bTWWai = 0;
        bCustomPlugBeforHole = 0;
        bCustomPlugAll = 0;
        bCustomPlugafterHole = 0;
        bCustomPlugState = 0;
        CustomPlug->OneHolePlug = 0;
	}
    else if(SpeedParam->ChaXiaoMode == 3)
    {
        bNOChaXiao = 0;
        bTWNei = 0;
        bTWWai = 0;
        bCustomPlugBeforHole = 0;
        bCustomPlugAll = 0;
        bCustomPlugafterHole = 0;
        bCustomPlugState = 1;
        CustomPlug->OneHolePlug = 0;
    }
    else if(SpeedParam->ChaXiaoMode == 4)
    {
        bNOChaXiao = 0;
        bTWNei = 0;
        bTWWai = 0;
        bCustomPlugBeforHole = 0;
        bCustomPlugAll = 1;
        bCustomPlugafterHole = 0;
        bCustomPlugState = 0;
        CustomPlug->OneHolePlug = 0;
    }
    else if(SpeedParam->ChaXiaoMode == 5)
    {
        bNOChaXiao = 0;
        bTWNei = 0;
        bTWWai = 0;
        bCustomPlugBeforHole = 0;
        bCustomPlugAll = 0;
        bCustomPlugafterHole = 1;
        bCustomPlugState = 0;
        CustomPlug->OneHolePlug = 0;
    }
    else if(SpeedParam->ChaXiaoMode == 6)
    {
        bNOChaXiao = 0;
        bTWNei = 0;
        bTWWai = 0;
        bCustomPlugBeforHole = 1;
        bCustomPlugAll = 0;
        bCustomPlugafterHole = 0;
        bCustomPlugState = 0;
        CustomPlug->OneHolePlug = 0;
    }
    else if(SpeedParam->ChaXiaoMode == 0)
	{
        bNOChaXiao = 1;
        bTWNei = 0;
        bTWWai = 0;
        bCustomPlugBeforHole = 0;
        bCustomPlugAll = 0;
        bCustomPlugafterHole = 0;
        bCustomPlugState = 0;
        CustomPlug->OneHolePlug = 0;
    }
    else if(SpeedParam->ChaXiaoMode == 7)
	{
        bNOChaXiao = 0;
        bTWNei = 0;
        bTWWai = 0;
        bCustomPlugBeforHole = 0;
        bCustomPlugAll = 0;
        bCustomPlugafterHole = 0;
        bCustomPlugState = 0;
        bPlugforOneHole = 1;
        bPlugforTwoHole = 0;
        CustomPlug->OneHolePlug = 0;
    }
    else if(SpeedParam->ChaXiaoMode == 8)
	{
        bNOChaXiao = 0;
        bTWNei = 0;
        bTWWai = 0;
        bCustomPlugBeforHole = 0;
        bCustomPlugAll = 0;
        bCustomPlugafterHole = 0;
        bCustomPlugState = 0;
        bPlugforOneHole = 0;
        bPlugforTwoHole = 1;
        CustomPlug->OneHolePlug = 0;
    }

    cMidleCoil[3].Val = cDataArea[150];
    cMidleCoil[4].Val = cDataArea[151];

    if(cMidleCoil[3].Val == 0xffff && cMidleCoil[4].Val == 0xffff)
    {
        bCustomPlugAll = 1;
        bCustomPlugafterHole = 0;
        bCustomPlugBeforHole = 0;
    }
    else if(cMidleCoil[3].Val == 0xffff)
    {
        bCustomPlugAll = 0;
        bCustomPlugafterHole = 0;
        bCustomPlugBeforHole = 1;
    }
    else if(cMidleCoil[4].Val == 0xffff)
    {
        bCustomPlugAll = 0;
        bCustomPlugafterHole = 1;
        bCustomPlugBeforHole = 0;
    }
}

void PointInit(void)
{
	VerData = (VerData_str *)(cDataArea + 3);
	CurBoardData=(CurSystemData_str *)(cDataArea + 6);
	CurAxisDistance=(CurAxisDistStr *)(cDataArea + CurAxisDistStr_Offset);//&cDataArea[EXAXISLCT];
	TimeParam=(TimeParam_Str *)(cDataArea + TimeParamStr_Offset);//&cDataArea[ETIMEPARAM];
	SpeedParam = (SpeedParam_Str *)(cDataArea + SpeedParamStr_Offset);
	DrillOffset=(DrillOffset_str *)(cDataArea + DrillOffsetstr_offset);
    DrillOffset2=(DrillOffset2_str *)(cDataArea + DrillOffset2str_offset);
    CustomPlug = (CustomPlug_Str *)(cDataArea + 150);
    HolePos = (HolePos_str *)(cDataArea + 300);
}
//运行变量初始化
void InitVariable(void)
{
	PointInit();
	FirstInit();
	VerControl();
	WorkModeInit();
	PlugInit();

	RebackXYFLG=1;
	cAxisRunState.Val=0;

	if (SystemMode > 1)
	{
        SystemMode = 0;
	}
	
	ServorDlay= dwTickCount + SERVORDLY;
	while(ServorDlay < dwTickCount);
}
/************************************************/
/*
初始化是否的速度

*/
/************************************************/
/*void ServoSpeedInit(void )
{
	/*X_ALARMEN=1; //驱动器故障使能
	Y_ALARMEN=1;
	MV_Set_Range(X_AXIS,100);
	MV_Set_Range(Y_AXIS,100);

	MV_Set_Acc(X_AXIS,200);
	MV_Set_Acc(Y_AXIS,100);

	MV_Set_Dec(X_AXIS,300);	
	MV_Set_Dec(Y_AXIS,100);	

	MV_Set_Startv(X_AXIS,10);
	MV_Set_Startv(Y_AXIS,10);

	MV_Set_Speed(X_AXIS,400);
	MV_Set_Speed(Y_AXIS,400);

	CalDecStop(X_CurSpeed,&XCurSpeedPusel);
	CalAccRun(X_CurSpeed,&XCurSpeedPusel);
	CalDecStop(Y_CurSpeed,&YCurSpeedPusel);
	CalAccRun(Y_CurSpeed,&YCurSpeedPusel);

	MV_Set_Command_Pos(X_AXIS,0);
	MV_Set_Command_Pos(Y_AXIS,0);

	//MV_Set_Startv(X_AXIS,10);
	//MV_Set_Speed(X_AXIS,300);
	//cSetPulse=4000;
	//MV_Pmov(X_AXIS, cSetPulse);
	
	//MV_Continue_Move(X_AXIS, 0);
	//MV_Continue_Move(Y_AXIS, 0);
	//MV_Pmov(X_AXIS, 100);
	
	//MV_Set_Command_SlMTN(X_AXIS, -1000);
	//MV_Set_Command_SlMTP(X_AXIS, 10000);
	//MV_Set_Command_SlMTN(Y_AXIS, -1000);
	//MV_Set_Command_SlMTP(Y_AXIS, 10000);

}

*/
