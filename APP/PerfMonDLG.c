

#include <stdlib.h>
#include <string.h>

#include "DIALOG.h"
#include "GRAPH.h"

#include "EmWinHZFont.h"//teng
/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define MAX_VALUE 100

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
static GRAPH_DATA_Handle  _ahData[3]; /* Array of handles for the GRAPH_DATA objects */
static GRAPH_SCALE_Handle _hScaleV;   /* Handle of vertical scale */
static GRAPH_SCALE_Handle _hScaleH;   /* Handle of horizontal scale */

static I16 _aValue[3];
static int _Stop;

static GUI_COLOR _aColor[] = {GUI_RED, GUI_GREEN, GUI_BLUE}; /* Array of colors for the GRAPH_DATA objects */

/* Dialog ressource */
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "资源监视器",  0                ,   0,   60, 240, 195, 0},
  { GRAPH_CreateIndirect,     0,                   GUI_ID_GRAPH0    ,   2,   3, 226, 105 },
  { TEXT_CreateIndirect,      "SX",        0                ,  2, 110,  12,  16 },
  { TEXT_CreateIndirect,      "SY",        0                ,  2, 130,  12,  16 },
  { SLIDER_CreateIndirect,    0,                   GUI_ID_SLIDER0   ,  20, 110,  60,  16 },
  { SLIDER_CreateIndirect,    0,                   GUI_ID_SLIDER1   ,  20, 130,  60,  16 },
  { CHECKBOX_CreateIndirect,  0,                   GUI_ID_CHECK0    , 85, 110,  50,   0 },
  { CHECKBOX_CreateIndirect,  0,                   GUI_ID_CHECK1    , 85, 130,  50,   0 },
  { TEXT_CreateIndirect,      "Border",            0                , 2,   150,  37,  15 },
  { CHECKBOX_CreateIndirect,  0,                   GUI_ID_CHECK2    , 30,  150,  35,   0 },
  { CHECKBOX_CreateIndirect,  0,                   GUI_ID_CHECK3    , 60,  150,  35,   0 },
  { CHECKBOX_CreateIndirect,  0,                   GUI_ID_CHECK4    , 90,  150,  35,   0 },
  { CHECKBOX_CreateIndirect,  0,                   GUI_ID_CHECK5    , 120,  150,  35,   0 },
//  { TEXT_CreateIndirect,      "Ef",            0                , 125, 225,  35,  15 },
//  { RADIO_CreateIndirect,     0,                   GUI_ID_RADIO0    , 125, 240,  35,   0, 0, 3 },
  { CHECKBOX_CreateIndirect,  0,                   GUI_ID_CHECK6    , 125, 110,  50,   0 },
  { CHECKBOX_CreateIndirect,  0,                   GUI_ID_CHECK7    , 125, 130,  50,   0 },
  { BUTTON_CreateIndirect,    "Full",      		   GUI_ID_BUTTON0   , 180, 110,  45,  18 },
  { CHECKBOX_CreateIndirect,  0,                   GUI_ID_CHECK8    , 180, 130,  70,   0 },
//  { TEXT_CreateIndirect,      "CPU:000%",              GUI_ID_TEXT0                , 3,   110,  55,  15 },
//  { TEXT_CreateIndirect,      "MEMIN:000&",            GUI_ID_TEXT1                , 60,   110,  60,  15 },
//  { TEXT_CreateIndirect,      "MEMEX:000&",            GUI_ID_TEXT2                , 185,   110,  60,  15 },
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*
*       _AddValues
*
* Purpose:
*   This routine calculates new random values in dependence of the previous added values
*   and adds them to the GRAPH_DATA objects
*/
extern OSStatTaskCPUUsage;
static void _AddValues(WM_HWIN hGraph) {
//	int i;
//	for (i = 0; i < GUI_COUNTOF(_aColor); i++) {
//		int Add = rand() % (2 + i * i);
//		int Vz  = ((rand() % 2) << 1) - 1;
//		_aValue[i] += Add * Vz;
//		if (_aValue[i] > MAX_VALUE) {
//			_aValue[i] = MAX_VALUE;
//		} else if (_aValue[i] < 0) {
//			_aValue[i] = 0;
//		}
//		GRAPH_DATA_YT_AddValue(_ahData[i], _aValue[i]);
//	}
	_aValue[0] = (I16)OSStatTaskCPUUsage/100;//CPU
	GRAPH_DATA_YT_AddValue(_ahData[0], _aValue[0]);
	_aValue[1] = (I16)my_mem_perused(0);//内部
	GRAPH_DATA_YT_AddValue(_ahData[1], _aValue[1]);
	_aValue[2] = (I16)my_mem_perused(1);//外部
	GRAPH_DATA_YT_AddValue(_ahData[2], _aValue[2]);
}

/*********************************************************************
*
*       _UserDraw
*
* Purpose:
*   This routine is called by the GRAPH object before anything is drawn
*   and after the last drawing operation.
*/
static void _UserDraw(WM_HWIN hWin, int Stage) {
  if (Stage == GRAPH_DRAW_LAST) {
	char acText[] = "usage";
    GUI_RECT Rect, RectInvalid;
    int FontSizeY;
//    GUI_SetFont(&GUI_Font13_ASCII);
	GUI_SetFont(&GUI_FontHZ12);
    FontSizeY = GUI_GetFontSizeY();
    WM_GetInsideRect(&Rect);
    WM_GetInvalidRect(hWin, &RectInvalid);
    Rect.x1 = Rect.x0 + FontSizeY;
    GUI_SetColor(GUI_YELLOW);
    GUI_DispStringInRectEx(acText, &Rect, GUI_TA_HCENTER, strlen(acText), GUI_ROTATE_CCW);
  }
}

/*********************************************************************
*
*       _ForEach
*
* Purpose:
*   This routine hides/shows all windows except the button, graph and scroll bar widgets
*/
static void _ForEach(WM_HWIN hWin, void * pData) {
  int Id, FullScreenMode;
  FullScreenMode = *(int *)pData;
  Id = WM_GetId(hWin);
  if ((Id == GUI_ID_GRAPH0) || (Id == GUI_ID_BUTTON0) || (Id == GUI_ID_VSCROLL) || (Id == GUI_ID_HSCROLL)) {
    return;
  }
  if (FullScreenMode) {
    WM_HideWindow(hWin);
  } else {
    WM_ShowWindow(hWin);
  }
}

/*********************************************************************
*
*       _ToggleFullScreenMode
*
* Purpose:
*   This routine switches between full screen mode and normal mode by hiding or showing the
*   widgets of the dialog, enlarging/shrinking the graph widget and modifying some other
*   attributes of the dialog widgets.
*/
static void _ToggleFullScreenMode(WM_HWIN hDlg) {
  static int FullScreenMode;
  static GUI_RECT Rect;
  static unsigned ScalePos;
  WM_HWIN hGraph, hButton;
  hGraph  = WM_GetDialogItem(hDlg, GUI_ID_GRAPH0);
  hButton = WM_GetDialogItem(hDlg, GUI_ID_BUTTON0);
  FullScreenMode ^= 1;
  if (FullScreenMode) {
    /* Enter the full screen mode */
    WM_HWIN hClient;
    GUI_RECT RectInside;
    hClient = WM_GetClientWindow(hDlg);
    BUTTON_SetText(hButton, "Back");
    WM_MoveWindow(hButton, 0, 40);
    FRAMEWIN_SetTitleVis(hDlg, 0);
    WM_GetInsideRectEx(hClient, &RectInside);
    WM_GetWindowRectEx(hGraph, &Rect);
    WM_ForEachDesc(hClient, _ForEach, &FullScreenMode); /* Hide all descendants */
    WM_SetWindowPos(hGraph, WM_GetWindowOrgX(hClient), WM_GetWindowOrgX(hClient)+60, RectInside.x1, RectInside.y1);
    ScalePos = GRAPH_SCALE_SetPos(_hScaleH, RectInside.y1 - 20);
  } else {
    /* Return to normal mode */
    BUTTON_SetText(hButton, "Full");
    WM_MoveWindow(hButton, 0, -40);
    WM_ForEachDesc(WM_GetClientWindow(hDlg), _ForEach, &FullScreenMode); /* Show all descendants */
    WM_SetWindowPos(hGraph, Rect.x0, Rect.y0, Rect.x1 - Rect.x0 + 1, Rect.y1 - Rect.y0 + 1);
    FRAMEWIN_SetTitleVis(hDlg, 1);
    GRAPH_SCALE_SetPos(_hScaleH, ScalePos);
  }
}

/*********************************************************************
*
*       _cbCallback
*
* Purpose:
*   Callback function of the dialog
*/
static void _cbCallback(WM_MESSAGE * pMsg) {
  int i, NCode, Id, Value;
  WM_HWIN hDlg, hItem;
  hDlg = pMsg->hWin;
  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
	GUI_SetDefaultFont(&GUI_FontHZ12);
	hItem = pMsg->hWin;
	FRAMEWIN_SetTitleHeight(hItem, 20);
	FRAMEWIN_SetTextColor(hItem,GUI_DARKGRAY);
	FRAMEWIN_SetFont(hItem,&GUI_FontHZ16);
	FRAMEWIN_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
	FRAMEWIN_AddCloseButton(hItem,FRAMEWIN_BUTTON_RIGHT,0);
  
    hItem = WM_GetDialogItem(hDlg, GUI_ID_GRAPH0);
    /* Add graphs */
    for (i = 0; i < GUI_COUNTOF(_aColor); i++) {
//      _aValue[i] = rand() % 180;
	  _aValue[i] = 0;
      _ahData[i] = GRAPH_DATA_YT_Create(_aColor[i], 226, 0, 0);
      GRAPH_AttachData(hItem, _ahData[i]);
    }
    /* Set graph attributes */
    GRAPH_SetGridDistY(hItem, 25);         //网格间距
    GRAPH_SetGridVis(hItem, 1);
    GRAPH_SetGridFixedX(hItem, 1);
	GRAPH_SetVSizeY(hItem,50);
//	GRAPH_SetLineStyle(hItem,GUI_LS_DASHDOTDOT);
	
//    GRAPH_SetUserDraw(hItem, _UserDraw);
    /* Create and add vertical scale */
    _hScaleV = GRAPH_SCALE_Create( 2, GUI_TA_LEFT, GRAPH_SCALE_CF_VERTICAL, 25);
    GRAPH_SCALE_SetTextColor(_hScaleV, GUI_YELLOW);
    GRAPH_AttachScale(hItem, _hScaleV);
    /* Create and add horizontal scale */
    _hScaleH = GRAPH_SCALE_Create(95, GUI_TA_HCENTER, GRAPH_SCALE_CF_HORIZONTAL, 50);
    GRAPH_SCALE_SetTextColor(_hScaleH, GUI_LIGHTMAGENTA);
    GRAPH_AttachScale(hItem, _hScaleH);
    /* Init check boxes */
    hItem = WM_GetDialogItem(hDlg, GUI_ID_CHECK2);
    CHECKBOX_SetText(hItem, "L");
    hItem = WM_GetDialogItem(hDlg, GUI_ID_CHECK3);
    CHECKBOX_SetText(hItem, "T");
    hItem = WM_GetDialogItem(hDlg, GUI_ID_CHECK4);
    CHECKBOX_SetText(hItem, "R");
    hItem = WM_GetDialogItem(hDlg, GUI_ID_CHECK5);
    CHECKBOX_SetText(hItem, "B");
    hItem = WM_GetDialogItem(hDlg, GUI_ID_CHECK0);
    CHECKBOX_SetText(hItem, "Stop");
    hItem = WM_GetDialogItem(hDlg, GUI_ID_CHECK1);
    CHECKBOX_SetText(hItem, "Grid");
    CHECKBOX_SetState(hItem, 1);
    hItem = WM_GetDialogItem(hDlg, GUI_ID_CHECK6);
    CHECKBOX_SetText(hItem, "HScroll");
//    CHECKBOX_SetState(hItem, 1);
    hItem = WM_GetDialogItem(hDlg, GUI_ID_CHECK7);
    CHECKBOX_SetText(hItem, "VScroll");
    hItem = WM_GetDialogItem(hDlg, GUI_ID_CHECK8);
    CHECKBOX_SetText(hItem, "Mirror");
    /* Init slider widgets */
    hItem = WM_GetDialogItem(hDlg, GUI_ID_SLIDER0);
    SLIDER_SetRange(hItem, 0, 10);
    SLIDER_SetValue(hItem, 5);
    SLIDER_SetNumTicks(hItem, 6);
    hItem = WM_GetDialogItem(hDlg, GUI_ID_SLIDER1);
    SLIDER_SetRange(hItem, 0, 20);
    SLIDER_SetValue(hItem, 5);
    SLIDER_SetNumTicks(hItem, 6);
    /* Init radio widget */
//    hItem = WM_GetDialogItem(hDlg, GUI_ID_RADIO0);
//    RADIO_SetText(hItem, "3D", 0);
//    RADIO_SetText(hItem, "flat", 1);
//    RADIO_SetText(hItem, "-", 2);
    /* Init button widget */
    hItem = WM_GetDialogItem(hDlg, GUI_ID_BUTTON0);
    WM_SetStayOnTop(hItem, 1);
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);      /* Id of widget */
    NCode = pMsg->Data.v;                 /* Notification code */
    switch (NCode) {
    case WM_NOTIFICATION_CLICKED:
      switch (Id) {
      case GUI_ID_BUTTON0:
        _ToggleFullScreenMode(hDlg);
        break;
      }
      break;
    case WM_NOTIFICATION_VALUE_CHANGED:
      switch (Id) {
      case GUI_ID_CHECK0:
        /* Toggle stop mode */
        _Stop ^= 1;
        break;
      case GUI_ID_CHECK1:
        /* Toggle grid */
        hItem = WM_GetDialogItem(hDlg, GUI_ID_GRAPH0);
        GRAPH_SetGridVis(hItem, CHECKBOX_IsChecked(WM_GetDialogItem(hDlg, GUI_ID_CHECK1)));
        break;
      case GUI_ID_CHECK2:
      case GUI_ID_CHECK3:
      case GUI_ID_CHECK4:
      case GUI_ID_CHECK5:
        /* Toggle border */
        hItem = WM_GetDialogItem(hDlg, GUI_ID_GRAPH0);
        GRAPH_SetBorder(hItem, 
                        CHECKBOX_IsChecked(WM_GetDialogItem(hDlg, GUI_ID_CHECK2)) * 17, 
                        CHECKBOX_IsChecked(WM_GetDialogItem(hDlg, GUI_ID_CHECK3)) * 3, 
                        CHECKBOX_IsChecked(WM_GetDialogItem(hDlg, GUI_ID_CHECK4)) * 3, 
                        CHECKBOX_IsChecked(WM_GetDialogItem(hDlg, GUI_ID_CHECK5)) * 3);
        break;
      case GUI_ID_SLIDER0:
        /* Set horizontal grid spacing */
        hItem = WM_GetDialogItem(hDlg, GUI_ID_GRAPH0);
        Value = SLIDER_GetValue(pMsg->hWinSrc) * 10;
        GRAPH_SetGridDistX(hItem, Value);
        GRAPH_SCALE_SetTickDist(_hScaleH, Value);
        break;
      case GUI_ID_SLIDER1:
        /* Set vertical grid spacing */
        hItem = WM_GetDialogItem(hDlg, GUI_ID_GRAPH0);
        Value = SLIDER_GetValue(pMsg->hWinSrc) * 5;
        GRAPH_SetGridDistY(hItem, Value);
        GRAPH_SCALE_SetTickDist(_hScaleV, Value);
        break;
//      case GUI_ID_RADIO0:
//        /* Set the widget effect */
//        hItem = WM_GetDialogItem(hDlg, GUI_ID_GRAPH0);
//        switch (RADIO_GetValue(pMsg->hWinSrc)) {
//        case 0:
//          WIDGET_SetEffect(hItem, &WIDGET_Effect_3D);
//          break;
//        case 1:
//          WIDGET_SetEffect(hItem, &WIDGET_Effect_Simple);
//          break;
//        case 2:
//          WIDGET_SetEffect(hItem, &WIDGET_Effect_None);
//          break;
//        }
//        break;
      case GUI_ID_CHECK6:
        /* Toggle horizontal scroll bar */
        hItem = WM_GetDialogItem(hDlg, GUI_ID_GRAPH0);
        if (CHECKBOX_IsChecked(WM_GetDialogItem(hDlg, GUI_ID_CHECK6))) {
          GRAPH_SetVSizeX(hItem, 500);
        } else {
          GRAPH_SetVSizeX(hItem, 0);
        }
        break;
      case GUI_ID_CHECK7:
        /* Toggle vertical scroll bar */
        hItem = WM_GetDialogItem(hDlg, GUI_ID_GRAPH0);
        if (CHECKBOX_IsChecked(WM_GetDialogItem(hDlg, GUI_ID_CHECK7))) {
          GRAPH_SetVSizeY(hItem, 200);
        } else {
          GRAPH_SetVSizeY(hItem, 0);
        }
        break;
      case GUI_ID_CHECK8:
        /* Toggle alignment */
        hItem = WM_GetDialogItem(hDlg, GUI_ID_GRAPH0);
        for (i = 0; i < GUI_COUNTOF(_aColor); i++) {
          if (CHECKBOX_IsChecked(WM_GetDialogItem(hDlg, GUI_ID_CHECK8))) {
            GRAPH_DATA_YT_SetAlign(_ahData[i], GRAPH_ALIGN_LEFT);
            GRAPH_DATA_YT_MirrorX (_ahData[i], 1);
          } else {
            GRAPH_DATA_YT_SetAlign(_ahData[i], GRAPH_ALIGN_RIGHT);
            GRAPH_DATA_YT_MirrorX (_ahData[i], 0);
          }
        }
        break;
      }
      break;
    }
    break;
  default:
    WM_DefaultProc(pMsg);
  }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       MainTask
*/
void CreatePerfMon(void);
void CreatePerfMon(void) {
WM_HWIN hDlg, hGraph = 0;
#if GUI_SUPPORT_MEMDEV
	WM_SetCreateFlags(WM_CF_MEMDEV);
#endif
	hDlg = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, 0, 0, 0);
	while (1) {
		if (!_Stop) {
			if (!hGraph) {
				hGraph = WM_GetDialogItem(hDlg, GUI_ID_GRAPH0);
			}
			_AddValues(hGraph);
		}
		GUI_Delay(200);
	}
}

/*************************** End of file ****************************/

