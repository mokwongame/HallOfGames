// OglTransform.h
// 게임 오브젝트의 타입, 색상, 변환(Transform) 정보를 담는 구조 클래스

#pragma once

#include <afxstr.h>     // CString
#include "Vector3.h"

// 게임 오브젝트 종류
enum class GameObjType
{
    NULL_DEF = 0,
    SPHERE,
    CUBE,
    CYLINDER,
    QUAD,
    TRI,
    PLANE,
    OBJ
};

// 게임 오브젝트의 변환 정보 클래스
// - 위치(Translate)
// - 회전(Rotate)
// - 크기(Scale)
// - 색상/알파
class OglTransform
{
public:
    OglTransform(void);

    // 기본값으로 초기화
    void SetDef(void);

    // 개별 변환
    void Translate(void) const;
    void Rotate(void) const;
    void Scale(void) const;

    // 변환 적용
    // Scale -> Rotate -> Translate 순서
    void Transform(void) const;

public:
    // 오브젝트 타입
    GameObjType m_nType;

    // 색상 및 투명도
    COLORREF m_nColor;
    GLfloat  m_alpha;

    // 변환 값
    Vector3 m_translate; // 위치
    Vector3 m_rotate;    // 회전 각도
    Vector3 m_scale;     // 스케일

    // OBJ 전용: 모델 파일 경로
    // 여러 OBJ를 동시에 다루기 위해 Transform 단위로 보관
    CString m_objPath;
};
