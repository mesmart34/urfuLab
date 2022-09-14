
// FGctrlDlg.h : header file
//

#include <afxsock.h>

#pragma once


// CFGctrlDlg dialog
class CFGctrlDlg : public CDialogEx
{
// Construction
public:
	CFGctrlDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FGCTRL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void connect_to_FG();
	CSliderCtrl TangageSlider;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	void UpdateControls();
	afx_msg void disconnect_from_FG();
	CSliderCtrl KrenSlider;
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void CheckSystem();
	CSliderCtrl ThrottleSlider;
	afx_msg void take_off();
	afx_msg void TestRecieve();
	CEdit RecievedData;
	CEdit KrenData;
	CEdit TangageData;
	CEdit TrottleData;

	void TakeOffFunc();
};
