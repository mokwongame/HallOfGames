// UnityModeler.cpp
// UnityModeler 애플리케이션 진입점(App) 구현 파일
// - MFC 응용 프로그램 초기화
// - 메인 다이얼로그(UnityModelerDlg) 실행
// - OpenGL 화면은 다이얼로그 내부에서 관리됨

#include "pch.h"
#include "framework.h"
#include "UnityModeler.h"
#include "UnityModelerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CUnityModelerApp 메시지 맵
BEGIN_MESSAGE_MAP(CUnityModelerApp, CWinApp)
    ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

// 전역 App 객체 (MFC 규칙)
CUnityModelerApp theApp;

// App 생성자
CUnityModelerApp::CUnityModelerApp()
{
    // Windows 재시작 관리자 지원
    m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
}

// 애플리케이션 초기화
BOOL CUnityModelerApp::InitInstance()
{
    // 공용 컨트롤 초기화 (XP 호환 포함)
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();

    // 다이얼로그 내부에서 ActiveX 컨트롤 사용 가능하도록 설정
    AfxEnableControlContainer();

    // Shell 컨트롤(Tree/List)을 위한 매니저
    // ※ 현재 프로젝트에서는 직접 사용하지 않지만
    //    MFC 다이얼로그 기본 구조 유지 목적
    CShellManager* pShellManager = new CShellManager;

    // Windows 기본 테마 적용
    CMFCVisualManager::SetDefaultManager(
        RUNTIME_CLASS(CMFCVisualManagerWindows)
    );

    // 레지스트리 키 설정 (설정값 저장 위치)
    SetRegistryKey(_T("UnityModeler"));

    // 메인 다이얼로그 실행
    CUnityModelerDlg dlg;
    m_pMainWnd = &dlg;

    // 모달 다이얼로그 실행
    dlg.DoModal();

    // ShellManager 정리
    if (pShellManager)
    {
        delete pShellManager;
        pShellManager = nullptr;
    }

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
    ControlBarCleanUp();
#endif

    // 다이얼로그 기반 앱이므로
    // 메시지 루프를 돌리지 않고 바로 종료
    return FALSE;
}
