#pragma once
#include <afxwin.h>
#include "Wall.h"
#include "Snake.h"
#include "Fruit.h"
#include "resource.h"
#include <afxwin.h>
#include "PoisonFruit.h"
#define SCREEN_TIMER    (1)
#define FPS (60) // frame per second
// #define POISON_FRUIT_TIMER 2  // 독사과 타이머 ID

class GameScreen : public CStatic
{
public:
    GameScreen();
    void startGame();

    virtual BOOL Create(LPCTSTR lpszText, DWORD dwStyle, 
                        const RECT& rect, CWnd* pParentWnd, UINT nID = 0xffff);

    int getScore(void)const
    {
        return m_nScore;
    }

    void setSnakeDir(SnakeDir nDir)
    {
        m_snake.setDir(nDir);
    }
    void drawScore(CDC* pDC);

    void inScore(void)
    {
        m_nScore++;
    }

    void resetGame();

    void DrawBitmapWithMask(CDC* pDC, int x, int y, int width, int height, UINT maskID, UINT bitmapID);


    void updatePoisonFruit();     // 독사과 위치 업데이트
    void movePoisonFruit();      // 독사과 이동 로직

protected:
    BOOL m_bGameStarted = FALSE; // 멤버 변수 기본 초기화
    int m_timerInterval = 1000 / FPS; // 기본 값 설정
    int m_nBkWid = 900;
    int m_nBkHt = 635;
    int m_nScore = 0;
    Wall m_wall;
    Snake m_snake;
    Fruit m_fruit;
    bool m_bGameOver = false;
    BOOL m_showLogo;      // 로고 표시 여부
    CBitmap m_logoBitmap; // 로고 비트맵
    void drawGameOver(CDC* pDC);
    const int MIN_INTERVAL = 50; // 최소 타이머 주기 (속도 한계)
// UINT_PTR m_poisonFruitTimerID; // 타이머 ID
    UINT_PTR m_poisonFruitTimerID = 0; // 타이머 ID 초기화
   
    
public:
    DECLARE_MESSAGE_MAP()
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    void DrawBitmap(CDC* pDC, CBitmap* pBitmap);
    afx_msg void OnPaint();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    void stopGame(); // 게임 종료 함수 선언
    void startPoisonMoveTimer(); // 독사과 이동 타이머 시작
    void stopPoisonMoveTimer();  // 독사과 이동 타이머 중지

private:
    PoisonFruit m_poisonFruit;   // 독사과 객체
    int m_poisonSpeedX; // 독사과 X축 이동 속도
    int m_poisonSpeedY; // 독사과 Y축 이동 속도
    UINT_PTR m_poisonMoveTimerID; // 독사과 이동 타이머 ID
};





