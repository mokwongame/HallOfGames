
// SnakegameDlg.h: 헤더 파일
//

#pragma once

#include "GameScreen.h"
// CSnakegameDlg 대화 상자
class CSnakegameDlg : public CDialogEx
{
// 생성입니다.
public:
	CSnakegameDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	afx_msg void OnStnClickedScreen();
	afx_msg void OnBnClickedOk();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnStnClickedStaticGame();
	void restartGame();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SNAKEGAME_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.
	HICON m_hIcon;
	GameScreen m_screen;
	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	GameScreen m_gameScreen; // 게임 화면 객체
	afx_msg void OnBnClickedButtonStart(); // 버튼 클릭 이벤트
	
public:
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButtonExit();
};
