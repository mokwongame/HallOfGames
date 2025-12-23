#pragma once
#include "afxdialogex.h"
#include "afxcolorbutton.h"
#include "OglTransform.h"

// 전방 선언: OpenGL 렌더링 화면 클래스
class MyScreen;

// Game Object 생성/편집 다이얼로그
// - 오브젝트 추가
// - Transform(이동/회전/스케일/알파) 수정
// - Grid 표시 여부 제어
class CDlgGameObj : public CDialogEx
{
    DECLARE_DYNAMIC(CDlgGameObj)

public:
    CDlgGameObj(CWnd* pParent = nullptr);
    virtual ~CDlgGameObj();

    // 외부(ListBox / Screen)와의 연결
    void SetLbGameObj(CListBox* ptr) { m_pLbGameObj = ptr; }
    void SetScreen(MyScreen* pScreen) { m_pScreen = pScreen; }

    // 현재 선택된 GameObject 인덱스
    // Apply Transform 시 대상 식별용
    int m_selectedIndex = -1;

    // Screen에 있는 Transform 값을 Inspector UI로 로드
    void LoadFromTransform(const OglTransform& ot);

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_GAMEOBJ };
#endif

protected:
    // 외부 컨트롤 포인터
    CListBox* m_pLbGameObj = nullptr;   // GameObject 목록
    MyScreen* m_pScreen = nullptr;   // OpenGL 렌더 화면

    // GameObject 기본 정보
    CString m_sGameObjName;

    // Transform 값(Inspector 입력용)
    double m_trX, m_trY, m_trZ;   // Translation
    double m_rotX, m_rotY, m_rotZ;  // Rotation
    double m_scaX, m_scaY, m_scaZ;  // Scale
    double m_alpha;                 // Transparency

    // 색상 선택 버튼
    CMFCColorButton m_btnColor;

    // 오브젝트 타입 선택(Sphere, Cube, Cylinder, ...)
    CComboBox m_cbType;

    // OBJ 파일 경로 입력
    CEdit   m_edtObjPath;
    CString m_objPath;

    // Grid 표시 On/Off
    CButton m_chkGrid;

    virtual void DoDataExchange(CDataExchange* pDX) override;
    DECLARE_MESSAGE_MAP()

public:
    virtual BOOL OnInitDialog() override;

    // GameObject 추가 버튼
    afx_msg void OnBnClickedButton1();

    // OBJ 파일 탐색 버튼
    afx_msg void OnBnClickedBrowseObj();

    // 오브젝트 타입 변경 시 UI 제어
    afx_msg void OnCbnSelchangeComboType();

    // 선택된 오브젝트에 Transform 적용
    afx_msg void OnBnClickedApplyTransform();

    // Grid 표시 토글
    afx_msg void OnBnClickedCheckGrid();
};
