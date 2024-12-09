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
// #define POISON_FRUIT_TIMER 2  // ����� Ÿ�̸� ID

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


    void updatePoisonFruit();     // ����� ��ġ ������Ʈ
    void movePoisonFruit();      // ����� �̵� ����

protected:
    BOOL m_bGameStarted = FALSE; // ��� ���� �⺻ �ʱ�ȭ
    int m_timerInterval = 1000 / FPS; // �⺻ �� ����
    int m_nBkWid = 900;
    int m_nBkHt = 635;
    int m_nScore = 0;
    Wall m_wall;
    Snake m_snake;
    Fruit m_fruit;
    bool m_bGameOver = false;
    BOOL m_showLogo;      // �ΰ� ǥ�� ����
    CBitmap m_logoBitmap; // �ΰ� ��Ʈ��
    void drawGameOver(CDC* pDC);
    const int MIN_INTERVAL = 50; // �ּ� Ÿ�̸� �ֱ� (�ӵ� �Ѱ�)
// UINT_PTR m_poisonFruitTimerID; // Ÿ�̸� ID
    UINT_PTR m_poisonFruitTimerID = 0; // Ÿ�̸� ID �ʱ�ȭ
   
    
public:
    DECLARE_MESSAGE_MAP()
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    void DrawBitmap(CDC* pDC, CBitmap* pBitmap);
    afx_msg void OnPaint();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    void stopGame(); // ���� ���� �Լ� ����
    void startPoisonMoveTimer(); // ����� �̵� Ÿ�̸� ����
    void stopPoisonMoveTimer();  // ����� �̵� Ÿ�̸� ����

private:
    PoisonFruit m_poisonFruit;   // ����� ��ü
    int m_poisonSpeedX; // ����� X�� �̵� �ӵ�
    int m_poisonSpeedY; // ����� Y�� �̵� �ӵ�
    UINT_PTR m_poisonMoveTimerID; // ����� �̵� Ÿ�̸� ID
};





