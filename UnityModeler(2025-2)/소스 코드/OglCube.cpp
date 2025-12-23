// OglCube.cpp

#include "pch.h"
#include "OglCube.h"
#include "OglScreen.h"

OglCube::OglCube(void)
{
    SetRgbaDef();
}

// 면(6개)마다 다른 색/알파를 지정
void OglCube::SetRgba(const COLORREF arColor[], const GLfloat arAlpha[])
{
    for (int i = 0; i < 6; i++)
    {
        m_arColor[i] = arColor[i];
        m_arAlpha[i] = arAlpha[i];
    }
}

// 모든 면을 동일한 색으로 설정
// alpha < 0 이면 디폴트(면별 색상)로 되돌린다
void OglCube::SetRgba(COLORREF col, GLfloat alpha)
{
    if (alpha < 0.f)
    {
        SetRgbaDef();
        return;
    }

    for (int i = 0; i < 6; i++)
    {
        m_arColor[i] = col;
        m_arAlpha[i] = alpha;
    }
}

// 디폴트: 면별로 색을 다르게 지정(시각적으로 방향 구분이 쉬움)
void OglCube::SetRgbaDef(void)
{
    m_arColor[0] = RGB(255, 0, 0);
    m_arColor[1] = RGB(0, 255, 0);
    m_arColor[2] = RGB(0, 0, 255);
    m_arColor[3] = RGB(255, 255, 0);
    m_arColor[4] = RGB(0, 255, 255);
    m_arColor[5] = RGB(255, 0, 255);

    for (int i = 0; i < 6; i++)
        m_arAlpha[i] = 1.0f;
}

// 단색 큐브(면별 컬러/알파 + 법선 포함)
void OglCube::Draw(GLfloat len) const
{
    GLfloat r, g, b;

    glBegin(GL_QUADS);

    // front (+Z)
    glNormal3f(0.f, 0.f, 1.f);
    OglScreen::colorrefToRgb(r, g, b, m_arColor[0]);
    glColor4f(r, g, b, m_arAlpha[0]);
    glVertex3f(-len, -len, len);
    glVertex3f(len, -len, len);
    glVertex3f(len, len, len);
    glVertex3f(-len, len, len);

    // back (-Z)
    glNormal3f(0.f, 0.f, -1.f);
    OglScreen::colorrefToRgb(r, g, b, m_arColor[2]);
    glColor4f(r, g, b, m_arAlpha[2]);
    glVertex3f(len, -len, -len);
    glVertex3f(-len, -len, -len);
    glVertex3f(-len, len, -len);
    glVertex3f(len, len, -len);

    // right (+X)
    glNormal3f(1.f, 0.f, 0.f);
    OglScreen::colorrefToRgb(r, g, b, m_arColor[1]);
    glColor4f(r, g, b, m_arAlpha[1]);
    glVertex3f(len, -len, len);
    glVertex3f(len, -len, -len);
    glVertex3f(len, len, -len);
    glVertex3f(len, len, len);

    // left (-X)
    glNormal3f(-1.f, 0.f, 0.f);
    OglScreen::colorrefToRgb(r, g, b, m_arColor[3]);
    glColor4f(r, g, b, m_arAlpha[3]);
    glVertex3f(-len, -len, -len);
    glVertex3f(-len, -len, len);
    glVertex3f(-len, len, len);
    glVertex3f(-len, len, -len);

    // top (+Y)
    glNormal3f(0.f, 1.f, 0.f);
    OglScreen::colorrefToRgb(r, g, b, m_arColor[4]);
    glColor4f(r, g, b, m_arAlpha[4]);
    glVertex3f(-len, len, len);
    glVertex3f(len, len, len);
    glVertex3f(len, len, -len);
    glVertex3f(-len, len, -len);

    // bottom (-Y)
    glNormal3f(0.f, -1.f, 0.f);
    OglScreen::colorrefToRgb(r, g, b, m_arColor[5]);
    glColor4f(r, g, b, m_arAlpha[5]);
    glVertex3f(-len, -len, -len);
    glVertex3f(len, -len, -len);
    glVertex3f(len, -len, len);
    glVertex3f(-len, -len, len);

    glEnd();
}

// 텍스처 큐브(기본적으로 흰색으로 그려 텍스처 색이 그대로 보이게 함)
// 주의: 이 버전은 법선을 설정하지 않아 조명(LIGHTING)이 켜져 있으면 밝기 결과가 일정하지 않을 수 있다.
void OglCube::Draw(GLfloat len, GLuint texId) const
{
    glBindTexture(GL_TEXTURE_2D, texId);

    glBegin(GL_QUADS);

    glColor3f(1.f, 1.f, 1.f);

    // front (+Z)
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-len, -len, len);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(len, -len, len);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(len, len, len);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-len, len, len);

    // back (-Z)
    glTexCoord2f(0.0f, 0.0f); glVertex3f(len, -len, -len);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-len, -len, -len);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-len, len, -len);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(len, len, -len);

    // right (+X)
    glTexCoord2f(0.0f, 0.0f); glVertex3f(len, -len, len);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(len, -len, -len);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(len, len, -len);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(len, len, len);

    // left (-X)
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-len, -len, -len);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-len, -len, len);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-len, len, len);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-len, len, -len);

    // top (+Y)
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-len, len, len);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(len, len, len);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(len, len, -len);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-len, len, -len);

    // bottom (-Y)
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-len, -len, -len);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(len, -len, -len);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(len, -len, len);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-len, -len, len);

    glEnd();
}

// Transform(이동/회전/스케일)을 적용한 뒤 큐브를 렌더링
void OglCube::Draw(const OglTransform& ot)
{
    glPushMatrix();

    ot.Transform();
    SetRgba(ot.m_nColor, ot.m_alpha);
    DrawDef();

    glPopMatrix();
}

void OglCube::DrawDef(void)
{
    Draw(m_defHalfLen);
}
