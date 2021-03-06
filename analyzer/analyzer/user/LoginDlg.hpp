#pragma once

#include "AccountComboBox.hpp"
#include "../Resource.h"

#include "../messagemaphpp.h"

// LoginDlg dialog
///
/// \brief The LoginDlg class
///登陆对话框

class LoginDlg : public CDialogEx
{
	//DECLARE_DYNAMIC(LoginDlg)
public:
    ///
    /// \brief epass
    ///密码输入框
	CEdit epass;
    ///
    /// \brief suser
    ///显示用户名
	CStatic suser;
    ///
    /// \brief spass
    ///显示密码
	CStatic spass;
    ///
    /// \brief sbmp
    ///显示图片
    /// 暂时不用
	CStatic sbmp;

    ///
    /// \brief m_usr
    ///用户名输入框
	AccountComboBox m_usr;

    ///
    /// \brief m_login
    ///登录按钮
	CMFCButton m_login;
    /// 暂时不用
	CMFCButton m_cancel;
    /// 暂时不用
	CImageList m_ImageList;

	AccountList al;
	int usridx;
public:
	LoginDlg(CWnd* pParent = NULL)	
		: CDialogEx(LoginDlg::IDD, pParent)
		, usridx(0)
	{

	};   // standard constructor
	virtual ~LoginDlg(){};

	// Dialog Data
	enum { IDD = IDD_DIALOG_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX)
	{
		CDialogEx::DoDataExchange(pDX);
	};    // DDX/DDV support

    //布局窗口
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if (CDialogEx::OnCreate(lpCreateStruct) == -1)
			return -1;

		// TODO:  Add your specialized creation code here

		CSize gap1(20,10);
		CSize gap2(10,10);

		CRect winrect;
		this->GetClientRect(&winrect);
		winrect.DeflateRect(gap1);

		int h=25,hs=10;
		CSize esz( winrect.Width(), h );
		CSize bsz( (winrect.Width()-gap2.cx)/2, h );
		CPoint pt(gap1);
		CString str;

		pt.y+=gap2.cy;

		str.LoadStringW(IDS_EDIT_USERNAME);
		if( suser.Create(
			str,
			WS_CHILD
			|SS_LEFT
			|SS_SIMPLE   
			|WS_VISIBLE, 
			CRect(pt,esz),
			this,
			IDS_EDIT_USERNAME)==0
			){
				return -1;
		}

		pt.y+=esz.cy-hs;

		m_usr.al=al;

		if(m_usr.CreateEx(NULL,
			CBS_DROPDOWN
			|CBS_AUTOHSCROLL
			|WS_CHILD
			|WS_VISIBLE,
			CRect(pt,CSize(esz.cx,100)),
			this,
			IDS_EDIT_USERNAME
			)==FALSE){
				return -1;
		}

		pt.y+=esz.cy;

		str.LoadStringW(IDS_EDIT_PASSWORD);

		if( spass.Create(
			str,
			WS_CHILD
			|SS_LEFT
			|WS_VISIBLE, 
			CRect(pt,esz),
			this,
			IDS_EDIT_PASSWORD)
			==0 ){
				return -1;
		}
		pt.y+=esz.cy-hs;

		if( epass.CreateEx(
			WS_EX_CLIENTEDGE,
			L"Edit", 
			L"a",
			ES_LEFT
			|WS_CHILD
			|WS_TABSTOP
			|ES_PASSWORD
			|WS_VISIBLE,
			CRect(pt,esz),
			this,
			IDS_EDIT_PASSWORD)
			==0 )
			return -1;


		pt.y+=esz.cy+gap2.cy*2;

		pt.x+=(winrect.Width()-bsz.cx)/2;

		str.LoadStringW(IDS_STRING_LOGIN);
		if(m_login.Create(str, WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, CRect(pt,bsz), this, IDOK)==FALSE){
			return -1;
		}

		//pt.x+=bsz.cx+gap2.cx;
		//str.LoadStringW(IDS_STRING_CANCEL);
		//if(m_cancel.Create(str, WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, CRect(pt,bsz), this, IDCANCEL)==FALSE){
		//	return -1;
		//}

		this->SetWindowTextW(str);

		return 0;
	};



    ///
    /// \brief OnOK
    ///验证并登录
	virtual void OnOK()
	{
		// TODO: Add your specialized code here and/or call the base class

		//UpdateData(TRUE); // 获取输入数据

		CString m_strUser,m_strPwd;

		//euser.GetWindowTextW(m_strUser);

		m_usr.GetWindowTextW(m_strUser);
		epass.GetWindowTextW(m_strPwd);

		usridx=al.CheckUserAccount(m_strUser,m_strPwd);
		if(usridx>=0){

			//if(m_strUser==L"Admin"&&m_strPwd==L"1234"){

			CDialogEx::OnOK(); // 假如用户名和密码正确，就关闭对话框

		}
		else{

			CString title,str;
			title.LoadStringW(IDS_STRING_ERROR);

			if(usridx==-1){
				//AfxMessageBox(L"password error");

				str.LoadStringW(IDS_STRING_WRONG_PASSWORD);
				epass.ShowBalloonTip(title,str);
				epass.SetSel(0,-1);
			}
			else{
				//AfxMessageBox(L"username error");

				//m_usr.GetComboBoxCtrl()->SetCueBanner(L"username error");


				str.LoadStringW(IDS_STRING_INVALID_USERNAME);

				m_usr.GetEditCtrl()->ShowBalloonTip(title,str);
				m_usr.GetEditCtrl()->SetSel(0,-1);
				//m_usr.Invalidate();
			}
		}

		///*假如用户名或密码错误，且还未超出登陆次数，就进行提示*/
		//
		//if((m_strUser!=L"Admin"||m_strPwd!=L"1234")&&(m_Time<3)) //假如密码和用户名正确
		//
		//   {
		//
		//AfxMessageBox("用户名或密码不正确");
		//
		//m_Time++;
		//
		//   }
		//
		///*假如超出登陆次数，提示并退出系统*/
		//
		//if(m_Time>2)
		//
		//   {
		//
		//AfxMessageBox("登陆错误次数超过3次");
		//
		//PostQuitMessage(0);
		//
		//   }




		//CDialogEx::OnOK();
	};



	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
	{
		HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

		// TODO:  Change any attributes of the DC here

		// TODO:  Return a different brush if the default is not desired

		if(nCtlColor == CTLCOLOR_STATIC )
		{
			pDC->SetBkMode(TRANSPARENT);//设置背景透明
            pDC->SetTextColor(RGB(0,0,0));//设置字体为黑色
			return (HBRUSH)::GetStockObject(NULL_BRUSH);
		}
		return hbr;
	};


//	DECLARE_MESSAGE_MAP()
//
//
//};
//
//BEGIN_MESSAGE_MAP(LoginDlg, CDialogEx)
//	ON_WM_CREATE()
//	ON_WM_CTLCOLOR()
//END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP_HPP(LoginDlg, CDialogEx)
	ON_WM_CREATE()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP_HPP()

};
