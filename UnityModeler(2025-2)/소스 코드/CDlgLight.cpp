// CDlgLight.cpp

#include "pch.h"
#include "UnityModeler.h"
#include "afxdialogex.h"
#include "CDlgLight.h"
#include "MyScreen.h"
#include "resource.h"

IMPLEMENT_DYNAMIC(CDlgLight, CDialogEx)

CDlgLight::CDlgLight(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_LIGHT, pParent)
{
    // 기본 라이트 위치 및 재질 값
    m_lx = 0.0; m_ly = 0.0; m_lz = 1.0;
    m_emitR = 0; m_emitG = 0; m_emitB = 0;
    m_shine = 32.0;

    // 0: Directional, 1: Point
    m_radioType = 0;
}

CDlgLight::~CDlgLight() {}

void CDlgLight::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);

    // 색상 버튼
    DDX_Control(pDX, IDC_BTN_BG_COLOR, m_btnBg);
    DDX_Control(pDX, IDC_BTN_AMBIENT, m_btnAmb);
    DDX_Control(pDX, IDC_BTN_DIFFUSE, m_btnDif);
    DDX_Control(pDX, IDC_BTN_SPECULAR, m_btnSpc);

    // 라이트 위치
    DDX_Text(pDX, IDC_EDIT_LX, m_lx);
    DDX_Text(pDX, IDC_EDIT_LY, m_ly);
    DDX_Text(pDX, IDC_EDIT_LZ, m_lz);

    // Emission
    DDX_Text(pDX, IDC_EDIT_EMIT_R, m_emitR);
    DDX_Text(pDX, IDC_EDIT_EMIT_G, m_emitG);
    DDX_Text(pDX, IDC_EDIT_EMIT_B, m_emitB);

    // Shininess
    DDX_Text(pDX, IDC_EDIT_SHINE, m_shine);

    // 라이트 타입 라디오(첫 번째 라디오 ID 기준)
    DDX_Radio(pDX, IDC_RADIO_DIRECTIONAL, m_radioType);
}

BEGIN_MESSAGE_MAP(CDlgLight, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON_APPLY_LIGHT, &CDlgLight::OnBnClickedApplyLight)
END_MESSAGE_MAP()

BOOL CDlgLight::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // 초기 UI 색상값(시작 상태)
    m_btnBg.SetColor(RGB(127, 127, 127));
    m_btnAmb.SetColor(RGB(60, 60, 60));
    m_btnDif.SetColor(RGB(200, 200, 200));
    m_btnSpc.SetColor(RGB(255, 255, 255));

    UpdateData(FALSE);
    return TRUE;
}

// Apply 버튼: 배경색 + 라이트/재질을 화면에 반영한다
void CDlgLight::OnBnClickedApplyLight()
{
    UpdateData(TRUE);
    if (!m_pScreen) return;

    // 배경색 적용(렌더링은 MyScreen::RenderScene에서 clear 시 반영됨)
    const COLORREF bg = m_btnBg.GetColor();
    m_pScreen->SetBackColor(bg);
    m_pScreen->Invalidate(FALSE);

    const bool bDirectional = (m_radioType == 0);

    // 라이트 색상
    const COLORREF amb = m_btnAmb.GetColor();
    const COLORREF dif = m_btnDif.GetColor();
    const COLORREF spc = m_btnSpc.GetColor();

    // Emission 입력값 보정(0~255)
    if (m_emitR < 0) m_emitR = 0; if (m_emitR > 255) m_emitR = 255;
    if (m_emitG < 0) m_emitG = 0; if (m_emitG > 255) m_emitG = 255;
    if (m_emitB < 0) m_emitB = 0; if (m_emitB > 255) m_emitB = 255;
    const COLORREF emission = RGB(m_emitR, m_emitG, m_emitB);

    // Shininess 보정(0~128)
    if (m_shine < 0) m_shine = 0;
    if (m_shine > 128) m_shine = 128;

    // OpenGL 라이트/재질 파라미터 적용
    m_pScreen->ApplyLightMaterial(
        bDirectional,
        amb, dif, spc,
        (float)m_lx, (float)m_ly, (float)m_lz,
        emission,
        (float)m_shine
    );

    // 라이트 변경도 즉시 확인 가능하도록 갱신
    m_pScreen->Invalidate(FALSE);
}
