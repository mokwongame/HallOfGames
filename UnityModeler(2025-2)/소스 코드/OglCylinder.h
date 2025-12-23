// OglCylinder.h
// 원기둥(OpenGL GLU Quadric) 렌더링 클래스

#pragma once
#include "OglTransform.h"

// 기본 분할 수 (원기둥 품질 조절용)
#define DEF_CYL_SLICE  (30)
#define DEF_CYL_STACK  (30)

// 원기둥 렌더링 클래스
class OglCylinder
{
public:
    OglCylinder(void);
    ~OglCylinder();

    // 색상/알파 설정
    void SetRgba(COLORREF col, GLfloat alpha = 1.0f)
    {
        m_nColor = col;
        m_alpha = alpha;
    }

    // GLU Quadric 객체 생성 (초기화 시 1회 호출)
    void Create(void);

    // 반지름/높이 지정 렌더링
    // slice, stack 값이 클수록 표면이 부드러움
    void Draw(
        GLdouble radius,
        GLdouble ht,
        GLint slice = DEF_CYL_SLICE,
        GLint stack = DEF_CYL_STACK
    ) const;

    // Transform 적용 후 기본 크기로 렌더링
    void Draw(const OglTransform& ot);

    // 기본 반지름(m_defRad) 기준 렌더링
    void DrawDef(void);

protected:
    // GLU Quadric 객체
    GLUquadric* m_pQobj = nullptr;

    // 색상/알파
    COLORREF m_nColor;
    GLfloat  m_alpha;

    // 기본 반지름 (지름 = m_defRad * 2)
    const GLdouble m_defRad = 20.;
};

// 매크로 범위 제한
#undef DEF_CYL_SLICE
#undef DEF_CYL_STACK
