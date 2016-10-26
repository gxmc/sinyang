#pragma once
#include "plotdata.hpp"
#include "drawfunc.h"

// PlotWnd

class PlotWnd : public CWnd
{
	DECLARE_DYNAMIC(PlotWnd)

public:
	double xmin;
	double xmax;
	double ymin;
	double ymax;
	double pct;
	CPoint m_mouseDownPoint;
	bool bMouseCursor;
	size_t selectPIdx;
	double zoomrate;

	PlotData pd;
	CToolTipCtrl m_tool;
	void ResetRange(void)
	{
		UpdateRange(pd.raw.xll,xmin,xmax,pct,true);
		UpdateRange(pd.raw.yll,ymin,ymax,pct,true);
	};


public:



	PlotWnd()
		: xmin(0)
		, xmax(0)
		, ymin(0)
		, ymax(0)
		, pct(0.010)
		, m_mouseDownPoint(CPoint())
		, bMouseCursor(false)
		, selectPIdx(0)
		, zoomrate(0.9)
	{};
	virtual ~PlotWnd(){};

protected:
	DECLARE_MESSAGE_MAP()





protected:
	afx_msg void OnPaint(){
		CPaintDC dc(this); // device context for painting
		// TODO: Add your message handler code here
		// Do not call CWnd::OnPaint() for painting messages





		CRect rect;
		this->GetClientRect(&rect);	

		//DrawData(rect,&dc,pd,xmin,xmax,ymin,ymax);
		//if(bMouseCursor && !pd.ps.empty()
		//	&& selectPIdx>=0 && selectPIdx<pd.xll.size()){
		//		DrawData1(rect,&dc,pd.xll[selectPIdx],pd.yll[selectPIdx],xmin,xmax,ymin,ymax,inv(pd.psp.bkgndC));
		//}

		CSize winsz=rect.Size();

		CDC dcMem;//���ڻ�����ͼ���ڴ�DC
		dcMem.CreateCompatibleDC(&dc);//��������DC���������ڴ�DC

		CBitmap bmp;//�ڴ��г�����ʱͼ���λͼ
		bmp.CreateCompatibleBitmap(&dc,winsz.cx,winsz.cy);//��������λͼ
		dcMem.SelectObject(&bmp);  	//��λͼѡ����ڴ�DC


		DrawData(rect,&dcMem,pd,xmin,xmax,ymin,ymax);

		if(bMouseCursor && !pd.ls.empty()
			&& selectPIdx>=0 && selectPIdx<pd.raw.xll.size()){
				DrawData1(rect,&dcMem,pd.raw.xll[selectPIdx],pd.raw.yll[selectPIdx],xmin,xmax,ymin,ymax,inv(pd.ps.bkgndC));
		}

		dc.BitBlt(0,0,winsz.cx,winsz.cy,&dcMem,0,0,SRCCOPY);//���ڴ�DC�ϵ�ͼ�󿽱���ǰ̨
		//pDC->BitBlt(100,100,winsz.cx,winsz.cy,&dcMem,0,0,SRCCOPY);//���ڴ�DC�ϵ�ͼ�󿽱���ǰ̨

		dcMem.DeleteDC(); //ɾ��DC
		bmp.DeleteObject(); //ɾ��λͼ

	};
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point){
		// TODO: Add your message handler code here and/or call default

		SetFocus();

		if(!pd.ls.empty()){


			CRect plotrect;
			this->GetClientRect(&plotrect);	

			int re=DownUpdate(plotrect
				, pd.ps.metricSize
				, pd.ps.labelSize
				, pd.ps.metricGridLong
				, pd.ps.gap
				, point
				, m_mouseDownPoint
				, xmin, xmax, ymin, ymax
				, bMouseCursor
				, pd.raw.xll
				, pd.raw.yll
				, selectPIdx);

			switch(re){
			case 1:
				SetCapture();
				break;
			case 2:
				this->ClientToScreen(&m_mouseDownPoint);
				::SetCursorPos(m_mouseDownPoint.x,m_mouseDownPoint.y);
				Invalidate(FALSE);
				break;
			default:
				break;
			}

		}


		CWnd::OnLButtonDown(nFlags, point);
	};

	afx_msg void OnLButtonUp(UINT nFlags, CPoint point){
		// TODO: Add your message handler code here and/or call default


		ReleaseCapture();

		HCURSOR hCur  =  LoadCursor( NULL  , IDC_ARROW ) ;
		::SetCursor(hCur);


		CWnd::OnLButtonUp(nFlags, point);
	};
	afx_msg void OnMouseMove(UINT nFlags, CPoint point){
		// TODO: Add your message handler code here and/or call default


		//size_t selectIdx=m_spBtn.GetPos32();
		if(!pd.ls.empty()){

			//if(pd!=NULL && !pd->ps.empty() ){
			CRect plotrect;
			this->GetClientRect(&plotrect);	
			//ScreenToClient(&point);

			if(GetCapture()==this){
				if(MoveUpdateA(plotrect
					, pd.ps.metricSize
					, pd.ps.labelSize
					, pd.ps.metricGridLong
					, pd.ps.gap
					, point
					, this->m_mouseDownPoint
					, xmin,xmax,ymin,ymax))
					this->Invalidate(FALSE);
			}
			else{

				CString str;
				if(MoveUpdateB(plotrect
					, pd.ps.metricSize
					, pd.ps.labelSize
					, pd.ps.metricGridLong
					, pd.ps.gap
					, point
					, this->m_mouseDownPoint
					, xmin,xmax,ymin,ymax
					, str))
					m_tool.UpdateTipText(str,this);

			}

		}


		CWnd::OnMouseMove(nFlags, point);
	};

	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt){
		// TODO: Add your message handler code here and/or call default

		if(!pd.ls.empty()){

			//if(pd!=NULL && !pd->ps.empty() ){
			ScreenToClient(&pt);
			CRect plotrect;
			this->GetClientRect(&plotrect);
			if( WheelUpdate(plotrect
				, pd.ps.metricSize
				, pd.ps.labelSize
				, pd.ps.metricGridLong
				, pd.ps.gap
				, pt
				, ((zDelta>0)?zoomrate:1/zoomrate)
				,xmin,xmax,ymin,ymax) ){
					this->Invalidate(FALSE);
			}

		}


		return CWnd::OnMouseWheel(nFlags, zDelta, pt);
	};


	virtual BOOL PreTranslateMessage(MSG* pMsg){
		// TODO: Add your specialized code here and/or call the base class

		m_tool.RelayEvent(pMsg);

		return CWnd::PreTranslateMessage(pMsg);
	};
	//virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct){
		if (CWnd::OnCreate(lpCreateStruct) == -1)
			return -1;

		// TODO:  Add your specialized creation code here



		m_tool.Create(this);
		m_tool.AddTool(this);
		m_tool.Activate(true);

		ModifyStyle(0, SS_NOTIFY);
		//this->SetFocus();
		return 0;
	};
	//afx_msg LRESULT OnNcHitTest(CPoint point);
};

//IMPLEMENT_DYNAMIC(PlotWnd, CWnd)
//
//BEGIN_MESSAGE_MAP(PlotWnd, CWnd)
//	ON_WM_PAINT()
//	ON_WM_LBUTTONDOWN()
//	ON_WM_LBUTTONUP()
//	ON_WM_MOUSEMOVE()
//	ON_WM_MOUSEWHEEL()
//	ON_WM_CREATE()
//	ON_WM_NCHITTEST()
//END_MESSAGE_MAP()