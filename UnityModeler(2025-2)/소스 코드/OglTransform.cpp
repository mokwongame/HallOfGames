// OglTransform.cpp
// 오브젝트의 변환 정보(이동/회전/스케일)를 관리하는 클래스
// OpenGL의 행렬 스택에 Transform을 적용하는 역할만 담당한다

#include "pch.h"
#include "OglTransform.h"

OglTransform::OglTransform(void)
{
    SetDef();
}

// Transform 기본값 설정
void OglTransform::SetDef(void)
{
    m_nType = GameObjType::NULL_DEF;
    m_nColor = RGB(0, 0, 0);
    m_alpha = 1.0f;

    m_translate = Vector3(0.f, 0.f, 0.f);
    m_rotate = Vector3(0.f, 0.f, 0.f);
    m_scale = Vector3(1.f, 1.f, 1.f);
}

// 이동 변환 적용
void OglTransform::Translate(void) const
{
    glTranslatef(m_translate.x, m_translate.y, m_translate.z);
}

// 회전 변환 적용(X → Y → Z 순서)
void OglTransform::Rotate(void) const
{
    glRotatef(m_rotate.x, 1.f, 0.f, 0.f);
    glRotatef(m_rotate.y, 0.f, 1.f, 0.f);
    glRotatef(m_rotate.z, 0.f, 0.f, 1.f);
}

// 스케일 변환 적용
void OglTransform::Scale(void) const
{
    glScalef(m_scale.x, m_scale.y, m_scale.z);
}

// Transform 적용 순서
// - Scale → Rotate → Translate
// OpenGL의 행렬 누적 특성상 실제 결과는
// Translate → Rotate → Scale처럼 동작하며,
// Unity의 Transform 개념과 동일한 효과를 만든다
void OglTransform::Transform(void) const
{
    Scale();
    Rotate();
    Translate();
}
