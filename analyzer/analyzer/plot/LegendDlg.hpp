#pragma once
#include "PlotWnd.h"

// LegendDlg dialog



class LegendDlg : public CDialogEx
{


protected:
	PlotWnd * ppw;
private:
	bool bInitComplete;
public:
	LegendDlg(PlotWnd * pParent)// standard constructor
		: ppw(pParent)
		, bInitComplete(false)
	{};

	virtual ~LegendDlg(){};

	// Dialog Data
	//enum { IDD = IDD_DIALOG4 };

protected:

	afx_msg LRESULT OnNcHitTest(CPoint point)
	{
		// TODO: Add your message handler code here and/or call default

		return ppw->pdex->lgs.bDock ? CDialogEx::OnNcHitTest(point) : HTCAPTION;
	};

	afx_msg void OnPaint()
	{
		CPaintDC dc(this); // device context for painting
		// TODO: Add your message handler code here
		// Do not call CDialogEx::OnPaint() for painting messages


		CRect rc;
		this->GetClientRect(&rc);	
		std::vector<LineSpec> ls(ppw->pdex->pd.ls);
		LegendSpec lgs=ppw->pdex->lgs;
		dc.FillSolidRect(&rc,lgs.bkColor);
		lgs.Draw(&dc,ls);

	};

	afx_msg void OnMove(int x, int y)
	{
		CDialogEx::OnMove(x, y);

		// TODO: Add your message handler code here

		if(bInitComplete)
			ppw->GetLegendPos(CPoint());
	};

	virtual BOOL OnInitDialog()
	{
		CDialogEx::OnInitDialog();

		// TODO:  Add extra initialization here
		CRect legendrect;
		bool flg=ppw->GetLegendRect(legendrect);
		this->MoveWindow(&legendrect);
		bInitComplete=true;
		return TRUE;  // return TRUE unless you set the focus to a control
		// EXCEPTION: OCX Property Pages should return FALSE
	};

	virtual void DoDataExchange(CDataExchange* pDX)
	{
		CDialogEx::DoDataExchange(pDX);
	};    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	//PTM_WARNING_DISABLE 

	//virtual const AFX_MSGMAP* GetMessageMap() const;

//	const AFX_MSGMAP* PASCAL LegendDlg::GetThisMessageMap() \
//	{ \
//		typedef LegendDlg ThisClass;						   \
//		typedef CDialogEx TheBaseClass;					   \
//		static const AFX_MSGMAP_ENTRY _messageEntries[] =  \
//		{
//
//			ON_WM_NCHITTEST()
//ON_WM_PAINT()
//ON_WM_MOVE()
//		{0, 0, 0, 0, AfxSig_end, (AFX_PMSG)0 } \
//	}; \
//		static const AFX_MSGMAP messageMap = \
//		{ &TheBaseClass::GetThisMessageMap, &_messageEntries[0] }; \
//		return &messageMap; \
//	};								  
//


	//PTM_WARNING_RESTORE
};


		//const AFX_MSGMAP* LegendDlg::GetMessageMap() const \
		//{ return GetThisMessageMap(); } ;
//IMPLEMENT_DYNCREATE(LegendDlg, CDialogEx)
BEGIN_MESSAGE_MAP(LegendDlg, CDialogEx) 
ON_WM_NCHITTEST()
ON_WM_PAINT()
ON_WM_MOVE()
END_MESSAGE_MAP() 