// EditItem.cpp : implementation file
//

#include "stdafx.h"
#include "EditItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditItem

CEditItem::CEditItem(int nItem, int nSubItem, CString &sContent )
	: typelimit(0)
{
	m_nItem = nItem;
	m_nSubItem = nSubItem;
	m_sContent = sContent;
	m_bVK_ESCAPE = FALSE; 
}

CEditItem::~CEditItem()
{
}


BEGIN_MESSAGE_MAP(CEditItem, CEdit)
	//{{AFX_MSG_MAP(CEditItem)
	ON_WM_CHAR()
	ON_WM_NCDESTROY()
	ON_WM_KILLFOCUS()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditItem message handlers

BOOL CEditItem::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_KEYDOWN )	
	{		
		if( pMsg->wParam == VK_RETURN || 
			pMsg->wParam == VK_DELETE ||
			pMsg->wParam == VK_ESCAPE ||
			GetKeyState( VK_CONTROL))
		{			
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return 1;
		}	
	}

	return CEdit::PreTranslateMessage(pMsg);
}

///////////////add////////////////
bool CheckChar(const UINT &nChar, const CString &str, const int &limtype)
{
	if( limtype==0 ){
		return true;
	}

	if((nChar>='0' && nChar<='9')||(nChar==0x08)||(nChar==0x10))
		return true;

	if( nChar=='.' && str.Find('.')==-1 && limtype==1 )
		return true;

	return false;
}
///////////////add////////////////


void CEditItem::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if( nChar == VK_ESCAPE ||
		nChar == VK_RETURN)	
	{		
		if( nChar == VK_ESCAPE)
			m_bVK_ESCAPE = 1;		
		GetParent()->SetFocus();
		return;	
	}
	///////////////add////////////////
	CString str1;	
	GetWindowText( str1 );	
	if(CheckChar(nChar,str1,typelimit))
	//////////////add///////////////////
	{

		CEdit::OnChar(nChar, nRepCnt, nFlags);	
		// Resize edit control if needed
		// Get text extent	
		CString str;	
		GetWindowText( str );	

		CWindowDC dc(this);
		CFont *pFont = GetParent()->GetFont();
		CFont *pFontDC = dc.SelectObject(pFont);
		CSize size = dc.GetTextExtent(str);	
		dc.SelectObject(pFontDC);
		size.cx += 5; // add some extra buffer	
		// Get client rect
		CRect rect, rcParent;	
		GetClientRect(&rect);
		GetParent()->GetClientRect(&rcParent);
		// Transform rect to parent coordinates	
		ClientToScreen(&rect);
		GetParent()->ScreenToClient(&rect);
		// Check whether control needs to be resized
		// and whether there is space to grow	
		if(size.cx > rect.Width())	
		{
			if(size.cx + rect.left < rcParent.right)
				rect.right = rect.left + size.cx;		
			else			
				rect.right = rcParent.right;
			MoveWindow(&rect);	
		}

	}
	//	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

void CEditItem::OnNcDestroy() 
{
	CEdit::OnNcDestroy();

	// TODO: Add your message handler code here
	delete this;
}

void CEditItem::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);

	// TODO: Add your message handler code here
	CString str;	GetWindowText(str);

	// Send Notification to parent of ListView ctrl	
	LV_DISPINFO lvDispInfo;
	lvDispInfo.hdr.hwndFrom = GetParent()->m_hWnd;
	lvDispInfo.hdr.idFrom = GetDlgCtrlID();	
	lvDispInfo.hdr.code = LVN_ENDLABELEDIT;
	lvDispInfo.item.mask = LVIF_TEXT;	
	lvDispInfo.item.iItem = m_nItem;
	lvDispInfo.item.iSubItem = m_nSubItem;
	lvDispInfo.item.pszText = m_bVK_ESCAPE ? NULL : LPTSTR((LPCTSTR)str);
	lvDispInfo.item.cchTextMax = str.GetLength();
	GetParent()->GetParent()->SendMessage( WM_NOTIFY, GetParent()->GetDlgCtrlID(),(LPARAM)&lvDispInfo);
	DestroyWindow();	
}

int CEditItem::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{

	if (CEdit::OnCreate(lpCreateStruct) == -1)		
		return -1;	

	CFont* font = GetParent()->GetFont();
	SetFont(font);
	SetWindowText(m_sContent);
	SetFocus();	
	SetSel(0, 0);
	return 0;
}
