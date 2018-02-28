#ifndef	_APP_H_
#define	_APP_H_

#include "GUI.h"
#include "DIALOG.h"

#define  APP_DEBUG(format,...)	//printf("File:"__FILE__",Line:%03d:"format"\n",__LINE__,##__VA_ARGS__) 
/* 用户定义的消息 */
#define APPBKCOLOR  	GUI_YELLOW
#define APPTEXTCOLOR    GUI_MAGENTA


extern GUI_BITMAP bmphone;
extern GUI_BITMAP bmmessages;
extern GUI_BITMAP bmbrowser;
extern GUI_BITMAP bmapp;

extern GUI_BITMAP bmebooks;
extern GUI_BITMAP bmphotos;
extern GUI_BITMAP bmmusic;
extern GUI_BITMAP bmvideos;

extern GUI_BITMAP bmclock;
extern GUI_BITMAP bmexeplay;
extern GUI_BITMAP bmgames;
extern GUI_BITMAP bmnotes;

extern GUI_BITMAP bmsettings;
extern GUI_BITMAP bmrecoder;
extern GUI_BITMAP bmcamera;
extern GUI_BITMAP bmcalculator;

extern GUI_BITMAP bmradio;
extern GUI_BITMAP bmusb;
extern GUI_BITMAP bminternet;
extern GUI_BITMAP bmwireless;

extern GUI_BITMAP bmwifi;
extern GUI_BITMAP bmcompass;
extern GUI_BITMAP bmbluetooth;
extern GUI_BITMAP bmgps;

extern GUI_BITMAP bmlogo;   //GUI_CONST_STORAGE
extern const unsigned char *icos_path_tbl[3][12];

extern uint8_t OnICON0;
extern void OnICON0Clicked(void);

extern uint8_t OnICON1;
extern void OnICON1Clicked(void);

extern uint8_t OnICON2;
extern void OnICON2Clicked(void);

extern uint8_t OnICON3;
extern void OnICON3Clicked(void);

extern uint8_t OnICON4;
extern void OnICON4Clicked(void);

extern uint8_t OnICON5;
extern void OnICON5Clicked(void);

extern uint8_t OnICON6;
extern void OnICON6Clicked(void);

extern uint8_t OnICON7;
extern void OnICON7Clicked(void);

extern uint8_t OnICON8;
extern void OnICON8Clicked(void);

extern uint8_t OnICON9;
extern void OnICON9Clicked(void);

extern uint8_t OnICON10;
extern void OnICON10Clicked(void);

extern uint8_t OnICON11;
extern void OnICON11Clicked(void);

extern uint8_t OnICON12;
extern void OnICON12Clicked(void);

extern uint8_t OnICON13;
extern void OnICON13Clicked(void);

extern uint8_t OnICON14;
extern void OnICON14Clicked(void);

extern uint8_t OnICON15;
extern void OnICON15Clicked(void);

extern uint8_t OnICON16;
extern void OnICON16Clicked(void);

extern uint8_t OnICON17;
extern void OnICON17Clicked(void);

extern uint8_t OnICON18;
extern void OnICON18Clicked(void);

//extern uint8_t OnICON19;
//extern void OnICON19Clicked(void);

/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/
void MainAPP(void);
void Soft_Reset(void);
void Touch_MainTask(void);
extern __IO uint8_t app_prevent_refresh;
#endif	//_APP_H_
