#include "pch.h"
#include "MyScreen.h"
#include "MemoryDC.h"
#include "resource.h"
#include "GameManager.h"
#include "ShootDucksGameDlg.h"
#include <cstdlib>

#define FIELD_MIN	(-270)
#define FIELD_MAX	(0)
#define FIELD_SPEED	(2)
#define TIMERID_RENDER	(1)

static const int Fish_Y_GAP = 80; // 두 오리 y 최소 간격(겹침 방지)

static const DWORD INK_DURATION_MS = 2000;
static const double OCTO_SPAWN_CHANCE = 0.008; // 문어 생성 확률(프레임당)

static const double EEL_SPAWN_CHANCE = 0.008;

MyScreen::MyScreen(void)
{
	SetBackColor(RGB(0, 0, 255));
	m_ptField = CPoint((FIELD_MAX + FIELD_MIN) / 2, -140);

	m_pRope = nullptr; // null pointer
	m_pFish1= nullptr;
	m_pFish2 = nullptr;
	m_pOctopus = nullptr;
	m_pEel = nullptr;

	m_gameOverSent = false;

	m_isInkOn = false;
	m_inkEndTick = 0;



}

MyScreen::~MyScreen()
{
	UnmakeRope();
}

void MyScreen::ResetGame()
{
	GameManager::initScore();
	GameManager::initHp(3);

	m_gameOverSent = false;

	UnmakeRope();
	UnmakeFish1();
	UnmakeFish2();
	UnmakeOctopus();
	UnmakeEel();

	KillTimer(TIMERID_RENDER);
	SetTimer(TIMERID_RENDER, m_nDeltaTime, NULL);

	Invalidate(FALSE);
	
}

int MyScreen::GetScore() const
{
	return GameManager::getScore();
}


void MyScreen::MoveHunterLeft(void)
{
	m_ptField.x -= FIELD_SPEED;
	if (m_ptField.x < FIELD_MIN) m_ptField.x = FIELD_MIN;
	m_Boat.MoveLeft();
}

void MyScreen::MoveHunterRight(void)
{
	m_ptField.x += FIELD_SPEED;
	if (m_ptField.x > FIELD_MAX) m_ptField.x = FIELD_MAX;
	m_Boat.MoveRight();
}


void MyScreen::DrawMount(CDC* pDC) const
{
	 BaseScreen::DrawBitmap(pDC, CPoint(-70, 155), 700, 400, IDB_MOUNT_BACK, IDB_MOUNT_FORE);
}

void MyScreen::DrawField(CDC* pDC) const
{
	 BaseScreen::DrawBitmap(pDC, m_ptField, 880, 300, IDB_FIELD_BACK, IDB_FIELD_FORE);
}

void MyScreen::MakeRope(void)
{
	if (m_pRope) return; // 이미 메모리 할당
	m_pRope = new Rope;
	m_pRope->SetPtStart(m_Boat.GetLineAnchor());
}

void MyScreen::MoveRope(void)
{
	if (m_pRope == nullptr) return;
	// 아래로 내려감
	m_pRope->MoveDown();
	// 화면 아래로 나가면 삭제
	if (m_pRope->Top() > m_rtClient.bottom + 10)
		UnmakeRope();
}

void MyScreen::MoveFish1(void)
{
	if (!m_pFish1) return;

	if (m_pFish1->m_dir > 0) m_pFish1->MoveRight();
	else m_pFish1->MoveLeft();

	// 화면을 나가면 삭제
	if (m_pFish1->Right() < -10 || m_pFish1->Left() > m_rtClient.right + 10)
		UnmakeFish1();
}

void MyScreen::MoveFish2(void)
{
	if (!m_pFish2) return;

	if (m_pFish2->m_dir > 0) m_pFish2->MoveRight();
	else m_pFish2->MoveLeft();

	if (m_pFish2->Right() < -10 || m_pFish2->Left() > m_rtClient.right + 10)
		UnmakeFish2();
}


void MyScreen::MakeFish1(void)
{
	if (m_pFish1) return;
	m_pFish1 = new Fish;
	m_pFish1->SetPtStart(m_rtClient);
}

void MyScreen::MakeFish2(void)
{
	if (m_pFish2) return;

	const int MAX_TRY = 20;

	for (int i = 0; i < MAX_TRY; ++i)
	{
		Fish* d = new Fish;
		d->SetPtStart(m_rtClient);

		// 1번 오리가 없으면 그냥 확정
		if (m_pFish1 == nullptr)
		{
			m_pFish2 = d;
			return;
		}

		// y 간격 체크
		int y1 = m_pFish1->GetPtCtr().y;
		int y2 = d->GetPtCtr().y;

		if (abs(y2 - y1) >= Fish_Y_GAP)
		{
			m_pFish2 = d;
			return;
		}

		// 너무 가까우면 버리고 다시 뽑기
		delete d;
	}

	// 무한루프 방지: 그래도 못 찾으면 그냥 하나 생성
	m_pFish2 = new Fish;
	m_pFish2->SetPtStart(m_rtClient);
}

void MyScreen::UnmakeRope(void)
{
	if (m_pRope)
	{
		delete m_pRope;
		m_pRope = nullptr;
	}
}



void MyScreen::CheckOctopus(void)
{
	if (m_pOctopus == nullptr || m_pRope == nullptr) return;

	if (m_pOctopus->HitTest(*m_pRope))
	{
		GameManager::decScore();	// 먹으면 점수 -1

		// 먹물 ON
		m_isInkOn = true;
		m_inkEndTick = GetTickCount64() + INK_DURATION_MS;

		// 문어/갈고리 처리(원하는대로)
		UnmakeOctopus();
		UnmakeRope();
	}
}

void MyScreen::UnmakeFish1(void)
{
	if (m_pFish1)
	{
		delete m_pFish1;
		m_pFish1 = nullptr;
	}
}

void MyScreen::UnmakeFish2(void)
{
	if (m_pFish2)
	{
		delete m_pFish2;
		m_pFish2 = nullptr;
	}
}


void MyScreen::MakeOctopus(void)
{
	if (m_pOctopus) return;
	m_pOctopus = new Octopus;
	m_pOctopus->SetPtStart(m_rtClient);
}

void MyScreen::MoveOctopus(void)
{
	if (!m_pOctopus) return;

	if (m_pOctopus->m_dir > 0) m_pOctopus->MoveRight();
	else                       m_pOctopus->MoveLeft();

	// 화면 밖으로 나가면 삭제
	if (m_pOctopus->Right() < -10 ||
		m_pOctopus->Left() > m_rtClient.right + 10)
	{
		UnmakeOctopus();
	}
}

void MyScreen::UnmakeOctopus(void)
{
	if (m_pOctopus)
	{
		delete m_pOctopus;
		m_pOctopus = nullptr;
	}
}

void MyScreen::MakeEel(void)
{
	if (m_pEel) return;
	m_pEel = new Eel;
	m_pEel->SetPtStart(m_rtClient);
}

void MyScreen::MoveEel(void)
{
	if (!m_pEel) return;

	if (m_pEel->m_dir > 0) m_pEel->MoveRight();
	else                   m_pEel->MoveLeft();

	// 화면 밖으로 나가면 삭제
	if (m_pEel->Right() < -10 || m_pEel->Left() > m_rtClient.right + 10)
		UnmakeEel();
}

void MyScreen::UnmakeEel(void)
{
	if (m_pEel)
	{
		delete m_pEel;
		m_pEel = nullptr;
	}
}

void MyScreen::CheckEel(void)
{
	if (!m_pEel || !m_pRope) return;

	if (m_pEel->HitTest(*m_pRope))
	{
		GameManager::decHp();

		if (GameManager::getHp() <= 0 && !m_gameOverSent)
		{
			m_gameOverSent = true;

			// ★ 게임오버 메시지창/메인복귀는 Dlg에서!
			CShootDucksGameDlg* pDlg = (CShootDucksGameDlg*)GetParent();
			if (pDlg)
				pDlg->GameOver();

			// 타이머는 마지막에 끊어도 되고, GameOver 전에 끊어도 됨
			KillTimer(TIMERID_RENDER);
			return;
		}
		

		UnmakeEel();
		UnmakeRope();
	}

}

void MyScreen::DrawHp(CDC* pDC) const
{
	int hp = GameManager::getHp();   // HP : 3


	CFont font;
	font.CreatePointFont(240, _T("맑은 고딕")); // 크기 조절 가능
	CFont* old = pDC->SelectObject(&font);

	pDC->SetBkMode(TRANSPARENT);

	// 위치(원하는 곳으로 바꿔)
	int x = 10;
	int y = 40;

	// 하트 3개 출력: 남은 HP는 빨강, 잃은 HP는 회색
	for (int i = 0; i < 3; i++)
	{
		if (i < hp) pDC->SetTextColor(RGB(255, 80, 80));
		else        pDC->SetTextColor(RGB(120, 120, 120));

		pDC->TextOut(x + i * 30, y, _T("♥"));
	}


	pDC->SelectObject(old);
}

void MyScreen::CheckFish1(void)
{
	if (m_pFish1 == nullptr || m_pRope == nullptr) return;
	if (m_pFish1->HitTest(*m_pRope))	// 충돌
	{
		GameManager::incScore();
		UnmakeFish1();
		UnmakeRope();
	}
}

void MyScreen::CheckFish2(void)
{
	if (m_pFish2 == nullptr || m_pRope == nullptr) return;
	if (m_pFish2->HitTest(*m_pRope))	// 충돌
	{
		GameManager::incScore();
		UnmakeFish2();
		UnmakeRope();
	}
}

void MyScreen::DrawInk(CDC* pDC) const
{
	if (!m_isInkOn) return;

	const int INK_W = 1024;
	const int INK_H = 1024;

	int x = (m_rtClient.Width() - INK_W) / 2;
	int y = (m_rtClient.Height() - INK_H) / 2;

	BaseScreen::DrawBitmap(pDC, CPoint(x, y), INK_W, INK_H, IDB_INK_BACK, IDB_INK);
}


void MyScreen::DrawScore(CDC* pDC) const
{
	CFont font;
	font.CreatePointFont(150, _T("맑은 고딕"));
	CString str;
	str.Format(_T("Score = %d"), GameManager::getScore());
	CFont* pOldFont = pDC->SelectObject(&font);
	COLORREF nTextCol = pDC->SetTextColor(RGB(0, 255, 77));
	COLORREF nBkCol = pDC->SetBkColor(RGB(255, 128, 0));
	pDC->TextOut(10, 10, str);
	pDC->SelectObject(pOldFont);
	pDC->SetTextColor(nTextCol);
	pDC->SetBkColor(nBkCol);
}

BEGIN_MESSAGE_MAP(MyScreen, BaseScreen)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

void MyScreen::OnPaint()
{
	CPaintDC viewDc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 BaseScreen::OnPaint()을(를) 호출하지 마십시오.
	MemoryDC dc(&viewDc);
	
	DrawBack(&dc);
	DrawMount(&dc);
	if (m_pFish1)	// 물고기 1
	{
		m_pFish1->Draw(&dc);
	}
	if (m_pFish2)	// 물고기 2
	{
		m_pFish2->Draw(&dc);
	}
	DrawField(&dc);
	if (m_pOctopus)	// 문어
	{
		m_pOctopus->Draw(&dc);
	}
	if (m_pEel)		// 전기뱀장어
	{
		m_pEel->Draw(&dc);
	}
	m_Boat.Draw(&dc);
	if (m_pRope)
	{
		CPoint a = m_Boat.GetLineAnchor() + CPoint(-38, 10); // 배에서 줄이 나오는 점
		CPoint b = m_pRope->GetHookTip();   // 갈고리 끝(현재 bullet 중심)

		b += CPoint(15, -40);
		CPen pen(PS_SOLID, 2, RGB(160, 134, 113)); // 굵기, 줄 색상 
		CPen* oldPen = dc.SelectObject(&pen);


		dc.MoveTo(a);
		dc.LineTo(b);

		dc.SelectObject(oldPen);
		
	}

	// 갈고리(끝) 비트맵은 마지막에
	if (m_pRope)
	{
		m_pRope->Draw(&dc);
	}

	DrawScore(&dc);
	DrawHp(&dc);
	DrawInk(&dc);

	
}

int MyScreen::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (BaseScreen::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	GetClientRect(m_rtClient);
	m_Boat.SetPtStart(m_rtClient);

	SetTimer(TIMERID_RENDER, m_nDeltaTime, NULL);
	return 0;
}

void MyScreen::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == TIMERID_RENDER)
	{
		// 보물
		if (m_pFish1 == nullptr) MakeFish1();
		else MoveFish1();

		if (m_pFish2 == nullptr) MakeFish2();
		else MoveFish2();


		// 문어
		if (m_pOctopus == nullptr)
		{
			if (BaseScreen::RandUni() < OCTO_SPAWN_CHANCE)
				MakeOctopus();
		}
		else
		{
			MoveOctopus();
		}

		// 뱀장어(가끔 등장)
		if (m_pEel == nullptr)
		{
			if (BaseScreen::RandUni() < EEL_SPAWN_CHANCE) 
				MakeEel();
		}
		else
		{
			MoveEel();
		}

		// 먹물 종료
		if (m_isInkOn && GetTickCount64() >= m_inkEndTick)
		{
			m_isInkOn = false;
		}
		Invalidate(FALSE);

		if (GameManager::isDead())
		{
			KillTimer(TIMERID_RENDER);
			// 원하면 GameOver 글자 출력/리셋 버튼 등 추가 가능
		}

		
		
		// 갈고리
		MoveRope();

		// 충돌
		CheckFish1();
		CheckFish2();
		CheckOctopus();
		CheckEel();

		
	}

	BaseScreen::OnTimer(nIDEvent);
}
