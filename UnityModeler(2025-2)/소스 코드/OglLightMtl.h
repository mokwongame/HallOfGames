#pragma once
#include <afxwin.h>
#include <GL/glew.h>

// OpenGL Light / Material 설정 관리 클래스
// - UI에서 입력된 조명/재질 값을 OpenGL 상태로 변환하여 적용
class OglLightMtl
{
public:
    OglLightMtl();

    // OpenGL Device Context / Rendering Context 설정
    void SetDCRC(HDC hDC, HGLRC hRC);

    // 기본 조명/재질 값 초기화
    void SetDefault();

    // ===== Light 설정(UI 입력) =====
    // w 값 설정: 0 = directional light, 1 = point light
    void SetDirectional(bool bDirectional);

    // Light position (x, y, z)
    void SetLightPos(float x, float y, float z);

    // Light color 설정
    void SetLightColors(COLORREF amb, COLORREF dif, COLORREF spc);

    // Material 설정
    void SetMaterial(COLORREF emission, float shininess);

    // ===== OpenGL 적용 =====
    // 현재 설정된 Light / Material 값을 OpenGL 상태에 반영
    void Apply();

public:
    // Light parameters
    GLfloat m_lightPos[4];   // x, y, z, w
    GLfloat m_ambient[4];
    GLfloat m_diffuse[4];
    GLfloat m_specular[4];

    // Material parameters
    GLfloat m_emission[4];
    GLfloat m_shininess;

private:
    // OpenGL 컨텍스트
    HDC   m_hDC = NULL;
    HGLRC m_hRC = NULL;

    // COLORREF -> RGBA(float) 변환 유틸
    static void CRtoRGBA(COLORREF c, GLfloat out4[4], float a = 1.0f);
};
