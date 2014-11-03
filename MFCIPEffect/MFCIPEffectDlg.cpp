// MFCIPEffectDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "bkimage.h"
#include "UIPages.h"
#include "UIButton.h"
#include "MFCIPEffect.h"
#include "MFCIPEffectDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define CRTDBT_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CMFCIPEffectDlg 对话框




CMFCIPEffectDlg::CMFCIPEffectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMFCIPEffectDlg::IDD, pParent),
	  m_nPageCount(10),
	  m_nCurPage(0),
	  m_uBtnCount(0),
	  m_bIsBtnDown(FALSE),
	  m_dwStartTime(0)
{
}

void CMFCIPEffectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCIPEffectDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(CUS_INVALIDATE, _Invalidate)
	ON_MESSAGE(CUS_LBUTTONUP, _OnLButtonUp)
	//}}AFX_MSG_MAP
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CMFCIPEffectDlg 消息处理程序

BOOL CMFCIPEffectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	_InitPages();

	_InitPageBtn();

	m_imgCache.LoadBitmap(IDB_BITMAP_LAGBAK);

	m_pDrawPng = DrawEngines::GetEngine(IDB_PNG_PAGE);

	m_pDrawPng->SetItemWidth(PAGE_BTN_WIDTH);

	MoveWindow(0, 0, WND_WIDTH, WND_HEIGHT, FALSE);
	CenterWindow();

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCIPEffectDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCIPEffectDlg::OnPaint()
{
	CPaintDC dc(this); // 用于绘制的设备上下文
	
	CDC destDC;
	destDC.CreateCompatibleDC(&dc);
	CBitmap compBmp;
	compBmp.CreateCompatibleBitmap(&dc, WND_WIDTH, WND_HEIGHT);
	CBitmap* pOldBmp = destDC.SelectObject(&compBmp);

	m_imgCache.Draw(destDC.m_hDC, 0, 0);

	m_vPages[m_nCurPage]->Draw(&destDC);

	_DrawPageFlags(&destDC);

	dc.BitBlt(0, 0, WND_WIDTH, WND_HEIGHT, &destDC, 0, 0, SRCCOPY);

	destDC.SelectObject(pOldBmp);
	destDC.DeleteDC();
	compBmp.DeleteObject();
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCIPEffectDlg::OnQueryDragIcon()
{
	return 0;
}


BOOL CMFCIPEffectDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//截断背景绘制防止闪烁
	return TRUE;
}

void CMFCIPEffectDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	if (m_bIsBtnDown)
	{
		if (m_dwStartTime == 0)
		{
			m_dwStartTime = GetTickCount();
		}
		LONG lCurX = point.x;
		LONG lOffset = lCurX - m_lLastX;
		m_bIsMoveLeft = lOffset > 0 ? FALSE : TRUE;

		CDC* pDc;
		CDC DestDC;
		pDc = GetDC();
		DestDC.CreateCompatibleDC(pDc);
		CBitmap combmp;
		combmp.CreateCompatibleBitmap(pDc, WND_WIDTH, WND_HEIGHT);
		CBitmap* pOldBmp = DestDC.SelectObject(&combmp);

		m_imgCache.Draw(DestDC.m_hDC, 0, 0);		/* 绘制背景到兼容DC */

		if (m_nCurPage > 0)
		{
			m_vPages[m_nCurPage - 1]->OnMouseMove(point, &DestDC, NULL);	/* 绘制左边的页面到兼容DC */
		}

		m_vPages[m_nCurPage]->OnMouseMove(point, &DestDC, NULL);			/* 绘制当前页 */

		if (m_nCurPage < m_nPageCount - 1)
		{
			m_vPages[m_nCurPage + 1]->OnMouseMove(point, &DestDC, NULL);	/* 绘制右边页面 */
		}

		_DrawPageFlags(&DestDC);		/* 绘制翻页按钮 */

		pDc->BitBlt(0, 0, WND_WIDTH, WND_HEIGHT, &DestDC, 0, 0, SRCCOPY);	/* 将兼容DC绘制到屏幕 */

		DestDC.SelectObject(pOldBmp);
		DestDC.DeleteDC();
		combmp.DeleteObject();
		ReleaseDC(pDc);
	}
	
	CDialog::OnMouseMove(nFlags, point);
}

void CMFCIPEffectDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	SetCapture();
	m_bIsBtnDown = TRUE;
	m_lLastX = point.x;
	m_vPages[m_nCurPage]->OnLButtonDown(point);

	if (m_nCurPage > 0)
	{
		m_vPages[m_nCurPage - 1]->OnLButtonDown(point);
		m_vPages[m_nCurPage - 1]->ReloadBtnPos(TRUE);
	}

	if (m_nCurPage < m_nPageCount - 1)
	{
		m_vPages[m_nCurPage + 1]->OnLButtonDown(point);
		m_vPages[m_nCurPage + 1]->ReloadBtnPos(FALSE);
	}
	CDialog::OnLButtonDown(nFlags, point);
}

void CMFCIPEffectDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	m_vPages[m_nCurPage]->OnLButtonUp(point);
	m_bIsBtnDown = FALSE;
	DWORD dwEndTime = GetTickCount();

	DWORD dwOptime = dwEndTime - m_dwStartTime;
	m_dwStartTime = 0;	
	LONG lCurX = point.x;
	LONG lOffset = lCurX - m_lLastX;
	
	CDC* pDC = GetDC();
	CDC destDC;
	destDC.CreateCompatibleDC(pDC);
	CBitmap compBmp;
	compBmp.CreateCompatibleBitmap(pDC, WND_WIDTH, WND_HEIGHT);
	CBitmap* pOldBmp = destDC.SelectObject(&compBmp);

	if (lOffset != 0)/* 有滑动时 */
	{
		if (abs(lOffset) > 30 && dwOptime < 200)	/* 快速滑动时 */
		{
			lOffset = lOffset > 0 ? (PAGE_CHANGE_OFFSET + 1) : (-PAGE_CHANGE_OFFSET - 1);
		}
		_Slide(lOffset, pDC, &destDC, point);
	}
	else if (lOffset == 0)
	{
		int nPage = _GetClickPage(point);
		if (nPage != -1)
		{
			_SlideToPage(nPage, pDC, &destDC, point);
		}
	}
	
	destDC.SelectObject(pOldBmp);
	destDC.DeleteDC();
	compBmp.DeleteObject();
	ReleaseDC(pDC);
	CDialog::OnLButtonUp(nFlags, point);
}

/*
 * 初始化页面	若页面组成不同，可重载此函数
 */
void CMFCIPEffectDlg::_InitPages()
{
	CDC *pDC = GetDC();
	for (int nPages = 0; nPages < m_nPageCount; nPages++)
	{
		UINT uStartPng = IDB_PNG_CALL;
		CDUIPage* page = new CDUIPage;
		page->InitPage(pDC, m_hWnd);
		for (int nRow = 0; nRow < ROW_NUM; nRow++)
		{
			int nY = BTN_GAP_Y + (nRow * (BTN_GAP_Y + BTN_HEIGHT));
			CRect rc(BTN_GAP_X, nY, BTN_GAP_X + BTN_WIDTH, nY + BTN_HEIGHT);
			for (int nRowCount = 0; nRowCount < ROW_COUNT; nRowCount++)
			{
				CDUIButton* pBtn = new CDUIButton;
				pBtn->CreateBtn(m_hWnd, uStartPng++, rc, ++m_uBtnCount);
				pBtn->SetBtnSubWidth(BTN_WIDTH);
				page->AddItem(pBtn, nRow);
				rc.left += (BTN_GAP_X + BTN_WIDTH);	
				rc.right = rc.left + BTN_WIDTH;
			}
		}
		m_vPages.push_back(page);
	}
	m_vPages[m_nCurPage]->SetCurPage(TRUE);
	ReleaseDC(pDC);
}

void CMFCIPEffectDlg::OnDestroy()
{
	CDialog::OnDestroy();
	vector<CDUIPage*>::iterator iter = m_vPages.begin();
	while (iter != m_vPages.end())
	{
		CDUIPage* page = *iter;
		if (page)
		{
			delete page;
			m_vPages.erase(iter);
			iter = m_vPages.begin();
		}
		else
			iter++;
	}
	if (m_imgCache.M_HOBJECT)
	{
		m_imgCache.DeleteObject();
	}
	HGUIPngPool::Release();
	DrawEngines::Release();
	
	// TODO: 在此处添加消息处理程序代码
}

LRESULT CMFCIPEffectDlg::_Invalidate( WPARAM wParam, LPARAM lParam)
{
	LPRECT lp = (LPRECT)lParam;
	CRect rc = lp;

	InvalidateRect(&rc, FALSE);

	return 0;
}

BOOL CMFCIPEffectDlg::_HandleBesidePage( BOOL bIsPageChange, CDC* destDC, CPoint point)
{
	BOOL bRet = TRUE;
	if (bIsPageChange)
	{
		if (m_bIsMoveLeft && m_nCurPage - 1 >= 0)
		{
			m_vPages[m_nCurPage - 1]->ReloadBtnDestPos(1);
			if (m_vPages[m_nCurPage - 1]->OnLButtonUp(destDC, point) == FALSE)
			{
				bRet = FALSE;
			}
		}
		else if (!m_bIsMoveLeft && m_nCurPage + 1 < m_nPageCount)
		{
			m_vPages[m_nCurPage + 1]->ReloadBtnDestPos(2);
			if (m_vPages[m_nCurPage + 1]->OnLButtonUp(destDC, point) == FALSE)
			{
				bRet = FALSE;
			}
		}
	}
	else
	{
		if (m_bIsMoveLeft && m_nCurPage + 1 < m_nPageCount)
		{
			m_vPages[m_nCurPage + 1]->ReloadBtnDestPos(2);
			if (m_vPages[m_nCurPage + 1]->OnLButtonUp(destDC, point) == FALSE)
			{
				bRet = FALSE;
			}
		}
		else if (!m_bIsMoveLeft && m_nCurPage - 1 >= 0)
		{
			m_vPages[m_nCurPage - 1]->ReloadBtnDestPos(1);
			if (m_vPages[m_nCurPage - 1]->OnLButtonUp(destDC, point) == FALSE)
			{
				bRet = FALSE;
			}
		}
	}
	return bRet;
}

BOOL CMFCIPEffectDlg::_HandleCurPage( CDC* destDC, CPoint point )
{
	BOOL bRet = TRUE;

	m_vPages[m_nCurPage]->ReloadBtnDestPos(0);

	if (m_vPages[m_nCurPage]->OnLButtonUp(destDC, point) == FALSE)
	{
		bRet = FALSE;
	}
	return bRet;
}

BOOL CMFCIPEffectDlg::_HandlePageChange( LONG lOffset )
{
	BOOL bIsPageChange = FALSE;

	if (lOffset > PAGE_CHANGE_OFFSET)
	{
		if (m_nCurPage > 0)
		{
			m_vPages[m_nCurPage]->SetCurPage(FALSE);
			m_nCurPage--;
			bIsPageChange = TRUE;
		}
	}
	else if (lOffset < (-PAGE_CHANGE_OFFSET))
	{
		if (m_nCurPage < m_nPageCount - 1)
		{
			m_vPages[m_nCurPage]->SetCurPage(FALSE);
			m_nCurPage++;
			bIsPageChange = TRUE;
		}
	}

	m_vPages[m_nCurPage]->SetCurPage(TRUE);

	return bIsPageChange;
}
//按键点击事件通知
LRESULT CMFCIPEffectDlg::_OnLButtonUp( WPARAM wParam, LPARAM lParam )
{
	UINT uBtnID = (UINT)wParam;
	CString str;
	str.Format(L"Btn CLick : %u", uBtnID);
		
	AfxMessageBox(str.GetBuffer(str.GetLength()));
	return TRUE;
}

void CMFCIPEffectDlg::_DrawPageFlags( CDC* pDC )
{
	for (int nPages = 0; nPages < m_nPageCount; nPages++)
	{
		if (nPages == m_nCurPage)
		{
			m_pDrawPng->DrawItem(pDC->m_hDC, m_vPageBtns[nPages], 1, 0.90f);
		}
		else
		{
			m_pDrawPng->DrawItem(pDC->m_hDC, m_vPageBtns[nPages], 0, 0.40f);
		}
	}
}

void CMFCIPEffectDlg::_InitPageBtn()
{
	int nLeft = (WND_WIDTH - (2 * PAGE_BTN_WIDTH * m_nPageCount)) / 2;
	int nTop = WND_HEIGHT - ( 2 * PAGE_BTN_WIDTH) - 8;
	for (int nPages = 0; nPages < m_nPageCount; nPages++)
	{
		CRect rc(nLeft, nTop, nLeft + 50, nTop + 50);
		m_vPageBtns.push_back(rc);
		nLeft += 50;
	}
}

void CMFCIPEffectDlg::_SlideToPage( int nPage, CDC* pDC, CDC* destDC, CPoint point )
{
	if (m_nCurPage == nPage)
	{
		return;
	}

	if (m_nCurPage < nPage)
	{
		m_vPages[nPage]->ReloadBtnPos(FALSE);
		m_vPages[m_nCurPage]->ReloadBtnDestPos(DEST_TO_LEFT);
	}
	else
	{
		m_vPages[nPage]->ReloadBtnPos(TRUE);
		m_vPages[m_nCurPage]->ReloadBtnDestPos(DEST_TO_RIGHT);
	}

	m_vPages[nPage]->ReloadBtnDestPos(DEST_TO_CUR);

	int nTmpPage = m_nCurPage;
	m_nCurPage = nPage;

	while(TRUE)
	{
		BOOL bIsSlideSuc = TRUE;

		m_imgCache.Draw(destDC->m_hDC, 0, 0);
		
		bIsSlideSuc = m_vPages[nTmpPage]->OnLButtonUp(destDC, point);

		bIsSlideSuc = m_vPages[nPage]->OnLButtonUp(destDC, point);
		
		_DrawPageFlags(destDC);

		pDC->BitBlt(0, 0, WND_WIDTH, WND_HEIGHT, destDC, 0, 0, SRCCOPY);

		if (bIsSlideSuc)
		{
			break;
		}
	}
	m_vPages[nTmpPage]->SetCurPage(FALSE);

	m_vPages[nPage]->SetCurPage(TRUE);

}

int CMFCIPEffectDlg::_GetClickPage( CPoint point )
{
	for (int nPage = 0; nPage < m_nPageCount; nPage ++)
	{
		if (m_vPageBtns[nPage].PtInRect(point))
		{
			return nPage;
		}
	}

	return -1;
}

void CMFCIPEffectDlg::_Slide( LONG lOffset, CDC* pDC, CDC* destDC, CPoint point )
{
	BOOL bIsPageChange = _HandlePageChange(lOffset);

	while(TRUE)	
	{
		BOOL bIsMoveSuc = TRUE;

		m_imgCache.Draw(destDC->m_hDC, 0, 0);

		bIsMoveSuc = _HandleBesidePage(bIsPageChange, destDC, point);
		
		bIsMoveSuc = _HandleCurPage(destDC, point);

		_DrawPageFlags(destDC);

		pDC->BitBlt(0, 0, WND_WIDTH, WND_HEIGHT, destDC, 0, 0, SRCCOPY);
			
		if (bIsMoveSuc)
		{
			break;
		}
	}
	
}

