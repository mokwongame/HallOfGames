// ObjFile.h
// OBJ 파일을 읽어 삼각형(3정점) 단위로 변환하는 로더 클래스

#pragma once

#include <fstream>
#include "Vector3.h"

// 삼각형 1개를 구성하는 정점 3개
struct Vertex3
{
    Vector3 pt1;
    Vector3 pt2;
    Vector3 pt3;
};

typedef Vertex3* PVertex3;

// OBJ 파일 로더
// - v / f 형식만 처리
// - 결과는 삼각형(Vertex3) 배열로 제공
class ObjFile
{
public:
    ObjFile(void);
    ~ObjFile();

    // 삼각형 배열 접근
    const PVertex3 GetVtx3Ptr(void) const { return m_pVtx3; }
    int GetVtx3Size(void) const { return m_nVtx3; }

    // OBJ 파일 내 요소 개수 파악
    void GetSizeObj(
        int& nVertex,
        int& nLine,
        int& nFace,
        const CString& sPathFile
    );

    // OBJ 파일 로드 및 삼각형 배열 생성
    void LoadObj(const CString& sPathFile);

    // 디버그용: 로드된 삼각형 정보 출력
    void PrintVtx3Msg(void) const;

protected:
    // 삼각형 배열
    Vertex3* m_pVtx3 = nullptr;
    int      m_nVtx3 = 0;

    // 내부 메모리 정리
    void QuitVtx3Ptr(void);

    // 스트림 파싱 보조 함수
    void StmEatWhite(std::wistream& stream);
    void StmEatLine(std::wistream& stream);
};
