// CUnityModelerDlg.h
// 메인 다이얼로그 클래스
// - OpenGL 화면(MyScreen) 관리
// - 탭 UI(Game Object / Light) 관리
// - 서브 다이얼로그와 화면 간 연결 담당

#pragma once

#include "framework.h"
#include "MyScreen.h"

// 전방 선언 (순환 include 방지)
class CDlgGameObj;
class CDlgLight;

class CUnityModelerDlg : public CDialogEx
{
public:
    CUnityModelerDlg(CWnd* pParent = nullptr);
    virtual ~CUnityModelerDlg();

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_UNITYMODELER_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX) override;

protected:
    // 애플리케이션 아이콘
    HICON m_hIcon;

    // OpenGL 렌더링 화면
    MyScreen m_screen;

    // UI 컨트롤
    CTabCtrl m_tabCtrl;     // 옵션 탭 (Game Object / Light)
    CListBox m_lbGameObj;   // 게임 오브젝트 목록

    // 서브 다이얼로그
    CDlgGameObj* m_pDlgGameObj = nullptr;
    CDlgLight* m_pDlgLight = nullptr;

    // 탭 초기화 및 서브 다이얼로그 생성
    void SetTabCtrl();

    // 다이얼로그 초기화
    virtual BOOL OnInitDialog() override;

    // 시스템 / 윈도우 메시지
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();

    // UI 이벤트
    afx_msg void OnSelchangeTabOpt(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnSelchangeListGameObj();

    DECLARE_MESSAGE_MAP()
};
