//#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h> // strlen() 사용
#include<stdlib.h> // abs() 사용
#include<time.h> // clock() 사용
#include"LibConsole.h"
#include"LibGameTool.h"
#define STR_MAX (1000) // 문자열의 문자 개수의 최대값
// 2023-06-16일자 영상 제작.

// 전역 변수 설정: 함수 정의 바깥에 선언한 변수; 누구나 접근 가능
char quotes[][STR_MAX] =
{ "The greatest glory in living lies not in never falling, but in rising every time we fall.",
"The way to get started is to quit talking and begin doing.",
	"If life were predictable it would cease to be life, and be without flavor.",
"Life is what happens when you're busy making other plans.",
"Spread love everywhere you go. Let no one ever come to you without leaving happier.",
"When you reach the end of your rope, tie a knot in it and hang on.",
"Always remember that you are absolutely unique. Just like everyone else.",
"The future belongs to those who believe in the beauty of their dreams.",
"Tell me and I forget. Teach me and I remember. Involve me and I learn.",
"It is during our darkest moments that we must focus to see the light." };

char Easy_q[][STR_MAX] =
{ "Seeing is believing", "Whatever you do, make it pay", "You will never know until you try",
 "No sweat, no sweet", "A day without laughter is a day wasted", "The die is cast.",
	"This too shall pass.", "Life is unfair, get used to it.", "All we have is now.",
	"Believe in yourself.", "No pain no gain." };
int nQuote;
int totalScore;
char strin[STR_MAX]; // char의 배열 = 문자열; 문자열 입력을 받기 위해 메모리 공간을 STR_MAX만큼 확보
//int score = 0;
// 함수 선언
void initGame();
int printRandStr();
void inputStr();
int calcScore(int nRand);
void printTypeScore(int score, double type_T);
int totalScore;
int ExitScore = 0; // 전역 변수 선언해서 어디에서든 사용할 수 있게하기
void LevelUP(); // 레벨업 
void GameStartScene();
bool Yee = true;
bool Exite = true;
char Retry[20];
void main()
{
	GameStartScene(); initGame();
	while (1)
	{
		int nRand = printRandStr();
		clock_t start_T = clock(); // 시간 측정: 프로그램 시작할 때 0으로 시간을 시작; 정밀도 msec
		inputStr();
		clock_t stop_T = clock();
		double type_T = (stop_T - start_T) * 0.001; // 초 단위로 계산한 시간
		int score = calcScore(nRand); // 스코어 계산하기
		printTypeScore(score, type_T);
		if (totalScore >= 100 && Yee)
		{
			Yee = false; LevelUP();
		}// 레벨업 호출 하기
		if (totalScore >= 500 && Exite) // 200점 이상 득점 할 시 게임 멈추기
		{
			settextcol(GRAY);
			puts("====================================================");
			puts("==                 게임 클리어!!!                 ==");
			puts("===================================================="); Exite = false;
			while (1)
			{
				settextcol(GREEN);
				puts("재도전? [Yes/No]");
				//scanf("%s ", Retry);
				gets(Retry);
				if (strcmp(Retry, "Yes") == 0)
				{
					ExitScore = 0; totalScore = 0; Exite = true; continue;
				}
				else if (strcmp(Retry, "No") == 0)
				{
					break;
				}
				else puts("Error:: 다시 시도해 주십시오.");
			}
		}
		else if (ExitScore <= -5 && Exite) // 아웃 5회 도달시 게임 멈추기
		{
			settextcol(GRAY);
			puts("====================================================");
			puts("==                  게임 아웃!!!                  ==");
			puts("===================================================="); Exite = false;
			while (1)
			{
				settextcol(GREEN);
				puts("재도전? [네/아니오]");
				//scanf("%s ", Retry);
				gets(Retry);
				if (strcmp(Retry, "네") == 0)
				{
					ExitScore = 0; totalScore = 0; Exite = true; break;
				}
				else if (strcmp(Retry, "아니오") == 0)
				{
					break;
				}
				else puts("Error:: 다시 시도해 주십시오.");
			}
		}
		settextcol(GREEN);
		if (strcmp(Retry, "네") == 0)
		{
			continue;
		}
		else if (strcmp(Retry, "아니오") == 0)
		{
			settextcol(GRAY);
			puts("====================================================");
			puts("==             프로그램을 종료합니다.             ==");
			puts("===================================================="); break;
		}
	}
	settextcol(YELLOW);
}
// 함수 정의
void initGame()
{
	randseed();
	if (totalScore >= 100) nQuote = (int)(sizeof(quotes) / STR_MAX);
	else if (totalScore < 100) nQuote = (int)(sizeof(Easy_q) / STR_MAX);
	totalScore = 0;
}
// 입력 없음, 출력 = int
int printRandStr()
{
	settextcol(BLUE);
	int nRand = randrange(0, nQuote); // 0, 1, 2, ,,,, nQuote -1 범위의 난수 발생
	if (totalScore < 100) puts(Easy_q[nRand]);
	else if (totalScore >= 100) puts(quotes[nRand]); return nRand;
}

void inputStr()
{
	settextcol(DARK_SKYBLUE); puts("문자열을 입력하시오.:::\n");
	settextcol(WHITE); gets_s(strin, STR_MAX - 1);
	settextcol(RED); puts("입력됨::");
	settextcol(BLUE); puts(strin);
}

int calcScore(int nRand)
{
	settextcol(GREEN);	int score = 0;
	if (totalScore < 100) {
		int nRandLen = (int)strlen(Easy_q[nRand]); // 난수 문자열의 길이
		int nInputlen = (int)strlen(strin); // 입력 문자열의 길이
		int nMinLen = (nRandLen < nInputlen) ? nRandLen : nInputlen;
		for (int i = 0; i < nMinLen; i++)
		{
			if (Easy_q[nRand][i] == strin[i])score++;
			else { ExitScore -= 1; score -= 1; } // (nRandLen - nInputlen);
		}
		totalScore += score; // totalScore = totalScore + score
		printf("득점::: %d\n", score);
		printf("총 득점::: %d\n", totalScore); return score; settextcol(BLUE);
	}
	else if (totalScore >= 100)
	{
		int nRandLen = (int)strlen(quotes[nRand]);
		int nInputlen = (int)strlen(strin); // 입력 문자열의 길이
		int nMinLen = (nRandLen < nInputlen) ? nRandLen : nInputlen;
		for (int i = 0; i < nMinLen; i++)
		{
			if (quotes[nRand][i] == strin[i]) score++;
			else { ExitScore -= 1; score -= 1; } // (nRandLen - nInputlen);
		}
		totalScore += score; // totalScore = totalScore + score
		printf("득점::: %d\n", score);
		printf("총 득점::: %d\n", totalScore); return score; settextcol(BLUE);
	}
}
void printTypeScore(int score, double type_T)
{
	double typeSpeed = score / type_T * 60; // score/type_T*60 = 분당 타수
	printf("분당 타수: %g\n\n", typeSpeed);
}
void GameStartScene()
{
	settextcol(VIOLET);
	puts("====================================================");
	puts("==    목원대 타자 게임에 오신 것을 환영합니다.    ==");
	puts("====================================================");
	puts("==                      Rule                      ==");
	puts("==     타자를 정확히 입력해 점수를 얻으세요!!     ==");
	puts("==               500점을 얻으면 클리어!!          ==");
	puts("==     5회 또는 5글자 이상 틀리면은 아웃입니다!!  ==");
	puts("==        점수가 올라갈수록 더 어려워 집니다!!    ==");
	puts("==                  행운을 빕니다!!               ==");
	puts("====================================================");
}
void LevelUP()
{
	settextcol(RED);
	puts("=====================Level Up!!=====================");
	puts("==        영어 문자의 난이도가 올라갑니다!!       ==");
	puts("====================================================");
}
