#include "stdafx.h"
#include "UIPages.h"

void CDUIPage::InitPage( CDC * pDC, HWND MainWnd)
{
	m_uWidth = WND_WIDTH;
	m_uHeight = WND_HEIGHT;
	m_hwnd = MainWnd;

	for (int nRow = 0; nRow < ROW_NUM; nRow++)
	{
		vector<CDUIButton*> vec;
		m_pVUICtrlContent.push_back(vec);
	}

	m_BackDC.CreateCompatibleDC(pDC);
	CBitmap compbmp;
	compbmp.CreateCompatibleBitmap(pDC, m_uWidth, m_uHeight);
	m_BackDC.SelectObject(&compbmp);
	compbmp.DeleteObject();
}

void CDUIPage::Draw( CDC * pDC )
{
	m_BackDC.BitBlt(0, 0, m_uWidth, m_uHeight, pDC, 0, 0, SRCCOPY);

	for (int nRow = 0; nRow < ROW_NUM; nRow++)
	{
		int nRowSize = m_pVUICtrlContent.at(nRow).size();
		for (int nItem = 0; nItem < nRowSize; nItem++)
		{
			m_pVUICtrlContent.at(nRow).at(nItem)->Draw(pDC);
		}
	}
}

BOOL CDUIPage::OnLButtonDown( POINT point )
{
	for (int nRow = 0; nRow < ROW_NUM; nRow ++)
	{
		int nRowSize = m_pVUICtrlContent.at(nRow).size();
		for (int nItem = 0; nItem < nRowSize; nItem++)
		{
			m_pVUICtrlContent.at(nRow).at(nItem)->SetLastX(point.x);
			if (m_bIsCurPage)
			{
				m_pVUICtrlContent.at(nRow).at(nItem)->OnLButtonDown(point);
			}
		}
	}
	m_lLastX = point.x;
	return FALSE;
}


BOOL CDUIPage::OnLButtonUp( POINT point )
{
	BOOL bRet = FALSE;
	for (int nRow = 0; nRow < ROW_NUM; nRow++)
	{
		int nRowSize = m_pVUICtrlContent.at(nRow).size();
		for (int nItem = 0; nItem < nRowSize; nItem++)
		{
			bRet = m_pVUICtrlContent.at(nRow).at(nItem)->OnLButtonUp(point);
		}
	}
	return bRet;
}

BOOL CDUIPage::OnLButtonUp( CDC* pDC, POINT point )
{
	BOOL bIsSlideSuc = TRUE;
	BOOL bRet = FALSE;
	for (int nRow = 0; nRow < ROW_NUM; nRow ++)
	{
		int nRowSize = m_pVUICtrlContent.at(nRow).size();
		for (int nItem = 0; nItem < nRowSize; nItem++)
		{
			bRet = m_pVUICtrlContent.at(nRow).at(nItem)->SlideToDestDC(pDC);
			if (bRet == FALSE)
			{
				bIsSlideSuc = FALSE;
			}
		}
	}
	return bIsSlideSuc;
}

int CDUIPage::OnMouseMove( POINT point, CDC * pDC, CDC * pBackDC )
{
	for (int nRow = 0; nRow < ROW_NUM; nRow ++)
	{
		int nRowSize = m_pVUICtrlContent.at(nRow).size();
		for (int nItem = 0; nItem < nRowSize; nItem++)
		{
			m_pVUICtrlContent.at(nRow).at(nItem)->OnMouseMove(point, pDC, &m_BackDC);
		}
	}
	return TRUE;
}

void CDUIPage::AddItem( CDUIButton * btn, int lineNum )
{
	m_pVUICtrlContent.at(lineNum).push_back(btn);
}

CDUIPage::~CDUIPage( void )
{
	CDUIButton* btn = NULL;
	for (int nRow = 0; nRow < ROW_NUM; nRow++)
	{
		vector<CDUIButton*>::iterator iter = m_pVUICtrlContent[nRow].begin();
		while (iter != m_pVUICtrlContent[nRow].end())
		{
			btn = *iter;
			if (btn)
			{
				delete btn;
				m_pVUICtrlContent[nRow].erase(iter);
				iter = m_pVUICtrlContent[nRow].begin();
			}
			else
				iter++;
		}
	}
	m_BackDC.DeleteDC();
}

void CDUIPage::ReloadBtnPos( BOOL bIsLeft /*= FALSE*/ )
{
	if (bIsLeft)
	{
		CRect rect(-WND_WIDTH + BTN_GAP_X, BTN_GAP_Y, -WND_WIDTH + BTN_WIDTH + BTN_GAP_X, BTN_GAP_Y + BTN_HEIGHT);
		for (int nRow = 0; nRow < ROW_NUM; nRow ++)
		{
			rect.left = -WND_WIDTH + BTN_GAP_X;
			rect.right = rect.left + BTN_WIDTH;
			rect.top = BTN_GAP_Y + ((BTN_GAP_Y + BTN_HEIGHT) * nRow);
			rect.bottom = rect.top + BTN_HEIGHT;
			int nRowSize = m_pVUICtrlContent.at(nRow).size();
			for (int nItem = 0; nItem < nRowSize; nItem++)
			{
				m_pVUICtrlContent.at(nRow).at(nItem)->SetBtnRc(rect);
				rect.left += (BTN_WIDTH + BTN_GAP_X);
				rect.right = rect.left + BTN_WIDTH;
			}
		}
	}
	else
	{
		CRect rect(WND_WIDTH + BTN_GAP_X, BTN_GAP_Y, WND_WIDTH + BTN_WIDTH + BTN_GAP_X, BTN_GAP_Y + BTN_HEIGHT);
		for (int nRow = 0; nRow < ROW_NUM; nRow ++)
		{
			rect.left = WND_WIDTH + BTN_GAP_X;
			rect.right = rect.left + BTN_WIDTH;
			rect.top = BTN_GAP_Y + ((BTN_GAP_Y + BTN_HEIGHT) * nRow);
			rect.bottom = rect.top + BTN_HEIGHT;
			int nRowSize = m_pVUICtrlContent.at(nRow).size();
			for (int nItem = 0; nItem < nRowSize; nItem++)
			{
				m_pVUICtrlContent.at(nRow).at(nItem)->SetBtnRc(rect);
				rect.left += (BTN_WIDTH + BTN_GAP_X);
				rect.right = rect.left + BTN_WIDTH;
			}
		}
	}
		
}

void CDUIPage::ReloadBtnDestPos( UINT uPos )
{
	if (uPos == DEST_TO_LEFT)
	{
		CRect rect(-WND_WIDTH + BTN_GAP_X, BTN_GAP_Y, -WND_WIDTH + BTN_WIDTH + BTN_GAP_X, BTN_GAP_Y + BTN_HEIGHT);
		for (int nRow = 0; nRow < ROW_NUM; nRow ++)
		{
			rect.left = -WND_WIDTH + BTN_GAP_X;
			rect.right = rect.left + BTN_WIDTH;
			rect.top = BTN_GAP_Y + ((BTN_GAP_Y + BTN_HEIGHT) * nRow);
			rect.bottom = rect.top + BTN_HEIGHT;
			int nRowSize = m_pVUICtrlContent.at(nRow).size();
			for (int nItem = 0; nItem < nRowSize; nItem++)
			{
				m_pVUICtrlContent.at(nRow).at(nItem)->SetBtnDestRc(rect);
				rect.left += (BTN_WIDTH + BTN_GAP_X);
				rect.right = rect.left + BTN_WIDTH;
			}
		}
	}
	else if (uPos == DEST_TO_RIGHT)
	{
		CRect rect(WND_WIDTH + BTN_GAP_X, BTN_GAP_Y, WND_WIDTH + BTN_WIDTH + BTN_GAP_X, BTN_GAP_Y + BTN_HEIGHT);
		for (int nRow = 0; nRow < ROW_NUM; nRow ++)
		{
			rect.left = WND_WIDTH + BTN_GAP_X;
			rect.right = rect.left + BTN_WIDTH;
			rect.top = BTN_GAP_Y + ((BTN_GAP_Y + BTN_HEIGHT) * nRow);
			rect.bottom = rect.top + BTN_HEIGHT;
			int nRowSize = m_pVUICtrlContent.at(nRow).size();
			for (int nItem = 0; nItem < nRowSize; nItem++)
			{
				m_pVUICtrlContent.at(nRow).at(nItem)->SetBtnDestRc(rect);
				rect.left += (BTN_WIDTH + BTN_GAP_X);
				rect.right = rect.left + BTN_WIDTH;
			}
		}
	}
	else if (uPos == DEST_TO_CUR)
	{
		CRect rect(BTN_GAP_X, BTN_GAP_Y, BTN_WIDTH + BTN_GAP_X, BTN_GAP_Y + BTN_HEIGHT);
		for (int nRow = 0; nRow < ROW_NUM; nRow ++)
		{
			rect.left = BTN_GAP_X;
			rect.right = rect.left + BTN_WIDTH;
			rect.top = BTN_GAP_Y + ((BTN_GAP_Y + BTN_HEIGHT) * nRow);
			rect.bottom = rect.top + BTN_HEIGHT;
			int nRowSize = m_pVUICtrlContent.at(nRow).size();
			for (int nItem = 0; nItem < nRowSize; nItem++)
			{
				m_pVUICtrlContent.at(nRow).at(nItem)->SetBtnDestRc(rect);
				rect.left += (BTN_WIDTH + BTN_GAP_X);
				rect.right = rect.left + BTN_WIDTH;
			}
		}
	}
}


void CDUIPage::SetCurPage( BOOL bIsCur )
{
	m_bIsCurPage = bIsCur;
}


