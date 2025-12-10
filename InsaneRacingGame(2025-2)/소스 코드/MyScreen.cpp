#include "pch.h"
#include "resource.h"
#include "MyScreen.h"
#include "MemoryDC.h"

#define TIMERID_RENDER (1)

const UINT MyScreen::COUNTDOWN_STEP_MS; // 정적 상수 정의

MyScreen::MyScreen(void)
{
	m_fps = 100.;
	m_nDeltaTime = int(1000. / m_fps);

	m_nBackColor = RGB(63, 63, 63);
	m_pEnemy = nullptr;
	m_pGasStation = nullptr;

	m_nEnemyDistMargin = 50;

	m_bInGasStation = false;
	m_nGasSpawnElapsedMs = 0;
	m_nGasSpawnIntervalMs = 5000; // 5초마다 한 번 등장

	m_titleMode = TitleMode::MainMenu;

	m_countdownPhase = CountdownPhase::None;
	m_nCountdownElapsedMs = 0;
}

MyScreen::~MyScreen()
{
	if (m_pEnemy)
	{
		delete m_pEnemy;
		m_pEnemy = nullptr;
	}
	if (m_pGasStation)
	{
		delete m_pGasStation;
		m_pGasStation = nullptr;
	}
}

BEGIN_MESSAGE_MAP(MyScreen, BaseScreen)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// 난이도, 랜덤 기반 Enemy 타입 선택
EnemyType MyScreen::ChooseEnemyTypeByDifficulty() const
{
	DifficultyLevel level = m_gameFlow.GetDifficultyLevel();
	int randPercent = BaseScreen::RandRange(0, 99); // [0,99]

	switch (level)
	{
	case DifficultyLevel::Easy:
		if (randPercent < 60)  return EnemyType::Type2_Bus;			// 60%
		if (randPercent < 90)  return EnemyType::Type3_Sedan;		// 30%
		return EnemyType::Type1_SportCar;							// 10%

	case DifficultyLevel::Hard:
		if (randPercent < 60)  return EnemyType::Type1_SportCar;	// 60%
		if (randPercent < 85)  return EnemyType::Type3_Sedan;		// 25%
		return EnemyType::Type2_Bus;								// 15%

	case DifficultyLevel::Normal:
	default:
		if (randPercent < 34)  return EnemyType::Type1_SportCar;	// 34%
		if (randPercent < 67)  return EnemyType::Type2_Bus;			// 33%
		return EnemyType::Type3_Sedan;								// 33%
	}
}

void MyScreen::CreateEnemy(void)
{
	if (m_pEnemy)
	{
		delete m_pEnemy;
		m_pEnemy = nullptr;
	}

	m_pEnemy = new Enemy;

	EnemyType type = ChooseEnemyTypeByDifficulty();
	m_pEnemy->SetType(type);

	m_pEnemy->SetPtStart(m_road.GetSafeRect());
}

void MyScreen::MoveEnemy(void)
{
	if (m_pEnemy)
	{
		m_pEnemy->Move(0, m_road.GetSpeed() - m_pEnemy->GetSpeed());
	}
}

void MyScreen::CheckEnemy(bool& outPassedFromBottom, bool& outPassedFromTop)
{
	outPassedFromBottom = false;
	outPassedFromTop = false;

	if (m_pEnemy == nullptr)
	{
		return;
	}

	if (m_pEnemy->Top() >= m_road.Bottom() + m_nEnemyDistMargin)
	{
		delete m_pEnemy;
		m_pEnemy = nullptr;

		outPassedFromBottom = true;
	}
	else if (m_pEnemy->Bottom() <= m_road.Top() - m_nEnemyDistMargin)
	{
		delete m_pEnemy;
		m_pEnemy = nullptr;

		outPassedFromTop = true;
	}
}

bool MyScreen::HitTest(void) const
{
	if (m_pEnemy == nullptr)
	{
		return false;
	}
	else
	{
		return m_player.HitTest(*m_pEnemy) || m_pEnemy->HitTest(m_player);
	}
}

void MyScreen::UpdateGasStationSpawn(UINT deltaMs, int roadSpeed)
{
	if (m_gameFlow.GetState() != GameState::Playing)
	{
		return;
	}

	// 이미 주유소가 있으면 이동 + 화면 밖 체크
	if (m_pGasStation != nullptr)
	{
		m_pGasStation->MoveWithRoad(roadSpeed);

		CRect worldRect;
		GetClientRect(&worldRect);

		if (m_pGasStation->IsOffScreen(worldRect))
		{
			delete m_pGasStation;
			m_pGasStation = nullptr;
		}
	}

	// 스폰 타이머 갱신
	m_nGasSpawnElapsedMs += deltaMs;

	// 없고, 타이머가 기준을 넘으면 생성
	if (m_pGasStation == nullptr && m_nGasSpawnElapsedMs >= m_nGasSpawnIntervalMs)
	{
		m_nGasSpawnElapsedMs = 0;

		m_pGasStation = new GasStation;
		m_pGasStation->SetPtStart(m_road.GetSafeRect()); // 우측 고정 스폰
	}
}

void MyScreen::UpdateGasStationFlag()
{
	m_bInGasStation = false;

	if (m_pGasStation == nullptr)
	{
		return;
	}

	CRect rcPlayer, rcGas, rcIntersect;
	rcPlayer = m_player.MakeRect();
	rcGas = m_pGasStation->MakeRect();

	if (rcIntersect.IntersectRect(&rcPlayer, &rcGas))
	{
		m_bInGasStation = true;
	}
}

void MyScreen::InitWorld(const CRect& rect)
{
	m_road.SetRect(rect);
	m_player.SetPtStart(rect);
}

void MyScreen::InitTitleUI(const CRect& rect)
{
	const int centerX = rect.Width() / 2;
	const int centerY = rect.Height() / 2;
	const CPoint center(centerX, centerY);

	// Title 메인 메뉴 UI
	m_uiTitleLogo.SetBitmap(400, 400, IDB_TITLE_BACK, IDB_TITLE_FORE);
	m_uiTitleLogo.SetCenter(CPoint(centerX, centerY - 120));

	// 시작 버튼
	m_uiBtnStart.SetBitmap(200, 86, IDB_START_BACK, IDB_START_FORE);
	m_uiBtnStart.SetCenter(CPoint(centerX, centerY + 60));

	// 난이도 선택 버튼
	m_uiBtnDifficulty.SetBitmap(200, 68, IDB_DIFFICULTY_BACK, IDB_DIFFICULTY_FORE);
	m_uiBtnDifficulty.SetCenter(CPoint(centerX, centerY + 146));

	// 종료 버튼
	m_uiBtnExit.SetBitmap(200, 81, IDB_EXIT_BACK, IDB_EXIT_FORE);
	m_uiBtnExit.SetCenter(CPoint(centerX, centerY + 232));

	const int diffCenterY = centerY;

	// 쉬움 난이도 버튼
	m_uiBtnEasy.SetBitmap(200, 82, IDB_EASY_BACK, IDB_EASY_FORE);
	m_uiBtnEasy.SetCenter(CPoint(centerX, diffCenterY - 80));

	// 보통 난이도 버튼
	m_uiBtnNormal.SetBitmap(200, 72, IDB_NORMAL_BACK, IDB_NORMAL_FORE);
	m_uiBtnNormal.SetCenter(CPoint(centerX, diffCenterY));

	// 어려움 난이도 버튼
	m_uiBtnHard.SetBitmap(200, 86, IDB_HARD_BACK, IDB_HARD_FORE);
	m_uiBtnHard.SetCenter(CPoint(centerX, diffCenterY + 80));

	// 뒤로 가기 버튼
	m_uiBtnBack.SetBitmap(200, 72, IDB_RESUME_BACK, IDB_RESUME_FORE);
	m_uiBtnBack.SetCenter(CPoint(centerX, diffCenterY + 166));

	// "Ready" UI
	m_uiReady.SetBitmap(146, 93, IDB_READY_BACK, IDB_READY_FORE);
	m_uiReady.SetCenter(center);

	// "3" UI
	m_uiCount3.SetBitmap(100, 98, IDB_COUNT3_BACK, IDB_COUNT3_FORE);
	m_uiCount3.SetCenter(center);

	// "2" UI
	m_uiCount2.SetBitmap(100, 93, IDB_COUNT2_BACK, IDB_COUNT2_FORE);
	m_uiCount2.SetCenter(center);

	// "1" UI
	m_uiCount1.SetBitmap(100, 125, IDB_COUNT1_BACK, IDB_COUNT1_FORE);
	m_uiCount1.SetCenter(center);

	// "RUN!" UI
	m_uiRun.SetBitmap(200, 89, IDB_RUN_BACK, IDB_RUN_FORE);
	m_uiRun.SetCenter(center);

	// Pause UI
	m_uiPausePanel.SetBitmap(400, 400, IDB_TITLE_BACK, IDB_TITLE_FORE);
	m_uiPausePanel.SetCenter(CPoint(centerX, centerY - 120));

	// Resume 버튼
	m_uiBtnPauseResume.SetBitmap(200, 72, IDB_RESUME_BACK, IDB_RESUME_FORE);
	m_uiBtnPauseResume.SetCenter(CPoint(centerX, centerY + 60));

	// Exit to Title 버튼
	m_uiBtnPauseExitToTitle.SetBitmap(200, 88,IDB_EXITTOTITLE_BACK, IDB_EXITTOTITLE_FORE);
	m_uiBtnPauseExitToTitle.SetCenter(CPoint(centerX, centerY + 140));

	// GameOver / Clear 결과 UI
	m_uiResultPanel.SetBitmap(400, 400, IDB_TITLE_BACK, IDB_TITLE_FORE);
	m_uiResultPanel.SetCenter(CPoint(centerX, centerY - 100));

	// Game Over 라벨
	m_uiResultLabelGameOver.SetBitmap(150, 113, IDB_GAMEOVER_BACK, IDB_GAMEOVER_FORE);
	m_uiResultLabelGameOver.SetCenter(CPoint(centerX, centerY - 30));

	// Game Cleared 라벨
	m_uiResultLabelCleared.SetBitmap(150, 128, IDB_GAMECLEAR_BACK, IDB_GAMECLEAR_FORE);
	m_uiResultLabelCleared.SetCenter(CPoint(centerX, centerY - 30));

	// Retry 버튼
	m_uiBtnResultRetry.SetBitmap(200, 82, IDB_RETRY_BACK, IDB_RETRY_FORE);
	m_uiBtnResultRetry.SetCenter(CPoint(centerX, centerY + 90));

	// Exit to Title 버튼
	m_uiBtnResultExitToTitle.SetBitmap(200, 88, IDB_EXITTOTITLE_BACK, IDB_EXITTOTITLE_FORE);
	m_uiBtnResultExitToTitle.SetCenter(CPoint(centerX, centerY + 180));
}

void MyScreen::DrawHUD(CDC* pDC) const
{
	CFont font;
	font.CreatePointFont(180, _T("맑은 고딕"));

	CFont* pOldFont = pDC->SelectObject(&font);
	COLORREF oldTextColor = pDC->SetTextColor(RGB(255, 255, 255));
	COLORREF oldBkColor = pDC->SetBkColor(RGB(63, 63, 63));

	CString str;

	str.Format(_T("Score: %.0f"), m_gameFlow.GetScore());	// 점수
	pDC->TextOut(20, 20, str);

	str.Format(_T("Fuel : %.0f / %.0f"), m_gameFlow.GetFuel(), m_gameFlow.GetFuelMax());	// 연료
	pDC->TextOut(20, 50, str);

	double elapsedSec = m_gameFlow.GetElapsedTimeMs() / 1000.0;	// 경과 시간
	double limitSec = m_gameFlow.GetDifficultyConfig().timeLimitMs / 1000.0; // 제한 시간
	str.Format(_T("Time : %.1f / %.1f"), elapsedSec, limitSec);
	pDC->TextOut(20, 80, str);

	LPCTSTR stateText = _T("");
	switch (m_gameFlow.GetState())
	{
	case GameState::Ready:			// 카운트다운 대기
		stateText = _T("READY");     
		break;

	case GameState::Playing:		// 게임 진행 중
		stateText = _T("PLAYING");   
		break;

	case GameState::Paused:			// 일시 정지
		stateText = _T("PAUSED");    
		break;

	case GameState::GameOver:		// 게임 오버
		stateText = _T("GAME OVER"); 
		break;

	case GameState::Cleared:		// 게임 클리어
		stateText = _T("CLEARED");   
		break;

	case GameState::Title:			
	default:						// 타이틀 화면
		stateText = _T("TITLE");
		break;
	}

	str.Format(_T("State: %s"), stateText);
	pDC->TextOut(20, 110, str);

	LPCTSTR diffText;
	switch (m_gameFlow.GetDifficultyLevel())
	{
	case DifficultyLevel::Easy:		// 쉬움
		diffText = _T("Easy");   
		break;

	case DifficultyLevel::Normal:	// 보통
		diffText = _T("Normal"); 
		break;

	case DifficultyLevel::Hard:		// 어려움
		diffText = _T("Hard");   
		break;

	default:                      
		diffText = _T("Unknown"); 
		break;
	}

	str.Format(_T("Difficulty : %s"), diffText);
	pDC->TextOut(20, 140, str);

	pDC->SelectObject(pOldFont);
	pDC->SetTextColor(oldTextColor);
	pDC->SetBkColor(oldBkColor);
}

void MyScreen::DrawTitleScreen(CDC* pDC) const
{
	if (m_titleMode == TitleMode::MainMenu)
	{
		m_uiTitleLogo.Draw(pDC);
		m_uiBtnStart.Draw(pDC);
		m_uiBtnDifficulty.Draw(pDC);
		m_uiBtnExit.Draw(pDC);
	}
	else if (m_titleMode == TitleMode::DifficultySelect)
	{
		CRect rect;
		GetClientRect(&rect);

		CBrush  brushOverlay(RGB(0, 0, 0));
		CBrush* pOldBrush = pDC->SelectObject(&brushOverlay);
		int oldBkMode = pDC->SetBkMode(TRANSPARENT);

		pDC->Rectangle(rect);

		pDC->SelectObject(pOldBrush);
		pDC->SetBkMode(oldBkMode);

		m_uiBtnEasy.Draw(pDC);
		m_uiBtnNormal.Draw(pDC);
		m_uiBtnHard.Draw(pDC);
		m_uiBtnBack.Draw(pDC);
	}
}

void MyScreen::DrawGasStation(CDC* pDC) const
{
	if (m_pGasStation != nullptr)
	{
		m_pGasStation->Draw(pDC);
	}
}

void MyScreen::DrawCountdown(CDC* pDC) const
{
	if (m_gameFlow.GetState() != GameState::Ready)
	{
		return;
	}

	switch (m_countdownPhase)
	{
	case CountdownPhase::Ready: 
		m_uiReady.Draw(pDC); 
		break;

	case CountdownPhase::Three: 
		m_uiCount3.Draw(pDC); 
		break;

	case CountdownPhase::Two:   
		m_uiCount2.Draw(pDC); 
		break;

	case CountdownPhase::One:   
		m_uiCount1.Draw(pDC); 
		break;

	case CountdownPhase::Run:   
		m_uiRun.Draw(pDC);   
		break;

	case CountdownPhase::None:
	case CountdownPhase::Done:
	default:
		break;
	}
}

void MyScreen::DrawPauseOverlay(CDC* pDC) const
{
	if (m_gameFlow.GetState() != GameState::Paused)
	{
		return;
	}

	CRect rect;
	GetClientRect(&rect);

	CBrush brushOverlay(RGB(0, 0, 0));
	CBrush* pOldBrush = pDC->SelectObject(&brushOverlay);
	int oldBkMode = pDC->SetBkMode(TRANSPARENT);

	pDC->Rectangle(rect);

	pDC->SelectObject(pOldBrush);
	pDC->SetBkMode(oldBkMode);

	m_uiPausePanel.Draw(pDC);			// Pause 패널 배경
	m_uiBtnPauseResume.Draw(pDC);		// Resume 버튼
	m_uiBtnPauseExitToTitle.Draw(pDC);	// Exit to Title 버튼
}

void MyScreen::DrawResultOverlay(CDC* pDC) const
{
	GameState state = m_gameFlow.GetState(); // 현재 게임 상태

	// 게임 오버 또는 클리어 상태가 아니면 리턴
	if (state != GameState::GameOver && state != GameState::Cleared) { return; }

	CRect rect;
	GetClientRect(&rect);

	CBrush  brushOverlay(RGB(0, 0, 0));
	CBrush* pOldBrush = pDC->SelectObject(&brushOverlay);
	int oldBkMode = pDC->SetBkMode(TRANSPARENT);

	pDC->Rectangle(rect);

	pDC->SelectObject(pOldBrush);
	pDC->SetBkMode(oldBkMode);

	m_uiResultPanel.Draw(pDC);

	if (state == GameState::GameOver)
	{
		m_uiResultLabelGameOver.Draw(pDC);
	}
	else if (state == GameState::Cleared)
	{
		m_uiResultLabelCleared.Draw(pDC);
	}

	m_uiBtnResultRetry.Draw(pDC);			// Retry 버튼
	m_uiBtnResultExitToTitle.Draw(pDC);		// Exit to Title 버튼
}

void MyScreen::DrawInGameScreen(CDC* pDC) const
{
	DrawGasStation(pDC);

	m_player.Draw(pDC);

	if (m_pEnemy != nullptr)
	{
		m_pEnemy->Draw(pDC);
	}

	DrawHUD(pDC);
	DrawCountdown(pDC);
	DrawPauseOverlay(pDC);
	DrawResultOverlay(pDC);
}

void MyScreen::UpdateGame()
{
	if (m_gameFlow.GetState() != GameState::Playing)
	{
		return;
	}

	m_road.MoveDown();

	bool enemyPassedFromBottom = false;
	bool enemyPassedFromTop = false;

	if (m_pEnemy != nullptr)
	{
		MoveEnemy();
		CheckEnemy(enemyPassedFromBottom, enemyPassedFromTop);
	}

	bool enemyExists = (m_pEnemy != nullptr);

	bool isCollidingWithEnemy = HitTest();

	UINT deltaMs = static_cast<UINT>(m_nDeltaTime);
	int  roadSpeed = m_road.GetSpeed();

	UpdateGasStationSpawn(deltaMs, roadSpeed);
	UpdateGasStationFlag();
	bool isInGasStation = m_bInGasStation;

	// 게임 플로우 업데이트
	m_gameFlow.Update(deltaMs, roadSpeed, isCollidingWithEnemy, isInGasStation, enemyPassedFromBottom, enemyPassedFromTop, enemyExists);

	if (!enemyExists && m_gameFlow.ConsumeEnemySpawnRequest())
	{
		CreateEnemy();
	}
}

void MyScreen::UpdateCountdown(UINT deltaMs)
{
	if (m_gameFlow.GetState() != GameState::Ready) { return; }


	if (m_countdownPhase == CountdownPhase::None || m_countdownPhase == CountdownPhase::Done) { return; }

	m_nCountdownElapsedMs += deltaMs;

	// 단계 시간 도달 전이면 리턴
	if (m_nCountdownElapsedMs < COUNTDOWN_STEP_MS) { return; }

	m_nCountdownElapsedMs = 0;

	switch (m_countdownPhase)
	{
	case CountdownPhase::Ready: m_countdownPhase = CountdownPhase::Three; break;	// READY → 3
	case CountdownPhase::Three: m_countdownPhase = CountdownPhase::Two;   break;	// 3 → 2
	case CountdownPhase::Two:   m_countdownPhase = CountdownPhase::One;   break;	// 2 → 1
	case CountdownPhase::One:   m_countdownPhase = CountdownPhase::Run;   break;	// 1 → RUN!
	case CountdownPhase::Run:						// RUN! → Done + 게임 시작
		m_countdownPhase = CountdownPhase::Done;
		m_gameFlow.SetState(GameState::Playing);
		break;
	case CountdownPhase::None:
	case CountdownPhase::Done:
	default:
		break;
	}
}

void MyScreen::StartGame()
{
	// 기존 적, 주유소 제거
	if (m_pEnemy != nullptr)
	{
		delete m_pEnemy;
		m_pEnemy = nullptr;
	}

	if (m_pGasStation != nullptr)
	{
		delete m_pGasStation;
		m_pGasStation = nullptr;
	}
	m_bInGasStation = false;	// 주유소 충돌 플래그 초기화
	m_nGasSpawnElapsedMs = 0;	// 주유소 스폰 타이머 초기화

	CRect rect;
	GetClientRect(&rect);

	InitWorld(rect);	// 월드 초기화

	m_gameFlow.ResetForNewGame();	// 게임 플로우 초기화

	m_countdownPhase = CountdownPhase::Ready;	// 카운트다운 시작
	m_nCountdownElapsedMs = 0;					// 카운트다운 타이머 초기화

	m_titleMode = TitleMode::MainMenu;	// Title 모드 초기화
}

void MyScreen::TogglePause()
{
	GameState state = m_gameFlow.GetState();

	if (state == GameState::Playing)
	{
		m_gameFlow.SetState(GameState::Paused);
	}
	else if (state == GameState::Paused)
	{
		m_gameFlow.SetState(GameState::Playing);
	}
}

void MyScreen::ReturnToTitle()
{
	if (m_pEnemy != nullptr)
	{
		delete m_pEnemy;
		m_pEnemy = nullptr;
	}

	if (m_pGasStation != nullptr)
	{
		delete m_pGasStation;
		m_pGasStation = nullptr;
	}
	m_bInGasStation = false;
	m_nGasSpawnElapsedMs = 0;

	CRect rect;
	GetClientRect(&rect);
	InitWorld(rect);

	m_countdownPhase = CountdownPhase::None;
	m_nCountdownElapsedMs = 0;

	m_gameFlow.SetState(GameState::Title);

	m_titleMode = TitleMode::MainMenu;
}

void MyScreen::OnPaint()
{
	CPaintDC viewDc(this);
	MemoryDC dc(&viewDc);

	DrawBack(&dc);
	m_road.Draw(&dc);

	if (m_gameFlow.GetState() == GameState::Title)
	{
		DrawTitleScreen(&dc);
	}
	else
	{
		DrawInGameScreen(&dc);
	}
}

int MyScreen::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (BaseScreen::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

	ModifyStyle(0, SS_NOTIFY);

	CRect rect;
	GetClientRect(&rect);

	InitWorld(rect);
	InitTitleUI(rect);

	SetTimer(TIMERID_RENDER, m_nDeltaTime, NULL);

	return 0;
}

void MyScreen::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIMERID_RENDER)
	{
		UpdateCountdown(static_cast<UINT>(m_nDeltaTime));
		UpdateGame();

		Invalidate(FALSE);
	}

	BaseScreen::OnTimer(nIDEvent);
}

void MyScreen::OnLButtonDown(UINT nFlags, CPoint point)
{
	// 현재 게임 상태에 따른 UI 버튼 클릭 처리
	// 마우스 클릭 위치가 각 버튼 영역에 속하는지 검사하여 해당 동작 수행

	GameState state = m_gameFlow.GetState();

	if (state == GameState::Title)
	{
		if (m_titleMode == TitleMode::MainMenu)
		{
			if (m_uiBtnStart.HitTestPoint(point))
			{
				m_gameFlow.SetDifficulty(DifficultyLevel::Normal);
				StartGame();
				return;
			}
			else if (m_uiBtnDifficulty.HitTestPoint(point))
			{
				m_titleMode = TitleMode::DifficultySelect;
				return;
			}
			else if (m_uiBtnExit.HitTestPoint(point))
			{
				CWnd* pParent = GetParent();

				if (pParent != nullptr)
				{
					pParent->PostMessageW(WM_CLOSE);
				}
				return;
			}
		}
		else if (m_titleMode == TitleMode::DifficultySelect)
		{
			if (m_uiBtnEasy.HitTestPoint(point))
			{
				m_gameFlow.SetDifficulty(DifficultyLevel::Easy);
				StartGame();
				return;
			}
			else if (m_uiBtnNormal.HitTestPoint(point))
			{
				m_gameFlow.SetDifficulty(DifficultyLevel::Normal);
				StartGame();
				return;
			}
			else if (m_uiBtnHard.HitTestPoint(point))
			{
				m_gameFlow.SetDifficulty(DifficultyLevel::Hard);
				StartGame();
				return;
			}
			else if (m_uiBtnBack.HitTestPoint(point))
			{
				m_titleMode = TitleMode::MainMenu;
				return;
			}
		}
	}
	else if (state == GameState::Paused)
	{
		if (m_uiBtnPauseResume.HitTestPoint(point))
		{
			m_gameFlow.SetState(GameState::Playing);
			return;
		}
		else if (m_uiBtnPauseExitToTitle.HitTestPoint(point))
		{
			ReturnToTitle();
			return;
		}
	}
	else if (state == GameState::GameOver || state == GameState::Cleared)
	{
		if (m_uiBtnResultRetry.HitTestPoint(point))
		{
			StartGame();
			return;
		}
		else if (m_uiBtnResultExitToTitle.HitTestPoint(point))
		{
			ReturnToTitle();
			return;
		}
	}

	BaseScreen::OnLButtonDown(nFlags, point);
}
