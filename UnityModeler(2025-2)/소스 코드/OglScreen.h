#pragma once
#include <afxwin.h>
#include <GL/glew.h>

// OpenGL 렌더링용 CStatic 기반 화면 클래스
// - OpenGL Context(HDC/HGLRC) 생성/파괴
// - 매 프레임 Viewport 설정 및 렌더 호출
// - 파생 클래스(MyScreen)가 실제 씬 렌더링(RenderScene)과 카메라 입력 처리 구현
class OglScreen : public CStatic
{
public:
    OglScreen();

    // COLORREF(RGB) -> OpenGL RGB(float) 변환 유틸
    static void colorrefToRgb(GLfloat& r, GLfloat& g, GLfloat& b, COLORREF color);

    // stb_image 로드 후, 현재 바인딩된 GL_TEXTURE_2D에 이미지 업로드
    void SetTexImage2D(const CString& imageFile);

protected:
    // OpenGL Context
    HDC   m_hDC = NULL;
    HGLRC m_hRC = NULL;

    // Background color/alpha (glClearColor에 사용)
    COLORREF m_nBackColor = RGB(127, 127, 127);
    float    m_backAlpha = 1.0f;

    // Viewport/Projection 설정 (OnPaint에서 매 프레임 호출)
    void SetViewport();

    // 픽셀 포맷 설정(더블버퍼, depth 포함)
    void AdjustPixelFormat();

    // wglMakeCurrent 래핑
    void StartRC();
    void StopRC();

    // OpenGL 초기화(1회), 프레임 렌더 전처리/후처리(매 프레임)
    virtual void InitOpenGL();
    virtual void InitRender();
    virtual void RenderScene();

    // ===== 3단계: 카메라 입력 훅(파생 클래스에서 구현) =====
    virtual void OnCameraBegin(UINT nFlags, CPoint pt) {}
    virtual void OnCameraDrag(UINT nFlags, CPoint pt) {}
    virtual void OnCameraEnd(UINT nFlags, CPoint pt) {}
    virtual void OnCameraWheel(short zDelta, CPoint pt) {}

public:
    // Background 설정
    void SetBackColor(COLORREF rgb) { m_nBackColor = rgb; }
    void SetBackAlpha(float a) { m_backAlpha = a; }

    // Dialog 리소스의 IDC_SCREEN을 실제 OpenGL 화면으로 교체하기 위한 Create
    BOOL Create(int nId, CWnd* pParent);

    // CStatic 기본 Create
    virtual BOOL Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect,
        CWnd* pParentWnd, UINT nID = 0xffff);

    DECLARE_MESSAGE_MAP()
    afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnPaint();
    afx_msg void OnDestroy();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);

    // Mouse input -> 카메라 입력 훅 호출
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};
