#include "stdafx.h"
#include "UIButton.h"
#include "PngManager.h"
CDUIButton::CDUIButton():m_dwState(0),m_ClickState(FALSE), m_bIsSuc(FALSE), m_uSpeed(MOVE_SPEED)
{
}

CDUIButton::~CDUIButton()
{
}
void CDUIButton::CreateBtn( /*CWnd * OwerWnd,*/ HWND mainWnd, UINT uResID, CRect rc, UINT uBtnID)
{
	m_uResID = uResID;
	m_btnRc = rc;
	m_btnDestRc = rc;
	m_pDrawEngine = DrawEngines::GetEngine(uResID);
	m_Hwnd = mainWnd;
	m_uID = uBtnID;
}

void CDUIButton::Draw( CDC * pDC )
{
	if (m_pDrawEngine)
	{
		m_pDrawEngine->DrawItem(pDC->m_hDC, m_btnRc, m_dwState);
	}
}

BOOL CDUIButton::OnLButtonDown( POINT point )
{
	if (m_btnRc.PtInRect(point))
	{
		m_ClickState = TRUE;
		m_dwState = BTN_STATE_DOWN;
		::SendMessage(m_Hwnd, CUS_INVALIDATE, (WPARAM)0, (LPARAM)&m_btnRc);
		return TRUE;
	}
	return FALSE;
}

BOOL CDUIButton::OnLButtonUp( POINT point )
{
	if (m_btnRc.PtInRect(point))
	{
		m_dwState = BTN_STATE_NORMAL;
		::SendMessage(m_Hwnd, CUS_INVALIDATE, (WPARAM)0, (LPARAM)&m_btnRc);
		if (m_ClickState)
		{
			::PostMessage(m_Hwnd, CUS_LBUTTONUP, (WPARAM)m_uID, (LPARAM)0);
			m_ClickState = FALSE;
		}
		return TRUE;
	}
	return FALSE;
}

int CDUIButton::OnMouseMove( POINT point, CDC * pDC, CDC * backDC )
{
	m_dwState = BTN_STATE_NORMAL;
	m_ClickState = FALSE;
	LONG lCurX = point.x;
	LONG lOffset = lCurX - m_lLastX;

	m_bMoveLeft = lOffset > 0 ? FALSE : TRUE;

	lOffset = abs(lOffset);
	
	if (m_bMoveLeft)
	{
		m_btnRc.left -= lOffset;
	}
	else
	{
		m_btnRc.left += lOffset;
	}

	m_btnRc.right = m_btnRc.left + BTN_WIDTH;

	m_pDrawEngine->DrawItem(pDC->m_hDC, m_btnRc, m_dwState);
	
	SetLastX(lCurX);
	
	return TRUE;
}

void CDUIButton::SetLastX( LONG lLastX )
{
	m_lLastX = lLastX;
}

void CDUIButton::SetBtnRc( CRect rc )
{
	m_btnRc = rc;
}

void CDUIButton::SetBtnDestRc( CRect rc )
{
	m_btnDestRc = rc;
}

BOOL CDUIButton::SlideToDestDC( CDC* pDC )
{
	BOOL bIsSlideSuc = FALSE;
	if (m_ClickState == FALSE)
	{
		if (m_btnRc.left > m_btnDestRc.left)
		{
			m_btnRc.left -= m_uSpeed;	
			if (m_btnRc.left < m_btnDestRc.left)
			{
				m_btnRc.left = m_btnDestRc.left;
				bIsSlideSuc = TRUE;
			}
		}
		else if (m_btnRc.left < m_btnDestRc.left)
		{
			m_btnRc.left += m_uSpeed;
			if (m_btnRc.left > m_btnDestRc.left)
			{
				m_btnRc.left = m_btnDestRc.left;
				bIsSlideSuc = TRUE;
			}
					
		}
		else if (m_btnRc.left == m_btnDestRc.left)
		{
			bIsSlideSuc = TRUE;
		}
		m_btnRc.right = m_btnRc.left + BTN_WIDTH;
		if (m_pDrawEngine)
		{
			m_pDrawEngine->DrawItem(pDC->m_hDC, m_btnRc, m_dwState);
		}
	}
	else
	{
		bIsSlideSuc = TRUE;
	}

	return bIsSlideSuc;
}

void CDUIButton::SetBtnSubWidth( UINT uWid )
{
	if (m_pDrawEngine)
	{
		m_pDrawEngine->SetItemWidth(uWid);
	}
}

CRect CDUIButton::GetBtnRc()
{
	return m_btnRc;
}

