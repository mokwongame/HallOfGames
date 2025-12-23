// OglCylinder.cpp

#include "pch.h"
#include "OglCylinder.h"
#include "OglScreen.h"
#include <GL/GLU.h>

OglCylinder::OglCylinder(void)
{
    // GLU quadric 객체는 Create()에서 생성
    m_pQobj = nullptr;

    // 기본 색/알파(Transform이 없을 때의 기본값)
    m_nColor = RGB(255, 0, 0);
    m_alpha = 1.0f;
}

OglCylinder::~OglCylinder()
{
    if (m_pQobj)
        gluDeleteQuadric(m_pQobj);
}

// quadric 생성 및 렌더링 옵션 설정
void OglCylinder::Create(void)
{
    if (m_pQobj)
        gluDeleteQuadric(m_pQobj);

    m_pQobj = gluNewQuadric();
    gluQuadricDrawStyle(m_pQobj, GLU_FILL);
    gluQuadricNormals(m_pQobj, GLU_SMOOTH);
}

// radius/height를 받아 실린더 + 위/아래 캡(원판)을 그린다
void OglCylinder::Draw(GLdouble radius, GLdouble ht, GLint slice, GLint stack) const
{
    if (!m_pQobj) return;

    GLfloat r, g, b;
    OglScreen::colorrefToRgb(r, g, b, m_nColor);
    glColor4f(r, g, b, m_alpha);

    // 옆면(법선은 quadric 설정에 의해 생성됨)
    gluCylinder(m_pQobj, radius, radius, ht, slice, stack);

    // 캡은 disk로 막아준다
    GLUquadric* disk = m_pQobj;

    // 아래 캡: z=0에서 -Z 방향을 향하도록 180도 뒤집어서 그린다
    glPushMatrix();
    glRotatef(180.f, 1.f, 0.f, 0.f);
    gluDisk(disk, 0.0, radius, slice, 1);
    glPopMatrix();

    // 위 캡: z=ht 위치로 올려서 그린다
    glPushMatrix();
    glTranslatef(0.f, 0.f, (GLfloat)ht);
    gluDisk(disk, 0.0, radius, slice, 1);
    glPopMatrix();
}

// Transform(이동/회전/스케일)을 적용해서 그린다
void OglCylinder::Draw(const OglTransform& ot)
{
    glPushMatrix();

    // 주의: 여기서 glLoadIdentity()를 하면 카메라(뷰) 행렬이 초기화되어 화면에서 사라진다
    ot.Transform();

    SetRgba(ot.m_nColor, ot.m_alpha);
    DrawDef();

    glPopMatrix();
}

// 기본 크기의 실린더를 그린다(기본 슬라이스/스택 조절 지점)
void OglCylinder::DrawDef(void)
{
    Draw(m_defRad, m_defRad * 4., 32, 8);
}
