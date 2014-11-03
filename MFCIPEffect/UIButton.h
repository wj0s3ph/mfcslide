/*************************************************
2014. Co., Ltd.
Edited by Wang.c
Class name:    CDUIButton    

Description:    
按钮类
*************************************************/
#pragma once
#include "PngManager.h"
#include "UIBase.h"
enum 
{
	BTN_STATE_NORMAL	= 0x00000000UL,
	BTN_STATE_DOWN		= 0x00000001UL,
	BTN_STATE_HOVER		= 0x00000000UL
};
class CDUIButton: public CDUIBase
{
public:
    CDUIButton(void);
    ~CDUIButton(void);
private:
    //按钮区域
    CRect m_btnRc;
    //标题的区域
    CRect m_FontRc;

	CRect m_btnDestRc;

	//按钮图片
	UINT m_uResID;
	
	//移动方向
	BOOL m_bMoveLeft;

    //按钮按下状态，TRUE为按下
    BOOL m_ClickState;
   
    //字体大小
    int m_fontSize;

    //按钮状态 
	DWORD m_dwState;

	LONG  m_lLastX;
	//按钮绘制引擎	
	HGUIPngDEngine* m_pDrawEngine;

	//真实窗口句柄
	HWND m_Hwnd;

	//ID
	UINT m_uID;

	BOOL m_bIsSuc;

	UINT m_uSpeed;
public:
    //初始化按钮
    void CreateBtn(/*CWnd * OwerWnd,*/ HWND mainWnd, UINT uResID, CRect rc, UINT uBtnID);
	//设置按键位置rect
	void SetBtnRc(CRect rc);
	//设置滑动目标rect
	void SetBtnDestRc(CRect rc);
	//设置子图(按键)宽度
	void SetBtnSubWidth(UINT uWid);
	//滑动到目标rc 并绘制在pdc上
	BOOL SlideToDestDC(CDC* pDC);
	//获取按键位置rect
	CRect GetBtnRc();
	//设置横坐标
	void SetLastX(LONG lLastX);
    //画图
    void Draw(CDC * pDC);
    //响应窗口OnLButtonDown
    BOOL OnLButtonDown(POINT point);
    //响应窗口OnLButtonUp
	BOOL OnLButtonUp(POINT point);
    //响应窗口OnMouseMove
    int OnMouseMove(POINT point, CDC * pDC, CDC * backDC);
    //画按钮的名称
    //void DrawBtnText(CDC * pDC);
    //.......... 
};