// OglSphere.cpp
// GLU quadric을 이용해 구(Sphere)를 그리는 클래스
// - Create()에서 quadric을 생성하고 옵션(채우기/법선)을 설정
// - Draw(ot)에서 Transform을 적용한 뒤 기본 구를 렌더링

#include "pch.h"
#include "OglSphere.h"
#include "OglScreen.h"

OglSphere::OglSphere(void)
{
    // GLU quadric 객체는 Create()에서 생성
    m_pQobj = nullptr;

    // 기본 색/알파(Transform이 없을 때의 기본값)
    m_nColor = RGB(255, 0, 0);
    m_alpha = 1.0f;
}

OglSphere::~OglSphere()
{
    if (m_pQobj)
        gluDeleteQuadric(m_pQobj);
}

// quadric 생성 및 렌더링 옵션 설정
void OglSphere::Create(void)
{
    if (m_pQobj)
        gluDeleteQuadric(m_pQobj);

    m_pQobj = gluNewQuadric();
    gluQuadricDrawStyle(m_pQobj, GLU_FILL);
    gluQuadricNormals(m_pQobj, GLU_SMOOTH);
}

// radius / slice / stack으로 구를 그린다
void OglSphere::Draw(GLdouble radius, GLint slice, GLint stack) const
{
    if (!m_pQobj) return;

    GLfloat r, g, b;
    OglScreen::colorrefToRgb(r, g, b, m_nColor);
    glColor4f(r, g, b, m_alpha);

    gluSphere(m_pQobj, radius, slice, stack);
}

// Transform(이동/회전/스케일)을 적용해서 그린다
void OglSphere::Draw(const OglTransform& ot)
{
    glPushMatrix();

    ot.Transform();
    SetRgba(ot.m_nColor, ot.m_alpha);
    DrawDef();

    glPopMatrix();
}

// 기본 구 렌더링(기본 반지름 사용)
void OglSphere::DrawDef(void)
{
    Draw(m_defRad);
}
