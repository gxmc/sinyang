
// analyzerView.cpp : implementation of the CanalyzerView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "analyzer.h"
#endif

#include "analyzerDoc.h"
#include "analyzerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#include "export\ExportDataDlg.hpp"




// CanalyzerView

IMPLEMENT_DYNCREATE(CanalyzerView, CView)

	BEGIN_MESSAGE_MAP(CanalyzerView, CView)
		// Standard printing commands
		ON_COMMAND(ID_FILE_PRINT, &CanalyzerView::OnFilePrint)
		ON_COMMAND(ID_FILE_PRINT_DIRECT, &CanalyzerView::OnFilePrint)
		ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CanalyzerView::OnFilePrintPreview)
		ON_WM_CONTEXTMENU()
		ON_WM_RBUTTONUP()
		ON_WM_CREATE()
		ON_WM_SIZE()
		ON_MESSAGE(MESSAGE_CHANGE_APPLOOK, &CanalyzerView::OnMessageChangeApplook)
		ON_MESSAGE(MESSAGE_UPDATE_VIEW, &CanalyzerView::OnMessageUpdateView)
		ON_MESSAGE(MESSAGE_UPDATE_RAW, &CanalyzerView::OnMessageUpdateRaw)
		ON_MESSAGE(MESSAGE_UPDATE_TEST, &CanalyzerView::OnMessageUpdateTest)
		ON_NOTIFY(UDN_DELTAPOS, SPIN_ID, &CanalyzerView::OnDeltaposSpin)
		ON_COMMAND(ID_VIEW_FITWINDOW, &CanalyzerView::OnViewFitwindow)
		ON_COMMAND(ID_EDIT_COPY, &CanalyzerView::OnEditCopy)
		ON_COMMAND(ID_VIEW_DATACURSOR, &CanalyzerView::OnViewDatacursor)
		ON_COMMAND(ID_OPTIONS_PLOTSETTINGS, &CanalyzerView::OnOptionsPlotsettings)
		ON_COMMAND(ID_ANALYSIS_EXPORTDATA, &CanalyzerView::OnAnalysisExportdata)
	END_MESSAGE_MAP()

	// CanalyzerView construction/destruction

	CanalyzerView::CanalyzerView()
		: spBtnSize(CSize(23*2,23))
		, b2(FALSE)
	{
		// TODO: add construction code here
		 newClr=RGB(255,0,0);
		 oldClr=RGB(80,80,100);
	}

	CanalyzerView::~CanalyzerView()
	{
	}

	BOOL CanalyzerView::PreCreateWindow(CREATESTRUCT& cs)
	{
		// TODO: Modify the Window class or styles here by modifying
		//  the CREATESTRUCT cs

		return CView::PreCreateWindow(cs);
	}

	// CanalyzerView drawing

	void CanalyzerView::OnDraw(CDC* pDC)
	{
		CanalyzerDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		if (!pDoc)
			return;

		// TODO: add draw code for native data here

	}


	// CanalyzerView printing


	void CanalyzerView::OnFilePrintPreview()
	{

#ifndef SHARED_HANDLERS
		AFXPrintPreview(this);
#endif
	}


	void CanalyzerView::OnFilePrint()
	{
		CView::OnFilePrint();
	}


	BOOL CanalyzerView::OnPreparePrinting(CPrintInfo* pInfo)
	{
		// default preparation
		return DoPreparePrinting(pInfo);
	}

	void CanalyzerView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
	{
		// TODO: add extra initialization before printing
	}

	void CanalyzerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
	{
		// TODO: add cleanup after printing
	}

	void CanalyzerView::OnRButtonUp(UINT /* nFlags */, CPoint point)
	{
		ClientToScreen(&point);
		OnContextMenu(this, point);
	}

	void CanalyzerView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
	{
		CMenu menu;
		menu.LoadMenuW(IDR_POPUP_EDIT);

#ifndef SHARED_HANDLERS
		//theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
		theApp.GetContextMenuManager()->ShowPopupMenu(menu.GetSubMenu(0)->GetSafeHmenu(), point.x, point.y, this, TRUE);
#endif
	}


	// CanalyzerView diagnostics

#ifdef _DEBUG
	void CanalyzerView::AssertValid() const
	{
		CView::AssertValid();
	}

	void CanalyzerView::Dump(CDumpContext& dc) const
	{
		CView::Dump(dc);
	}

	CanalyzerDoc* CanalyzerView::GetDocument() const // non-debug version is inline
	{
		ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CanalyzerDoc)));
		return (CanalyzerDoc*)m_pDocument;
	}
#endif //_DEBUG


	// CanalyzerView message handlers

	int CanalyzerView::UpdateSpinButton(int np, bool bLast)
	{
		int selecti;	
		if(np>1){
			m_spBtn.SetRange32(0,np-1);
			m_spBtn.ShowWindow(SW_SHOW);
			if(bLast){
				selecti=np-1;
			}
			else{
				selecti=m_spBtn.GetPos32();
				if(selecti<0 || selecti>=np){
					selecti=0;				
				}
			}
		}
		else{
			m_spBtn.SetRange32(0,0);
			m_spBtn.ShowWindow(SW_HIDE);
			selecti=np-1;			
		}

		m_spBtn.SetPos32(selecti);
		return selecti;

	}

	int CanalyzerView::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if (CView::OnCreate(lpCreateStruct) == -1)
			return -1;

		// TODO:  Add your specialized creation code here


		LONG style0=GetWindowLongW(this->GetSafeHwnd(), GWL_EXSTYLE);
		SetWindowLongW(GetSafeHwnd(), GWL_EXSTYLE, style0|WS_CLIPCHILDREN);

		//pw.pdex=NULL;
		if(pw.Create(_T("STATIC"), _T(""), WS_CHILD | WS_VISIBLE /*| WS_DISABLED*/, CRect(), this, PWIN_ID)==FALSE)
			return -1;

		if(	m_spBtn.CreateEx(
			WS_EX_TOPMOST			
			//|WS_EX_TRANSPARENT
			,WS_CHILD
			|WS_VISIBLE
			//|UDS_ALIGNRIGHT
			//|UDS_ARROWKEYS
			|UDS_HORZ
			|UDS_WRAP
			, CRect()
			, this
			, SPIN_ID
			)==FALSE )
			return -1;

		m_spBtn.SetRange32(0,0);


		return 0;
	}


	void CanalyzerView::OnSize(UINT nType, int cx, int cy)
	{
		CView::OnSize(nType, cx, cy);

		// TODO: Add your message handler code here

		m_spBtn.MoveWindow(CRect(CPoint(cx,cy)-spBtnSize,spBtnSize));
		pw.SetWindowPos(&CWnd::wndBottom, 0, 0, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE );
	}


	afx_msg LRESULT CanalyzerView::OnMessageChangeApplook(WPARAM wParam, LPARAM lParam)
	{
		COLORREF oc=(COLORREF)wParam;

		//COLORREF occ=::GetSysColor(COLOR_MENUBAR);

		pw.blankPS.RefreshWinCr(oc);

		for(size_t i=0;i<pdl.size();i++){
			pdl[i].pd.ps.RefreshWinCr(oc);
		}

		pw.Invalidate(FALSE);
		return 0;
	}

	afx_msg LRESULT CanalyzerView::OnMessageUpdateView(WPARAM wParam, LPARAM lParam)
	{

		int selecti=UpdateSpinButton(pdl.size(),wParam&PW_LAST);
		pw.pdex=(selecti<0)? NULL : &pdl[selecti];
		pw.selectPIdx=0;
		pw.bMouseCursor=false;

		if(wParam&PW_SHOW_ALL){
			pw.ResetRange();
		}

		pw.SetLegend();
		pw.Invalidate(FALSE);

		return 0;
	}

	void CanalyzerView::OnDeltaposSpin(NMHDR* pNMHDR, LRESULT* pResult)
	{
		NM_UPDOWN* pNMUpDown=(NM_UPDOWN*)pNMHDR;	

		::PostMessage(this->GetSafeHwnd(),MESSAGE_UPDATE_VIEW,NULL,NULL);

		*pResult = 0;
	}

	void CanalyzerView::OnViewFitwindow()
	{
		// TODO: Add your command handler code here

		pw.ResetRange();
		pw.Invalidate(FALSE);

	}

	void CanalyzerView::OnEditCopy()
	{
		// TODO: Add your command handler code here

		if(m_spBtn.IsWindowVisible()){
			m_spBtn.ShowWindow(SW_HIDE);
			this->UpdateWindow();				
		}

		CopyWndToClipboard(this);

		UpdateSpinButton();

	}

	void CanalyzerView::OnViewDatacursor()
	{
		// TODO: Add your command handler code here
		pw.bMouseCursor=!pw.bMouseCursor;
	}


	void CanalyzerView::OnOptionsPlotsettings()
	{
		// TODO: Add your command handler code here

		pw.PlotSettingSheet(b2);
		if(!pdl.empty()){
			oldClr=pw.pdex->pd.GetOldCr(oldClr);
			newClr=pw.pdex->pd.GetNewCr(newClr);
		}
	}

	void CanalyzerView::OnAnalysisExportdata()
	{
		// TODO: Add your command handler code here

		ExportDataDlg edd;

		for(size_t i=0;i<pdl.size();i++){
			RawDataEx rdex(pdl[i].pd.raw,L"c",pdl[i].pd.ps.xlabel,pdl[i].pd.ps.ylabel);
			for(size_t j=0;j<pdl[i].pd.ls.size();j++){
				rdex.title[j]=pdl[i].pd.ls[j].name;
			}
			edd.lcl.rde.AppendData(rdex);
		}

		edd.DoModal();
	}


	void CanalyzerView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
	{
		// TODO: Add your specialized code here and/or call the base class



		//pDC->SetMapMode(MM_ANISOTROPIC); //转换坐标映射方式
		//CRect rect;
		//this->GetClientRect(&rect);

		//if(rect.IsRectEmpty()==TRUE)
		//	return;

		//CSize wsize = rect.Size(); 
		//pDC->SetWindowExt(wsize); 

		//HDC hdc=::GetDC(this->GetSafeHwnd());
		//int wmm=::GetDeviceCaps(hdc,HORZSIZE);
		//int hmm=::GetDeviceCaps(hdc,VERTSIZE);
		//int wpxl=::GetDeviceCaps(hdc,HORZRES);
		//int hpxl=::GetDeviceCaps(hdc,VERTRES);
		//int xLogPixPerInch0 = ::GetDeviceCaps(hdc,LOGPIXELSX); 
		//int yLogPixPerInch0 = ::GetDeviceCaps(hdc,LOGPIXELSY); 
		//::ReleaseDC(this->GetSafeHwnd(),hdc);

		////得到实际设备每逻辑英寸的象素数量
		//int xLogPixPerInch = pDC->GetDeviceCaps(LOGPIXELSX); 
		//int yLogPixPerInch = pDC->GetDeviceCaps(LOGPIXELSY); 
		////得到设备坐标和逻辑坐标的比例


		//CSize vsize(wsize.cx * xLogPixPerInch/xLogPixPerInch0, wsize.cy * yLogPixPerInch/yLogPixPerInch0);
		//pDC->SetViewportExt(vsize); //确定视口大小

		CView::OnPrepareDC(pDC, pInfo);
	}

	void CanalyzerView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
	{
		// TODO: Add your specialized code here and/or call the base class

		int   nPrintW   =   pDC->GetDeviceCaps(HORZRES); 
		int   nPrintH   =   pDC->GetDeviceCaps(VERTRES); 

		CBitmap bitmap;
		CClientDC dc(this);
		CRect rect;
		BOOL bRet = FALSE;

		GetClientRect(rect);
		bitmap.CreateCompatibleBitmap(&dc,rect.Width(),rect.Height()); 
		CBitmap * oldbitmap = pDC->SelectObject(&bitmap);

		CSize rectsz=rect.Size();
		if(nPrintW*rectsz.cy>nPrintH*rectsz.cx){
			int newW=nPrintH*rectsz.cx/rectsz.cy;
			bRet = pDC->StretchBlt((nPrintW-newW)/2,0,newW,nPrintH,&dc,0,0,rect.Width(),rect.Height(),SRCCOPY);
		}
		else{
			int newH=nPrintW*rectsz.cy/rectsz.cx;	
			bRet = pDC->StretchBlt(0,(nPrintH-newH)/2,nPrintW,newH,&dc,0,0,rect.Width(),rect.Height(),SRCCOPY);
		}

		CView::OnPrint(pDC, pInfo);

	}




	afx_msg LRESULT CanalyzerView::OnMessageUpdateTest(WPARAM wParam, LPARAM lParam)
	{
		CanalyzerDoc* pDoc = GetDocument();

		CSingleLock singleLock(&(pDoc->m_CritSection));

		if(singleLock.Lock()) // Resource has been locked
		{
			UINT flg=DataOutAList2PlotDataExList(pDoc->da.dol, pDoc->da.p1, pw.GetPlotSpec()->winbkC, pdl,(bool)(lParam));
			if(flg==2){
				::PostMessageW(this->GetParentFrame()->GetSafeHwnd(),WM_COMMAND,ID_ANALYSIS_ABORTANALYSIS,0);
			}

			// Now that we are finished, 
			// unlock the resource for others.
			singleLock.Unlock();
		}

		WPARAM wParamNew=(PW_SHOW_ALL);
		if(wParam&PW_INIT)
			wParamNew|=PW_LAST;

		::PostMessage(this->GetSafeHwnd(),MESSAGE_UPDATE_VIEW,wParamNew,NULL);
		//::SendMessage(mf->GetCaptionBar()->GetSafeHwnd(),MESSAGE_OVER,(WPARAM)str.GetBuffer(),NULL);


		return 0;
	}



	afx_msg LRESULT CanalyzerView::OnMessageUpdateRaw(WPARAM wParam, LPARAM lParam)
	{
		CanalyzerDoc* pDoc = GetDocument();

		CSingleLock singleLock(&(pDoc->m_CritSection));
		//singleLock.Lock();

		//if (singleLock.IsLocked())  // Resource has been locked
		if(singleLock.Lock())
		{
			UINT flg=RawData2PlotDataList(pDoc->da.raw,pDoc->da.dol,pw.GetPlotSpec()->winbkC, pdl, b2, newClr, oldClr);

			// Now that we are finished, 
			// unlock the resource for others.
			singleLock.Unlock();
		}

		WPARAM wParamNew=(PW_SHOW_ALL);
		if(wParam&PW_INIT)
			wParamNew|=PW_LAST;		

		::PostMessage(this->GetSafeHwnd(),MESSAGE_UPDATE_VIEW,wParamNew,NULL);

		return 0;
	}