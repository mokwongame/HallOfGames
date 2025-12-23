// UnityModelerDlg.cpp
// 메인 다이얼로그(탭 UI + OpenGL Screen) 관리
// - 좌측 리스트(GameObject 목록)와 탭(Game Object / Light) 다이얼로그를 연동
// - OpenGL 렌더링 화면은 MyScreen(m_screen)에서 담당

#include "pch.h"
#include "framework.h"
#include "UnityModeler.h"
#include "UnityModelerDlg.h"
#include "afxdialogex.h"

#include "CDlgGameObj.h"
#include "CDlgLight.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// About 다이얼로그
// 프로젝트 기능과 직접 관련은 없지만 MFC 기본 메뉴(About)를 유지하려면 필요
class CAboutDlg : public CDialogEx
{
public:
    CAboutDlg() : CDialogEx(IDD_ABOUTBOX) {}

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_ABOUTBOX };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX) override
    {
        CDialogEx::DoDataExchange(pDX);
    }
    DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

CUnityModelerDlg::CUnityModelerDlg(CWnd* pParent)
    : CDialogEx(IDD_UNITYMODELER_DIALOG, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CUnityModelerDlg::~CUnityModelerDlg()
{
    // 탭 다이얼로그는 new로 생성했으므로 해제 필요
    delete m_pDlgGameObj;
    m_pDlgGameObj = nullptr;

    delete m_pDlgLight;
    m_pDlgLight = nullptr;
}

void CUnityModelerDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);

    // 탭 컨트롤, 오브젝트 리스트 컨트롤 연결
    DDX_Control(pDX, IDC_TAB_OPT, m_tabCtrl);
    DDX_Control(pDX, IDC_LIST1, m_lbGameObj);
}

BEGIN_MESSAGE_MAP(CUnityModelerDlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()

    ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_OPT, &CUnityModelerDlg::OnSelchangeTabOpt)
    ON_LBN_SELCHANGE(IDC_LIST1, &CUnityModelerDlg::OnSelchangeListGameObj)
END_MESSAGE_MAP()

BOOL CUnityModelerDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // About 메뉴 추가(MFC 기본 기능)
    // 과제/프로젝트 요구에서 About이 필요 없으면 이 블록은 제거 가능
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu)
    {
        CString strAboutMenu;
        const BOOL bLoaded = strAboutMenu.LoadString(IDS_ABOUTBOX);
        ASSERT(bLoaded);

        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    // 창 아이콘 설정
    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);

    // OpenGL Screen(MyScreen) 생성: 리소스(IDC_SCREEN) 위치에 OpenGL 렌더 영역 생성
    m_screen.Create(IDC_SCREEN, this);

    // 우측 탭(Game Object / Light) 다이얼로그 구성
    SetTabCtrl();

    return TRUE;
}

void CUnityModelerDlg::SetTabCtrl()
{
    // 탭 추가
    m_tabCtrl.InsertItem(0, _T("Game Object"));
    m_tabCtrl.InsertItem(1, _T("Light"));
    m_tabCtrl.SetCurSel(0);

    // 탭 내부 클라이언트 영역 크기
    CRect rect;
    m_tabCtrl.GetClientRect(&rect);

    // Game Object 탭 다이얼로그
    m_pDlgGameObj = new CDlgGameObj;
    m_pDlgGameObj->Create(IDD_GAMEOBJ, &m_tabCtrl);
    m_pDlgGameObj->MoveWindow(0, 20, rect.Width(), rect.Height());
    m_pDlgGameObj->ShowWindow(SW_SHOW);

    // 리스트 박스/스크린 포인터 전달(연동 핵심)
    m_pDlgGameObj->SetLbGameObj(&m_lbGameObj);
    m_pDlgGameObj->SetScreen(&m_screen);

    // Light 탭 다이얼로그
    m_pDlgLight = new CDlgLight;
    m_pDlgLight->Create(IDD_LIGHT, &m_tabCtrl);
    m_pDlgLight->MoveWindow(0, 20, rect.Width(), rect.Height());
    m_pDlgLight->ShowWindow(SW_HIDE);

    // 스크린 포인터 전달(조명/배경 적용)
    m_pDlgLight->SetScreen(&m_screen);
}

void CUnityModelerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    // About 메뉴 선택 시 About 다이얼로그 표시
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
        return;
    }

    CDialogEx::OnSysCommand(nID, lParam);
}

void CUnityModelerDlg::OnPaint()
{
    // 아이콘화 상태에서는 기본 아이콘만 중앙에 그림(MFC 기본 처리)
    if (IsIconic())
    {
        CPaintDC dc(this);
        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        const int cxIcon = GetSystemMetrics(SM_CXICON);
        const int cyIcon = GetSystemMetrics(SM_CYICON);

        CRect rect;
        GetClientRect(&rect);

        const int x = (rect.Width() - cxIcon + 1) / 2;
        const int y = (rect.Height() - cyIcon + 1) / 2;

        dc.DrawIcon(x, y, m_hIcon);
        return;
    }

    CDialogEx::OnPaint();
}

HCURSOR CUnityModelerDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

// 탭 변경: 해당 탭 다이얼로그만 보이게 전환
void CUnityModelerDlg::OnSelchangeTabOpt(NMHDR* pNMHDR, LRESULT* pResult)
{
    const int sel = m_tabCtrl.GetCurSel();

    if (sel == 0)
    {
        if (m_pDlgGameObj) m_pDlgGameObj->ShowWindow(SW_SHOW);
        if (m_pDlgLight)   m_pDlgLight->ShowWindow(SW_HIDE);
    }
    else
    {
        if (m_pDlgGameObj) m_pDlgGameObj->ShowWindow(SW_HIDE);
        if (m_pDlgLight)   m_pDlgLight->ShowWindow(SW_SHOW);
    }

    *pResult = 0;
}

// 리스트에서 오브젝트 선택 시
// 1) 화면에서 선택 인덱스 갱신
// 2) GameObject 탭 Inspector UI에 값 채움
// 3) 화면 다시 그리기
void CUnityModelerDlg::OnSelchangeListGameObj()
{
    const int idx = m_lbGameObj.GetCurSel();

    // 렌더러 쪽 선택 상태 업데이트(선택 윤곽선 등)
    m_screen.SelectObject(idx);

    // Apply Transform 버튼이 적용할 대상 인덱스 전달
    if (m_pDlgGameObj)
        m_pDlgGameObj->m_selectedIndex = idx;

    // Inspector에 Transform 값 로드
    if (m_pDlgGameObj && idx >= 0)
    {
        OglTransform ot;
        if (m_screen.GetObject(idx, ot))
            m_pDlgGameObj->LoadFromTransform(ot);
    }

    // 화면 갱신
    m_screen.Invalidate(FALSE);
}
