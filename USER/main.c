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

//START任务
//设置任务的优先级
#define START_TASK_PRIO				3
//任务堆栈大小 
#define START_STK_SIZE			  256
//任务控制块
OS_TCB StartTaskTCB;
//任务堆栈	
CPU_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void StartTask(void *p_arg);

//LED任务
//设置任务优先级
#define LED_TASK_PRIO 				9
//任务堆栈大小
#define LED_STK_SIZE				64
//任务控制块
OS_TCB LedTaskTCB;
//任务堆栈
CPU_STK LED_TASK_STK[LED_STK_SIZE];
//led任务
void LedTask(void *p_arg);

//设置任务优先级
#define GUI_TASK_PRIO			5
//任务堆栈大小
#define GUI_STK_SIZE			1024
//任务控制块
OS_TCB GUITaskTCB;
//任务堆栈
CPU_STK GUI_TASK_STK[GUI_STK_SIZE];
//task任务
void GUITask(void *p_arg);


//设置任务优先级
#define GUIKEYPAD_TASK_PRIO			6
//任务堆栈大小
#define GUIKEYPAD_STK_SIZE			256
//任务控制块
OS_TCB GUIKEYPADTaskTCB;
//任务堆栈
CPU_STK GUIKEYPAD_TASK_STK[GUIKEYPAD_STK_SIZE];
//task任务
void GUIKEYPADTask(void *p_arg);


//设置任务优先级
#define PRTSC_TASK_PRIO			7
//任务堆栈大小
#define PRTSC_STK_SIZE			256
//任务控制块
OS_TCB PRTSCTaskTCB;
//任务堆栈
CPU_STK PRTSC_TASK_STK[PRTSC_STK_SIZE];
//task任务
void PRTSCTask(void *p_arg);





int main(void)
{	
	OS_ERR err;
	CPU_SR_ALLOC();
	
	delay_init();	    	//延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
 	LED_Init();			    //LED端口初始化
	TFTLCD_Init();			//LCD初始化
	TP_Init();				//触摸屏初始化
	KEY_Init();	 			//按键初始化
	BEEP_Init();			//初始化蜂鸣器
	TIM3_Int_Init(999,71);	//1KHZ 定时器1ms      teng
	FSMC_SRAM_Init();		//初始化SRAM
	my_mem_init(SRAMIN); 	//初始化内部内存池
	my_mem_init(SRAMEX);  	//初始化外部内存池
	if(RTC_Init())       //为fatfs文件系统分配内存
	{
		LCD_ShowString(30,0,200,16,16,"RTC_Init error");
	}
	if(exfuns_init())       //为fatfs文件系统分配内存
	{
		LCD_ShowString(30,0,200,16,16,"exfuns error");
	}		
	f_mount(fs[0],"0:",1);	//挂载SD卡
	f_mount(fs[1],"1:",1);	//挂载FLASH
	while(SD_Initialize())		//检测SD卡spi
	{
		LCD_ShowString(30,90,200,16,16,"SD Card Failed!");
		delay_ms(200);
		LCD_Fill(30,90,200+30,70+16,WHITE);
		delay_ms(200);
	}
	while(font_init())		//初始化字库
	{
		LCD_ShowString(30,70,200,16,16,"Font Error!");
//		update_font(30,90,16,0);	//如果字库不存在就更新字库
		delay_ms(2000);
		LCD_Clear(WHITE);	//清屏
		break;
	}
	OSInit(&err);		//初始化UCOSIII
	OS_CRITICAL_ENTER();//进入临界区
	//创建开始任务
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//任务控制块
				 (CPU_CHAR	* )"StartTask", 		//任务名字
                 (OS_TASK_PTR )StartTask, 			//任务函数
                 (void		* )0,					//传递给任务函数的参数
                 (OS_PRIO	  )START_TASK_PRIO,     //任务优先级
                 (CPU_STK   * )&START_TASK_STK[0],	//任务堆栈基地址
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//任务堆栈深度限位
                 (CPU_STK_SIZE)START_STK_SIZE,		//任务堆栈大小
                 (OS_MSG_QTY  )0,					//任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
                 (OS_TICK	  )0,					//当使能时间片轮转时的时间片长度，为0时为默认长度，
                 (void   	* )0,					//用户补充的存储区
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //任务选项
                 (OS_ERR 	* )&err);				//存放该函数错误时的返回值
	OS_CRITICAL_EXIT();	//退出临界区	 
	OSStart(&err);  //开启UCOSIII
	while(1);
}


//开始任务函数
void StartTask(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;

	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
	OSStatTaskCPUUsageInit(&err);  	//统计任务                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//如果使能了测量中断关闭时间
    CPU_IntDisMeasMaxCurReset();	
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //当使用时间片轮转的时候
	 //使能时间片轮转调度功能,时间片长度为1个系统时钟节拍，既1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif		
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC,ENABLE);//开启CRC时钟
	WM_SetCreateFlags(WM_CF_MEMDEV);
	GUI_Init();  			//STemWin初始化

	OS_CRITICAL_ENTER();	//进入临界区
	//UI任务	
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
		 
	//LED任务
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
	//keypad任务
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
				 
	//PRTSC任务
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
				 
	OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err);		//挂起开始任务			 
	OS_CRITICAL_EXIT();	//退出临界区
}

//GUI任务
void GUITask(void *p_arg)
{
//	GUI_CURSOR_Show();
	GUI_SetDefaultFont(&GUI_FontHZ16);
//	while(1)
//	{
//		GUIDEMO_Main();   //emWin例程
//	}

	while (1) 
	{
		MainAPP();
	}
}

//LED任务
void LedTask(void *p_arg)
{
	OS_ERR err;
	while(1)
	{
		LED0 = !LED0;
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_PERIODIC,&err);//延时500ms
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
	(void)p_arg;		/* 避免编译器告警 */	
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
*	函 数 名: _WriteByte2File()
*	功能说明: 
*	形    参：  	
*	返 回 值: 
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
*	函 数 名: PRTSCTask
*	功能说明: 
*	形    参：p_arg 是在创建该任务时传递的形参
*	返 回 值: 无
	优 先 级：3
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
			/* 创建截图 */
			result = f_open(&Prtscfile,buf, FA_WRITE|FA_CREATE_ALWAYS);
			/* 向SD卡绘制BMP图片 */
			GUI_BMP_Serialize(_WriteByte2File,&Prtscfile);
			
			/* 创建完成后关闭file */
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










//硬件错误提示
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
		temp=SCB->CFSR;					//fault状态寄存器(@0XE000ED28)包括:MMSR,BFSR,UFSR
		printf("CFSR:%8X\r\n",temp);	//显示错误值
		temp=SCB->HFSR;					//硬件fault状态寄存器
		printf("HFSR:%8X\r\n",temp);	//显示错误值
		temp=SCB->DFSR;					//调试fault状态寄存器
		printf("DFSR:%8X\r\n",temp);	//显示错误值
		temp=SCB->AFSR;					//辅助fault状态寄存器
		printf("AFSR:%8X\r\n",temp);	//显示错误值
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




