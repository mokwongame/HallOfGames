#pragma once

#include "OglScreen.h"
#include "OglLightMtl.h"
#include "OglSphere.h"
#include "OglCube.h"
#include "OglCylinder.h"
#include "OglTransform.h"
#include "ObjFile.h"
#include "Vector3.h"

#include <afxtempl.h>
#include <GL/GLU.h>

// OpenGL 렌더링을 담당하는 메인 Screen 클래스
// - GameObject 관리
// - Grid 표시
// - Light / Material 적용
// - 3단계 카메라 조작(Orbit / Pan / Zoom)
class MyScreen : public OglScreen
{
public:
    MyScreen(void);

    // GameObject 추가
    void AddGameObject(OglTransform ot);

    // Light / Material 설정 적용
    void ApplyLightMaterial(
        bool bDirectional,
        COLORREF amb, COLORREF dif, COLORREF spc,
        float lx, float ly, float lz,
        COLORREF emission,
        float shininess
    );

    // Grid 표시 제어
    void SetGridEnabled(bool b) { m_bGrid = b; }
    bool GetGridEnabled() const { return m_bGrid; }

    // 선택된 오브젝트 인덱스 설정(Inspector 연동)
    void SelectObject(int idx) { m_selectedIndex = idx; }

    // GameObject 접근 API
    int  GetObjectCount() const { return (int)m_arGameObj.GetCount(); }
    bool GetObject(int idx, OglTransform& out) const;
    OglTransform* GetObjectW(int idx);               // 수정용 접근
    bool SetObject(int idx, const OglTransform& in); // Apply Transform용

protected:
    // ===== 렌더링 리소스 =====
    OglLightMtl m_lightMtl;
    OglSphere   m_sphere;
    OglCube     m_cube;
    OglCylinder m_cylinder;

    // GameObject 목록
    CArray<OglTransform, OglTransform&> m_arGameObj;

    // ===== Grid =====
    bool m_bGrid = true;

    // ===== 선택 상태 =====
    int m_selectedIndex = -1;

    // ===== 3단계: 카메라 상태 =====
    bool   m_camDragging = false;
    CPoint m_prevMouse = CPoint(0, 0);

    float   m_yawDeg = 35.f;
    float   m_pitchDeg = 25.f;
    float   m_distance = 450.f;
    Vector3 m_target = Vector3(0.f, 0.f, 0.f);

    // 카메라 변환 적용(매 프레임 1회)
    void ApplyCamera() const;

    // 카메라 입력 처리
    virtual void OnCameraBegin(UINT nFlags, CPoint pt) override;
    virtual void OnCameraDrag(UINT nFlags, CPoint pt) override;
    virtual void OnCameraEnd(UINT nFlags, CPoint pt) override;
    virtual void OnCameraWheel(short zDelta, CPoint pt) override;

    // ===== 기본 샘플 오브젝트 =====
    void SampleMultiGameObj(void);

    // ===== 보조 렌더링 =====
    void DrawGrid() const;
    void DrawQuad(const OglTransform& ot);
    void DrawTri(const OglTransform& ot);
    void DrawObj(const OglTransform& ot);

    // OpenGL 초기화 및 렌더링
    virtual void InitOpenGL(void) override;
    virtual void RenderScene(void) override;
};
