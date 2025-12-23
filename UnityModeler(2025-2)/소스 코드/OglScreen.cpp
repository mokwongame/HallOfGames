// OglScreen.cpp
// OpenGL을 CStatic 컨트롤 안에서 렌더링하기 위한 기반 클래스
// - DC/RC 생성 및 해제
// - 매 프레임 Viewport/Projection 갱신
// - 마우스 입력을 파생 클래스(MyScreen)로 전달

#include "pch.h"
#include "OglScreen.h"
#include <GL/GLU.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

OglScreen::OglScreen(void)
{
    // 기본 배경색/알파
    m_nBackColor = RGB(127, 127, 127);
    m_backAlpha = 1.0f;
}

// stb_image로 이미지를 로드해서 현재 바인딩된 텍스처 객체에 업로드한다
// 주의: glBindTexture/glTexParameteri는 외부에서 준비되어 있어야 한다
void OglScreen::SetTexImage2D(const CString& imageFile)
{
    int wid, ht, chan;
    unsigned char* data = stbi_load(CStringA(imageFile), &wid, &ht, &chan, 0);
    if (!data) return;

    if (chan == 3)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wid, ht, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    else if (chan == 4)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, wid, ht, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
}

// COLORREF -> float RGB
void OglScreen::colorrefToRgb(GLfloat& r, GLfloat& g, GLfloat& b, COLORREF color)
{
    r = GetRValue(color) / GLfloat(255);
    g = GetGValue(color) / GLfloat(255);
    b = GetBValue(color) / GLfloat(255);
}

// 윈도우 크기에 맞게 viewport와 projection을 재설정한다
// - 보통 WM_SIZE에서 처리해도 되지만, OnPaint마다 갱신하면 단순하고 안정적이다
void OglScreen::SetViewport(void)
{
    CRect rect;
    GetClientRect(rect);

    const int w = rect.Width();
    const int h = rect.Height();
    if (w <= 0 || h <= 0) return;

    glViewport(0, 0, w, h);

    const double aspect = (double)w / (double)h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, aspect, 1.0, 5000.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// OpenGL을 위한 픽셀 포맷 설정(더블 버퍼 + depth)
void OglScreen::AdjustPixelFormat(void)
{
    PIXELFORMATDESCRIPTOR PFD =
    {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW |
        PFD_SUPPORT_OPENGL |
        PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        32,
        0,0,0,0,0,0,
        0,
        0,
        0,
        0,0,0,0,
        24, // depth buffer
        0,
        0,
        PFD_MAIN_PLANE,
        0,
        0,0,0
    };

    int nFormat = ::ChoosePixelFormat(m_hDC, &PFD);
    ::SetPixelFormat(m_hDC, nFormat, &PFD);
}

// 현재 스레드에 OpenGL 컨텍스트를 바인딩
void OglScreen::StartRC(void)
{
    ::wglMakeCurrent(m_hDC, m_hRC);
}

// OpenGL 컨텍스트 바인딩 해제
void OglScreen::StopRC(void)
{
    ::wglMakeCurrent(m_hDC, NULL);
}

// OpenGL 초기 상태 설정(컨텍스트 생성 직후 1회)
void OglScreen::InitOpenGL(void)
{
    StartRC();

    glEnable(GL_DEPTH_TEST);

    // 기본적으로 back-face culling을 켜둔다
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    // 비정규화된 normal을 자동 정규화(스케일 변환이 있어도 조명이 깨지지 않게)
    glEnable(GL_NORMALIZE);

    StopRC();
}

// 파생 클래스에서 필요하면 override해서 사용
void OglScreen::InitRender(void)
{
    // 기반 클래스에서는 아무것도 하지 않는다
    // 배경 clear/실제 렌더는 MyScreen::RenderScene에서 처리하는 구조를 사용 중
}

// 파생 클래스에서 override해서 실제 draw를 구현
void OglScreen::RenderScene(void)
{
    // 기반 클래스에서는 아무것도 하지 않는다
}

// 리소스 상의 CStatic(IDC_SCREEN)을 숨기고 동일 위치에 OpenGL용 CStatic을 새로 만든다
BOOL OglScreen::Create(int nId, CWnd* pParent)
{
    CWnd* pWnd = pParent->GetDlgItem(nId);
    pWnd->ShowWindow(SW_HIDE);

    CRect rect;
    pWnd->GetWindowRect(rect);
    pParent->ScreenToClient(rect);

    // SS_NOTIFY: CStatic이 마우스 메시지를 받을 수 있도록 함
    return Create(NULL,
        WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | SS_NOTIFY,
        rect, pParent);
}

BOOL OglScreen::Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
    return CStatic::Create(lpszText, dwStyle, rect, pParentWnd, nID);
}

BEGIN_MESSAGE_MAP(OglScreen, CStatic)
    ON_WM_CREATE()
    ON_WM_PAINT()
    ON_WM_DESTROY()
    ON_WM_ERASEBKGND()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_RBUTTONDOWN()
    ON_WM_RBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

// 윈도우가 생성될 때 DC/RC를 만든다
int OglScreen::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CStatic::OnCreate(lpCreateStruct) == -1)
        return -1;

    m_hDC = ::GetDC(GetSafeHwnd());
    AdjustPixelFormat();

    m_hRC = ::wglCreateContext(m_hDC);
    InitOpenGL();

    return 0;
}

// WM_PAINT: 한 프레임 렌더링
// - StartRC 후 SetViewport -> InitRender -> RenderScene -> SwapBuffers
void OglScreen::OnPaint()
{
    CPaintDC dc(this);

    StartRC();

    SetViewport();

    InitRender();
    RenderScene();

    ::SwapBuffers(m_hDC);
    StopRC();
}

void OglScreen::OnDestroy()
{
    CStatic::OnDestroy();

    StopRC();
    ::wglDeleteContext(m_hRC);
    ::ReleaseDC(GetSafeHwnd(), m_hDC);
}

// 배경 지우기 방지(깜빡임 감소). 실제 배경은 OpenGL에서 clear한다
BOOL OglScreen::OnEraseBkgnd(CDC* pDC)
{
    return TRUE;
}

// 마우스 입력은 파생 클래스의 카메라 핸들러로 전달한다
void OglScreen::OnLButtonDown(UINT nFlags, CPoint point)
{
    SetFocus();
    SetCapture();
    OnCameraBegin(nFlags | MK_LBUTTON, point);
}

void OglScreen::OnLButtonUp(UINT nFlags, CPoint point)
{
    ReleaseCapture();
    OnCameraEnd(nFlags & ~MK_LBUTTON, point);
}

void OglScreen::OnRButtonDown(UINT nFlags, CPoint point)
{
    SetFocus();
    SetCapture();
    OnCameraBegin(nFlags | MK_RBUTTON, point);
}

void OglScreen::OnRButtonUp(UINT nFlags, CPoint point)
{
    ReleaseCapture();
    OnCameraEnd(nFlags & ~MK_RBUTTON, point);
}

void OglScreen::OnMouseMove(UINT nFlags, CPoint point)
{
    if (GetCapture() == this)
    {
        OnCameraDrag(nFlags, point);
        Invalidate(FALSE);
    }
}

BOOL OglScreen::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    OnCameraWheel(zDelta, pt);
    Invalidate(FALSE);
    return TRUE;
}
