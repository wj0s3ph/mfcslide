/*************************************************
2014. Co., Ltd.
Edited by Wang.c
Class name:    CDUIPage    

Description:    
页面类，保存按钮，统一处理鼠标事件
*************************************************/
#pragma once
#include <vector>
#include "UIBase.h"
#include "UIButton.h"
using namespace std;

typedef vector<vector<CDUIButton *> > TypeBtnVec;
class CDUIPage:public CDUIBase
{
public:
	CDUIPage(void): m_bIsCurPage(FALSE){};
	~CDUIPage(void);

public:
    //保存界面按钮的二维容器
    vector<vector<CDUIButton *> > m_pVUICtrlContent;
	
	//界面大小
	UINT m_uWidth;
	UINT m_uHeight;
	LONG m_lLastX;

	//是否是当前页
	BOOL m_bIsCurPage;

    //内存DC，保存滑动时候背景
    CDC m_BackDC;
	
	//真实窗口句柄
	HWND m_hwnd;

    void InitPage(CDC * pDC, HWND MainWnd);

    //画图
    void Draw(CDC * pDC);

    //响应窗口OnLButtonDown
    BOOL OnLButtonDown(POINT point);

	//设置是否当前页面
	void SetCurPage(BOOL bIsCur);

    //响应窗口OnLButtonUp
    BOOL OnLButtonUp(POINT point);

	BOOL OnLButtonUp(CDC* pDC, POINT point);

    //响应窗口OnMouseMove
    int OnMouseMove(POINT point, CDC * pDC, CDC * pBackDC);

    //添加新的按钮
    void AddItem(CDUIButton * btn, int lineNum);

    //重新计算按钮位置
    void ReloadBtnPos(BOOL bIsLeft = FALSE);

	//重新计算按钮滑动目标
	void ReloadBtnDestPos(UINT uPos); /* 0 curpage,1 left, 2 right */

};