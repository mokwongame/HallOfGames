// OglCube.h
// 정육면체 렌더링 클래스 (색상/알파, 텍스처 지원)

#pragma once
#include <GL/glew.h>
#include "OglTransform.h"

// 정육면체(OpenGL Immediate Mode 기반)
class OglCube
{
public:
    OglCube(void);

    // 면별 색상/알파 설정
    void SetRgba(const COLORREF arColor[], const GLfloat arAlpha[]);

    // 모든 면에 동일한 색상/알파 적용
    void SetRgba(COLORREF col, GLfloat alpha = 1.0f);

    // 기본 색상 설정(면마다 다른 RGB)
    void SetRgbaDef(void);

    // 색상만 사용한 렌더링
    // 실제 한 변의 길이 = halfLen * 2
    void Draw(GLfloat halfLen) const;

    // 텍스처를 적용한 렌더링
    // 실제 한 변의 길이 = halfLen * 2
    void Draw(GLfloat halfLen, GLuint texId) const;

    // Transform 적용 후 기본 크기로 렌더링
    void Draw(const OglTransform& ot);

    // 기본 크기(m_defHalfLen)로 렌더링
    void DrawDef(void);

protected:
    // 6면 각각의 색상과 알파
    COLORREF m_arColor[6];
    GLfloat  m_arAlpha[6];

    // 기본 반길이 (정육면체 한 변 = m_defHalfLen * 2)
    const GLfloat m_defHalfLen = 20.f;
};
