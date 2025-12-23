// CDlgGameObj.cpp

#include "pch.h"
#include "UnityModeler.h"
#include "afxdialogex.h"
#include "CDlgGameObj.h"

#include "MyScreen.h"
#include "resource.h"

IMPLEMENT_DYNAMIC(CDlgGameObj, CDialogEx)

CDlgGameObj::CDlgGameObj(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_GAMEOBJ, pParent)
{
    // Inspector 기본값
    m_trX = m_trY = m_trZ = 0.0;
    m_rotX = m_rotY = m_rotZ = 0.0;
    m_scaX = m_scaY = m_scaZ = 1.0;
    m_alpha = 1.0;

    // OBJ 경로(선택 시에만 사용)
    m_objPath = _T("");
}

CDlgGameObj::~CDlgGameObj() {}

void CDlgGameObj::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);

    // 이름(리스트에 표시될 문자열)
    DDX_Text(pDX, IDC_EDIT1, m_sGameObjName);

    // Transform: Translate
    DDX_Text(pDX, IDC_EDIT_TRX, m_trX);
    DDX_Text(pDX, IDC_EDIT_TRY, m_trY);
    DDX_Text(pDX, IDC_EDIT_TRZ, m_trZ);

    // Transform: Rotate
    DDX_Text(pDX, IDC_EDIT_ROTX, m_rotX);
    DDX_Text(pDX, IDC_EDIT_ROTY, m_rotY);
    DDX_Text(pDX, IDC_EDIT_ROTZ, m_rotZ);

    // Transform: Scale
    DDX_Text(pDX, IDC_EDIT_SCAX, m_scaX);
    DDX_Text(pDX, IDC_EDIT_SCAY, m_scaY);
    DDX_Text(pDX, IDC_EDIT_SCAZ, m_scaZ);

    // 알파(0~1)
    DDX_Text(pDX, IDC_EDIT_ALPHA, m_alpha);

    // 컨트롤 핸들(Subclass)
    DDX_Control(pDX, IDC_BTN_COLOR, m_btnColor);
    DDX_Control(pDX, IDC_COMBO_TYPE, m_cbType);
    DDX_Control(pDX, IDC_EDIT_OBJPATH, m_edtObjPath);
    DDX_Control(pDX, IDC_CHECK_GRID, m_chkGrid);
}

BEGIN_MESSAGE_MAP(CDlgGameObj, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON1, &CDlgGameObj::OnBnClickedButton1)
    ON_BN_CLICKED(IDC_BUTTON_BROWSE_OBJ, &CDlgGameObj::OnBnClickedBrowseObj)
    ON_CBN_SELCHANGE(IDC_COMBO_TYPE, &CDlgGameObj::OnCbnSelchangeComboType)
    ON_BN_CLICKED(IDC_BUTTON_APPLY_TRANSFORM, &CDlgGameObj::OnBnClickedApplyTransform)
    ON_BN_CLICKED(IDC_CHECK_GRID, &CDlgGameObj::OnBnClickedCheckGrid)
END_MESSAGE_MAP()

BOOL CDlgGameObj::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // 기본 색상(빨강)
    m_btnColor.SetColor(RGB(255, 0, 0));

    // 타입 목록 구성
    m_cbType.ResetContent();
    m_cbType.AddString(_T("Sphere"));
    m_cbType.AddString(_T("Cube"));
    m_cbType.AddString(_T("Cylinder"));
    m_cbType.AddString(_T("Quad"));
    m_cbType.AddString(_T("Tri"));
    m_cbType.AddString(_T("OBJ"));
    m_cbType.SetCurSel(0);

    // 시작 시 OBJ 경로는 비워둔다(OBJ 선택 시에만 사용)
    m_edtObjPath.SetWindowText(_T(""));

    // 기본 Grid ON
    m_chkGrid.SetCheck(BST_CHECKED);
    if (m_pScreen) m_pScreen->SetGridEnabled(true);

    UpdateData(FALSE);
    return TRUE;
}

// OBJ 파일 선택(경로만 저장)
void CDlgGameObj::OnBnClickedBrowseObj()
{
    CFileDialog dlg(TRUE, _T("obj"), nullptr,
        OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
        _T("OBJ Files (*.obj)|*.obj|All Files (*.*)|*.*||"),
        this);

    if (dlg.DoModal() == IDOK)
    {
        m_objPath = dlg.GetPathName();
        m_edtObjPath.SetWindowText(m_objPath);
    }
}

// 타입 변경 시 OBJ 관련 UI 활성/비활성
void CDlgGameObj::OnCbnSelchangeComboType()
{
    const int sel = m_cbType.GetCurSel();
    const bool isObj = (sel == 5);

    m_edtObjPath.EnableWindow(isObj);

    if (CWnd* pBrowse = GetDlgItem(IDC_BUTTON_BROWSE_OBJ))
        pBrowse->EnableWindow(isObj);

    if (!isObj)
        m_edtObjPath.SetWindowText(_T(""));
}

// 오브젝트 추가
// - 리스트박스에 이름 추가
// - Screen(MyScreen)에 Transform/색/타입을 포함한 OglTransform을 추가
void CDlgGameObj::OnBnClickedButton1()
{
    UpdateData(TRUE);

    if (m_pLbGameObj)
        m_pLbGameObj->AddString(m_sGameObjName);

    if (!m_pScreen) return;

    OglTransform ot;
    ot.SetDef();

    const int sel = m_cbType.GetCurSel();
    switch (sel)
    {
    case 0: ot.m_nType = GameObjType::SPHERE;   break;
    case 1: ot.m_nType = GameObjType::CUBE;     break;
    case 2: ot.m_nType = GameObjType::CYLINDER; break;
    case 3: ot.m_nType = GameObjType::QUAD;     break;
    case 4: ot.m_nType = GameObjType::TRI;      break;
    case 5: ot.m_nType = GameObjType::OBJ;      break;
    default: ot.m_nType = GameObjType::SPHERE;  break;
    }

    // 색상
    ot.m_nColor = m_btnColor.GetColor();

    // 알파 보정(0~1)
    if (m_alpha < 0.0) m_alpha = 0.0;
    if (m_alpha > 1.0) m_alpha = 1.0;
    ot.m_alpha = (GLfloat)m_alpha;

    // Transform
    ot.m_translate = Vector3((GLfloat)m_trX, (GLfloat)m_trY, (GLfloat)m_trZ);
    ot.m_rotate = Vector3((GLfloat)m_rotX, (GLfloat)m_rotY, (GLfloat)m_rotZ);
    ot.m_scale = Vector3((GLfloat)m_scaX, (GLfloat)m_scaY, (GLfloat)m_scaZ);

    // OBJ는 경로를 같이 저장한다
    if (ot.m_nType == GameObjType::OBJ)
    {
        CString path;
        m_edtObjPath.GetWindowText(path);
        ot.m_objPath = path;
    }

    m_pScreen->AddGameObject(ot);
    m_pScreen->Invalidate(FALSE);
}

// 선택된 오브젝트 값을 Inspector UI로 로드한다
void CDlgGameObj::LoadFromTransform(const OglTransform& ot)
{
    m_trX = ot.m_translate.x; m_trY = ot.m_translate.y; m_trZ = ot.m_translate.z;
    m_rotX = ot.m_rotate.x;   m_rotY = ot.m_rotate.y;   m_rotZ = ot.m_rotate.z;
    m_scaX = ot.m_scale.x;    m_scaY = ot.m_scale.y;    m_scaZ = ot.m_scale.z;
    m_alpha = ot.m_alpha;

    m_btnColor.SetColor(ot.m_nColor);

    int sel = 0;
    switch (ot.m_nType)
    {
    case GameObjType::SPHERE:   sel = 0; break;
    case GameObjType::CUBE:     sel = 1; break;
    case GameObjType::CYLINDER: sel = 2; break;
    case GameObjType::QUAD:     sel = 3; break;
    case GameObjType::TRI:      sel = 4; break;
    case GameObjType::OBJ:      sel = 5; break;
    default: sel = 0; break;
    }
    m_cbType.SetCurSel(sel);

    const bool isObj = (ot.m_nType == GameObjType::OBJ);
    m_edtObjPath.EnableWindow(isObj);

    if (CWnd* pBrowse = GetDlgItem(IDC_BUTTON_BROWSE_OBJ))
        pBrowse->EnableWindow(isObj);

    m_edtObjPath.SetWindowText(isObj ? ot.m_objPath : _T(""));

    UpdateData(FALSE);
}

// Inspector 값을 선택된 오브젝트에 적용한다
void CDlgGameObj::OnBnClickedApplyTransform()
{
    UpdateData(TRUE);

    if (!m_pScreen) return;
    if (m_selectedIndex < 0) return;

    OglTransform ot;
    if (!m_pScreen->GetObject(m_selectedIndex, ot))
        return;

    // 색상
    ot.m_nColor = m_btnColor.GetColor();

    // 알파 보정(0~1)
    if (m_alpha < 0.0) m_alpha = 0.0;
    if (m_alpha > 1.0) m_alpha = 1.0;
    ot.m_alpha = (GLfloat)m_alpha;

    // Transform
    ot.m_translate = Vector3((GLfloat)m_trX, (GLfloat)m_trY, (GLfloat)m_trZ);
    ot.m_rotate = Vector3((GLfloat)m_rotX, (GLfloat)m_rotY, (GLfloat)m_rotZ);
    ot.m_scale = Vector3((GLfloat)m_scaX, (GLfloat)m_scaY, (GLfloat)m_scaZ);

    // OBJ는 경로도 함께 갱신 가능
    if (ot.m_nType == GameObjType::OBJ)
    {
        CString path;
        m_edtObjPath.GetWindowText(path);
        ot.m_objPath = path;
    }

    m_pScreen->SetObject(m_selectedIndex, ot);
    m_pScreen->Invalidate(FALSE);
}

// Grid 표시 On/Off
void CDlgGameObj::OnBnClickedCheckGrid()
{
    if (!m_pScreen) return;

    const bool on = (m_chkGrid.GetCheck() == BST_CHECKED);
    m_pScreen->SetGridEnabled(on);
    m_pScreen->Invalidate(FALSE);
}
