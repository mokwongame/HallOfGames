
// MyCarRaceDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "MyCarRace.h"
#include "MyCarRaceDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	// 구현입니다.
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


// CMyCarRaceDlg 대화 상자



CMyCarRaceDlg::CMyCarRaceDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MYCARRACE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyCarRaceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMyCarRaceDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_START, &CMyCarRaceDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_STAT, &CMyCarRaceDlg::OnBnClickedBtnStat)
	ON_BN_CLICKED(IDC_BTN_EXIT, &CMyCarRaceDlg::OnBnClickedBtnExit)
	ON_STN_CLICKED(IDC_LOGO, &CMyCarRaceDlg::OnStnClickedLogo)
END_MESSAGE_MAP()


// CMyCarRaceDlg 메시지 처리기

BOOL CMyCarRaceDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	m_screen.Create(IDC_SCREEN, this);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMyCarRaceDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMyCarRaceDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMyCarRaceDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


BOOL CMyCarRaceDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_LEFT || pMsg->wParam == 'A')
		{
			m_screen.m_player.MoveLeft();
			m_screen.Invalidate(FALSE);
			return TRUE;
		}
		else if (pMsg->wParam == VK_RIGHT || pMsg->wParam == 'D')
		{
			m_screen.m_player.MoveRight();
			m_screen.Invalidate(FALSE);
			return TRUE;
		}
		else if (pMsg->wParam == VK_UP || pMsg->wParam == 'W')
		{
			m_screen.m_road.IncSpeed();
			return TRUE;
		}
		else if (pMsg->wParam == VK_DOWN || pMsg->wParam == 'S')
		{
			m_screen.m_road.DecSpeed();
			return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CMyCarRaceDlg::OnStnClickedLogo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CMyCarRaceDlg::OnBnClickedBtnStart()
{
	CWnd* pLogo = GetDlgItem(IDC_LOGO);
	if (pLogo)
		pLogo->ShowWindow(SW_HIDE);

	// 2) 게임 리셋 (점수/체력/적/아이템 초기화)
	m_screen.ResetGame();

	GetDlgItem(IDC_BTN_START)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_STAT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_EXIT)->ShowWindow(SW_HIDE);
}

void CMyCarRaceDlg::OnBnClickedBtnStat()
{
	// m_score는 MyScreen 안에 있으니까, GetScore()로 가져오기
	double score = m_screen.GetScore();
	int timeSec = static_cast<int>(score);   // 1초당 1점이면 시간도 비슷

	CString msg;
	msg.Format(_T("점수: %.0f 점\n게임 시간: %d 초"), score, timeSec);

	AfxMessageBox(msg, MB_OK | MB_ICONINFORMATION);
}

void CMyCarRaceDlg::OnBnClickedBtnExit()
{
	EndDialog(IDOK);   // 또는 OnOK();
}
