// dlg1.cpp : implementation file
//

#include "stdafx.h"
#include "analyzer.h"
#include "dlg1.h"

#include <algorithm>
//#include "../../funT/xRescaleT.h"
//#include "../../funT/calgridT.h"
#include "xRescaleT.h"
#include "calgridT.h"
#include "colormapT.h"

//#include "../../W/funT/splineT.h"
//#include "../../W/funT/splintT.h"
#include "CSpline.cpp"
//#include "plotsp.h"

// dlg1

//void splintA(const std::vector<double> &xa, const std::vector<double> &ya, const std::vector<int> &x, std::vector<int> &y);


IMPLEMENT_DYNCREATE(dlg1, CFormView)

	dlg1::dlg1()
	: CFormView(dlg1::IDD)
	, xmin(0)
	, xmax(0)
	, ymin(0)
	, ymax(0)
	//, xlabel(_T(""))
	//, ylabel(_T(""))
	, m_mouseDownPoint(0)
	, metricGridLong(5)
	, metricGridShort(3)
{
	fs.bkgndC=white;
	fs.borderC=black;
	fs.gridC=black;
	fs.gridType=PS_DASHDOT;
	fs.labelC=green;
	fs.labelSize=20;
	fs.metricC=black;
	fs.metricSize=15;
}

dlg1::~dlg1()
{
}

void dlg1::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(dlg1, CFormView)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_CHAR()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// dlg1 diagnostics

#ifdef _DEBUG
void dlg1::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void dlg1::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// dlg1 message handlers


void dlg1::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	Invalidate();

}


void dlg1::genPointToPlot(const std::vector<double> &px, const std::vector<double> &py, const CRect &rect, std::vector<CPoint> &pointlist)
{

	//clock_t t=clock();

	//std::vector<CPoint> pointlist;
	//std::vector<long> pointx(px.size());
	//std::vector<long> pointy(py.size());;
	//long *p;
	//p=pointx.data();
	//xRescaleVT(px.data(),px.size(),xmin,xmax,p,rect.left,rect.right);
	//p=pointy.data();
	//xRescaleVT(py.data(),py.size(),ymin,ymax,p,rect.bottom,rect.top);

	//long nd=( (px.size()>py.size())? py.size():px.size() );

	//pointlist.clear();
	//for(long i=0;i<nd;i++){
	//	pointlist.push_back(CPoint(pointx[i],pointy[i]));
	//}

	size_t nd=( (px.size()>py.size())? py.size():px.size() );
	double tx1=(double)rect.Width()/(double)(xmax-xmin);
	int tx2=rect.left-xmin*tx1;
	double ty1=-(double)rect.Height()/(double)(ymax-ymin);
	int ty2=rect.bottom-ymin*ty1;
	pointlist.assign(nd,CPoint(tx2,ty2));
	for(size_t i=0;i<nd;i++){
		pointlist[i]+=CPoint(px[i]*tx1,py[i]*ty1);
	}


	//t=clock()-t;
	//TRACE("%dms\n",t);
	//return pointlist;
}


void dlg1::drawRectangle(const CRect &rect, CDC * pDC, COLORREF insidecolor, COLORREF bordercolor)
{
	// create and select a solid blue brush

	CBrush brushBlue(insidecolor);
	CBrush* pOldBrush = pDC->SelectObject(&brushBlue);

	// create and select a thick, black pen

	CPen penBlack;
	penBlack.CreatePen(PS_SOLID, 0, bordercolor);
	CPen* pOldPen = pDC->SelectObject(&penBlack);

	// draw a thick black rectangle filled with blue

	pDC->SetBkMode(TRANSPARENT);

	pDC->Rectangle(rect);

	// put back the old objects

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);

}


//not use
//CRect dlg1::DrawXYAxis(CRect rect, CDC* pdc)
//{
//	//if(rect.IsRectEmpty()){
//
//	//	return false;
//	//}
//	//else{
//
//	//CPoint ptOffset1(rect.left,rect.top);//y
//	//CPoint ptOffset2(rect.left,rect.bottom);//origin
//	//CPoint ptOffset3(rect.right,rect.top);
//	//CPoint ptOffset4(rect.right,rect.bottom);//x
//	CFont font;
//	CFont *pOldFont;
//	CString str;
//	CPen * pOldPen;
//	CSize sz,szt;
//	CRect newrect=rect;
//	CPoint textLocate;
//	CPen pen;
//	int metricH=fs.metricSize;
//	int labelH=fs.labelSize;
//	int lc=5;
//	int lcs=lc-2;
//	double gridi,XMAX,XMIN,YMAX,YMIN;
//	int tmp;
//	COLORREF oc;
//	CRect xmrect(rect.left,rect.bottom+lc,rect.left,rect.bottom+lc);
//	CRect ymrect(rect.left-lc,rect.bottom,rect.left-lc,rect.bottom);
//	XMAX=xmax;
//	XMIN=xmin;
//	YMAX=ymax;
//	YMIN=ymin;
//
//	CString fontName=L"Arial";
//
//	COLORREF bkgndC=fs.bkgndC;
//	COLORREF axisC=fs.gridC;
//	COLORREF labelC=fs.labelC;
//
//	drawRectangle(rect,pdc,fs.bkgndC,fs.borderC);
//
//	////////////////////////////////////////////////////////////////////
//
//
//	//draw x axis
//
//	pen.CreatePen(PS_SOLID, 1, axisC);
//	pOldPen=pdc->SelectObject(&pen);
//	//pdc->MoveTo(ptOffset2);
//	//pdc->LineTo(ptOffset4);
//
//	//draw x metric
//
//
//	font.CreateFont(
//		metricH,                        // nHeight
//		0,                         // nWidth
//		0,                         // nEscapement
//		0,                         // nOrientation
//		FW_NORMAL,                 // nWeight
//		FALSE,                     // bItalic
//		FALSE,                     // bUnderline
//		0,                         // cStrikeOut
//		ANSI_CHARSET,              // nCharSet
//		OUT_DEFAULT_PRECIS,        // nOutPrecision
//		CLIP_DEFAULT_PRECIS,       // nClipPrecision
//		DEFAULT_QUALITY,           // nQuality
//		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
//		fontName);                 // lpszFacename
//
//	pOldFont=pdc->SelectObject(&font);
//
//	//double resox=pow(10.0,calgrid(XMAX-XMIN));
//	oc=pdc->SetTextColor(fs.metricC);
//
//	double resox=calreso(XMAX-XMIN);
//
//	for(gridi=resox*ceil(XMIN/resox);gridi<=XMAX;gridi+=resox){
//
//		tmp=xRescale(gridi,XMIN,XMAX,rect.left,rect.right);
//
//		pdc->MoveTo(tmp,rect.bottom);
//		pdc->LineTo(tmp,rect.top);
//
//
//		if(fs.metricSize>=0){
//			if(gridi<resox && -gridi<resox)
//				gridi=0;
//
//			str.Format(L"%g",gridi);
//			sz=pdc->GetTextExtent(str);
//
//
//			textLocate.x=tmp-sz.cx/2;
//			textLocate.y=rect.bottom+lc;
//			if(xmrect.right<textLocate.x){
//				if(textLocate.x+sz.cx<rect.right){
//					pdc->TextOutW(textLocate.x,textLocate.y,str);
//					xmrect.right=textLocate.x+sz.cx;
//					xmrect.bottom=textLocate.y+sz.cy;
//
//					pdc->MoveTo(tmp,rect.bottom);
//					pdc->LineTo(tmp,rect.bottom+lc);
//
//				}
//			}
//			else{
//				pdc->MoveTo(tmp,rect.bottom);
//				pdc->LineTo(tmp,rect.bottom+lcs);
//			}
//		}
//
//	}
//	newrect.bottom+=lc+sz.cy;
//	pdc->SelectObject(pOldFont);
//	font.DeleteObject();
//	pdc->SetTextColor(oc);
//	pdc->SelectObject(pOldPen);
//	pen.DeleteObject();
//
//	if(labelH>=0){
//		//draw x axis label
//		font.CreateFont(
//			labelH,                        // nHeight
//			0,                         // nWidth
//			0,                         // nEscapement
//			0,                         // nOrientation
//			FW_NORMAL,                 // nWeight
//			FALSE,                     // bItalic
//			FALSE,                     // bUnderline
//			0,                         // cStrikeOut
//			ANSI_CHARSET,              // nCharSet
//			OUT_DEFAULT_PRECIS,        // nOutPrecision
//			CLIP_DEFAULT_PRECIS,       // nClipPrecision
//			DEFAULT_QUALITY,           // nQuality
//			DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
//			fontName);                 // lpszFacename
//		oc=pdc->SetTextColor(labelC);
//		//font.CreatePointFont(200,L"MS Gothic",NULL);
//		//str.Format(L"time(s)");
//		str.Format(pd.xlabel);
//		//str=dtlist.back().label[0];
//		pOldFont=pdc->SelectObject(&font);
//		sz=pdc->GetTextExtent(str);
//		//pdc->SetTextAlign(TA_UPDATECP);
//		pdc->TextOutW(rect.CenterPoint().x-(sz.cx/2),newrect.bottom,str);
//		pdc->SelectObject(pOldFont);
//		font.DeleteObject();
//		pdc->SetTextColor(oc);
//		//TRACE("%d,",sz.cy);
//		newrect.bottom+=sz.cy;
//	}
//	///////////////////////////////////////////////////////
//
//
//
//
//	//draw y axis
//
//	pen.CreatePen(PS_SOLID, 1, axisC);
//	pOldPen=pdc->SelectObject(&pen);
//	//pdc->MoveTo(ptOffset2);
//	//pdc->LineTo(ptOffset1);
//
//
//
//	font.CreateFont(
//		metricH,                        // nHeight
//		0,                         // nWidth
//		900,                         // nEscapement
//		0,                         // nOrientation
//		FW_NORMAL,                 // nWeight
//		FALSE,                     // bItalic
//		FALSE,                     // bUnderline
//		0,                         // cStrikeOut
//		ANSI_CHARSET,              // nCharSet
//		OUT_DEFAULT_PRECIS,        // nOutPrecision
//		CLIP_DEFAULT_PRECIS,       // nClipPrecision
//		DEFAULT_QUALITY,           // nQuality
//		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
//		fontName);                 // lpszFacename
//	oc=pdc->SetTextColor(fs.metricC);
//
//	//draw y metric
//	double resoy=calreso(YMAX-YMIN);
//	//double resoy=pow(10.0,calgrid(YMAX-YMIN));
//	pOldFont=pdc->SelectObject(&font);
//	for(gridi=resoy*ceil(YMIN/resoy);gridi<=YMAX;gridi+=resoy){
//		tmp=xRescale(gridi,YMIN,YMAX,rect.bottom,rect.top);
//		pdc->MoveTo(rect.left,tmp);
//		pdc->LineTo(rect.left-lcs,tmp);
//
//		pdc->MoveTo(rect.left,tmp);
//		pdc->LineTo(rect.right,tmp);
//
//		if(gridi<resoy && -gridi<resoy)
//			gridi=0;
//
//		str.Format(L"%g",gridi);
//		sz=pdc->GetTextExtent(str);
//
//		textLocate.x=rect.left-lc-sz.cy;
//		textLocate.y=tmp+sz.cx/2;
//		if(ymrect.top>textLocate.y){
//			if(textLocate.y-sz.cx>rect.top){
//				pdc->TextOutW(textLocate.x,textLocate.y,str);
//				ymrect.left=textLocate.x;
//				ymrect.top=textLocate.y-sz.cx;
//
//				pdc->MoveTo(rect.left,tmp);
//				pdc->LineTo(rect.left-lc,tmp);
//
//
//			}
//		}
//
//
//	}
//	newrect.left-=lc+sz.cy;
//	pdc->SelectObject(pOldFont);
//	font.DeleteObject();
//	pdc->SetTextColor(oc);
//	pdc->SelectObject(pOldPen);
//	pen.DeleteObject();
//
//
//	if(labelH>=0){
//		//draw y axis label
//		font.CreateFont(
//			labelH,                        // nHeight
//			0,                         // nWidth
//			900,                         // nEscapement
//			0,                         // nOrientation
//			FW_NORMAL,                 // nWeight
//			FALSE,                     // bItalic
//			FALSE,                     // bUnderline
//			0,                         // cStrikeOut
//			ANSI_CHARSET,              // nCharSet
//			OUT_DEFAULT_PRECIS,        // nOutPrecision
//			CLIP_DEFAULT_PRECIS,       // nClipPrecision
//			DEFAULT_QUALITY,           // nQuality
//			DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
//			fontName);                 // lpszFacename
//
//		oc=pdc->SetTextColor(labelC);
//
//		//str.Format(L"current(A)");
//		str.Format(pd.ylabel);
//		//str=dtlist.back().label[1];
//		pOldFont=pdc->SelectObject(&font);
//		sz=pdc->GetTextExtent(str);
//		pdc->TextOutW(newrect.left-sz.cy,rect.CenterPoint().y+(sz.cx/2),str);
//		pdc->SelectObject(pOldFont);
//		font.DeleteObject();
//		//TRACE("%d,",sz.cy);
//		pdc->SetTextColor(oc);
//		newrect.left-=sz.cy;
//
//	}
//
//	///////////////////////////////////////////////////////////////////////
//
//
//
//
//	//return true;
//	return newrect;
//	//}
//}



// update xmin,xmax,ymin,ymax
void dlg1::updatePlotRange(const std::vector<double> &x, const std::vector<double> &y, bool flg)
{

	double iv,pct=0.02;
	double tmin,tmax;
	auto resultx=std::minmax_element(x.begin(),x.end());
	tmin=*resultx.first;
	tmax=*resultx.second;
	iv=tmax-tmin;
	tmin-=iv*pct;
	tmax+=iv*pct;

	if(flg||xmin>tmin)
		xmin=tmin;
	if(flg||xmax<tmax)
		xmax=tmax;

	if(xmin==xmax){
		xmin-=1;
		xmax+=1;
	}


	auto resulty=std::minmax_element(y.begin(),y.end());
	tmin=*resulty.first;
	tmax=*resulty.second;

	iv=tmax-tmin;
	tmin-=iv*pct;
	tmax+=iv*pct;


	if(flg||ymin>tmin)
		ymin=tmin;
	if(flg||ymax<tmax)
		ymax=tmax;

	if(ymin==ymax){
		ymin-=1;
		ymax+=1;
	}

}

// update xmin,xmax,ymin,ymax
void dlg1::updatePlotRange(bool flg)
{

	double iv,pct=0.02;
	double tmin,tmax;
	auto resultx=std::minmax_element(pd.xll.begin(),pd.xll.end());
	tmin=*resultx.first;
	tmax=*resultx.second;
	iv=tmax-tmin;
	tmin-=iv*pct;
	tmax+=iv*pct;

	if(flg||xmin>tmin)
		xmin=tmin;
	if(flg||xmax<tmax)
		xmax=tmax;

	if(xmin==xmax){
		xmin-=1;
		xmax+=1;
	}


	auto resulty=std::minmax_element(pd.yll.begin(),pd.yll.end());
	tmin=*resulty.first;
	tmax=*resulty.second;

	iv=tmax-tmin;
	tmin-=iv*pct;
	tmax+=iv*pct;


	if(flg||ymin>tmin)
		ymin=tmin;
	if(flg||ymax<tmax)
		ymax=tmax;

	if(ymin==ymax){
		ymin-=1;
		ymax+=1;
	}

}

void dlg1::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CFormView::OnPaint() for painting messages


	/////////////////////////////////////////////////////////////////////////////////////
	//CRect plotrect;
	//GetPlotRect(plotrect);
	////GetDlgItem(IDC_PLOT)->GetWindowRect(&plotrect);
	//if( !xlist.empty() && !ylist.empty() ){
	//	if(!plotrect.IsRectEmpty()){

	//		DrawXYAxis(plotrect,&dc);

	//		CRect mainrt;
	//		CRgn rgn;
	//		rgn.CreateRectRgnIndirect(&plotrect);	
	//		dc.GetClipBox(&mainrt);
	//		dc.SelectClipRgn(&rgn);
	//		//clock_t t=clock();

	//		//DrawPoint(plotrect,&dc,0);
	//		DrawCurve(plotrect,&dc);

	//		//TRACE("%dms\n",clock()-t);
	//		//CString str;
	//		//str.Format(L"%dms\n",clock()-t);
	//		//AfxMessageBox(str);
	//		rgn.SetRectRgn(&mainrt);
	//		dc.SelectClipRgn(&rgn);

	//		//CRect legendrect=DrawLegend1( CRect(plotrect.right-plotrect.Width()/4,plotrect.top,plotrect.right,plotrect.top+plotrect.Height()/4), &dc);
	//		//legendrect.InflateRect(0,0,1,0);
	//		//drawRectangle(legendrect,&dc,white,black);
	//		//legendrect=DrawLegend(plotrect, &dc);

	//	}
	//}
	////////////////////////////////////////////////////////////////////////////////////

	CDC dcMem;                                                  //用于缓冲作图的内存DC
	CBitmap bmp;                                                 //内存中承载临时图象的位图
	CRect rect;
	GetClientRect(&rect);
	dcMem.CreateCompatibleDC(&dc);               //依附窗口DC创建兼容内存DC
	bmp.CreateCompatibleBitmap(&dc,rect.Width(),rect.Height());//创建兼容位图
	dcMem.SelectObject(&bmp);                          //将位图选择进内存DC
	dcMem.FillSolidRect(rect,fs.bkgndC);//按原来背景填充客户区，不然会是黑色

	CRect plotrect;
	GetPlotRect(plotrect);

	//if( !xlist.empty() && !ylist.empty() ){
	if(!plotrect.IsRectEmpty() && !pd.ps.empty()){

		//dcMem.SetBkColor(fs.bkgndC);
		//DrawXYAxis(plotrect,&dcMem);
		DrawXYAxis1(plotrect,&dcMem);

		CRect mainrt;
		CRgn rgn;
		rgn.CreateRectRgnIndirect(&plotrect);	
		dcMem.GetClipBox(&mainrt);
		dcMem.SelectClipRgn(&rgn);
		//clock_t t=clock();

		//DrawPoint(plotrect,&dcMem,0);
		//DrawCurve(plotrect,&dcMem);
		DrawCurveA(plotrect,&dcMem);
		//DrawCurveB(plotrect,&dcMem);

		//TRACE("%dms\n",clock()-t);
		//CString str;
		//str.Format(L"%dms\n",clock()-t);
		//AfxMessageBox(str);
		rgn.SetRectRgn(&mainrt);
		dcMem.SelectClipRgn(&rgn);

		CRect legendrect=DrawLegend1( CRect(plotrect.right-plotrect.Width()/4,plotrect.top,plotrect.right,plotrect.top+plotrect.Height()/4), &dcMem);

	}
	//}


	dc.BitBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,SRCCOPY);//将内存DC上的图象拷贝到前台
	dcMem.DeleteDC(); //删除DC
	bmp.DeleteObject(); //删除位图


}



//
//void dlg1::plot2d(const std::vector<double> &x, const std::vector<double> &y, const CString &xla, const CString &yla )
//{
//	if(x.size()!=y.size())
//		return;
//
//
//	//xlist.push_back(x);
//	//ylist.push_back(y);
//
//	xll.resize(xll.size()+x.size());
//	std::copy_backward(x.begin(),x.end(),xll.end());
//	yll.resize(yll.size()+y.size());
//	std::copy_backward(y.begin(),y.end(),yll.end());
//	ll.push_back(x.size());
//
//	CString str;
//	str.Format(L"%d",clist.size());
//	names.push_back(str);
//
//	clist.push_back( genColor( genColorvFromIndex<float>( clist.size() ) ) );
//
//	updatePlotRange(x,y,(ll.size()==1));
//	xlabel=xla;
//	ylabel=yla;
//
//	Invalidate();
//}


//void dlg1::plot2d(const std::vector<double> &x, const std::vector<double> &y, const CString &xla, const CString &yla, const CString &title )
//{
//	if(x.size()!=y.size())
//		return;
//
//	//xlist.push_back(x);
//	//ylist.push_back(y);
//
//	xll.resize(xll.size()+x.size());
//	std::copy_backward(x.begin(),x.end(),xll.end());
//	yll.resize(yll.size()+y.size());
//	std::copy_backward(y.begin(),y.end(),yll.end());
//	ll.push_back(x.size());
//	names.push_back(title);
//
//	clist.push_back( genColor( genColorvFromIndex<float>( clist.size() ) ) );
//
//	updatePlotRange(x,y,(ll.size()==1));
//	xlabel=xla;
//	ylabel=yla;
//
//	Invalidate();
//}
//
//
//void dlg1::plot2d(const std::vector<double> &x, const std::vector<double> &y, const CString &xla, const CString &yla, const CString &title, const int &LineStl )
//{
//	if(x.size()!=y.size())
//		return;
//
//	//xlist.push_back(x);
//	//ylist.push_back(y);
//
//	xll.resize(xll.size()+x.size());
//	std::copy_backward(x.begin(),x.end(),xll.end());
//	yll.resize(yll.size()+y.size());
//	std::copy_backward(y.begin(),y.end(),yll.end());
//	ll.push_back(x.size());
//
//
//	names.push_back(title);
//
//	clist.push_back( genColor( genColorvFromIndex<float>( clist.size() ) ) );
//
//	updatePlotRange(x,y,(ll.size()==1));
//
//	xlabel=xla;
//	ylabel=yla;
//	LineStyle.push_back(LineStl);
//
//	Invalidate();
//}


//void dlg1::plot2d(const std::vector<double> &x, const std::vector<double> &y, const plotspec &plotsp, const CString &xla, const CString &yla)
//{
//	pd.AddNew(x,y,plotsp,xla,yla);
//	updatePlotRange(x,y,(pd.ll.size()==1));
//	Invalidate();
//}

void dlg1::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default


	// Check if we have captured the mouse
	if (GetCapture()==this && !pd.ps.empty())
	{
		CRect plotrect;
		GetPlotRect(plotrect);

		double kx=(double)(point.x-m_mouseDownPoint.x)*(xmax-xmin)/(double)plotrect.Width();
		double ky=(double)(point.y-m_mouseDownPoint.y)*(ymax-ymin)/(double)plotrect.Height();

		//Increment the object rotation angles
		xmin-=kx;
		xmax-=kx;
		ymin+=ky;
		ymax+=ky;

		//Redraw the view
		//InvalidateRect(NULL,FALSE);
		Invalidate();
		//Set the mouse point
		m_mouseDownPoint=point;
	}


	CFormView::OnMouseMove(nFlags, point);
}


void dlg1::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	m_mouseDownPoint=point;
	SetCapture();

	CFormView::OnLButtonDown(nFlags, point);
}


void dlg1::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	m_mouseDownPoint=CPoint(0,0);
	ReleaseCapture();

	CFormView::OnLButtonUp(nFlags, point);
}


BOOL dlg1::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	CRect plotrect;
	GetPlotRect(plotrect);
	ScreenToClient(&pt);

	if(!pd.ps.empty()){
		double x=xRescale(pt.x, plotrect.left, plotrect.right, xmin, xmax);
		double y=xRescale(pt.y, plotrect.bottom, plotrect.top, ymin, ymax);

		double k1=(zDelta>0)?0.8:1.25;	
		double k2=1-k1;

		int w=20;

		bool flg=false;
		plotrect.InflateRect(w,0,0,0);
		if( plotrect.PtInRect(pt) ){
			ymin=y*k2+ymin*k1;
			ymax=y*k2+ymax*k1;
			flg=true;
		}
		plotrect.InflateRect(-w,0,0,w);
		if( plotrect.PtInRect(pt) ){
			xmin=x*k2+xmin*k1;
			xmax=x*k2+xmax*k1;
			flg=true;
		}
		if(flg)
			Invalidate();

	}


	return CFormView::OnMouseWheel(nFlags, zDelta, pt);
}

//not use
//CRect dlg1::DrawLegend(CRect rect, CDC* pDC)
//{
//
//
//	CFont font;
//	CFont *pOldFont;
//	CString str;
//	CPen * pOldPen;
//	CSize sz;
//	//CRect newrect=rect;
//
//	CSize rectsz(0,0);
//	CPen pen;
//	int metricH=15;
//	//int labelH=20;
//	int lc=20;
//	int gap=2;
//	CPoint textLocate(rect.right-gap,rect.top);
//	//int lcs=lc-2;
//	//double gridi,XMAX,XMIN,YMAX,YMIN;
//	int tmp;
//	COLORREF oc;
//	CString fontName=L"Arial";
//
//
//	font.CreateFont(
//		metricH,                        // nHeight
//		0,                         // nWidth
//		0,                         // nEscapement
//		0,                         // nOrientation
//		FW_NORMAL,                 // nWeight
//		FALSE,                     // bItalic
//		FALSE,                     // bUnderline
//		0,                         // cStrikeOut
//		ANSI_CHARSET,              // nCharSet
//		OUT_DEFAULT_PRECIS,        // nOutPrecision
//		CLIP_DEFAULT_PRECIS,       // nClipPrecision
//		DEFAULT_QUALITY,           // nQuality
//		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
//		fontName);                 // lpszFacename
//
//
//
//	pOldFont=pDC->SelectObject(&font);	
//
//
//	//pDC->SetBkColor(white);
//	//pDC->SetBkMode(TRANSPARENT);
//
//	for(size_t i=0;i<pd.ps.size();i++){
//
//		//pen.CreatePen(PS_SOLID, 1, clist[i]);
//		//pOldPen=pDC->SelectObject(&pen);
//		//oc=pDC->SetTextColor(clist[i]);
//
//		//oc=pDC->SetTextColor(black);
//		sz=pDC->GetTextExtent(pd.ps[i].name);
//
//		if(sz.cx>rectsz.cx)
//			rectsz.cx=sz.cx;
//
//		//pDC->TextOutW(textLocate.x-sz.cx,textLocate.y,names[i]);
//
//		//pDC->MoveTo(textLocate.x-sz.cx-gap,textLocate.y+sz.cy/2);
//		//pDC->LineTo(textLocate.x-sz.cx-lc,textLocate.y+sz.cy/2);
//		textLocate.y+=sz.cy;
//
//		//pDC->SelectObject(pOldPen);
//		//pen.DeleteObject();
//		//pDC->SetTextColor(oc);
//	}
//
//	//pDC->SelectObject(pOldFont);
//	//font.DeleteObject();
//
//
//	rectsz.cx+=lc;
//	//rectsz.cy=textLocate.y;
//
//
//	CRect legendrect(textLocate.x-rectsz.cx,rect.top,textLocate.x+1,textLocate.y);
//
//	//return CRect(textLocate.x-rectsz.cx,rect.top,textLocate.x,textLocate.y);
//
//	////////////////////////////////////////////////////////
//
//	drawRectangle(legendrect,pDC,white,black);
//
//
//	textLocate=legendrect.TopLeft();
//
//	pOldFont=pDC->SelectObject(&font);	
//
//
//	pDC->SetBkColor(white);
//	pDC->SetBkMode(TRANSPARENT);
//
//	for(size_t i=0;i<pd.ps.size();i++){
//
//		pen.CreatePen(PS_SOLID, 1, pd.ps[i].colour);
//		pOldPen=pDC->SelectObject(&pen);
//		//oc=pDC->SetTextColor(clist[i]);
//
//		oc=pDC->SetTextColor(black);
//		sz=pDC->GetTextExtent(pd.ps[i].name);
//
//		pDC->MoveTo(textLocate.x,textLocate.y+sz.cy/2);
//		pDC->LineTo(textLocate.x+lc-gap,textLocate.y+sz.cy/2);	
//
//		pDC->TextOutW(textLocate.x+lc,textLocate.y,pd.ps[i].name);
//
//		textLocate.y+=sz.cy;
//
//		pDC->SelectObject(pOldPen);
//		pen.DeleteObject();
//		pDC->SetTextColor(oc);
//	}
//
//	pDC->SelectObject(pOldFont);
//	font.DeleteObject();
//
//
//	rectsz.cx+=lc;
//
//
//	return CRect(textLocate.x-rectsz.cx,rect.top,textLocate.x,textLocate.y);
//
//}



CRect dlg1::DrawLegend1(CRect rect, CDC* pDC)
{


	int lc=25;
	int gap=2;
	int Hmax=15;
	CString fontName=L"Arial";

	CFont font;
	CFont *pOldFont;
	CString str;
	CPen * pOldPen;
	CSize sz;

	CSize rectsz(0,0);
	CPen pen;

	int metricH;
	if(rect.Height()>pd.ps.size()){
		metricH=rect.Height()/pd.ps.size();
		if(metricH>Hmax)
			metricH=Hmax;
	}
	else{
		metricH=1;
	}
	

	CPoint textLocate;
	CPoint topright(rect.right,rect.top);
	int tmp;
	COLORREF oc;



	font.CreateFont(
		metricH,                   // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		fontName);                 // lpszFacename



	pOldFont=pDC->SelectObject(&font);	

	for(size_t i=0;i<pd.ps.size();i++){
		sz=pDC->GetTextExtent(pd.ps[i].name);
		if(sz.cx>rectsz.cx)
			rectsz.cx=sz.cx;
		rectsz.cy+=sz.cy;
	}
	rectsz.cx+=lc+gap*3;

	CRect borderrect(topright.x-rectsz.cx,topright.y,topright.x,topright.y+rectsz.cy);
	CRect legendrect(borderrect);
	legendrect.DeflateRect(gap,0,gap,0);

	////////////////////////////////////////////////////////

	drawRectangle(borderrect,pDC,fs.bkgndC,fs.bkgndC^0x00ffffff);

	textLocate=legendrect.TopLeft();
	pOldFont=pDC->SelectObject(&font);	
	oc=pDC->SetTextColor(fs.bkgndC^0x00ffffff);

	pDC->SetBkColor(fs.bkgndC);
	pDC->SetBkMode(TRANSPARENT);

	for(size_t i=0;i<pd.ps.size();i++){
		//pen.CreatePen(PS_SOLID, 1, pd.ps[i].colour);
		//pOldPen=pDC->SelectObject(&pen);
		//oc=pDC->SetTextColor(clist[i]);


		sz=pDC->GetTextExtent(pd.ps[i].name);

		//pDC->TextOutW(textLocate.x+lc+gap,textLocate.y,pd.ps[i].name);

		//if(pd.ps[i].showLine){
		if(pd.ps[i].lineType>=0){
			//pen.CreatePen(PS_SOLID, 1, pd.ps[i].colour);
			pen.CreatePen(pd.ps[i].lineType, 1, pd.ps[i].colour);
			pOldPen=pDC->SelectObject(&pen);
			pDC->MoveTo(textLocate.x,textLocate.y+sz.cy/2);
			pDC->LineTo(textLocate.x+lc,textLocate.y+sz.cy/2);
			pDC->SelectObject(pOldPen);
			pen.DeleteObject();

		}

		if(pd.ps[i].dotSize==1){
			pDC->SetPixelV(textLocate.x+lc/2,textLocate.y+sz.cy/2,pd.ps[i].colour);
		}

		if(pd.ps[i].dotSize>1){
			CRect prect(0,0,1,1);
			prect.InflateRect(pd.ps[i].dotSize-1,pd.ps[i].dotSize-1);
			prect.MoveToXY(textLocate.x+lc/2-pd.ps[i].dotSize+1,textLocate.y+sz.cy/2-pd.ps[i].dotSize+1);
			pDC->FillSolidRect(&prect,pd.ps[i].colour);
		}



		pDC->TextOutW(textLocate.x+lc+gap,textLocate.y,pd.ps[i].name);

		textLocate.y+=sz.cy;

		//pDC->SelectObject(pOldPen);
		//pen.DeleteObject();
		
	}

	pDC->SetTextColor(oc);
	pDC->SelectObject(pOldFont);
	font.DeleteObject();

	return borderrect;

}



//
//void dlg1::DrawPoint(CRect rect, CDC * pDC, int d)
//{
//	std::vector<CPoint> pointlist;
//
//	if(d>0){
//		CRect prect(0,0,2*d,2*d);
//		CSize ppoc=CSize(d,d);
//
//		//for(size_t j=0;j<xlist.size();j++){
//		//	genPointToPlot(xlist[j],ylist[j],rect,pointlist);
//		//	for(size_t i=0;i<pointlist.size();i++){
//		//		prect.MoveToXY(pointlist[i]-ppoc);
//		//		drawRectangle(prect,pDC,clist[j],clist[j]);
//		//	}
//		//}
//
//		genPointToPlot(xll,yll,rect,pointlist);
//		size_t si=0;
//		for(size_t j=0;j<ll.size();j++){
//			for(size_t i=0;i<ll[j];i++){
//				prect.MoveToXY(pointlist[i+si]-ppoc);
//				drawRectangle(prect,pDC,clist[j],clist[j]);
//			}
//			si+=ll[j];
//		}
//
//	}
//	else{
//
//		//for(size_t j=0;j<xlist.size();j++){
//		//	genPointToPlot(xlist[j],ylist[j],rect,pointlist);
//		//	for(size_t i=0;i<pointlist.size();i++){
//		//		pDC->SetPixelV(pointlist[i],clist[j]);					
//		//	}
//		//}
//
//		genPointToPlot(xll,yll,rect,pointlist);
//		size_t si=0;
//		for(size_t j=0;j<ll.size();j++){
//			for(size_t i=0;i<ll[j];i++){
//				pDC->SetPixelV(pointlist[i+si],ps[j].colour);	
//			}
//			si+=ll[j];
//		}
//
//
//	}
//
//
//
//	//	//oc=dc.SetTextColor(clist[j]);
//	//	//sz=dc.GetTextExtent(dtlist[j].FileName);
//	//	//MemDC.TextOutW(plotrect.right,plotrect.top+sz.cy*j,dtlist[j].FileName);
//	//	//dc.SetTextColor(oc);
//
//}
//
//
//void dlg1::DrawCurve(CRect rect, CDC* pDC)
//{
//	std::vector<CPoint> pointlist;
//	CPen pen;
//	CPen * pOldPen;
//
//	//for(size_t j=0;j<xlist.size();j++){
//	//	genPointToPlot(xlist[j],ylist[j],rect,pointlist);
//	//	pen.CreatePen(PS_SOLID,1,clist[j]);
//	//	pOldPen=pDC->SelectObject(&pen);
//	//	pDC->Polyline(pointlist.data(),pointlist.size());
//	//	//std::vector<BYTE> styl(pointlist.size(),PT_LINETO);
//	//	//styl[0]=PT_MOVETO;
//	//	//dc.PolyDraw(pointlist.data(),styl.data(),pointlist.size());
//	//	pDC->SelectObject(pOldPen);
//	//	pen.DeleteObject();
//	//}
//
//
//	genPointToPlot(xll,yll,rect,pointlist);
//	size_t si=0;
//	CPoint *pp=pointlist.data();
//	for(size_t j=0;j<ll.size();j++){
//		pen.CreatePen(PS_SOLID,1,clist[j]);
//		pOldPen=pDC->SelectObject(&pen);		
//		pDC->Polyline(&pp[si],ll[j]);
//		si+=ll[j];
//		//std::vector<BYTE> styl(pointlist.size(),PT_LINETO);
//		//styl[0]=PT_MOVETO;
//		//dc.PolyDraw(pointlist.data(),styl.data(),pointlist.size());
//		pDC->SelectObject(pOldPen);
//		pen.DeleteObject();
//	}
//
//
//	//////////////////////////////fast///////////////////////////
//	//genPointToPlot(xll,yll,rect,pointlist);
//	//pen.CreatePen(PS_SOLID,1,clist[0]);
//	//pOldPen=pDC->SelectObject(&pen);
//	//pDC->PolyPolyline(pointlist.data(),ll.data(),ll.size());
//	////dc.PolyDraw(pointlist.data(),styl.data(),pointlist.size());
//	//pDC->SelectObject(pOldPen);
//	//pen.DeleteObject();
//	//////////////////////////////////////////////////////////////////
//
//
//}
//



void dlg1::DrawCurveA(CRect rect, CDC* pDC)
{
	std::vector<CPoint> pointlist;
	CPen pen;
	CPen * pOldPen;

	COLORREF oc;

	genPointToPlot(pd.xll,pd.yll,rect,pointlist);
	size_t si=0;
	CPoint *pp=pointlist.data();	


	for(size_t j=0;j<pd.ll.size();j++){

		//if(pd.ps[j].showLine){
		if(pd.ps[j].lineType>=0){	

			//pen.CreatePen(PS_SOLID,1,pd.ps[j].colour);
			pen.CreatePen(pd.ps[j].lineType,1,pd.ps[j].colour);
			pOldPen=pDC->SelectObject(&pen);

			if(pd.ll[j]>2){
				if(pd.ps[j].smoothLine==1){
					DrawSpline(&pp[si],pd.ll[j],rect,pDC);
				}
				if(pd.ps[j].smoothLine==2){
					pDC->PolyBezier(&pp[si],pd.ll[j]);
				}
				if(pd.ps[j].smoothLine==0){
					pDC->Polyline(&pp[si],pd.ll[j]);
				}
			}
			else{
				if(pd.ll[j]==2){
					pDC->Polyline(&pp[si],pd.ll[j]);
				}

				//if(ll[j]==1){

			}

			pDC->SelectObject(pOldPen);
			pen.DeleteObject();

		}


		if(pd.ps[j].dotSize==1){
			for(size_t i=0;i<pd.ll[j];i++){
				pDC->SetPixelV(pointlist[i+si],pd.ps[j].colour);	
			}
			//si+=ll[j];
		}
		if(pd.ps[j].dotSize>1){

			CSize ppoc=CSize(pd.ps[j].dotSize-1,pd.ps[j].dotSize-1);
			CRect prect(0,0,1,1);
			prect.InflateRect(ppoc);

			oc=pDC->GetBkColor();
			for(size_t i=0;i<pd.ll[j];i++){
				prect.MoveToXY(pointlist[i+si]-ppoc);
				//drawRectangle(prect,pDC,pd.ps[j].colour,pd.ps[j].colour);
				pDC->FillSolidRect(&prect,pd.ps[j].colour);
			}
			//si+=ll[j];
			pDC->SetBkColor(oc);

		}

		si+=pd.ll[j];

	}

	if(pd.ps.back().traceLast){
		CRect prect(0,0,1,1);
		CSize ppoc=CSize(4,4);
		prect.InflateRect(ppoc);

		oc=pDC->GetBkColor();
		prect.MoveToXY(pointlist.back()-ppoc);
		//drawRectangle(prect,pDC,pd.ps.back().colour,pd.ps.back().colour);
		pDC->FillSolidRect(&prect,pd.ps.back().colour);
		pDC->SetBkColor(oc);
	}

	//////////////////////////////fast///////////////////////////
	//genPointToPlot(xll,yll,rect,pointlist);
	//pen.CreatePen(PS_SOLID,1,clist[0]);
	//pOldPen=pDC->SelectObject(&pen);
	//pDC->PolyPolyline(pointlist.data(),ll.data(),ll.size());
	////dc.PolyDraw(pointlist.data(),styl.data(),pointlist.size());
	//pDC->SelectObject(pOldPen);
	//pen.DeleteObject();
	//////////////////////////////////////////////////////////////////


}

//not use
//void dlg1::DrawCurveB(CRect rect, CDC* pDC)
//{
//	std::vector<CPoint> pointlist;
//	CPen pen;
//	CPen * pOldPen;
//
//
//	//genPointToPlot(pd.xll,pd.yll,rect,pointlist);
//	size_t si=0;
//
//
//	for(size_t j=0;j<pd.ps.size();j++){
//
//		genPointToPlot(pd.xlist[j],pd.ylist[j],rect,pointlist);
//		CPoint *pp=pointlist.data();
//
//		//if(pd.ps[j].showLine){
//		if(pd.ps[j].lineType>=0){
//			//pen.CreatePen(PS_SOLID,1,pd.ps[j].colour);
//			pen.CreatePen(pd.ps[j].lineType,1,pd.ps[j].colour);
//			pOldPen=pDC->SelectObject(&pen);
//
//			if(pointlist.size()>2){
//				if(pd.ps[j].smoothLine==1){
//					DrawSpline(pp,pointlist.size(),rect,pDC);
//				}
//				if(pd.ps[j].smoothLine==2){
//					pDC->PolyBezier(pp,pointlist.size());
//				}
//				if(pd.ps[j].smoothLine==0){
//					pDC->Polyline(pp,pointlist.size());
//				}
//			}
//			else{
//				if(pointlist.size()==2){
//					pDC->Polyline(pp,pointlist.size());
//				}
//
//			}
//
//			pDC->SelectObject(pOldPen);
//			pen.DeleteObject();
//
//		}
//
//
//		if(pd.ps[j].dotSize==0){
//			for(size_t i=0;i<pointlist.size();i++){
//				pDC->SetPixelV(pointlist[i],pd.ps[j].colour);	
//			}
//			//si+=ll[j];
//		}
//		if(pd.ps[j].dotSize>0){
//			CRect prect(0,0,1,1);
//			CSize ppoc=CSize(pd.ps[j].dotSize,pd.ps[j].dotSize);
//			prect.InflateRect(ppoc);
//			for(size_t i=0;i<pointlist.size();i++){
//				prect.MoveToXY(pointlist[i]-ppoc);
//				drawRectangle(prect,pDC,pd.ps[j].colour,pd.ps[j].colour);
//			}
//			//si+=ll[j];
//		}
//
//		//si+=pd.ll[j];
//
//	}
//
//	//if(pd.ps.back().traceLast){
//	//	CRect prect(0,0,1,1);
//	//	CSize ppoc=CSize(4,4);
//	//	prect.InflateRect(ppoc);			
//	//	prect.MoveToXY(pointlist.back()-ppoc);
//	//	drawRectangle(prect,pDC,pd.ps.back().colour,pd.ps.back().colour);
//	//}
//
//	//////////////////////////////fast///////////////////////////
//	//genPointToPlot(xll,yll,rect,pointlist);
//	//pen.CreatePen(PS_SOLID,1,clist[0]);
//	//pOldPen=pDC->SelectObject(&pen);
//	//pDC->PolyPolyline(pointlist.data(),ll.data(),ll.size());
//	////dc.PolyDraw(pointlist.data(),styl.data(),pointlist.size());
//	//pDC->SelectObject(pOldPen);
//	//pen.DeleteObject();
//	//////////////////////////////////////////////////////////////////
//
//
//}




void dlg1::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class

	////plot region
	//CButton * pPlot=new CButton;
	//CRect plotrect(0,0,1,1);
	//CString str;
	//str.LoadStringW(IDC_PLOT);
	//pPlot->Create( str, WS_CHILD|BS_GROUPBOX|WS_VISIBLE, plotrect, this, IDC_PLOT); 


}


void dlg1::GetPlotRect(CRect & plotRect)
{
	this->GetClientRect(&plotRect);
	//plotRect.bottom=plotRect.CenterPoint().y;

	int gap0=10;
	int gap=gap0;
	if(fs.labelSize>=0){
		gap+=fs.labelSize;
	}
	if(fs.metricSize>=0){
		gap+=metricGridLong+fs.metricSize;
	}
	plotRect.DeflateRect(gap,gap0,gap0,gap);
}


void dlg1::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	switch(nChar){
	case 'f':
		updatePlotRange();
		Invalidate();
		break;
	default:
		break;
	}


	CFormView::OnChar(nChar, nRepCnt, nFlags);
}


BOOL dlg1::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
	return CFormView::OnEraseBkgnd(pDC);
}


//void dlg1::showall(void)
//{
//	this->updatePlotRange(pd.xll,pd.yll,true);
//	Invalidate();
//}

void dlg1::DrawSpline( CPoint *lpPoints, int np, CRect rect, CDC * pDC)
{

	std::vector<double> x(np);
	std::vector<double> y(np);

	for(int i=0;i<np;i++){
		x[i]=lpPoints[i].x;
		y[i]=lpPoints[i].y;
	}

	//std::vector<int> xx;
	//for(int j=x.front()+1;j<x.back();j++){
	//	xx.push_back(j);
	//}
	//std::vector<int> yy(xx.size());
	//splintA(x,y,xx,yy);

	std::vector<CPoint> pr;
	int j=x.front()+1;
	if(j<rect.left+1)
		j=rect.left+1;

	for(;j<x.back() && j<rect.right;j++){
		pr.push_back(CPoint(j,0));
	}
	splintA(x,y,pr);


	pDC->Polyline(pr.data(),pr.size());
	//for(size_t k=0;k<pr.size();k++){
	//	pDC->SetPixelV(pr[k],pDC->GetDCPenColor());
	//}

}


//void dlg1::plot2dfollow(const std::vector<double> & x, const std::vector<double> & y)
//{
//	pd.AddFollow(x,y);
//	updatePlotRange(x,y,false);
//	Invalidate();
//}


void dlg1::smoothLine(void)
{
	//pd.ps.back().showLine=true;
	pd.ps.back().lineType=0;
	pd.ps.back().smoothLine=1;
}


void dlg1::clear(void)
{
	xmin=xmax=ymin=ymax=0;
	pd.clear();
}





CRect dlg1::DrawXYAxis1(CRect rect, CDC * pdc)
{
	CRect newrect=rect;
	double XMAX,XMIN,YMAX,YMIN;
	XMAX=xmax;
	XMIN=xmin;
	YMAX=ymax;
	YMIN=ymin;

	drawRectangle(rect,pdc,fs.bkgndC,fs.borderC);

	std::vector<double> gridx;
	calgridVT(XMAX,XMIN,gridx);
	std::vector<long> gridV(gridx.size());
	xRescaleVT(gridx.data(),gridx.size(),XMIN,XMAX,gridV.data(),rect.left,rect.right);
	std::vector<double> gridy;
	calgridVT(YMAX,YMIN,gridy);
	std::vector<long> gridH(gridy.size());
	xRescaleVT(gridy.data(),gridy.size(),YMIN,YMAX,gridH.data(),rect.bottom,rect.top);

	DrawGridLine(rect,pdc,fs.gridType,fs.gridC,gridH,gridV);

	newrect=DrawMetric(rect,pdc,fs.metricSize,fs.metricC,fs.gridC,gridH,gridV,gridx,gridy);

	newrect=DrawLabel(newrect,pdc,rect.CenterPoint(),fs.labelSize,fs.labelC);

	return newrect;

}


void dlg1::DrawGridLine(const CRect & rect, CDC * pdc, int gridType, COLORREF gridC, const std::vector<long> & gridH, const std::vector<long> & gridV)
{
	int lineWidth=1;
	//draw xy grid line
	if(gridType>=0 && gridType<5){

		CPen pen;
		CPen * pOldPen;

		pen.CreatePen(gridType, lineWidth, gridC);
		pOldPen=pdc->SelectObject(&pen);

		std::vector<DWORD> npo(gridV.size()+gridH.size(),2);
		std::vector<CPoint> gridline(npo.size()*2);
		for(size_t i=0;i<gridV.size();i++){
			gridline[i*2].x=gridV[i];
			gridline[i*2].y=rect.bottom;
			gridline[i*2+1].x=gridV[i];
			gridline[i*2+1].y=rect.top;
		}
		for(size_t i=0;i<gridH.size();i++){
			gridline[(i+gridV.size())*2].x=rect.left;
			gridline[(i+gridV.size())*2].y=gridH[i];
			gridline[(i+gridV.size())*2+1].x=rect.right;
			gridline[(i+gridV.size())*2+1].y=gridH[i];
		}	
		pdc->PolyPolyline(gridline.data(),npo.data(),npo.size());
		pdc->SelectObject(pOldPen);
		pen.DeleteObject();
	}


}


CRect dlg1::DrawMetric(const CRect & rect,
	CDC * pdc,
	int metricSize,
	COLORREF metricC,
	COLORREF gridC,
	const std::vector<long> & gridH,
	const std::vector<long> & gridV,
	const std::vector<double> & gridx,
	const std::vector<double> & gridy)
{
	int lineWidth=1;
	CRect newrect=rect;
	int metricH=metricSize;
	int lc=metricGridLong;
	int lcs=metricGridShort;

	if(fs.metricSize>0){

		CFont font;
		CFont *pOldFont;
		CString str;
		CPen pen;
		CPen * pOldPen;
		CSize sz;

		CPoint textLocate;
		COLORREF oc;
		CString fontName=L"Arial";



		pen.CreatePen(PS_SOLID, lineWidth, gridC);
		pOldPen=pdc->SelectObject(&pen);
		oc=pdc->SetTextColor(fs.metricC);


		//draw x metric
		CRect xmrect(rect.left,rect.bottom+lc,rect.left,rect.bottom+lc);

		font.CreateFont(
			metricH,                        // nHeight
			0,                         // nWidth
			0,                         // nEscapement
			0,                         // nOrientation
			FW_NORMAL,                 // nWeight
			FALSE,                     // bItalic
			FALSE,                     // bUnderline
			0,                         // cStrikeOut
			ANSI_CHARSET,              // nCharSet
			OUT_DEFAULT_PRECIS,        // nOutPrecision
			CLIP_DEFAULT_PRECIS,       // nClipPrecision
			DEFAULT_QUALITY,           // nQuality
			DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
			fontName);                 // lpszFacename
		pOldFont=pdc->SelectObject(&font);

		textLocate.y=rect.bottom+lc;
		for(size_t i=0;i<gridV.size();i++){

			pdc->MoveTo(gridV[i],rect.bottom);
			pdc->LineTo(gridV[i],rect.bottom+lcs);

			str.Format(L"%g",gridx[i]);
			sz=pdc->GetTextExtent(str);
			textLocate.x=gridV[i]-sz.cx/2;

			if(xmrect.right<textLocate.x &&
				textLocate.x+sz.cx<rect.right){
					pdc->TextOutW(textLocate.x,textLocate.y,str);
					xmrect.right=textLocate.x+sz.cx;
					if(sz.cy>xmrect.Height()){
						xmrect.bottom=textLocate.y+sz.cy;
					}
					pdc->MoveTo(gridV[i],rect.bottom);
					pdc->LineTo(gridV[i],rect.bottom+lc);
			}					

		}
		newrect.bottom=xmrect.bottom;

		pdc->SelectObject(pOldFont);
		font.DeleteObject();
		////////////////////////////////////////////////////////////////////////////
		//draw y metric
		CRect ymrect(rect.left-lc,rect.bottom,rect.left-lc,rect.bottom);

		font.CreateFont(
			metricH,                        // nHeight
			0,                         // nWidth
			900,                         // nEscapement
			0,                         // nOrientation
			FW_NORMAL,                 // nWeight
			FALSE,                     // bItalic
			FALSE,                     // bUnderline
			0,                         // cStrikeOut
			ANSI_CHARSET,              // nCharSet
			OUT_DEFAULT_PRECIS,        // nOutPrecision
			CLIP_DEFAULT_PRECIS,       // nClipPrecision
			DEFAULT_QUALITY,           // nQuality
			DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
			fontName);                 // lpszFacename
		pOldFont=pdc->SelectObject(&font);


		for(size_t i=0;i<gridH.size();i++){
			pdc->MoveTo(rect.left,gridH[i]);
			pdc->LineTo(rect.left-lcs,gridH[i]);

			str.Format(L"%g",gridy[i]);
			sz=pdc->GetTextExtent(str);

			textLocate.x=rect.left-lc-sz.cy;
			textLocate.y=gridH[i]+sz.cx/2;
			if(ymrect.top>textLocate.y &&
				textLocate.y-sz.cx>rect.top){
					pdc->TextOutW(textLocate.x,textLocate.y,str);
					if(ymrect.left>textLocate.x){
						ymrect.left=textLocate.x;
					}
					ymrect.top=textLocate.y-sz.cx;

					pdc->MoveTo(rect.left,gridH[i]);
					pdc->LineTo(rect.left-lc,gridH[i]);

			}


		}
		newrect.left=ymrect.left;
		pdc->SelectObject(pOldFont);
		font.DeleteObject();
		//////////////////////////////////////////////



		pdc->SetTextColor(oc);
		pdc->SelectObject(pOldPen);
		pen.DeleteObject();


	}


	return newrect;

}


CRect dlg1::DrawLabel(const CRect & rect, CDC * pdc, CPoint centerP, int labelSize, COLORREF labelC)
{


	int labelH=labelSize;
	CRect newrect=rect;

	if(labelSize>0){

		CFont font;
		CFont *pOldFont;
		CString str;

		COLORREF oc;
		CSize sz;
		CString fontName=L"Arial";


		oc=pdc->SetTextColor(labelC);

		//draw x axis label
		font.CreateFont(
			labelH,                        // nHeight
			0,                         // nWidth
			0,                         // nEscapement
			0,                         // nOrientation
			FW_NORMAL,                 // nWeight
			FALSE,                     // bItalic
			FALSE,                     // bUnderline
			0,                         // cStrikeOut
			ANSI_CHARSET,              // nCharSet
			OUT_DEFAULT_PRECIS,        // nOutPrecision
			CLIP_DEFAULT_PRECIS,       // nClipPrecision
			DEFAULT_QUALITY,           // nQuality
			DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
			fontName);                 // lpszFacename
		//font.CreatePointFont(200,L"MS Gothic",NULL);
		pOldFont=pdc->SelectObject(&font);

		str.Format(pd.xlabel);
		sz=pdc->GetTextExtent(str);
		pdc->TextOutW(centerP.x-(sz.cx/2),newrect.bottom,str);
		newrect.bottom+=sz.cy;

		pdc->SelectObject(pOldFont);
		font.DeleteObject();


		//draw y axis label
		font.CreateFont(
			labelH,                        // nHeight
			0,                         // nWidth
			900,                         // nEscapement
			0,                         // nOrientation
			FW_NORMAL,                 // nWeight
			FALSE,                     // bItalic
			FALSE,                     // bUnderline
			0,                         // cStrikeOut
			ANSI_CHARSET,              // nCharSet
			OUT_DEFAULT_PRECIS,        // nOutPrecision
			CLIP_DEFAULT_PRECIS,       // nClipPrecision
			DEFAULT_QUALITY,           // nQuality
			DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
			fontName);                 // lpszFacename
		pOldFont=pdc->SelectObject(&font);

		str.Format(pd.ylabel);
		sz=pdc->GetTextExtent(str);
		pdc->TextOutW(newrect.left-sz.cy-1,centerP.y+(sz.cx/2),str);
		newrect.left-=sz.cy+1;

		pdc->SelectObject(pOldFont);
		font.DeleteObject();

		pdc->SetTextColor(oc);
	}

	return newrect;
}
