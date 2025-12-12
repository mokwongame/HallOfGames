#include "pch.h"
#include "MyScreen.h"
#include "MemoryDC.h"

#define TIMERID_RENDER	(1)

MyScreen::MyScreen(void)
{
	m_fps = 100.;
	m_nDeltaTime = int(1000. / m_fps);

	m_nBackColor = RGB(63, 63, 63);
	
	m_pEnemy = nullptr;	// null pointer
	m_pManhole = nullptr;
	m_pWater = nullptr;
	m_pDrink = nullptr;

	m_nEnemyDistMargin = 50;
	m_bGameOver = false;
	m_score = 0.0;
	
	m_nHP = MAX_HP;
}

MyScreen::~MyScreen()
{
	if (m_pEnemy) delete m_pEnemy;
	if (m_pManhole) delete m_pManhole;
	if (m_pWater)   delete m_pWater;
	if (m_pDrink)   delete m_pDrink;
}

void MyScreen::ResetGame()
{
	// 기본 상태 리셋
	m_bGameOver = false;
	m_score = 0.0;
	m_nHP = MAX_HP;   // 체력 풀피로

	// 도로/플레이어 위치 리셋
	CRect rect;
	GetClientRect(rect);
	m_road.SetRect(rect);
	m_player.SetPtStart(rect);

	// 적/장애물 포인터 정리
	if (m_pEnemy) { delete m_pEnemy;   m_pEnemy = nullptr; }
	if (m_pManhole) { delete m_pManhole; m_pManhole = nullptr; }
	if (m_pWater) { delete m_pWater;   m_pWater = nullptr; }
	if (m_pDrink) { delete m_pDrink;   m_pDrink = nullptr; }

	// 새로 생성
	CreateEnemy();
	CreateManhole();
	CreateWater();
	CreateDrink();
}

BEGIN_MESSAGE_MAP(MyScreen, BaseScreen)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

void MyScreen::CreateEnemy(void)
{
	if (m_pEnemy) delete m_pEnemy;
	m_pEnemy = new Enemy;
	m_pEnemy->SetPtStart(m_road.GetSafeRect());
}

void MyScreen::MoveEnemy(void)
{
	// 상대 속도: Player 속도 - Enemy 속도
	if (m_pEnemy) m_pEnemy->Move(0, m_road.GetSpeed()- m_pEnemy -> GetSpeed());
}

void MyScreen::CheckEnemy(void)
{
	if (m_pEnemy) 
	{
		if (m_pEnemy->Top() >= m_road.Bottom() + m_nEnemyDistMargin)	// 추월 성공 -> 득점
		{
			delete m_pEnemy;
			m_pEnemy = nullptr;
			m_score += 100.;
		}
		else if (m_pEnemy->Bottom() < m_road.Top() - m_nEnemyDistMargin)	// 추월 실패 -> 감점
		{
			delete m_pEnemy;
			m_pEnemy = nullptr;
			m_score -= 100.;
			if (m_score < 0.) m_score = 0.;
		}
	} 
}

bool MyScreen::HitTest(void) const
{
	if (m_pEnemy == nullptr) return false;
	else
	{
		return m_player.HitTest(*m_pEnemy) || m_pEnemy->HitTest(m_player);
	}
}

void MyScreen::DrawScore(CDC* pDC) const
{
	CFont font;
	font.CreatePointFont(150, _T("맑은 고딕"));
	CString str;
	str.Format(_T("Score = %g"), m_score);
	CFont* pOldFont = pDC->SelectObject(&font);
	COLORREF nTextCol = pDC->SetTextColor(RGB(255, 128, 0));
	COLORREF nBkCol = pDC->SetBkColor(RGB(63, 63, 63));
	pDC->TextOut(50, 50, str);
	pDC->SelectObject(pOldFont);
	pDC->SetTextColor(nTextCol);
	pDC->SetBkColor(nBkCol);

	DrawHpBar(pDC);
}

void MyScreen::DrawHpBar(CDC* pDC) const
{
	int x = 50;
	int y = 80;          // 점수 텍스트 아래쪽
	int barWidth = 200; // 전체 바 길이
	int barHeight = 20;  // 바 높이

	// 배경 바 (빈 HP 영역)
	CRect backRect(x, y, x + barWidth, y + barHeight);
	pDC->FillSolidRect(backRect, RGB(80, 80, 80));  // 회색 배경

	// 남은 HP 비율 계산
	double ratio = 0.0;
	if (MAX_HP > 0)
		ratio = static_cast<double>(m_nHP) / static_cast<double>(MAX_HP);
	if (ratio < 0.0) ratio = 0.0;
	if (ratio > 1.0) ratio = 1.0;

	int hpWidth = static_cast<int>(barWidth * ratio);

	// HP 바
	CRect hpRect(x, y, x + hpWidth, y + barHeight);
	pDC->FillSolidRect(hpRect, RGB(255, 0, 0));

	// 테두리 그리기
	CPen pen(PS_SOLID, 1, RGB(255, 255, 255));
	CPen* pOldPen = pDC->SelectObject(&pen);
	pDC->MoveTo(x, y);
	pDC->LineTo(x + barWidth, y);
	pDC->LineTo(x + barWidth, y + barHeight);
	pDC->LineTo(x, y + barHeight);
	pDC->LineTo(x, y);
	pDC->SelectObject(pOldPen);
}

void MyScreen::CreateManhole(void)
{
	if (m_pManhole) delete m_pManhole;

	m_pManhole = new Manhole;

	CRect safeRect = m_road.GetSafeRect();
	m_pManhole->SetPtStart(safeRect);

	// 50% 확률로 열려있게
	bool bOpen = (RandUni() < 0.5);
	m_pManhole->SetOpen(bOpen);
}

void MyScreen::MoveManhole(void)
{
	if (!m_pManhole) return;

	// 도로가 내려가는 속도만큼 맨홀도 아래로 이동
	m_pManhole->Move(0, m_road.GetSpeed());

	// 화면 아래로 완전히 내려가면 새로 생성
	if (m_pManhole->Top() > m_road.Bottom())
	{
		CRect safeRect = m_road.GetSafeRect();
		m_pManhole->SetPtStart(safeRect);

		bool bOpen = (RandUni() < 0.5);
		m_pManhole->SetOpen(bOpen);
	}
}

void MyScreen::CheckManhole(void)
{
	if (!m_pManhole) return;

	// Player와 맨홀 사각형 겹침 검사
	CRect playerRect = m_player.MakeRect();
	CRect manRect = m_pManhole->MakeRect();
	CRect inter;
	if (!inter.IntersectRect(playerRect, manRect))
		return;

	
	// 닫혀 있으면 그냥 통과
	if (!m_pManhole->IsOpen())
	{
		
	}
	else
	{
		// 열린 맨홀: HP 감소
		if (m_nHP > 0)
		{
			m_score -= 50.0;
			m_nHP--;
		}
			
		// HP가 0 되면 게임 오버 처리
		if (m_nHP <= 0)
		{
			m_nHP = 0;
			m_bGameOver = true;   // Enemy 충돌처럼 게임 종료
		}
	}

	// 한 번 밟으면 맨홀은 새로 리스폰
	CRect safeRect = m_road.GetSafeRect();
	m_pManhole->SetPtStart(safeRect);
	bool bOpen = (RandUni() < 0.5);
	m_pManhole->SetOpen(bOpen);
}

void MyScreen::CreateWater(void)
{
	if (m_pWater)
		delete m_pWater;

	m_pWater = new Water;
	m_pWater->SetPtStart(m_road.GetSafeRect());   // 4레인 중앙 중 랜덤
}

void MyScreen::MoveWater(void)
{
	if (!m_pWater) return;

	// 도로가 내려가는 속도만큼 맨홀도 아래로 이동
	m_pWater->Move(0, m_road.GetSpeed());

	// 화면 아래로 완전히 내려가면 새로 생성
	if (m_pWater->Top() > m_road.Bottom())
	{
		CRect safeRect = m_road.GetSafeRect();
		m_pWater->SetPtStart(safeRect);
	}
}

void MyScreen::CheckWater(void)
{
	if (!m_pWater) return;

	// 겹침 검사
	CRect playerRect = m_player.MakeRect();
	CRect waterRect = m_pWater->MakeRect();
	CRect inter;
	if (!inter.IntersectRect(playerRect, waterRect))
		return;

	// 물웅덩이를 밟으면 50점 획득
	m_score += 10.0;

	// 미끄러짐 효과
	// 0~1 랜덤 뽑아서 왼쪽/오른쪽 결정
	int dir = BaseScreen::RandRange(0, 2); // 0 또는 1

	if (dir == 0)
		m_player.MoveLeft();   // 왼쪽 레인으로 미끄러짐
	else
		m_player.MoveRight();  // 오른쪽 레인으로 미끄러짐

	// 한 번 밟으면 물웅덩이를 새로 리스폰
	CRect safeRect = m_road.GetSafeRect();
	m_pWater->SetPtStart(safeRect);
}

void MyScreen::CreateDrink(void)
{
	if (m_pDrink)
		delete m_pDrink;

	m_pDrink = new Drink;

	CRect safeRect = m_road.GetSafeRect();
	m_pDrink->SetPtStart(safeRect);
}

void MyScreen::MoveDrink(void)
{
	if (!m_pDrink) return;

	// 도로 내려가는 속도만큼 같이 이동
	m_pDrink->Move(0, m_road.GetSpeed());

	// 화면 아래로 완전히 내려가면 새 위치로 리스폰
	if (m_pDrink->Top() > m_road.Bottom())
	{
		CRect safeRect = m_road.GetSafeRect();
		m_pDrink->SetPtStart(safeRect);
	}
}

void MyScreen::CheckDrink(void)
{
	if (!m_pDrink) return;

	CRect playerRect = m_player.MakeRect();
	CRect drinkRect = m_pDrink->MakeRect();
	CRect inter;
	if (!inter.IntersectRect(playerRect, drinkRect))
		return;

	// 체력 회복
	const int MAX_HP = 5;        // 최대 체력 값은 취향대로
	if (m_nHP < MAX_HP)
		m_nHP++;

	// 속도 증가 (도로 속도 ↑ → 체감상 더 빨라짐)
	m_road.IncSpeed();

	// 원하면 점수 보너스도 추가 가능
	 m_score += 100.0;

	//  한 번 먹으면 새 위치로 리스폰
	CRect safeRect = m_road.GetSafeRect();
	m_pDrink->SetPtStart(safeRect);
}

void MyScreen::OnPaint()
{
	CPaintDC viewDc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 BaseScreen::OnPaint()을(를) 호출하지 마십시오.
	MemoryDC dc(&viewDc);
	DrawBack(&dc);
	m_road.Draw(&dc);
	if (!m_bGameOver)
	{
		if (m_pWater)	// 물웅덩이
		{
			m_pWater->Draw(&dc);
		}
			

		if (m_pManhole)	// 맨홀
		{
			m_pManhole->Draw(&dc);
		}

		if (m_pDrink) 
		{
			m_pDrink->Draw(&dc);
		}

		m_player.Draw(&dc);

		if (m_pEnemy == nullptr) CreateEnemy();
		else MoveEnemy();
		
		m_pEnemy->Draw(&dc);
	}
	DrawScore(&dc);
}

int MyScreen::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (BaseScreen::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	CRect rect;
	GetClientRect(rect);
	m_road.SetRect(rect);
	m_player.SetPtStart(rect);
	
	CreateManhole();
	CreateWater();
	CreateDrink();

	SetTimer(TIMERID_RENDER, m_nDeltaTime, NULL);

	return 0;
}

void MyScreen::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == TIMERID_RENDER)
	{
		m_road.MoveDown();
		if (!m_bGameOver)
		{
			MoveManhole();
			MoveWater();
			MoveDrink();

			CheckEnemy();
			CheckManhole();
			CheckWater();
			CheckDrink();

			if (HitTest()) m_bGameOver = true;
			m_score += m_nDeltaTime / 1000.;	//1초에 1점씩 득점
		}
		Invalidate(FALSE);
	}

	BaseScreen::OnTimer(nIDEvent);
}
