#pragma once

class GameManager
{
public:
	static int getScore(void) { return m_nScore; }
	static void incScore(void) { m_nScore++; }
	static void initScore(void) { m_nScore = 0; }
	static void decScore(void) { if (m_nScore > 0) m_nScore--; }
	// HP
	static void initHp(int hp = 3) { m_nHp = hp; }
	static int getHp() { return m_nHp; }
	static void decHp() { if (m_nHp > 0) m_nHp--; }
	static bool isDead() { return m_nHp <= 0; }

protected:
	static int m_nScore;
	static int m_nHp;
};

