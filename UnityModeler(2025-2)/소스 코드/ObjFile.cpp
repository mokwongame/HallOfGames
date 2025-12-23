// ObjFile.cpp

#include "pch.h"
#include "ObjFile.h"

ObjFile::ObjFile(void)
{
    // face(삼각형) 리스트를 Vertex3 배열로 보관
    m_pVtx3 = nullptr;
    m_nVtx3 = 0;
}

ObjFile::~ObjFile()
{
    QuitVtx3Ptr();
}

// OBJ 파일에서 v/l/f 개수를 먼저 세어서 배열 크기를 잡는다
void ObjFile::GetSizeObj(int& nVertex, int& nLine, int& nFace, const CString& sPathFile)
{
    using namespace std;

    nVertex = nLine = nFace = 0;

    // ios::_Nocreate: 파일이 없으면 생성하지 않음(열기 실패 처리용)
    wifstream file(sPathFile, ios::_Nocreate);

    while (1)
    {
        StmEatWhite(file);
        if (file.eof()) break;

        TCHAR ch = file.get();
        if (ch == 'v' || ch == 'V')      nVertex++;
        else if (ch == 'l' || ch == 'L') nLine++;
        else if (ch == 'f' || ch == 'F') nFace++;
        else
            StmEatLine(file);
    }
}

// OBJ 파일을 읽어 삼각형(face)들을 Vertex3 배열로 변환해 저장한다
void ObjFile::LoadObj(const CString& sPathFile)
{
    using namespace std;

    int nMaxVtx, nMaxLine, nMaxFace;
    GetSizeObj(nMaxVtx, nMaxLine, nMaxFace, sPathFile);

    // vertex 또는 face가 없으면 렌더링 데이터가 성립하지 않는다
    if (nMaxVtx == 0 || nMaxFace == 0)
    {
        QuitVtx3Ptr();
        return;
    }

    Vector3* pVtx = nullptr;   // v 리스트
    Point2* pLine = nullptr;  // l 리스트(현재는 렌더링에 사용하지 않음)
    Point3* pFace = nullptr;  // f 리스트(정점 인덱스 3개)

    if (nMaxVtx)  pVtx = new Vector3[nMaxVtx];
    if (nMaxLine) pLine = new Point2[nMaxLine];
    if (nMaxFace) pFace = new Point3[nMaxFace];

    LONG64 nVtx = 0, nLine = 0, nFace = 0;

    wifstream file(sPathFile, ios::_Nocreate);
    while (1)
    {
        StmEatWhite(file);
        if (file.eof()) break;

        TCHAR ch = file.get();

        if (ch == 'v' || ch == 'V')
        {
            // v x y z
            GLfloat pos;
            file >> pos; GLfloat x = pos;
            file >> pos; GLfloat y = pos;
            file >> pos; GLfloat z = pos;

            pVtx[nVtx] = Vector3(x, y, z);
            nVtx++;
        }
        else if (ch == 'l' || ch == 'L')
        {
            // l i j (정점 인덱스 2개)
            int nNode;
            file >> nNode; int x = nNode;
            file >> nNode; int y = nNode;

            pLine[nLine] = Point2(x, y);
            nLine++;
        }
        else if (ch == 'f' || ch == 'F')
        {
            // f i j k (정점 인덱스 3개)
            // 주의: 이 구현은 "f 1 2 3" 형태만 처리한다
            // "f 1/1/1 2/2/2 3/3/3" 같은 표준 OBJ 포맷은 별도 파싱이 필요
            int nNode;
            file >> nNode; int x = nNode;
            file >> nNode; int y = nNode;
            file >> nNode; int z = nNode;

            pFace[nFace] = Point3(x, y, z);
            nFace++;
        }
        else
        {
            StmEatLine(file);
        }
    }

    // face를 실제 좌표 삼각형(Vertex3) 배열로 변환해서 보관
    if (m_pVtx3) delete[] m_pVtx3;

    m_pVtx3 = new Vertex3[nMaxFace];
    m_nVtx3 = nMaxFace;

    for (int i = 0; i < nMaxFace; i++)
    {
        Vertex3 vtx3;
        const Point3 node = pFace[i];

        // OBJ 인덱스는 1부터 시작하므로 -1 보정
        vtx3.pt1 = pVtx[node.x - 1];
        vtx3.pt2 = pVtx[node.y - 1];
        vtx3.pt3 = pVtx[node.z - 1];

        m_pVtx3[i] = vtx3;
    }

    if (pVtx)  delete[] pVtx;
    if (pLine) delete[] pLine;
    if (pFace) delete[] pFace;
}

// 디버그용: 현재 삼각형 데이터를 메시지 박스로 출력
void ObjFile::PrintVtx3Msg(void) const
{
    CString sMsg;
    const PVertex3 ptr = GetVtx3Ptr();

    for (int i = 0; i < GetVtx3Size(); i++)
    {
        CString str;
        const Vertex3 vtx3 = ptr[i];

        str.Format(_T("[%d face]\n"), i);
        sMsg += str;

        str.Format(_T("pt1: (%g, %g, %g)\n"), vtx3.pt1.x, vtx3.pt1.y, vtx3.pt1.z);
        sMsg += str;

        str.Format(_T("pt2: (%g, %g, %g)\n"), vtx3.pt2.x, vtx3.pt2.y, vtx3.pt2.z);
        sMsg += str;

        str.Format(_T("pt3: (%g, %g, %g)\n"), vtx3.pt3.x, vtx3.pt3.y, vtx3.pt3.z);
        sMsg += str;
    }

    AfxMessageBox(sMsg);
}

// Vertex3 배열 해제
void ObjFile::QuitVtx3Ptr(void)
{
    if (m_pVtx3)
    {
        delete[] m_pVtx3;
        m_pVtx3 = nullptr;
        m_nVtx3 = 0;
    }
}

// 공백/개행을 스킵하고 다음 토큰 시작 위치로 이동
void ObjFile::StmEatWhite(std::wistream& stream)
{
    TCHAR ch;
    while ((ch = stream.peek()) != (TCHAR)EOF &&
        (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r'))
    {
        stream.ignore();
    }
}

// 현재 줄 끝까지 읽어서 버린다
void ObjFile::StmEatLine(std::wistream& stream)
{
    while (1)
    {
        if (stream.eof()) break;

        TCHAR ch = stream.get();
        if (ch == '\n') break;

        if (ch == '\r')
        {
            if (stream.peek() == '\n') stream.ignore();
            break;
        }
    }
}
