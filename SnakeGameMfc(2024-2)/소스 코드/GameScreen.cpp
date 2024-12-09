#include "pch.h"
#include "GameScreen.h"
#ifndef SCREEN_TIMER
#define SCREEN_TIMER 1
#endif#define SCREEN_TIMER 1            // 일반 게임 타이머
#define POISON_FRUIT_TIMER 2      // 독사과 타이머

// 메시지 맵 정의
BEGIN_MESSAGE_MAP(GameScreen, CStatic)
	ON_WM_ERASEBKGND() // 배경 지우기 메시지 처리
	ON_WM_PAINT() // 페인팅 메시지 처리
	ON_WM_TIMER() // 타이머 메시지 처리
END_MESSAGE_MAP()




GameScreen::GameScreen()
	: m_showLogo(TRUE) // 로고 초기화
{
	// 로고 비트맵 로드
	m_logoBitmap.LoadBitmap(IDB_LOGO); // 리소스에 추가한 비트맵 ID
}

// 점수를 그리는 함수
void GameScreen::drawScore(CDC* pDC)
{
	CString str;
	str.Format(_T("score = %d"), m_nScore); // 점수 문자열 포맷
	CFont font;
	font.CreatePointFont(200, _T("맑은 고딕")); // 폰트 생성
	CFont* pOldFont;
	COLORREF oldTextCol = pDC->GetTextColor(); // 이전 텍스트 색상 저장
	COLORREF oldBkCol = pDC->GetBkColor(); // 이전 배경 색상 저장
	pDC->SetTextColor(RGB(255, 0, 0)); // 텍스트 색상 설정 (빨간색)
	pDC->SetBkColor(RGB(0, 255, 255)); // 배경 색상 설정 (청록색)
	pOldFont = pDC->SelectObject(&font); // 새 폰트 선택
	pDC->TextOut(100, 100, str); // 점수 출력
	pDC->SetTextColor(oldTextCol); // 이전 텍스트 색상 복원
	pDC->SetBkColor(oldBkCol); // 이전 배경 색상 복원
	pDC->SelectObject(pOldFont); // 이전 폰트 복원
}

// 게임 오버 메시지를 그리는 함수
void GameScreen::drawGameOver(CDC* pDC)
{
	COLORREF oldTextCol = pDC->GetTextColor(); // 이전 텍스트 색상 저장
	COLORREF oldBkCol = pDC->GetBkColor(); // 이전 배경 색상 저장
	pDC->TextOut(200, 200, _T("Game Over")); // 게임 오버 메시지 출력
	pDC->SetTextColor(oldTextCol); // 이전 텍스트 색상 복원
	pDC->SetBkColor(oldBkCol); // 이전 배경 색상 복원
	return void(); // 불필요한 return 문, 제거 가능
}





// 배경을 지우는 함수
BOOL GameScreen::OnEraseBkgnd(CDC* pDC)
{
	// 배경 지우기 처리
	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_BKND2); // 비트맵 로드
	CDC memDc;
	memDc.CreateCompatibleDC(pDC); // 메모리 DC 생성
	CBitmap* pOldBitmap = memDc.SelectObject(&bitmap); // 비트맵 선택
	pDC->BitBlt(0, 0, m_nBkWid, m_nBkHt, &memDc, 0, 0, SRCCOPY); // 비트맵을 화면에 복사
	memDc.SelectObject(pOldBitmap); // 이전 비트맵 복원

	m_wall.draw(pDC); // 벽 그리기

	return CStatic::OnEraseBkgnd(pDC); // 기본 처리 호출
}

// 페인팅 처리 함수
void GameScreen::OnPaint()
{

	CPaintDC dc(this); // 페인팅을 위한 디바이스 컨텍스트 생성

	// 배경 비트맵 그리기
	CBitmap backgroundBitmap;
	backgroundBitmap.LoadBitmap(IDB_BKND2); // 배경화면 비트맵 로드
	DrawBitmap(&dc, &backgroundBitmap);

	m_wall.draw(&dc);

	// 기존 코드 유지
	if (m_bGameOver) // 게임 오버 상태 확인
	{
		drawGameOver(&dc); // 게임 오버 메시지 그리기
		return;
	}
	if (!m_fruit.getEnable()) // 과일이 활성화되지 않은 경우
	{
		m_fruit.setRandPt(); // 과일의 랜덤 위치 설정
		m_fruit.setEnable(true); // 과일 활성화
	}
	m_snake.draw(&dc); // 뱀 그리기
	m_fruit.draw(&dc); // 과일 그리기
	drawScore(&dc); // 점수 그리기



	if (m_showLogo) // 로고 표시 상태
	{
		CRect rect;
		GetClientRect(&rect);

		CDC memDC;
		memDC.CreateCompatibleDC(&dc);
		CBitmap* pOldBitmap = memDC.SelectObject(&m_logoBitmap);

		// 로고 비트맵 출력
		dc.BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
		memDC.SelectObject(pOldBitmap);

		return; // 로고 출력 후 종료
	}

	dc.TextOut(10, 10, _T("게임 화면입니다."));

		CPaintDC paintDC(this); // device context for painting

		// 메모리 DC 생성
		CDC memDC;
		memDC.CreateCompatibleDC(&paintDC); // 화면 DC와 호환되는 메모리 DC 생성

		// 클라이언트 영역 크기 가져오기
		CRect rect;
		GetClientRect(&rect);

		// 비트맵을 위한 메모리 비트맵 생성
		CBitmap bitmap;
		bitmap.CreateCompatibleBitmap(&paintDC, rect.Width(), rect.Height());
		CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

		// 배경을 흰색으로 채우기 (또는 원하는 색으로)
		memDC.FillSolidRect(&rect, RGB(255, 255, 255));

		// 여기에 비트맵 그리기
		m_fruit.draw(&memDC);
		m_snake.draw(&memDC);
		// 추가적인 그리기 작업...

		// 메모리 DC를 화면 DC에 복사
		paintDC.BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

		// 메모리 비트맵과 DC 정리
		memDC.SelectObject(pOldBitmap);
		bitmap.DeleteObject();

		m_poisonFruit.draw(&dc); // 독사과 그리기


		// 더블 버퍼링을 위한 메모리 DC 생성
	
		GetClientRect(&rect);
		CBitmap bufferBitmap;
		CDC bufferDC;
		bufferDC.CreateCompatibleDC(&dc);
		bufferBitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
		

	

		// 벽 렌더링
		m_wall.draw(&bufferDC);

		// 게임 오브젝트 렌더링
		if (m_fruit.getEnable())
			m_fruit.draw(&bufferDC);
		if (m_poisonFruit.getEnable())
			m_poisonFruit.draw(&bufferDC);
		m_snake.draw(&bufferDC);

		// 최종 화면에 복사
		dc.BitBlt(0, 0, rect.Width(), rect.Height(), &bufferDC, 0, 0, SRCCOPY);

		// 메모리 DC 정리
		bufferDC.SelectObject(pOldBitmap);
}



// 배경 비트맵을 그리는 함수
void GameScreen::DrawBitmap(CDC* pDC, CBitmap* pBitmap)
{
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);

	CBitmap* pOldBitmap = memDC.SelectObject(pBitmap);

	CRect rect;
	GetClientRect(&rect);

	// 벽 영역만큼 배경 비트맵을 렌더링하지 않도록 설정
	rect.DeflateRect(m_wall.getThick(), m_wall.getThick());

	// 배경 비트맵을 클라이언트 영역에 맞춰 렌더링
	pDC->StretchBlt(
		rect.left, rect.top, rect.Width(), rect.Height(),
		&memDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);

	// pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBitmap);
}

void GameScreen::startGame()
{
	SetTimer(SCREEN_TIMER, 100, NULL);// 100ms 간격
	m_showLogo = FALSE; // 로고 숨기기
	m_timerInterval = 1000 / FPS; // 초기 타이머 주기 설정
	startPoisonMoveTimer();
	SetTimer(SCREEN_TIMER, 1000 / FPS, NULL); // 타이머 설정
	Invalidate();       // 화면 갱신 요청
	UpdateWindow();     // 즉시 갱신
}
void GameScreen::startPoisonMoveTimer()
{
	m_poisonSpeedX = rand() % 5 + 1; // X축 이동 속도 증가
	m_poisonSpeedY = rand() % 5 + 1; // Y축 이동 속도 증가
	m_poisonMoveTimerID = SetTimer(3, 100, NULL); // 100ms 간격 타이머
}

// GameScreen 객체 생성 함수
BOOL GameScreen::Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	BOOL bResult = CStatic::Create(lpszText, dwStyle, rect, pParentWnd, nID); // 기본 클래스 생성 호출
	// 기존 코드...
	m_bGameStarted = TRUE; // 게임 시작 상태 설정
	CRect rtClient;
	GetClientRect(rtClient); // 클라이언트 영역 크기 가져오기
	m_wall.setRect(rtClient); // 벽의 크기 설정
	m_snake.setPt(rtClient.CenterPoint()); // 뱀의 초기 위치 설정
	m_fruit.setClientRect(rtClient); // 과일의 클라이언트 영역 설정
	m_fruit.setRandPt(); // 과일의 랜덤 위치 설정
	m_fruit.setEnable(true); // 과일 활성화
	m_snake.setDir(SnakeDir::UP); // 초기 방향 설정

	m_poisonFruit.setClientRect(rtClient); // 독사과의 클라이언트 영역 설정
	m_poisonFruit.setRandPt();            // 독사과의 랜덤 위치 설정
	m_poisonFruit.setEnable(true);        // 독사과 활성화

	return bResult; // 생성 결과 반환
}

void GameScreen::OnTimer(UINT_PTR nIDEvent)
{

	// 독사과 이동 타이머 처리
	if (nIDEvent == 3) // 3은 독사과 이동 타이머 ID
	{
		movePoisonFruit(); // 독사과 이동 로직
		Invalidate(FALSE); // 화면 갱신 요청
	}
	// 일반 게임 타이머 처리 (뱀 이동 및 충돌 검사 등)
	else if (nIDEvent == SCREEN_TIMER)
	{
		if (m_bGameOver) // 게임 오버 상태 확인
		{
			KillTimer(SCREEN_TIMER); // 타이머 종료
			return;
		}

		m_snake.move(); // 뱀 이동

		// 과일과 충돌 여부 확인
		if (m_fruit.isHit(m_snake.getRect()))
		{
			inScore();               // 점수 증가
			m_fruit.setEnable(false); // 과일 비활성화
			m_snake.increaseStep(); // 뱀 속도 증가

			// 게임 속도 증가
			if (m_timerInterval > MIN_INTERVAL)
			{
				m_timerInterval -= 50;
				KillTimer(SCREEN_TIMER); // 기존 타이머 종료
				SetTimer(SCREEN_TIMER, m_timerInterval, NULL); // 새로운 주기로 설정
			}
		}

		// 독사과와 충돌 여부 확인
		if (m_poisonFruit.isHit(m_snake.getRect()))
		{
			m_nScore--; // 점수 감소
			m_poisonFruit.setEnable(false); // 독사과 비활성화

			if (m_nScore < 0) // 점수가 음수면 게임 오버
			{
				m_bGameOver = true;
			}
		}

		// 벽과 충돌 여부 확인
		if (m_wall.isHit(m_snake.getRect()))
		{
			m_bGameOver = true; // 게임 오버 상태 설정
		}

		Invalidate(FALSE); // 화면 갱신 요청
	}

	// 기본 타이머 처리 호출
	CStatic::OnTimer(nIDEvent);
}



void GameScreen::resetGame()
{
	m_bGameOver = FALSE;      // 게임 오버 상태 해제
	m_nScore = 0;             // 점수 초기화
	m_snake.setPt(CPoint(500, 300)); // 뱀 초기 위치
	m_snake.setDir(SnakeDir::RIGHT); // 뱀 초기 방향	
	m_snake.setStep(5);      // 초기 이동 거리 설정

	m_fruit.setRandPt();      // 과일 새 위치 설정
	m_fruit.setEnable(true);  // 과일 활성화

	m_poisonFruit.setRandPt(); // 독사과 새 위치 설정
	m_poisonFruit.setEnable(true); // 독사과 활성화

	Invalidate();             // 화면 갱신 요청
}

void GameScreen::DrawBitmapWithMask(CDC* pDC, int x, int y, int width, int height, UINT maskID, UINT bitmapID)
{
	// 메모리 DC 생성
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);

	// 마스크 비트맵 로드
	CBitmap maskBitmap;
	maskBitmap.LoadBitmap(maskID);
	CBitmap* pOldMaskBitmap = memDC.SelectObject(&maskBitmap);

	// 마스크 비트맵 적용 (AND 연산)
	pDC->BitBlt(x, y, width, height, &memDC, 0, 0, SRCAND);

	// 오브젝트 비트맵 로드
	CBitmap objBitmap;
	objBitmap.LoadBitmap(bitmapID);
	memDC.SelectObject(&objBitmap);

	// 오브젝트 비트맵 적용 (OR 연산)
	pDC->BitBlt(x, y, width, height, &memDC, 0, 0, SRCPAINT);

	// 메모리 DC 정리
	memDC.SelectObject(pOldMaskBitmap);
}



void GameScreen::stopGame()
{
	KillTimer(SCREEN_TIMER); // 일반 타이머 종료
	m_bGameOver = true;                   // 게임 오버 상태 설정
}



void GameScreen::movePoisonFruit()
{
	CRect clientRect;
	GetClientRect(&clientRect); // 클라이언트 영역 크기 가져오기

	// 독사과의 현재 위치 가져오기
	CRect poisonRect = m_poisonFruit.getRect();

	// 새로운 위치로 이동
	poisonRect.OffsetRect(m_poisonSpeedX, m_poisonSpeedY);

	// 화면 경계를 벗어나면 이동 방향 반전
	if (poisonRect.left < clientRect.left || poisonRect.right > clientRect.right)
	{
		m_poisonSpeedX = -m_poisonSpeedX;
	}
	if (poisonRect.top < clientRect.top || poisonRect.bottom > clientRect.bottom)
	{
		m_poisonSpeedY = -m_poisonSpeedY;
	}

	// 독사과 위치 업데이트
	m_poisonFruit.setRect(poisonRect);
}

void GameScreen::stopPoisonMoveTimer()
{
	if (m_poisonMoveTimerID)
	{
		KillTimer(m_poisonMoveTimerID); // 타이머 종료
		m_poisonMoveTimerID = 0;
	}
}

