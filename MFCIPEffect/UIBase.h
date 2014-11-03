
/*************************************************
2014. Co., Ltd.
Edited by Wang.c
Class name:    CDUIBase    

Description:    
绘图控件基类，定义了绘画操作的动作
*************************************************/

#pragma once

class CDUIBase
{
public:
	CDUIBase(void){};
	~CDUIBase(void){};

public:
    //画图
    virtual void Draw(CDC * pDC) = 0;

    //响应窗口OnLButtonDown
    virtual BOOL OnLButtonDown(POINT point) = 0;

    //响应窗口OnLButtonUp
    virtual BOOL OnLButtonUp(POINT point) = 0;

    //响应窗口OnMouseMove
    virtual int OnMouseMove(POINT point, CDC * pDC, CDC * pBackDC) = 0;
};