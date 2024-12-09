
// SnakegameDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "Snakegame.h"
#include "SnakegameDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "SnakeGameFun.h"


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
public:
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonStart();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

void CAboutDlg::OnPaint()
{
}

void CAboutDlg::OnBnClickedButtonStart()
{
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_WM_PAINT()
	
END_MESSAGE_MAP()

void CSnakegameDlg::restartGame()
{
	m_gameScreen.resetGame(); // GameScreen의 resetGame 호출
	m_gameScreen.startGame(); // 게임 시작
}

// CSnakegameDlg 대화 상자



CSnakegameDlg::CSnakegameDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SNAKEGAME_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSnakegameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSnakegameDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_STN_CLICKED(IDC_SCREEN, &CSnakegameDlg::OnStnClickedScreen)
	ON_BN_CLICKED(IDOK, &CSnakegameDlg::OnBnClickedOk)
	ON_STN_CLICKED(IDC_STATIC_GAME, &CSnakegameDlg::OnStnClickedStaticGame)
	ON_BN_CLICKED(IDC_BUTTON_START, &CSnakegameDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON2, &CSnakegameDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CSnakegameDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, &CSnakegameDlg::OnBnClickedButtonExit)
END_MESSAGE_MAP()

void CSnakegameDlg::OnBnClickedButton2()
{
	if (MessageBox(_T("게임을 다시 시작하시겠습니까?"), _T("재시작"), MB_YESNO | MB_ICONQUESTION) == IDYES)
	{
		restartGame();
	}
}

// CSnakegameDlg 메시지 처리기

BOOL CSnakegameDlg::OnInitDialog()
{

	CDialogEx::OnInitDialog();

	// GameScreen 초기화
	CRect rect;
	GetDlgItem(IDC_STATIC_GAME)->GetWindowRect(&rect);
	ScreenToClient(&rect);

	if (!m_gameScreen.Create(NULL, WS_CHILD | WS_VISIBLE, rect, this, IDC_STATIC_GAME))
	{
		MessageBox(_T("GameScreen 초기화 실패"), _T("오류"), MB_ICONERROR);
		return FALSE;
	}

	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.



	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.
	/*
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


	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	randseed();
	CWnd* pWnd = GetDlgItem(IDC_SCREEN);
	pWnd->ShowWindow(SW_HIDE);
	
	pWnd->GetWindowRect(rect); // 화면(screen) 좌표계
	ScreenToClient(rect); // 화면 좌표계 -> 클라이언트 좌표계

	// 다이얼로그 단위(dialogunit) -> 화면 단위(픽셀)
	// 디버그로 확인: F5로 실행 시작, F10으로 한줄씩 실행
	 CRect rtTest(0, 0, 1000, 1000); // (lt), (rb)
	 MapDialogRect(rtTest); // (1000, 1000) -> (1750, 1750)

	m_screen.Create(NULL, WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | 
					WS_CLIPSIBLINGS, rect, this);
	m_screen.SetTimer(SCREEN_TIMER, 1000/FPS, NULL);


	CDialogEx::OnInitDialog();
	*/


}

void CSnakegameDlg::OnBnClickedButtonStart()
{
	m_gameScreen.startGame(); // 게임 시작
}

void CSnakegameDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSnakegameDlg::OnPaint()
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
HCURSOR CSnakegameDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSnakegameDlg::OnStnClickedScreen()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CSnakegameDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}


BOOL CSnakegameDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_LEFT)
		{
			m_gameScreen.setSnakeDir(SnakeDir::LEFT); // 수정된 코드
			return TRUE;
		}
		else if (pMsg->wParam == VK_RIGHT)
		{
			m_gameScreen.setSnakeDir(SnakeDir::RIGHT); // 수정된 코드
			return TRUE;
		}
		else if (pMsg->wParam == VK_UP)
		{
			m_gameScreen.setSnakeDir(SnakeDir::UP); // 수정된 코드
			return TRUE;
		}
		else if (pMsg->wParam == VK_DOWN)
		{
			m_gameScreen.setSnakeDir(SnakeDir::DOWN); // 수정된 코드
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}







void CSnakegameDlg::OnStnClickedStaticGame()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}



void CSnakegameDlg::OnBnClickedButton3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (MessageBox(_T("게임을 종료하시겠습니까?"), _T("종료"), MB_YESNO | MB_ICONQUESTION) == IDYES)
	{
		CDialogEx::OnOK(); // 다이얼로그 종료
	}
}


void CSnakegameDlg::OnBnClickedButtonExit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
