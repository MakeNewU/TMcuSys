#include "led.h"
#include "beep.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "ILI93xx.h"
#include "usart.h"	 
#include "24cxx.h"
#include "touch.h"
#include "sram.h"
#include "timer.h"
#include "rtc.h"//teng
#include "sdio_sdcard.h"
#include "mmc_sd.h"//teng
#include "malloc.h"
#include "GUI.h"
#include "ff.h"
#include "exfuns.h"
#include "w25qxx.h"
#include "fontupd.h"
#include "textfont.h"//teng
#include "EmWinHZFont.h"
#include "pngdisplay.h"
#include "gifdisplay.h"
#include "bmpdisplay.h"
#include "jpegdisplay.h"
#include "includes.h"
#include "WM.h"
#include "DIALOG.h"
#include "GUIDemo.h"//teng
#include "app.h"//teng
/************************************************

************************************************/

//START����
//������������ȼ�
#define START_TASK_PRIO				3
//�����ջ��С 
#define START_STK_SIZE			  256
//������ƿ�
OS_TCB StartTaskTCB;
//�����ջ	
CPU_STK START_TASK_STK[START_STK_SIZE];
//������
void StartTask(void *p_arg);

//LED����
//�����������ȼ�
#define LED_TASK_PRIO 				9
//�����ջ��С
#define LED_STK_SIZE				64
//������ƿ�
OS_TCB LedTaskTCB;
//�����ջ
CPU_STK LED_TASK_STK[LED_STK_SIZE];
//led����
void LedTask(void *p_arg);

//�����������ȼ�
#define GUI_TASK_PRIO			5
//�����ջ��С
#define GUI_STK_SIZE			1024
//������ƿ�
OS_TCB GUITaskTCB;
//�����ջ
CPU_STK GUI_TASK_STK[GUI_STK_SIZE];
//task����
void GUITask(void *p_arg);


//�����������ȼ�
#define GUIKEYPAD_TASK_PRIO			6
//�����ջ��С
#define GUIKEYPAD_STK_SIZE			256
//������ƿ�
OS_TCB GUIKEYPADTaskTCB;
//�����ջ
CPU_STK GUIKEYPAD_TASK_STK[GUIKEYPAD_STK_SIZE];
//task����
void GUIKEYPADTask(void *p_arg);


//�����������ȼ�
#define PRTSC_TASK_PRIO			7
//�����ջ��С
#define PRTSC_STK_SIZE			256
//������ƿ�
OS_TCB PRTSCTaskTCB;
//�����ջ
CPU_STK PRTSC_TASK_STK[PRTSC_STK_SIZE];
//task����
void PRTSCTask(void *p_arg);





int main(void)
{	
	OS_ERR err;
	CPU_SR_ALLOC();
	
	delay_init();	    	//��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
 	LED_Init();			    //LED�˿ڳ�ʼ��
	TFTLCD_Init();			//LCD��ʼ��
	TP_Init();				//��������ʼ��
	KEY_Init();	 			//������ʼ��
	BEEP_Init();			//��ʼ��������
	TIM3_Int_Init(999,71);	//1KHZ ��ʱ��1ms      teng
	FSMC_SRAM_Init();		//��ʼ��SRAM
	my_mem_init(SRAMIN); 	//��ʼ���ڲ��ڴ��
	my_mem_init(SRAMEX);  	//��ʼ���ⲿ�ڴ��
	if(RTC_Init())       //Ϊfatfs�ļ�ϵͳ�����ڴ�
	{
		LCD_ShowString(30,0,200,16,16,"RTC_Init error");
	}
	if(exfuns_init())       //Ϊfatfs�ļ�ϵͳ�����ڴ�
	{
		LCD_ShowString(30,0,200,16,16,"exfuns error");
	}		
	f_mount(fs[0],"0:",1);	//����SD��
	f_mount(fs[1],"1:",1);	//����FLASH
	while(SD_Initialize())		//���SD��spi
	{
		LCD_ShowString(30,90,200,16,16,"SD Card Failed!");
		delay_ms(200);
		LCD_Fill(30,90,200+30,70+16,WHITE);
		delay_ms(200);
	}
	while(font_init())		//��ʼ���ֿ�
	{
		LCD_ShowString(30,70,200,16,16,"Font Error!");
//		update_font(30,90,16,0);	//����ֿⲻ���ھ͸����ֿ�
		delay_ms(2000);
		LCD_Clear(WHITE);	//����
		break;
	}
	OSInit(&err);		//��ʼ��UCOSIII
	OS_CRITICAL_ENTER();//�����ٽ���
	//������ʼ����
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//������ƿ�
				 (CPU_CHAR	* )"StartTask", 		//��������
                 (OS_TASK_PTR )StartTask, 			//������
                 (void		* )0,					//���ݸ��������Ĳ���
                 (OS_PRIO	  )START_TASK_PRIO,     //�������ȼ�
                 (CPU_STK   * )&START_TASK_STK[0],	//�����ջ����ַ
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//�����ջ�����λ
                 (CPU_STK_SIZE)START_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	  )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	* )0,					//�û�����Ĵ洢��
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����ѡ��
                 (OS_ERR 	* )&err);				//��Ÿú�������ʱ�ķ���ֵ
	OS_CRITICAL_EXIT();	//�˳��ٽ���	 
	OSStart(&err);  //����UCOSIII
	while(1);
}


//��ʼ������
void StartTask(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;

	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
	OSStatTaskCPUUsageInit(&err);  	//ͳ������                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//���ʹ���˲����жϹر�ʱ��
    CPU_IntDisMeasMaxCurReset();	
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //��ʹ��ʱ��Ƭ��ת��ʱ��
	 //ʹ��ʱ��Ƭ��ת���ȹ���,ʱ��Ƭ����Ϊ1��ϵͳʱ�ӽ��ģ���1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif		
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC,ENABLE);//����CRCʱ��
	WM_SetCreateFlags(WM_CF_MEMDEV);
	GUI_Init();  			//STemWin��ʼ��

	OS_CRITICAL_ENTER();	//�����ٽ���
	//UI����	
	OSTaskCreate((OS_TCB*     )&GUITaskTCB,		
				 (CPU_CHAR*   )"GUITask", 		
                 (OS_TASK_PTR )GUITask, 			
                 (void*       )0,					
                 (OS_PRIO	  )GUI_TASK_PRIO,     
                 (CPU_STK*    )&GUI_TASK_STK[0],	
                 (CPU_STK_SIZE)GUI_STK_SIZE/10,	
                 (CPU_STK_SIZE)GUI_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);
		 
	//LED����
	OSTaskCreate((OS_TCB*     )&LedTaskTCB,		
				 (CPU_CHAR*   )"LedTask", 		
                 (OS_TASK_PTR )LedTask, 			
                 (void*       )0,					
                 (OS_PRIO	  )LED_TASK_PRIO,     
                 (CPU_STK*    )&LED_TASK_STK[0],	
                 (CPU_STK_SIZE)LED_STK_SIZE/10,	
                 (CPU_STK_SIZE)LED_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);
	//keypad����
	OSTaskCreate((OS_TCB*     )&GUIKEYPADTaskTCB,		
				 (CPU_CHAR*   )"GUIKEYPADTask", 		
                 (OS_TASK_PTR )GUIKEYPADTask, 			
                 (void*       )0,					
                 (OS_PRIO	  )GUIKEYPAD_TASK_PRIO,     
                 (CPU_STK*    )&GUIKEYPAD_TASK_STK[0],	
                 (CPU_STK_SIZE)GUIKEYPAD_STK_SIZE/10,	
                 (CPU_STK_SIZE)GUIKEYPAD_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);
				 
	//PRTSC����
	OSTaskCreate((OS_TCB*     )&PRTSCTaskTCB,		
				 (CPU_CHAR*   )"PRTSCTask", 		
                 (OS_TASK_PTR )PRTSCTask, 			
                 (void*       )0,					
                 (OS_PRIO	  )PRTSC_TASK_PRIO,     
                 (CPU_STK*    )&PRTSC_TASK_STK[0],	
                 (CPU_STK_SIZE)PRTSC_STK_SIZE/10,	
                 (CPU_STK_SIZE)PRTSC_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);			 
				 
	OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err);		//����ʼ����			 
	OS_CRITICAL_EXIT();	//�˳��ٽ���
}

//GUI����
void GUITask(void *p_arg)
{
//	GUI_CURSOR_Show();
	GUI_SetDefaultFont(&GUI_FontHZ16);
//	while(1)
//	{
//		GUIDEMO_Main();   //emWin����
//	}

	while (1) 
	{
		MainAPP();
	}
}

//LED����
void LedTask(void *p_arg)
{
	OS_ERR err;
	while(1)
	{
		LED0 = !LED0;
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_PERIODIC,&err);//��ʱ500ms
//		delay_ms(500);
	}
}

extern keyflag;  //teng
static void GUIKEYPADTask(void *p_arg)
{
	OS_ERR   err;
	uint16_t *msg;
	OS_MSG_SIZE	msg_size;
	CPU_TS			ts;
	(void)p_arg;		/* ����������澯 */	
	while (1) 
	{
		msg = OSTaskQPend(0,
						OS_OPT_PEND_BLOCKING,
						&msg_size,
						&ts,
						&err);
		if(err == OS_ERR_NONE)
		{
			OSSchedLock(&err);    //teng
			//printf("pend size=%d,hedit[0]=%d,hedit[1]=%d\n",msg_size,msg[0],msg[1]);
			keypad(msg);
			keyflag = 1;          //teng
			OSSchedUnlock(&err);  //teng
		}
    }	
}

/*
*********************************************************************************************************
*	�� �� ��: _WriteByte2File()
*	����˵��: 
*	��    �Σ�  	
*	�� �� ֵ: 
*********************************************************************************************************
*/
FIL Prtscfile;
void _WriteByte2File(uint8_t Data, void * p) 
{
	FRESULT result;
	result = f_write(p, &Data, 1, &bw);
}

/*
*********************************************************************************************************
*	�� �� ��: PRTSCTask
*	����˵��: 
*	��    �Σ�p_arg ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
	�� �� ����3
*********************************************************************************************************
*/
static void PRTSCTask(void *p_arg)
{
	OS_ERR err;
	CPU_TS Ts;
	char buf[35];
	FRESULT result;
	(void)p_arg;
//	f_mkdir("0:/PRTSC");
	while(1)
	{	
		if(KEY_Scan(0) == KEY1_PRES)
		{
			sprintf(buf,"0:/PRTSC/PRTSC%02d%02d%02d%02d%02d%02d.bmp",
						calendar.w_year,calendar.w_month,calendar.w_date,calendar.hour,calendar.min,calendar.sec);
			OSSchedLock(&err);
			/* ������ͼ */
			result = f_open(&Prtscfile,buf, FA_WRITE|FA_CREATE_ALWAYS);
			/* ��SD������BMPͼƬ */
			GUI_BMP_Serialize(_WriteByte2File,&Prtscfile);
			
			/* ������ɺ�ر�file */
			result = f_close(&Prtscfile);
			OSSchedUnlock(&err);
			BEEP=1;
			OSTimeDlyHMSM(0, 0, 0, 120,OS_OPT_TIME_HMSM_STRICT,&err);
			BEEP=0;
		}
		OSTimeDlyHMSM(0, 0, 0, 10,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);		
	}   
}










//Ӳ��������ʾ
void HardFault_Handler(void)
{
	/* Go to infinite loop when Hard Fault exception occurs */
//	BEEP = 1;
//	delay_ms(200);
//	BEEP = 0;
	while (1)
	{
		u32 i;
		u8 t=0;
		u32 temp;
		temp=SCB->CFSR;					//fault״̬�Ĵ���(@0XE000ED28)����:MMSR,BFSR,UFSR
		printf("CFSR:%8X\r\n",temp);	//��ʾ����ֵ
		temp=SCB->HFSR;					//Ӳ��fault״̬�Ĵ���
		printf("HFSR:%8X\r\n",temp);	//��ʾ����ֵ
		temp=SCB->DFSR;					//����fault״̬�Ĵ���
		printf("DFSR:%8X\r\n",temp);	//��ʾ����ֵ
		temp=SCB->AFSR;					//����fault״̬�Ĵ���
		printf("AFSR:%8X\r\n",temp);	//��ʾ����ֵ
		LED1=!LED1;
		while(t<35)
		{
			t++;
			LED0=!LED0;
			//BEEP=!BEEP;
			for(i=0;i<0x1FFFFF;i++);
		}
	}
}




