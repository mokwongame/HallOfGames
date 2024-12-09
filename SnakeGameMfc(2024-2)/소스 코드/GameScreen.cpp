#include "pch.h"
#include "GameScreen.h"
#ifndef SCREEN_TIMER
#define SCREEN_TIMER 1
#endif#define SCREEN_TIMER 1            // �Ϲ� ���� Ÿ�̸�
#define POISON_FRUIT_TIMER 2      // ����� Ÿ�̸�

// �޽��� �� ����
BEGIN_MESSAGE_MAP(GameScreen, CStatic)
	ON_WM_ERASEBKGND() // ��� ����� �޽��� ó��
	ON_WM_PAINT() // ������ �޽��� ó��
	ON_WM_TIMER() // Ÿ�̸� �޽��� ó��
END_MESSAGE_MAP()




GameScreen::GameScreen()
	: m_showLogo(TRUE) // �ΰ� �ʱ�ȭ
{
	// �ΰ� ��Ʈ�� �ε�
	m_logoBitmap.LoadBitmap(IDB_LOGO); // ���ҽ��� �߰��� ��Ʈ�� ID
}

// ������ �׸��� �Լ�
void GameScreen::drawScore(CDC* pDC)
{
	CString str;
	str.Format(_T("score = %d"), m_nScore); // ���� ���ڿ� ����
	CFont font;
	font.CreatePointFont(200, _T("���� ���")); // ��Ʈ ����
	CFont* pOldFont;
	COLORREF oldTextCol = pDC->GetTextColor(); // ���� �ؽ�Ʈ ���� ����
	COLORREF oldBkCol = pDC->GetBkColor(); // ���� ��� ���� ����
	pDC->SetTextColor(RGB(255, 0, 0)); // �ؽ�Ʈ ���� ���� (������)
	pDC->SetBkColor(RGB(0, 255, 255)); // ��� ���� ���� (û�ϻ�)
	pOldFont = pDC->SelectObject(&font); // �� ��Ʈ ����
	pDC->TextOut(100, 100, str); // ���� ���
	pDC->SetTextColor(oldTextCol); // ���� �ؽ�Ʈ ���� ����
	pDC->SetBkColor(oldBkCol); // ���� ��� ���� ����
	pDC->SelectObject(pOldFont); // ���� ��Ʈ ����
}

// ���� ���� �޽����� �׸��� �Լ�
void GameScreen::drawGameOver(CDC* pDC)
{
	COLORREF oldTextCol = pDC->GetTextColor(); // ���� �ؽ�Ʈ ���� ����
	COLORREF oldBkCol = pDC->GetBkColor(); // ���� ��� ���� ����
	pDC->TextOut(200, 200, _T("Game Over")); // ���� ���� �޽��� ���
	pDC->SetTextColor(oldTextCol); // ���� �ؽ�Ʈ ���� ����
	pDC->SetBkColor(oldBkCol); // ���� ��� ���� ����
	return void(); // ���ʿ��� return ��, ���� ����
}





// ����� ����� �Լ�
BOOL GameScreen::OnEraseBkgnd(CDC* pDC)
{
	// ��� ����� ó��
	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_BKND2); // ��Ʈ�� �ε�
	CDC memDc;
	memDc.CreateCompatibleDC(pDC); // �޸� DC ����
	CBitmap* pOldBitmap = memDc.SelectObject(&bitmap); // ��Ʈ�� ����
	pDC->BitBlt(0, 0, m_nBkWid, m_nBkHt, &memDc, 0, 0, SRCCOPY); // ��Ʈ���� ȭ�鿡 ����
	memDc.SelectObject(pOldBitmap); // ���� ��Ʈ�� ����

	m_wall.draw(pDC); // �� �׸���

	return CStatic::OnEraseBkgnd(pDC); // �⺻ ó�� ȣ��
}

// ������ ó�� �Լ�
void GameScreen::OnPaint()
{

	CPaintDC dc(this); // �������� ���� ����̽� ���ؽ�Ʈ ����

	// ��� ��Ʈ�� �׸���
	CBitmap backgroundBitmap;
	backgroundBitmap.LoadBitmap(IDB_BKND2); // ���ȭ�� ��Ʈ�� �ε�
	DrawBitmap(&dc, &backgroundBitmap);

	m_wall.draw(&dc);

	// ���� �ڵ� ����
	if (m_bGameOver) // ���� ���� ���� Ȯ��
	{
		drawGameOver(&dc); // ���� ���� �޽��� �׸���
		return;
	}
	if (!m_fruit.getEnable()) // ������ Ȱ��ȭ���� ���� ���
	{
		m_fruit.setRandPt(); // ������ ���� ��ġ ����
		m_fruit.setEnable(true); // ���� Ȱ��ȭ
	}
	m_snake.draw(&dc); // �� �׸���
	m_fruit.draw(&dc); // ���� �׸���
	drawScore(&dc); // ���� �׸���



	if (m_showLogo) // �ΰ� ǥ�� ����
	{
		CRect rect;
		GetClientRect(&rect);

		CDC memDC;
		memDC.CreateCompatibleDC(&dc);
		CBitmap* pOldBitmap = memDC.SelectObject(&m_logoBitmap);

		// �ΰ� ��Ʈ�� ���
		dc.BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
		memDC.SelectObject(pOldBitmap);

		return; // �ΰ� ��� �� ����
	}

	dc.TextOut(10, 10, _T("���� ȭ���Դϴ�."));

		CPaintDC paintDC(this); // device context for painting

		// �޸� DC ����
		CDC memDC;
		memDC.CreateCompatibleDC(&paintDC); // ȭ�� DC�� ȣȯ�Ǵ� �޸� DC ����

		// Ŭ���̾�Ʈ ���� ũ�� ��������
		CRect rect;
		GetClientRect(&rect);

		// ��Ʈ���� ���� �޸� ��Ʈ�� ����
		CBitmap bitmap;
		bitmap.CreateCompatibleBitmap(&paintDC, rect.Width(), rect.Height());
		CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

		// ����� ������� ä��� (�Ǵ� ���ϴ� ������)
		memDC.FillSolidRect(&rect, RGB(255, 255, 255));

		// ���⿡ ��Ʈ�� �׸���
		m_fruit.draw(&memDC);
		m_snake.draw(&memDC);
		// �߰����� �׸��� �۾�...

		// �޸� DC�� ȭ�� DC�� ����
		paintDC.BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

		// �޸� ��Ʈ�ʰ� DC ����
		memDC.SelectObject(pOldBitmap);
		bitmap.DeleteObject();

		m_poisonFruit.draw(&dc); // ����� �׸���


		// ���� ���۸��� ���� �޸� DC ����
	
		GetClientRect(&rect);
		CBitmap bufferBitmap;
		CDC bufferDC;
		bufferDC.CreateCompatibleDC(&dc);
		bufferBitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
		

	

		// �� ������
		m_wall.draw(&bufferDC);

		// ���� ������Ʈ ������
		if (m_fruit.getEnable())
			m_fruit.draw(&bufferDC);
		if (m_poisonFruit.getEnable())
			m_poisonFruit.draw(&bufferDC);
		m_snake.draw(&bufferDC);

		// ���� ȭ�鿡 ����
		dc.BitBlt(0, 0, rect.Width(), rect.Height(), &bufferDC, 0, 0, SRCCOPY);

		// �޸� DC ����
		bufferDC.SelectObject(pOldBitmap);
}



// ��� ��Ʈ���� �׸��� �Լ�
void GameScreen::DrawBitmap(CDC* pDC, CBitmap* pBitmap)
{
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);

	CBitmap* pOldBitmap = memDC.SelectObject(pBitmap);

	CRect rect;
	GetClientRect(&rect);

	// �� ������ŭ ��� ��Ʈ���� ���������� �ʵ��� ����
	rect.DeflateRect(m_wall.getThick(), m_wall.getThick());

	// ��� ��Ʈ���� Ŭ���̾�Ʈ ������ ���� ������
	pDC->StretchBlt(
		rect.left, rect.top, rect.Width(), rect.Height(),
		&memDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);

	// pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBitmap);
}

void GameScreen::startGame()
{
	SetTimer(SCREEN_TIMER, 100, NULL);// 100ms ����
	m_showLogo = FALSE; // �ΰ� �����
	m_timerInterval = 1000 / FPS; // �ʱ� Ÿ�̸� �ֱ� ����
	startPoisonMoveTimer();
	SetTimer(SCREEN_TIMER, 1000 / FPS, NULL); // Ÿ�̸� ����
	Invalidate();       // ȭ�� ���� ��û
	UpdateWindow();     // ��� ����
}
void GameScreen::startPoisonMoveTimer()
{
	m_poisonSpeedX = rand() % 5 + 1; // X�� �̵� �ӵ� ����
	m_poisonSpeedY = rand() % 5 + 1; // Y�� �̵� �ӵ� ����
	m_poisonMoveTimerID = SetTimer(3, 100, NULL); // 100ms ���� Ÿ�̸�
}

// GameScreen ��ü ���� �Լ�
BOOL GameScreen::Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	BOOL bResult = CStatic::Create(lpszText, dwStyle, rect, pParentWnd, nID); // �⺻ Ŭ���� ���� ȣ��
	// ���� �ڵ�...
	m_bGameStarted = TRUE; // ���� ���� ���� ����
	CRect rtClient;
	GetClientRect(rtClient); // Ŭ���̾�Ʈ ���� ũ�� ��������
	m_wall.setRect(rtClient); // ���� ũ�� ����
	m_snake.setPt(rtClient.CenterPoint()); // ���� �ʱ� ��ġ ����
	m_fruit.setClientRect(rtClient); // ������ Ŭ���̾�Ʈ ���� ����
	m_fruit.setRandPt(); // ������ ���� ��ġ ����
	m_fruit.setEnable(true); // ���� Ȱ��ȭ
	m_snake.setDir(SnakeDir::UP); // �ʱ� ���� ����

	m_poisonFruit.setClientRect(rtClient); // ������� Ŭ���̾�Ʈ ���� ����
	m_poisonFruit.setRandPt();            // ������� ���� ��ġ ����
	m_poisonFruit.setEnable(true);        // ����� Ȱ��ȭ

	return bResult; // ���� ��� ��ȯ
}

void GameScreen::OnTimer(UINT_PTR nIDEvent)
{

	// ����� �̵� Ÿ�̸� ó��
	if (nIDEvent == 3) // 3�� ����� �̵� Ÿ�̸� ID
	{
		movePoisonFruit(); // ����� �̵� ����
		Invalidate(FALSE); // ȭ�� ���� ��û
	}
	// �Ϲ� ���� Ÿ�̸� ó�� (�� �̵� �� �浹 �˻� ��)
	else if (nIDEvent == SCREEN_TIMER)
	{
		if (m_bGameOver) // ���� ���� ���� Ȯ��
		{
			KillTimer(SCREEN_TIMER); // Ÿ�̸� ����
			return;
		}

		m_snake.move(); // �� �̵�

		// ���ϰ� �浹 ���� Ȯ��
		if (m_fruit.isHit(m_snake.getRect()))
		{
			inScore();               // ���� ����
			m_fruit.setEnable(false); // ���� ��Ȱ��ȭ
			m_snake.increaseStep(); // �� �ӵ� ����

			// ���� �ӵ� ����
			if (m_timerInterval > MIN_INTERVAL)
			{
				m_timerInterval -= 50;
				KillTimer(SCREEN_TIMER); // ���� Ÿ�̸� ����
				SetTimer(SCREEN_TIMER, m_timerInterval, NULL); // ���ο� �ֱ�� ����
			}
		}

		// ������� �浹 ���� Ȯ��
		if (m_poisonFruit.isHit(m_snake.getRect()))
		{
			m_nScore--; // ���� ����
			m_poisonFruit.setEnable(false); // ����� ��Ȱ��ȭ

			if (m_nScore < 0) // ������ ������ ���� ����
			{
				m_bGameOver = true;
			}
		}

		// ���� �浹 ���� Ȯ��
		if (m_wall.isHit(m_snake.getRect()))
		{
			m_bGameOver = true; // ���� ���� ���� ����
		}

		Invalidate(FALSE); // ȭ�� ���� ��û
	}

	// �⺻ Ÿ�̸� ó�� ȣ��
	CStatic::OnTimer(nIDEvent);
}



void GameScreen::resetGame()
{
	m_bGameOver = FALSE;      // ���� ���� ���� ����
	m_nScore = 0;             // ���� �ʱ�ȭ
	m_snake.setPt(CPoint(500, 300)); // �� �ʱ� ��ġ
	m_snake.setDir(SnakeDir::RIGHT); // �� �ʱ� ����	
	m_snake.setStep(5);      // �ʱ� �̵� �Ÿ� ����

	m_fruit.setRandPt();      // ���� �� ��ġ ����
	m_fruit.setEnable(true);  // ���� Ȱ��ȭ

	m_poisonFruit.setRandPt(); // ����� �� ��ġ ����
	m_poisonFruit.setEnable(true); // ����� Ȱ��ȭ

	Invalidate();             // ȭ�� ���� ��û
}

void GameScreen::DrawBitmapWithMask(CDC* pDC, int x, int y, int width, int height, UINT maskID, UINT bitmapID)
{
	// �޸� DC ����
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);

	// ����ũ ��Ʈ�� �ε�
	CBitmap maskBitmap;
	maskBitmap.LoadBitmap(maskID);
	CBitmap* pOldMaskBitmap = memDC.SelectObject(&maskBitmap);

	// ����ũ ��Ʈ�� ���� (AND ����)
	pDC->BitBlt(x, y, width, height, &memDC, 0, 0, SRCAND);

	// ������Ʈ ��Ʈ�� �ε�
	CBitmap objBitmap;
	objBitmap.LoadBitmap(bitmapID);
	memDC.SelectObject(&objBitmap);

	// ������Ʈ ��Ʈ�� ���� (OR ����)
	pDC->BitBlt(x, y, width, height, &memDC, 0, 0, SRCPAINT);

	// �޸� DC ����
	memDC.SelectObject(pOldMaskBitmap);
}



void GameScreen::stopGame()
{
	KillTimer(SCREEN_TIMER); // �Ϲ� Ÿ�̸� ����
	m_bGameOver = true;                   // ���� ���� ���� ����
}



void GameScreen::movePoisonFruit()
{
	CRect clientRect;
	GetClientRect(&clientRect); // Ŭ���̾�Ʈ ���� ũ�� ��������

	// ������� ���� ��ġ ��������
	CRect poisonRect = m_poisonFruit.getRect();

	// ���ο� ��ġ�� �̵�
	poisonRect.OffsetRect(m_poisonSpeedX, m_poisonSpeedY);

	// ȭ�� ��踦 ����� �̵� ���� ����
	if (poisonRect.left < clientRect.left || poisonRect.right > clientRect.right)
	{
		m_poisonSpeedX = -m_poisonSpeedX;
	}
	if (poisonRect.top < clientRect.top || poisonRect.bottom > clientRect.bottom)
	{
		m_poisonSpeedY = -m_poisonSpeedY;
	}

	// ����� ��ġ ������Ʈ
	m_poisonFruit.setRect(poisonRect);
}

void GameScreen::stopPoisonMoveTimer()
{
	if (m_poisonMoveTimerID)
	{
		KillTimer(m_poisonMoveTimerID); // Ÿ�̸� ����
		m_poisonMoveTimerID = 0;
	}
}

