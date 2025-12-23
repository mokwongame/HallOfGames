// MyScreen.cpp

#include "pch.h"
#include "MyScreen.h"
#include <cmath>

// Transform에 저장된 COLORREF/alpha를 OpenGL 현재 색으로 적용
static void SetColorFromTransform(const OglTransform& ot)
{
    GLfloat r = GetRValue(ot.m_nColor) / 255.0f;
    GLfloat g = GetGValue(ot.m_nColor) / 255.0f;
    GLfloat b = GetBValue(ot.m_nColor) / 255.0f;
    glColor4f(r, g, b, ot.m_alpha);
}

// 삼각형 3점으로 면 법선 계산(OBJ 렌더링에서 사용)
static void CalcNormal(const Vector3& a, const Vector3& b, const Vector3& c, GLfloat& nx, GLfloat& ny, GLfloat& nz)
{
    const GLfloat ux = b.x - a.x;
    const GLfloat uy = b.y - a.y;
    const GLfloat uz = b.z - a.z;

    const GLfloat vx = c.x - a.x;
    const GLfloat vy = c.y - a.y;
    const GLfloat vz = c.z - a.z;

    nx = uy * vz - uz * vy;
    ny = uz * vx - ux * vz;
    nz = ux * vy - uy * vx;

    const GLfloat len = std::sqrt(nx * nx + ny * ny + nz * nz);
    if (len > 1e-6f) { nx /= len; ny /= len; nz /= len; }
    else { nx = 0.f; ny = 0.f; nz = 1.f; }
}

// 범위 제한
static float Clampf(float v, float lo, float hi)
{
    return (v < lo) ? lo : (v > hi) ? hi : v;
}

// COLORREF + alpha -> OpenGL rgba(float)
static void ToRGBA(COLORREF c, float a, GLfloat& r, GLfloat& g, GLfloat& b, GLfloat& aa)
{
    r = GetRValue(c) / 255.0f;
    g = GetGValue(c) / 255.0f;
    b = GetBValue(c) / 255.0f;
    aa = a;
}

MyScreen::MyScreen(void)
{
    // 기본 배경색/알파
    SetBackColor(RGB(127, 127, 127));
    SetBackAlpha(1.0f);
}

// 오브젝트 목록에 추가(Inspector/리스트와 연동되는 실제 데이터)
void MyScreen::AddGameObject(OglTransform ot)
{
    m_arGameObj.Add(ot);
}

// 읽기용: Inspector 채우기
bool MyScreen::GetObject(int idx, OglTransform& out) const
{
    if (idx < 0 || idx >= m_arGameObj.GetCount()) return false;
    out = m_arGameObj[idx];
    return true;
}

// 쓰기용 포인터 접근(필요 시 직접 수정)
OglTransform* MyScreen::GetObjectW(int idx)
{
    if (idx < 0 || idx >= m_arGameObj.GetCount()) return nullptr;
    return &m_arGameObj[idx];
}

// 값 기반 Set(Apply Transform에서 사용)
bool MyScreen::SetObject(int idx, const OglTransform& in)
{
    if (idx < 0 || idx >= m_arGameObj.GetCount()) return false;
    m_arGameObj[idx] = in;
    return true;
}

// 라이트/재질 변경은 OpenGL 컨텍스트 내에서 적용해야 한다
void MyScreen::ApplyLightMaterial(
    bool bDirectional,
    COLORREF amb, COLORREF dif, COLORREF spc,
    float lx, float ly, float lz,
    COLORREF emission,
    float shininess
)
{
    StartRC();

    m_lightMtl.SetDirectional(bDirectional);
    m_lightMtl.SetLightColors(amb, dif, spc);
    m_lightMtl.SetLightPos(lx, ly, lz);
    m_lightMtl.SetMaterial(emission, shininess);
    m_lightMtl.Apply();

    StopRC();
}

// OpenGL 초기화(컨텍스트 생성 후 1회)
void MyScreen::InitOpenGL(void)
{
    OglScreen::InitOpenGL();

    StartRC();

    m_lightMtl.SetDCRC(m_hDC, m_hRC);
    m_lightMtl.SetDefault();
    m_lightMtl.Apply();

    glShadeModel(GL_SMOOTH);

    m_sphere.Create();
    m_cylinder.Create();

    StopRC();
}

// Orbit 카메라를 LookAt으로 적용(Z-up)
void MyScreen::ApplyCamera() const
{
    const float yaw = m_yawDeg * 3.1415926f / 180.f;
    const float pitch = m_pitchDeg * 3.1415926f / 180.f;

    const float cp = cosf(pitch);
    const float sp = sinf(pitch);
    const float cy = cosf(yaw);
    const float sy = sinf(yaw);

    const float camX = m_target.x + m_distance * cp * cy;
    const float camY = m_target.y + m_distance * cp * sy;
    const float camZ = m_target.z + m_distance * sp;

    gluLookAt(
        camX, camY, camZ,
        m_target.x, m_target.y, m_target.z,
        0.0, 0.0, 1.0
    );
}

void MyScreen::OnCameraBegin(UINT nFlags, CPoint pt)
{
    m_camDragging = true;
    m_prevMouse = pt;
}

void MyScreen::OnCameraEnd(UINT nFlags, CPoint pt)
{
    m_camDragging = false;
}

// 좌클릭: Orbit / 우클릭: Pan / 휠: Zoom
void MyScreen::OnCameraDrag(UINT nFlags, CPoint pt)
{
    if (!m_camDragging) return;

    const int dx = pt.x - m_prevMouse.x;
    const int dy = pt.y - m_prevMouse.y;
    m_prevMouse = pt;

    if (nFlags & MK_LBUTTON)
    {
        m_yawDeg += dx * 0.4f;
        m_pitchDeg -= dy * 0.4f;
        m_pitchDeg = Clampf(m_pitchDeg, -85.f, 85.f);
    }
    else if (nFlags & MK_RBUTTON)
    {
        const float panScale = (m_distance / 450.f) * 0.8f;

        const float yaw = m_yawDeg * 3.1415926f / 180.f;
        Vector3 right(cosf(yaw), sinf(yaw), 0.f);
        Vector3 up(0.f, 0.f, 1.f);

        m_target.x -= right.x * dx * panScale;
        m_target.y -= right.y * dx * panScale;
        m_target.z += up.z * dy * panScale;
    }
}

void MyScreen::OnCameraWheel(short zDelta, CPoint pt)
{
    const float step = (zDelta > 0) ? -30.f : 30.f;
    m_distance += step;
    m_distance = Clampf(m_distance, 80.f, 2000.f);
}

// Grid/Axis는 라이트 영향을 받지 않게 LIGHTING을 껐다 켠다
void MyScreen::DrawGrid() const
{
    glDisable(GL_LIGHTING);

    glLineWidth(1.0f);

    const int   half = 20;
    const float step = 25.f;

    glColor4f(0.20f, 0.20f, 0.20f, 1.f);
    glBegin(GL_LINES);
    for (int i = -half; i <= half; ++i)
    {
        const float v = i * step;
        glVertex3f(v, -half * step, 0.f);
        glVertex3f(v, half * step, 0.f);

        glVertex3f(-half * step, v, 0.f);
        glVertex3f(half * step, v, 0.f);
    }
    glEnd();

    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glColor4f(1.f, 0.f, 0.f, 1.f); glVertex3f(0.f, 0.f, 0.f); glVertex3f(200.f, 0.f, 0.f);
    glColor4f(0.f, 1.f, 0.f, 1.f); glVertex3f(0.f, 0.f, 0.f); glVertex3f(0.f, 200.f, 0.f);
    glColor4f(0.f, 0.f, 1.f, 1.f); glVertex3f(0.f, 0.f, 0.f); glVertex3f(0.f, 0.f, 200.f);
    glEnd();

    glEnable(GL_LIGHTING);
}

// 오브젝트가 하나도 없을 때만 임시로 샘플 도형들을 렌더링한다
// 실제 m_arGameObj에 추가하지 않고 "그리기만" 한다
void MyScreen::SampleMultiGameObj(void)
{
    CArray<OglTransform, OglTransform&> ar;
    OglTransform ot;

    ot.SetDef();
    ot.m_nType = GameObjType::SPHERE;
    ot.m_nColor = RGB(255, 0, 0);
    ot.m_alpha = 1.f;
    ot.m_translate = Vector3(-120.f, 80.f, 30.f);
    ot.m_scale = Vector3(5.f, 5.f, 5.f);
    ar.Add(ot);

    ot.SetDef();
    ot.m_nType = GameObjType::CUBE;
    ot.m_nColor = RGB(0, 200, 255);
    ot.m_alpha = 1.f;
    ot.m_translate = Vector3(0.f, 80.f, 30.f);
    ot.m_scale = Vector3(4.f, 4.f, 4.f);
    ot.m_rotate = Vector3(25.f, 35.f, 0.f);
    ar.Add(ot);

    ot.SetDef();
    ot.m_nType = GameObjType::CYLINDER;
    ot.m_nColor = RGB(255, 0, 255);
    ot.m_alpha = 1.f;
    ot.m_translate = Vector3(120.f, 80.f, 30.f);
    ot.m_scale = Vector3(4.f, 4.f, 4.f);
    ot.m_rotate = Vector3(70.f, 35.f, 0.f);
    ar.Add(ot);

    ot.SetDef();
    ot.m_nType = GameObjType::QUAD;
    ot.m_nColor = RGB(0, 255, 0);
    ot.m_alpha = 1.f;
    ot.m_translate = Vector3(-80.f, -60.f, 1.f);
    ot.m_scale = Vector3(6.f, 6.f, 1.f);
    ar.Add(ot);

    ot.SetDef();
    ot.m_nType = GameObjType::TRI;
    ot.m_nColor = RGB(255, 255, 0);
    ot.m_alpha = 1.f;
    ot.m_translate = Vector3(40.f, -60.f, 1.f);
    ot.m_scale = Vector3(6.f, 6.f, 1.f);
    ar.Add(ot);

    for (int i = 0; i < ar.GetCount(); i++)
    {
        const OglTransform& e = ar[i];
        switch (e.m_nType)
        {
        case GameObjType::SPHERE:   m_sphere.Draw(e);   break;
        case GameObjType::CUBE:     m_cube.Draw(e);     break;
        case GameObjType::CYLINDER: m_cylinder.Draw(e); break;
        case GameObjType::QUAD:     DrawQuad(e);        break;
        case GameObjType::TRI:      DrawTri(e);         break;
        default: break;
        }
    }
}

void MyScreen::DrawQuad(const OglTransform& ot)
{
    glPushMatrix();
    ot.Transform();

    SetColorFromTransform(ot);

    const GLfloat s = 20.f;

    glBegin(GL_QUADS);
    glNormal3f(0.f, 0.f, 1.f);
    glVertex3f(-s, -s, 0.f);
    glVertex3f(s, -s, 0.f);
    glVertex3f(s, s, 0.f);
    glVertex3f(-s, s, 0.f);
    glEnd();

    glPopMatrix();
}

void MyScreen::DrawTri(const OglTransform& ot)
{
    glPushMatrix();
    ot.Transform();

    SetColorFromTransform(ot);

    const GLfloat s = 25.f;

    glBegin(GL_TRIANGLES);
    glNormal3f(0.f, 0.f, 1.f);
    glVertex3f(0.f, s, 0.f);
    glVertex3f(-s, -s, 0.f);
    glVertex3f(s, -s, 0.f);
    glEnd();

    glPopMatrix();
}

// OBJ는 매 프레임 로드해서 그린다(간단 구현).
// 과제 품질을 올리려면 로드/캐싱 구조로 분리하는 게 더 자연스럽다.
void MyScreen::DrawObj(const OglTransform& ot)
{
    if (ot.m_objPath.IsEmpty()) return;

    ObjFile obj;
    obj.LoadObj(ot.m_objPath);

    const PVertex3 tris = obj.GetVtx3Ptr();
    const int triCount = obj.GetVtx3Size();
    if (!tris || triCount <= 0) return;

    glPushMatrix();
    ot.Transform();

    SetColorFromTransform(ot);

    glBegin(GL_TRIANGLES);
    for (int i = 0; i < triCount; ++i)
    {
        const Vertex3& t = tris[i];

        GLfloat nx, ny, nz;
        CalcNormal(t.pt1, t.pt2, t.pt3, nx, ny, nz);
        glNormal3f(nx, ny, nz);

        glVertex3f(t.pt1.x, t.pt1.y, t.pt1.z);
        glVertex3f(t.pt2.x, t.pt2.y, t.pt2.z);
        glVertex3f(t.pt3.x, t.pt3.y, t.pt3.z);
    }
    glEnd();

    glPopMatrix();
}

// MyScreen이 배경을 직접 clear한다.
// base(OglScreen)에서 clear를 해버리면 파생에서 설정한 배경색이 덮어씌워질 수 있다.
void MyScreen::RenderScene(void)
{
    GLfloat r, g, b, a;
    ToRGBA(m_nBackColor, m_backAlpha, r, g, b, a);

    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    ApplyCamera();

    if (m_bGrid)
        DrawGrid();

    if (m_arGameObj.GetCount() > 0)
    {
        for (int i = 0; i < m_arGameObj.GetCount(); ++i)
        {
            OglTransform e = m_arGameObj[i];
            switch (e.m_nType)
            {
            case GameObjType::SPHERE:   m_sphere.Draw(e);   break;
            case GameObjType::CUBE:     m_cube.Draw(e);     break;
            case GameObjType::CYLINDER: m_cylinder.Draw(e); break;
            case GameObjType::QUAD:     DrawQuad(e);        break;
            case GameObjType::TRI:      DrawTri(e);         break;
            case GameObjType::OBJ:      DrawObj(e);         break;
            default: break;
            }
        }
    }
    else
    {
        SampleMultiGameObj();
    }
}
