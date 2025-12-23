#pragma once
#include "afxdialogex.h"
#include "afxcolorbutton.h"

// 전방 선언: OpenGL 렌더링 화면 클래스
class MyScreen;

// Light/Material + Background 설정 다이얼로그
// - 배경색 변경
// - Light(ambient/diffuse/specular, position, type) 설정
// - Material(emission, shininess) 설정
class CDlgLight : public CDialogEx
{
    DECLARE_DYNAMIC(CDlgLight)

public:
    CDlgLight(CWnd* pParent = nullptr);
    virtual ~CDlgLight();

    // 외부에서 렌더 화면(MyScreen) 포인터 주입
    void SetScreen(MyScreen* p) { m_pScreen = p; }

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_LIGHT };
#endif

protected:
    // 실제 OpenGL 상태/값을 적용할 대상 화면
    MyScreen* m_pScreen = nullptr;

    // 색상 버튼(UI)
    CMFCColorButton m_btnBg;   // Background color
    CMFCColorButton m_btnAmb;  // Ambient
    CMFCColorButton m_btnDif;  // Diffuse
    CMFCColorButton m_btnSpc;  // Specular

    // Light position (x, y, z)
    double m_lx, m_ly, m_lz;

    // Material emission color (0~255)
    int m_emitR, m_emitG, m_emitB;

    // Material shininess (0~128 권장)
    double m_shine;

    // Light type
    // 0: directional (w=0), 1: point (w=1)
    int m_radioType;

    virtual void DoDataExchange(CDataExchange* pDX) override;
    DECLARE_MESSAGE_MAP()

public:
    virtual BOOL OnInitDialog() override;

    // Apply 버튼: UI 값들을 Screen에 반영
    afx_msg void OnBnClickedApplyLight();
};
