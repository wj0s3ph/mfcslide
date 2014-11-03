
/*************************************************
2014. Co., Ltd.
Edited by Wang.c
Class name:    Common.h    

Description:    
公共头
*************************************************/

//窗口大小
#define WND_WIDTH			920
#define WND_HEIGHT			768	

//按键大小
#define BTN_WIDTH			199
#define BTN_HEIGHT			186

//按钮间隔
#define BTN_GAP_X			80
#define BTN_GAP_Y			50	

//移动速度(像素)
#define MOVE_SPEED			80

//行数
#define ROW_NUM				3

//每行按钮个数
#define ROW_COUNT			3

//滑动目标
#define DEST_TO_LEFT		1
#define DEST_TO_RIGHT		2
#define DEST_TO_CUR			0

//翻页距离
#define PAGE_CHANGE_OFFSET	360

//翻页按键宽度
#define PAGE_BTN_WIDTH		25

//自定义消息
#define CUS_INVALIDATE		WM_USER + 1000
#define CUS_LBUTTONUP		WM_USER + 1001
