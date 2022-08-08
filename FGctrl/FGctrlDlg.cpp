
// FGctrlDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "FGctrl.h"
#include "FGctrlDlg.h"
#include "afxdialogex.h"

#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>

#pragma warning(disable: 4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CFGctrlDlg dialog



CFGctrlDlg::CFGctrlDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FGCTRL_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFGctrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER1, TangageSlider);
	DDX_Control(pDX, IDC_SLIDER2, KrenSlider);
	DDX_Control(pDX, IDC_SLIDER3, ThrottleSlider);
}

BEGIN_MESSAGE_MAP(CFGctrlDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CFGctrlDlg::connect_to_FG)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON2, &CFGctrlDlg::disconnect_from_FG)
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_BUTTON3, &CFGctrlDlg::CheckSystem)
	ON_BN_CLICKED(IDC_BUTTON4, &CFGctrlDlg::take_off)
END_MESSAGE_MAP()


// CFGctrlDlg message handlers

BOOL CFGctrlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);

	if (WSAStartup(DLLVersion, &wsaData) != 0) AfxMessageBox(_T("Ошибка библиотеки"), MB_ICONINFORMATION);

	int Positions = 100;
	TangageSlider.SetRange(-Positions, Positions, 1);
	TangageSlider.SetPos(0);

	KrenSlider.SetRange(-Positions, Positions, 1);
	KrenSlider.SetPos(0);

	ThrottleSlider.SetRange(0, Positions, 1);
	ThrottleSlider.SetPos(Positions);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFGctrlDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFGctrlDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFGctrlDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

SOCKET Connection;
SOCKADDR_IN addr;


void CFGctrlDlg::connect_to_FG()
{
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(5401);
	addr.sin_family = AF_INET;

	Connection = socket(AF_INET, SOCK_DGRAM, NULL);
	if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) AfxMessageBox(_T("Ошибка подключения"), MB_ICONINFORMATION);
	else AfxMessageBox(_T("Подключено!"), MB_ICONINFORMATION);
}


void CFGctrlDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CSliderCtrl* pSlider = reinterpret_cast<CSliderCtrl*>(pScrollBar);

	if (pSlider == &KrenSlider) UpdateControls();

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CFGctrlDlg::UpdateControls()
{
	float valueTangage = double(TangageSlider.GetPos()) / 100;
	float valueKren = double(KrenSlider.GetPos()) / 100;
	float valueThrottle = double(ThrottleSlider.GetPos()) / 100 - 1.0;
	char msg[256];

	sprintf(msg, "%.2f;%.2f;%.2f\n", -1.0 * valueKren, valueTangage, valueThrottle);
	send(Connection, msg, strlen(msg), NULL);
}


void CFGctrlDlg::disconnect_from_FG()
{
	/*sprintf(msg, "quit\r\n");
	send(Connection, msg, sizeof(msg), NULL);
	if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) AfxMessageBox(_T("Соединение разорвано"), MB_ICONINFORMATION);*/
}


void CFGctrlDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CSliderCtrl* pSlider = reinterpret_cast<CSliderCtrl*>(pScrollBar);

	if (pSlider == &TangageSlider || pSlider == &ThrottleSlider) UpdateControls();

	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CFGctrlDlg::CheckSystem()
{
	while (1)
	{
		for (double x = -1.0; x <= 1.0; x += 0.01)
		{
			KrenSlider.SetPos(int(x * 100));
			TangageSlider.SetPos(int(x * 100));
			if (x >= 0)	ThrottleSlider.SetPos(int(x * 100));
			UpdateControls();
			Sleep(25);
		}

		for (double x = 1.0; x >= -1.0; x -= 0.01)
		{
			KrenSlider.SetPos(int(x * 100));
			TangageSlider.SetPos(int(x * 100));
			if (x >= 0)	ThrottleSlider.SetPos(int(x * 100));
			UpdateControls();
			Sleep(25);
		}
	}
}


void CFGctrlDlg::take_off()
{
	ThrottleSlider.SetPos(100 - 20);

	for (double x = ThrottleSlider.GetPos(); x >= 30; x -= 1)
	{
		ThrottleSlider.SetPos(x);
		UpdateControls();
		Sleep(25);
	}

	for (double x = ThrottleSlider.GetPos(); x >= ThrottleSlider.GetRangeMin(); x -= 1)
	{
		ThrottleSlider.SetPos(x);
		UpdateControls();
		Sleep(1000);
	}

	for (double x = TangageSlider.GetPos(); x < 100 / 2; x += 1)
	{
		TangageSlider.SetPos(x);
		UpdateControls();
		Sleep(250);
	}

	for (double x = TangageSlider.GetPos(); x > 0; x -= 1)
	{
		TangageSlider.SetPos(x);
		UpdateControls();
		Sleep(250);
	}
}
