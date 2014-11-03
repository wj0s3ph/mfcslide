// MFCIPEffectDlg.h : 头文件
//

#pragma once

#include "UIPages.h"
#include "bkimage.h"
#include "PngManager.h"
// CMFCIPEffectDlg 对话框
class CMFCIPEffectDlg : public CDialog
{
// 构造
public:
	CMFCIPEffectDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MFCIPEFFECT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	CBkImage m_imgCache;				/* 保存背景图片 */
	BOOL m_bIsBtnDown;					/* 鼠标是否点击 */
	int m_nCurPage;						/* 当前页 */
	int m_nPageCount;					/* 总页数 */
	LONG m_lLastX;						/* 鼠标按下时的横坐标 */
	BOOL m_bIsMoveLeft;					/* 是否左移 */
	UINT m_uBtnCount;					/* 按键总数,用于计算按键的ID */
	std::vector<CDUIPage*> m_vPages;	/* 页面缓存 */
	HGUIPngDEngine* m_pDrawPng;			/* 绘制png图片,用于绘制翻页按钮 */
	std::vector<CRect> m_vPageBtns;		/* 保存所有翻页按键的Rect */
	DWORD m_dwStartTime;
	
	//初始化页面
	void _InitPages();

	//绘制页面显示按键
	void _DrawPageFlags(CDC* pDC);

	//刷新区域
	LRESULT _Invalidate(WPARAM wParam, LPARAM lParam);
	//按钮事件
	LRESULT _OnLButtonUp(WPARAM wParam, LPARAM lParam);

	//初始化翻页按钮
	void _InitPageBtn();

	//滑动到指定页
	void _SlideToPage(int nPage, CDC* pDC, CDC* destDC, CPoint point);

	//滑动处理
	void _Slide(LONG lOffset, CDC* pDC, CDC* destDC, CPoint point);

	//获取点击的页面转换按键
	int _GetClickPage(CPoint point);

	//buttonup 时处理两边的页面
	BOOL _HandleBesidePage(BOOL bIsPageChange, CDC* destDC, CPoint point);

	//buttonup 时处理当前页面
	BOOL _HandleCurPage(CDC* destDC, CPoint point);

	//判断是否翻页
	BOOL _HandlePageChange(LONG lOffset);

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
};
