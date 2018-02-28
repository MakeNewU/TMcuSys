#include "includes.h"
#include "app.h"
#include "rtc.h"   //teng
#include "EmWinHZFont.h"//teng
#include "ff.h"//teng
#include "malloc.h"//teng
/*
*********************************************************************************************************
*                                         �궨�� 
*********************************************************************************************************
*/
#define ICONVIEW_Width     48   /* �ؼ�ICONVIEW�Ŀ� */  
#define ICONVIEW_Height    48   /* �ؼ�ICONVIEW�ĸߣ�����ͼ���Y������ */  
#define ICONVIEW_XSpace    10   /* �ؼ�ICONVIEW��X������ */ 
#define ICONVIEW_YSpace    10   /* �ؼ�ICONVIEW��Y������ */ 
#define ICONVIEW_XPos	   4    /* �ؼ�ICONVIEW��X�����������*/
#define VIRTUAL_WIN_NUM	   2    /* icon��ҳ�� */
#define ICONVIEW_ImagNum   20   /* �ؼ�ICONVIEW�е�ͼ����� */ 
#define BOTWIN_YSpace	   60   /* ��Ļ�·����ִ���Y������*/
#define MIDWIN_yPos		   20   /* �м䴰��Y������ʼλ�ü��������ֿ��ø߶�*/
#define MIDWIN_xPos		   0 	/* �м䴰��Y������ʼλ��*/

/*
*********************************************************************************************************
*                                      ����
*********************************************************************************************************
*/ 
/*ʱ��ṹ��*/
//extern struct rtc_time systmtime;
extern _calendar_obj calendar;//ʱ�ӽṹ��
extern char TimeDisplay;
//GUI_XBF_DATA XBF_Data;
//GUI_FONT     XBF_Font;

uint8_t OnICON0  = 0;   /* ICONVIEW�ؼ����µı�־��0��ʾδ���£�1��ʾ���� */
uint8_t OnICON1  = 0;
uint8_t OnICON2  = 0;
uint8_t OnICON3  = 0;
uint8_t OnICON4  = 0;
uint8_t OnICON5  = 0;
uint8_t OnICON6  = 0;
uint8_t OnICON7  = 0;
uint8_t OnICON8  = 0;
uint8_t OnICON9  = 0;
uint8_t OnICON10  = 0;
uint8_t OnICON11  = 0;
uint8_t OnICON12  = 0;
uint8_t OnICON13  = 0;
uint8_t OnICON14  = 0;
uint8_t OnICON15  = 0;
uint8_t OnICON16  = 0;
uint8_t OnICON17  = 0;
uint8_t OnICON18  = 0;
uint8_t OnICON19  = 0;
uint8_t OnICON20  = 0;
uint8_t OnICON21  = 0;
uint8_t OnICON22  = 0;
uint8_t OnICON23  = 0;



/* ��������ICONVIEWͼ��Ĵ��� */
typedef struct {
  GUI_BITMAP * pBitmap;
  const char       * pText;
} BITMAP_ITEM;

typedef struct WIN_PARA{			//����ʹ�õ����û���������������ڻص�������ʹ��
	int xSizeLCD, ySizeLCD;			//LCD��Ļ�ߴ�
	int xPosWin,  yPosWin;			//���ڵ���ʼλ��
	int xSizeWin, ySizeWin;			//���ڳߴ�	
	int xSizeBM,  ySizeBM;
	int ySizeBotWin;
	
	WM_HWIN hWinBot;				//���ƴ��ڵľ�����ײ���
	WM_HWIN hWinMid;				//����ʾ���ľ�����в���
	
}WIN_PARA;
WIN_PARA WinPara;			//�û�ʹ�õĴ��ڶ�������
__IO uint8_t app_prevent_refresh = 0;  /* ��������Ƚ���Ҫ�����ڷ�ֹICON�ؼ������ʱ�ʹ����ص�������WM_PAINT��Ϣ*/
/* ��������ICONVIEW0ͼ��Ĵ��� */
static BITMAP_ITEM _aBitmapItem0[] = {
	{&bmphone,     "����" 	},
	{&bmmessages,  "����"	},
	{&bmbrowser,   "�����" 	},
	{&bmapp,  	   "Ӧ��"	},
};
/* ��������ICONVIEW1ͼ��Ĵ��� */
static BITMAP_ITEM _aBitmapItem1[] = {
	{&bmebooks,		"������"	},
	{&bmphotos,  	"���"	},  
//	{&bmmusic,      "����"	},
//	{&bmvideos,		"��Ƶ"	},

//	{&bmclock,		"ʱ��" 	},
//	{&bmexeplay, 	"������"},
//	{&bmgames,   	"��Ϸ" 	},
//	{&bmnotes,  	"���±�"},

//	{&bmsettings, 	"����"	},  
//	{&bmrecoder,  	"¼����" },
//	{&bmcamera,     "�����" },
//	{&bmcalculator,	"������"	}
};

/* ��������ICONVIEW2ͼ��Ĵ��� */
static BITMAP_ITEM _aBitmapItem2[] = {
	{&bmradio,  	"������"		},
//	{&bmusb,		"USB����" 	},
//	{&bminternet,   "������" 	},
//	{&bmwireless,   "����"		},
//	
//	{&bmwifi,		"WIFI"		},  
//	{&bmcompass,   	"����" 		},	
//	{&bmbluetooth,  "����" 		},
//	{&bmgps,    	"GPS"		},
	
//	{&bmcalendar,	"����"		},  
//	{&bmxxx,   		"xxx" 		},	
//	{&bmxxx,  		"xxx" 		},
//	{&bmxxx,    	"xxx"		}
};



//void OnICON0Clicked(void)  {}
//void OnICON1Clicked(void)  {}
//void OnICON2Clicked(void)  {}
//void OnICON3Clicked(void)  {}
//void OnICON4Clicked(void)  {}
//void OnICON5Clicked(void)  {}
//void OnICON6Clicked(void)  {}
//void OnICON7Clicked(void)  {}
//void OnICON8Clicked(void)  {}
//void OnICON9Clicked(void)  {}
//void OnICON10Clicked(void) {}
//void OnICON11Clicked(void) {}
//void OnICON12Clicked(void) {}
//void OnICON13Clicked(void) {}
//void OnICON14Clicked(void) {}
//void OnICON15Clicked(void) {}
//void OnICON16Clicked(void) {}
//void OnICON17Clicked(void) {}
//void OnICON18Clicked(void) {}
//void OnICON19Clicked(void) {}
//void OnICON20Clicked(void) {}
//void OnICON21Clicked(void) {}
//void OnICON22Clicked(void) {}
//void OnICON23Clicked(void) {}
	


u8 *ICONBuff[3][12];//ͼ�껺��Ҫ������Ч,һ��ͼ���Լ9-12KB
//���ⲿ����ͼ��.bin�ļ�
//ix:��ǰͼ��
//�ĸ�����ͼ��0�ײ�1��һҳ2�ڶ�ҳ
u8 Load_ICON(u8 ix, GUI_BITMAP *pBitmapx, u8 itemx)
{
	FIL ICONFile;	  		//�ļ�1
	UINT br,bw;				//��д����
	u8 res;
//	u8 *databuf = NULL;

	res = f_open(&ICONFile, (u8*)icos_path_tbl[itemx][ix], FA_READ);     //��ͼ���ļ�
	if(res == FR_OK)
	{
		ICONBuff[itemx][ix] = (u8*)mymalloc(SRAMEX, ICONFile.fsize);     //����ͼ���С���ⲿ�ڴ�
		if(ICONBuff[itemx][ix] == NULL)
		{
			f_close(&ICONFile);                                          //����ʧ���˳�
			return 0x01;
		}
		res = f_read(&ICONFile, ICONBuff[itemx][ix], ICONFile.fsize, &br);    //��ȡͼ���ļ����뻺����
		if(res == FR_OK)
		{
			pBitmapx->pData = ICONBuff[itemx][ix];                            //����ͼ�껺��ָ��
		}
	}
	else
	{
		return res;
	}
	f_close(&ICONFile);                                                       //�ر�ͼ���ļ�
//	myfree(SRAMEX, databuf);                                                  //����ͼ����Ч�����ͷ�
	return res;
}





/*
*********************************************************************************************************
*	�� �� ��: _cbBkWindow
*	����˵��: ���洰�ڵĻص����� 
*	��    �Σ�WM_MESSAGE * pMsg
*	�� �� ֵ: ��
*********************************************************************************************************
*/
char TimeDisplay=0;//teng
void _cbBkWindow(WM_MESSAGE * pMsg) 
{
	char text_buffer[20]={0};
	WM_HWIN hText;
	switch (pMsg->MsgId) 
	{
		case WM_TIMER://����ʱ����ʾ����Ϣ��ÿ�����޸�һ��ʱ��
//			if(!TimeDisplay)
//			{
//				WM_RestartTimer(pMsg->Data.v, 1000);
//				break;
//			}
			/* ��ȡtext��� */      
			hText = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT1);
			
			/* ת��rtcֵ������ʱ�� */
//			RTC_TimeCovr(&systmtime);
            RTC_Get();
			/* ת�����ַ��� */
			sprintf(text_buffer,"%02d:%02d:%02d",calendar.hour,calendar.min,calendar.sec);  //teng
			/* ���ʱ�� */
			TEXT_SetText(hText,text_buffer);
		
			/* ��ȡtext��� */      
			hText = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT2);
			sprintf(text_buffer,"CPU:%3d.%02d%%",OSStatTaskCPUUsage/100,OSStatTaskCPUUsage%100);
			/* ���CPU */
			TEXT_SetText(hText,text_buffer);
		
			WM_RestartTimer(pMsg->Data.v, 1000);
		break;
		/* �ػ���Ϣ*/
		case WM_PAINT:		
				GUI_SetBkColor(0x00fd71a9);      //GUI_ORANGE
				GUI_Clear();
//				GUI_DrawBitmap(&bmlogo,1,1);
				ICONVIEW_SetSel(WM_GetDialogItem(WinPara.hWinMid, GUI_ID_ICONVIEW2),-1);
				ICONVIEW_SetSel(WM_GetDialogItem(WinPara.hWinMid, GUI_ID_ICONVIEW1),-1);
				ICONVIEW_SetSel(WM_GetDialogItem(WinPara.hWinBot, GUI_ID_ICONVIEW0),-1);			
		break;
			
	 default:
		WM_DefaultProc(pMsg);
		break;
	}
}
/*
*********************************************************************************************************
*	�� �� ��: _cbMidWin
*	����˵��: 
*	��    �Σ�WM_MESSAGE * pMsg
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void _cbMidWin(WM_MESSAGE * pMsg) 
{
	int NCode, Id;
	WM_MOTION_INFO * pInfo;
	switch (pMsg->MsgId) 
	{
		case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);      /* Id of widget */
			NCode = pMsg->Data.v;                 /* Notification code */
			switch (Id) 
			{
				case GUI_ID_ICONVIEW1:
					switch (NCode) 
					{
						/* ICON�ؼ������Ϣ */
						case WM_NOTIFICATION_CLICKED:
							app_prevent_refresh = 1;
							break;
						
						/* ICON�ؼ��ͷ���Ϣ */
						case WM_NOTIFICATION_RELEASED: 
							
							/* ����Ӧѡ�� */
							switch(ICONVIEW_GetSel(pMsg->hWinSrc))
							{
								/* LED  ******************************************************************/
								case 0:	
									OnICON4 = 1;
//									OnICON4Clicked();			
									break;	
								
								/* KEY ***********************************************************************/
								case 1:
									OnICON5 = 1;
//									OnICON5Clicked();
									break;
								
								/* SRAM *********************************************************************/
								case 2:
									OnICON6 = 1;
//									OnICON6Clicked();	
									break;
								
								/* CAN ********************************************************************/
								case 3:
									OnICON7 = 1;
//									OnICON7Clicked();
									break;
								
								/* EEPROM **********************************************************************/
								case 4:
									OnICON8 = 1;
//									OnICON8Clicked();
									break;
								
								/* ADC ********************************************************************/
								case 5:
									OnICON9 = 1;
//									OnICON9Clicked();
									break;
								
								/* DS18B20 ******************************************************************/
								case 6:					
									OnICON10 = 1;
//									OnICON10Clicked();
									break;
								
								/* TEMP *******************************************************************/
								case 7:
									OnICON11 = 1;
//									OnICON11Clicked();
									break;
								
								/* Breathing ******************************************************************/
								case 8:
									OnICON12 = 1;
//									OnICON12Clicked();
									break;
								 
								 /* Clock ******************************************************************/
								case 9:
									OnICON13 = 1;
//									OnICON13Clicked();
									break;
								 
								 /* FM ******************************************************************/
								case 10:
									OnICON14 = 1;
//									OnICON14Clicked();
									break;
								 
								/* Music *******************************************************************/
								case 11:
									OnICON15 = 1;
//									OnICON15Clicked();
									break;
								default:break;
								}
							 break;
						}
					break;
				case GUI_ID_ICONVIEW2:
					switch (NCode) 
					{
						/* ICON�ؼ������Ϣ */
						case WM_NOTIFICATION_CLICKED:
							app_prevent_refresh = 1;
							break;
						
						/* ICON�ؼ��ͷ���Ϣ */
						case WM_NOTIFICATION_RELEASED: 
							
							/* ����Ӧѡ�� */
							switch(ICONVIEW_GetSel(pMsg->hWinSrc))
							{			
								
								/* USB *******************************************************************/
								case 0:
									OnICON16 = 1;
//									OnICON16Clicked();
									break;
								
								/* Calculator ***************************************************************/
								case 1:
									OnICON17 = 1;
//									OnICON17Clicked();
									break;

								/* Compass ***************************************************************/
								case 2:
									OnICON18 = 1;
								//	OnICON18Clicked();
									break;
								
								/* Picture ********************************************************************/
								case 3:
									OnICON19 = 1;
//									OnICON19Clicked();
									break;
								
								/* Browser **************************************************************/
								case 4:
									OnICON20 = 1;
//									OnICON20Clicked();
									break;
								
								/* Camera ***************************************************************/
								case 5:
									OnICON21 = 1;
//									OnICON21Clicked();
									break;
								
								/* Bluetooth **********************************************************/
								case 6:
									OnICON22 = 1;
//									OnICON22Clicked();
									break;
								
								/* Zigbee ************************************************************/
								case 7:
									OnICON23 = 1;
//									OnICON23Clicked();
									break;
								default:break;
							}	
						 break;
					}
				break;
			}
			break;
		
		case WM_MOTION:
			 pInfo = (WM_MOTION_INFO *)pMsg->Data.p;
			 if (pInfo) 
			 {
				//printf("mooton_message=%d\n",pInfo->Cmd);
				 switch (pInfo->Cmd) 
				 {				    
					case WM_MOTION_INIT:
						pInfo->SnapX = 240;		//���ض���
						pInfo->SnapY = 0;
						//pInfo->Flags = WM_CF_MOTION_X | WM_MOTION_MANAGE_BY_WINDOW;
						//pInfo->Period = 500;
					break;
					
					case WM_MOTION_MOVE:
						WM_DisableWindow(WM_GetDialogItem(pMsg->hWin,GUI_ID_ICONVIEW1));
						WM_DisableWindow(WM_GetDialogItem(pMsg->hWin,GUI_ID_ICONVIEW2));
					break;							
					case WM_MOTION_GETPOS:
//						printf("\n dx = %d",pInfo->dx);
//						printf(" xpos = %d\n",pInfo->xPos);
//						printf(" da = %d\n",pInfo->da);
//						printf(" SnapX = %d\n",pInfo->SnapX);
					break;								
					default:
						break;						
				  }
				/* �ƶ����� */
				if (pInfo->FinalMove || pInfo->dx==0)
				{
					WM_EnableWindow(WM_GetDialogItem(pMsg->hWin,GUI_ID_ICONVIEW1));
					WM_EnableWindow(WM_GetDialogItem(pMsg->hWin,GUI_ID_ICONVIEW2));
				}
			 }	
			
			break;
			
		/* �ػ���Ϣ*/
		case WM_PAINT:
			{
				GUI_DrawGradientV(0,0,WinPara.xSizeWin,WinPara.ySizeWin,0x00f7aac9,0x00f95d9b);
			//	GUI_DrawGradientV(0,0,WinPara.xSizeWin,WinPara.ySizeWin,GUI_ORANGE+0x303000,GUI_LIGHTYELLOW-0x303000);
			//	GUI_SetBkColor(GUI_RED);
			//	GUI_Clear();
			}			
		break;			
	 default:
		WM_DefaultProc(pMsg);
		break;
	}
}


/**
  * @brief  _cbBotWin,�������ص�����
  * @param  none
  * @retval none
  */
static void _cbBotWin(WM_MESSAGE * pMsg)
{
	int NCode, Id;
	switch (pMsg->MsgId) 
	{			 		 
		case WM_PAINT:	                             //�ػ汳��	
			GUI_SetBkColor(0x00ed9cbc);         //GUI_LIGHTYELLOW
			GUI_Clear();
		break;
		case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);      /* Id of widget */
			NCode = pMsg->Data.v;                 /* Notification code */
		switch (Id) 
		{
			case GUI_ID_ICONVIEW0:
				switch (NCode) 
				{
					/* ICON�ؼ������Ϣ */
					case WM_NOTIFICATION_CLICKED:
						app_prevent_refresh = 1;
						break;
					
					/* ICON�ؼ��ͷ���Ϣ */
					case WM_NOTIFICATION_RELEASED: 
						
						/* ����Ӧѡ�� */
						switch(ICONVIEW_GetSel(pMsg->hWinSrc))
						{
							/* Phone *******************************************************************/
							case 0:	
								OnICON0 = 1;
								//printf("this is the NO.1 ICON\n");
								CreatePerfMon();
//								OnICON0Clicked();
								break;	
							
							/* Message ***********************************************************************/
							case 1:
								OnICON1 = 1;
								//printf("this is the NO.2 ICON\n");
//								OnICON1Clicked();
//								CreatePicture();
								break;
							
							/* Note *********************************************************************/
							case 2:
								OnICON2 = 1;
								//printf("this is the NO.3 ICON\n");
//								CreateBrowser();
//								OnICON2Clicked();	
								break;
							
							/* Ethernet *******************************************************************/
							case 3:
								OnICON3 = 1;
								//printf("this is the NO.4 ICON\n");
//								OnICON3Clicked();
								break;
						}	
				 break;
			}
		break;
	}
	break;		
	default:		
		WM_DefaultProc(pMsg);
	break;		 
	}
}

/**
  * @brief  CreateTopWin�����������Ĵ���
  * @param  none
  * @retval none
  */
static void CreateTopWin(void)
{
	WM_HWIN hText;
	/* �������ı� */
	hText = TEXT_CreateEx(3, 2, 60 , 16, WM_HBKWIN, WM_CF_SHOW, GUI_TA_LEFT|TEXT_CF_VCENTER, GUI_ID_TEXT0, "WZTENG");
	TEXT_SetFont(hText,GUI_FONT_10_ASCII);
	TEXT_SetTextColor(hText,GUI_DARKGRAY);
	
//	GUI_DrawBitmap(&bmlogo,1,1);
	
	/* ״̬����ʱ����ʾ�ı� */
	hText = TEXT_CreateEx(WinPara.xSizeLCD/2-25,2,50,16,WM_HBKWIN,WM_CF_SHOW,GUI_TA_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT1,"11:01:29");
	//TEXT_SetBkColor(hText,GUI_INVALID_COLOR);
	TEXT_SetTextColor(hText,GUI_DARKGRAY);
	TEXT_SetFont(hText,GUI_FONT_10_ASCII);
	
	/* ״̬����CPU��ʾ�ı� */
	hText = TEXT_CreateEx(170,2,70,16,WM_HBKWIN,WM_CF_SHOW,TEXT_CF_VCENTER,GUI_ID_TEXT2,"CPU:000.00%");
	//TEXT_SetBkColor(hText,GUI_INVALID_COLOR);
	TEXT_SetTextColor(hText,GUI_DARKGRAY);
	TEXT_SetFont(hText,GUI_FONT_10_ASCII);
}
/**
  * @brief  CreateMidWin�������м�Ĵ���
  * @param  none
  * @retval none
  */
static void CreateMidWin(void)
{
	uint8_t i=0;
	WM_HWIN MIDWIN;
	WinPara.hWinMid= WM_CreateWindowAsChild(
											WinPara.xPosWin,											
											WinPara.yPosWin,
											WinPara.xSizeWin,
											WinPara.ySizeWin,	
											WM_HBKWIN, 
											WM_CF_SHOW | WM_CF_MOTION_X, 
											_cbMidWin, 
											sizeof(WIN_PARA *)
											);
/*-------------------------------------------------------------------------------------*/
	/*��ָ��λ�ô���ָ���ߴ��ICONVIEW1 С����*/
	MIDWIN=ICONVIEW_CreateEx(
							 ICONVIEW_XPos, 					/* С���ߵ��������أ��ڸ������У�*/
							 0, 								/* С���ߵ��������أ��ڸ������У�*/
							 WinPara.xSizeLCD-ICONVIEW_XPos,	/* С���ߵ�ˮƽ�ߴ磨��λ�����أ�*/
							 3*(WinPara.ySizeBM+30), 			/* С���ߵĴ�ֱ�ߴ磨��λ�����أ�*/
							 WinPara.hWinMid, 				    /* �����ڵľ�������Ϊ0 ������С���߽���Ϊ���棨�������ڣ����Ӵ��� */
							 WM_CF_SHOW | WM_CF_HASTRANS,       /* ���ڴ�����ǡ�ΪʹС���������ɼ���ͨ��ʹ�� WM_CF_SHOW */ 
							 0,//ICONVIEW_CF_AUTOSCROLLBAR_V, 	/* Ĭ����0�����������ʵ������������ֱ������ */
							 GUI_ID_ICONVIEW1, 			        /* С���ߵĴ���ID */
							 ICONVIEW_Width, 				    /* ͼ���ˮƽ�ߴ� */
							 ICONVIEW_Height+13);				/* ͼ��Ĵ�ֱ�ߴ� */
											 
	/* ��ICONVIEW С���������ͼ�� */
	for (i = 0; i < GUI_COUNTOF(_aBitmapItem1); i++) 
	{
		Load_ICON(i, _aBitmapItem1[i].pBitmap, 1);
		ICONVIEW_AddBitmapItem(MIDWIN, _aBitmapItem1[i].pBitmap,_aBitmapItem1[i].pText);
		ICONVIEW_SetTextColor(MIDWIN,i,GUI_DARKGRAY);
	}	
	/* ����С���ߵı���ɫ 32 λ��ɫֵ��ǰ8 λ������alpha��ϴ���Ч��*/
	ICONVIEW_SetBkColor(MIDWIN, ICONVIEW_CI_SEL, GUI_WHITE | 0x80000000);
	/* �������� */
//	ICONVIEW_SetFont(MIDWIN, &GUI_Font10_ASCII);
	ICONVIEW_SetFont(MIDWIN, &GUI_FontHZ12);
	/* ���ó�ʼѡ���ͼ��Ϊ -1 (��ʾ��δѡ��)*/
	ICONVIEW_SetSel(MIDWIN,-1);
	/* ����ͼ����x ��y �����ϵļ�ࡣ*/
	ICONVIEW_SetSpace(MIDWIN, GUI_COORD_X, ICONVIEW_XSpace);
	ICONVIEW_SetSpace(MIDWIN, GUI_COORD_Y, ICONVIEW_YSpace);
//	ICONVIEW_SetSpace(hWin, GUI_COORD_Y, ICONVIEW_YSpace);
	/* ���ö��뷽ʽ ��5.22�汾�����¼���� */
	ICONVIEW_SetIconAlign(MIDWIN, ICONVIEW_IA_HCENTER|ICONVIEW_IA_TOP);
	
/*-------------------------------------------------------------------------------------*/
	/*��ָ��λ�ô���ָ���ߴ��ICONVIEW2С����*/
	MIDWIN=ICONVIEW_CreateEx(
							 WinPara.xSizeLCD+ICONVIEW_XPos,	/* С���ߵ��������أ��ڸ������У�*/
							 0, 								/* С���ߵ��������أ��ڸ������У�*/
							 WinPara.xSizeLCD-ICONVIEW_XPos,	/* С���ߵ�ˮƽ�ߴ磨��λ�����أ�*/
							 2*(WinPara.ySizeBM+30),			/* С���ߵĴ�ֱ�ߴ磨��λ�����أ�*/
							 WinPara.hWinMid, 				    /* �����ڵľ�������Ϊ0 ������С���߽���Ϊ���棨�������ڣ����Ӵ��� */
							 WM_CF_SHOW | WM_CF_HASTRANS,       /* ���ڴ�����ǡ�ΪʹС���������ɼ���ͨ��ʹ�� WM_CF_SHOW */ 
							 0,//ICONVIEW_CF_AUTOSCROLLBAR_V, 	/* Ĭ����0�����������ʵ������������ֱ������ */
							 GUI_ID_ICONVIEW2, 			        /* С���ߵĴ���ID */
							 ICONVIEW_Width, 				    /* ͼ���ˮƽ�ߴ� */
							 ICONVIEW_Height+13);				/* ͼ��Ĵ�ֱ�ߴ� */
	/*-------------------------------------------------------------------------------------*/						 
//	WM_EnableMemdev(WinPara.hWinMid); //teng
	/* ��ICONVIEW С���������ͼ�� */
	for (i = 0; i < GUI_COUNTOF(_aBitmapItem2); i++) 
	{
		Load_ICON(i, _aBitmapItem2[i].pBitmap, 2);
		ICONVIEW_AddBitmapItem(MIDWIN, _aBitmapItem2[i].pBitmap,_aBitmapItem2[i].pText);
		ICONVIEW_SetTextColor(MIDWIN,i,GUI_DARKGRAY);
	}	
	
	/* ����С���ߵı���ɫ 32 λ��ɫֵ��ǰ8 λ������alpha��ϴ���Ч��*/
	ICONVIEW_SetBkColor(MIDWIN, ICONVIEW_CI_SEL, GUI_WHITE | 0x80000000);
	/* �������� */
//	ICONVIEW_SetFont(MIDWIN, &GUI_Font10_ASCII);
	ICONVIEW_SetFont(MIDWIN, &GUI_FontHZ12);
	/* ���ó�ʼѡ���ͼ��Ϊ -1 (��ʾ��δѡ��)*/
	ICONVIEW_SetSel(MIDWIN,-1);
	/* ����ͼ����x ��y �����ϵļ�ࡣ*/
	ICONVIEW_SetSpace(MIDWIN, GUI_COORD_X, ICONVIEW_XSpace);
	ICONVIEW_SetSpace(MIDWIN, GUI_COORD_Y, ICONVIEW_YSpace);
//	ICONVIEW_SetSpace(hWin, GUI_COORD_Y, ICONVIEW_YSpace);
	/* ���ö��뷽ʽ ��5.22�汾�����¼���� */
	ICONVIEW_SetIconAlign(MIDWIN, ICONVIEW_IA_HCENTER|ICONVIEW_IA_TOP);
}
/**
  * @brief  CreateBotWin�������ײ��Ĵ���
  * @param  none
  * @retval none
  */
static void CreateBotWin(void)
{
	uint8_t i;
	WM_HWIN BOTWIN;
	WinPara.hWinBot= WM_CreateWindowAsChild(
											0,											
											WinPara.ySizeLCD-WinPara.ySizeBotWin,	//�ײ�λ��
											WinPara.xSizeLCD,
											WinPara.ySizeBotWin,	//�ײ�ʣ����
											WM_HBKWIN, 
											WM_CF_SHOW,
											_cbBotWin, 
											sizeof(WIN_PARA *)
											);
	/*��ָ��λ�ô���ָ���ߴ��ICONVIEW С����*/
	BOTWIN=ICONVIEW_CreateEx(
							 ICONVIEW_XPos, 					/* С���ߵ��������أ��ڸ������У�*/
							 0, 								/* С���ߵ��������أ��ڸ������У�*/
							 WinPara.xSizeLCD-ICONVIEW_XPos,	/* С���ߵ�ˮƽ�ߴ磨��λ�����أ�*/
							 WinPara.ySizeBotWin,				/* С���ߵĴ�ֱ�ߴ磨��λ�����أ�*/
							 WinPara.hWinBot, 				    /* �����ڵľ�������Ϊ0 ������С���߽���Ϊ���棨�������ڣ����Ӵ��� */
							 WM_CF_SHOW | WM_CF_HASTRANS,       /* ���ڴ�����ǡ�ΪʹС���������ɼ���ͨ��ʹ�� WM_CF_SHOW */ 
							 0,//ICONVIEW_CF_AUTOSCROLLBAR_V, 	/* Ĭ����0�����������ʵ������������ֱ������ */
							 GUI_ID_ICONVIEW0, 			        /* С���ߵĴ���ID */
							 ICONVIEW_Width, 				    /* ͼ���ˮƽ�ߴ� */
							 ICONVIEW_Height);					/* ͼ��Ĵ�ֱ�ߴ� */
	WM_BringToTop(WinPara.hWinBot);
	/* ��ICONVIEW С���������ͼ�� */
	for (i = 0; i < GUI_COUNTOF(_aBitmapItem0); i++) 
	{
		Load_ICON(i, _aBitmapItem0[i].pBitmap, 0);
		ICONVIEW_AddBitmapItem(BOTWIN, _aBitmapItem0[i].pBitmap,"");
	}	
	/* ����С���ߵı���ɫ 32 λ��ɫֵ��ǰ8 λ������alpha��ϴ���Ч��*/
	ICONVIEW_SetBkColor(BOTWIN, ICONVIEW_CI_SEL, GUI_WHITE | 0x80000000);
	/* �������� */
//	ICONVIEW_SetFont(hWinicon0, &GUI_Font10_ASCII);
	/* ���ó�ʼѡ���ͼ��Ϊ -1 (��ʾ��δѡ��)*/
	ICONVIEW_SetSel(BOTWIN,-1);
	/* ����ͼ����x ��y �����ϵļ�ࡣ*/
	ICONVIEW_SetSpace(BOTWIN, GUI_COORD_X, ICONVIEW_XSpace);
//	ICONVIEW_SetSpace(hWin, GUI_COORD_Y, ICONVIEW_YSpace);
	/* ���ö��뷽ʽ ��5.22�汾�����¼���� */
	ICONVIEW_SetIconAlign(BOTWIN, ICONVIEW_IA_HCENTER|ICONVIEW_IA_TOP);
}
/*
*********************************************************************************************************
*	�� �� ��: MainAPP
*	����˵��: GUI������ 
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void MainAPP(void)
{
	app_prevent_refresh = 0;
	//׼������3�����ڣ�������ʹ�õ����û���������������ڻص�������ʹ��
	WinPara.xSizeLCD = LCD_GetXSize();				//LCD��Ļ�ߴ�
	WinPara.ySizeLCD = LCD_GetYSize();				//LCD��Ļ�ߴ�
	WinPara.xSizeBM  = ICONVIEW_Width;				//ͼ����
	WinPara.ySizeBM  = ICONVIEW_Height;				//ͼ��߶�
	WinPara.ySizeBotWin=BOTWIN_YSpace;				//�����·����ڸ߶�
	WinPara.xSizeWin = WinPara.xSizeLCD*VIRTUAL_WIN_NUM;						//���ڳߴ�
	WinPara.ySizeWin = WinPara.ySizeLCD-WinPara.ySizeBotWin-WinPara.yPosWin;	//���ڳߴ�
	WinPara.xPosWin	 = MIDWIN_xPos;							//���ڵ���ʼλ��
	WinPara.yPosWin  = MIDWIN_yPos;							//���ڵ���ʼλ��
	WM_SetSize(WM_HBKWIN,WinPara.xSizeLCD,WinPara.ySizeLCD);//���ñ������ڴ�С����֤�м䴰���ƶ�Ч��
	/***************************����Ƥ��ɫ*****************************/
//	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	FRAMEWIN_SetDefaultSkin(FRAMEWIN_SKIN_FLEX);
	BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX);
//	CHECKBOX_SetDefaultSkin(CHECKBOX_SKIN_FLEX);
//	DROPDOWN_SetDefaultSkin(DROPDOWN_SKIN_FLEX);
//	SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);
//	SLIDER_SetDefaultSkin(SLIDER_SKIN_FLEX);
//	HEADER_SetDefaultSkin(HEADER_SKIN_FLEX);
//	RADIO_SetDefaultSkin(RADIO_SKIN_FLEX);
//	MULTIPAGE_SetDefaultSkin(MULTIPAGE_SKIN_FLEX);
//	MENU_SetDefaultSkin(MENU_SKIN_FLEX);
//	SPINBOX_SetDefaultSkin(SPINBOX_SKIN_FLEX);
	/***************************����Ĭ������**********************************/
	GUI_SetDefaultFont(&GUI_FontHZ16);
	/*************************************************************************/
	
	WM_MOTION_Enable(1);
	WM_MOTION_SetDefaultPeriod(500);
	WM_SetCallback(WM_HBKWIN, _cbBkWindow);
	WM_CreateTimer(WM_HBKWIN, 0, 1000, 0);
	/* ������������ ״̬������������������*/
	CreateMidWin();
	CreateTopWin();
	CreateBotWin();
    
	while(1)
	{
		GUI_Delay(30);
	}
}
