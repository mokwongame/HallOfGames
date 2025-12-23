// OglLightMtl.cpp

#include "pch.h"
#include "OglLightMtl.h"

OglLightMtl::OglLightMtl()
{
    SetDefault();
}

// (현재 구조에서는 MyScreen에서 StartRC/StopRC를 관리하고 있으니)
// 이 핸들들은 "보관"만 하고 직접 wglMakeCurrent에 쓰지 않는다.
void OglLightMtl::SetDCRC(HDC hDC, HGLRC hRC)
{
    m_hDC = hDC;
    m_hRC = hRC;
}

// COLORREF -> RGBA(float[4]) 변환
void OglLightMtl::CRtoRGBA(COLORREF c, GLfloat out4[4], float a)
{
    out4[0] = GetRValue(c) / 255.0f;
    out4[1] = GetGValue(c) / 255.0f;
    out4[2] = GetBValue(c) / 255.0f;
    out4[3] = a;
}

// 기본 라이트/재질 값 초기화
void OglLightMtl::SetDefault()
{
    // light position: w=0이면 directional, w=1이면 point light
    m_lightPos[0] = 0.f;
    m_lightPos[1] = 0.f;
    m_lightPos[2] = 200.f;
    m_lightPos[3] = 0.f;

    // light colors
    m_ambient[0] = 0.2f; m_ambient[1] = 0.2f; m_ambient[2] = 0.2f; m_ambient[3] = 1.f;
    m_diffuse[0] = 0.8f; m_diffuse[1] = 0.8f; m_diffuse[2] = 0.8f; m_diffuse[3] = 1.f;
    m_specular[0] = 0.3f; m_specular[1] = 0.3f; m_specular[2] = 0.3f; m_specular[3] = 1.f;

    // material
    m_emission[0] = 0.f; m_emission[1] = 0.f; m_emission[2] = 0.f; m_emission[3] = 1.f;
    m_shininess = 16.f;
}

// directional / point 선택
void OglLightMtl::SetDirectional(bool bDirectional)
{
    m_lightPos[3] = bDirectional ? 0.0f : 1.0f;
}

// 라이트 위치 지정(x,y,z)
void OglLightMtl::SetLightPos(float x, float y, float z)
{
    m_lightPos[0] = x;
    m_lightPos[1] = y;
    m_lightPos[2] = z;
}

// UI에서 선택한 색을 OpenGL 라이트 파라미터로 변환
void OglLightMtl::SetLightColors(COLORREF amb, COLORREF dif, COLORREF spc)
{
    CRtoRGBA(amb, m_ambient, 1.0f);
    CRtoRGBA(dif, m_diffuse, 1.0f);
    CRtoRGBA(spc, m_specular, 1.0f);
}

// emission/shininess 설정
void OglLightMtl::SetMaterial(COLORREF emission, float shininess)
{
    CRtoRGBA(emission, m_emission, 1.0f);

    if (shininess < 0.f)   shininess = 0.f;
    if (shininess > 128.f) shininess = 128.f;

    m_shininess = shininess;
}

// OpenGL 상태에 라이트/재질을 반영
void OglLightMtl::Apply()
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT0, GL_POSITION, m_lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, m_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, m_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, m_specular);

    // glColor() 값을 재질의 ambient/diffuse로 쓰도록 연결
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    // material (emission/specular/shininess)
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, m_emission);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, m_shininess);
}
