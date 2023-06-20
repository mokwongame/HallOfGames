//#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h> // strlen() ���
#include<stdlib.h> // abs() ���
#include<time.h> // clock() ���
#include"LibConsole.h"
#include"LibGameTool.h"
#define STR_MAX (1000) // ���ڿ��� ���� ������ �ִ밪
// 2023-06-16���� ���� ����.

// ���� ���� ����: �Լ� ���� �ٱ��� ������ ����; ������ ���� ����
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
char strin[STR_MAX]; // char�� �迭 = ���ڿ�; ���ڿ� �Է��� �ޱ� ���� �޸� ������ STR_MAX��ŭ Ȯ��
//int score = 0;
// �Լ� ����
void initGame();
int printRandStr();
void inputStr();
int calcScore(int nRand);
void printTypeScore(int score, double type_T);
int totalScore;
int ExitScore = 0; // ���� ���� �����ؼ� ��𿡼��� ����� �� �ְ��ϱ�
void LevelUP(); // ������ 
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
		clock_t start_T = clock(); // �ð� ����: ���α׷� ������ �� 0���� �ð��� ����; ���е� msec
		inputStr();
		clock_t stop_T = clock();
		double type_T = (stop_T - start_T) * 0.001; // �� ������ ����� �ð�
		int score = calcScore(nRand); // ���ھ� ����ϱ�
		printTypeScore(score, type_T);
		if (totalScore >= 100 && Yee)
		{
			Yee = false; LevelUP();
		}// ������ ȣ�� �ϱ�
		if (totalScore >= 500 && Exite) // 200�� �̻� ���� �� �� ���� ���߱�
		{
			settextcol(GRAY);
			puts("====================================================");
			puts("==                 ���� Ŭ����!!!                 ==");
			puts("===================================================="); Exite = false;
			while (1)
			{
				settextcol(GREEN);
				puts("�絵��? [Yes/No]");
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
				else puts("Error:: �ٽ� �õ��� �ֽʽÿ�.");
			}
		}
		else if (ExitScore <= -5 && Exite) // �ƿ� 5ȸ ���޽� ���� ���߱�
		{
			settextcol(GRAY);
			puts("====================================================");
			puts("==                  ���� �ƿ�!!!                  ==");
			puts("===================================================="); Exite = false;
			while (1)
			{
				settextcol(GREEN);
				puts("�絵��? [��/�ƴϿ�]");
				//scanf("%s ", Retry);
				gets(Retry);
				if (strcmp(Retry, "��") == 0)
				{
					ExitScore = 0; totalScore = 0; Exite = true; break;
				}
				else if (strcmp(Retry, "�ƴϿ�") == 0)
				{
					break;
				}
				else puts("Error:: �ٽ� �õ��� �ֽʽÿ�.");
			}
		}
		settextcol(GREEN);
		if (strcmp(Retry, "��") == 0)
		{
			continue;
		}
		else if (strcmp(Retry, "�ƴϿ�") == 0)
		{
			settextcol(GRAY);
			puts("====================================================");
			puts("==             ���α׷��� �����մϴ�.             ==");
			puts("===================================================="); break;
		}
	}
	settextcol(YELLOW);
}
// �Լ� ����
void initGame()
{
	randseed();
	if (totalScore >= 100) nQuote = (int)(sizeof(quotes) / STR_MAX);
	else if (totalScore < 100) nQuote = (int)(sizeof(Easy_q) / STR_MAX);
	totalScore = 0;
}
// �Է� ����, ��� = int
int printRandStr()
{
	settextcol(BLUE);
	int nRand = randrange(0, nQuote); // 0, 1, 2, ,,,, nQuote -1 ������ ���� �߻�
	if (totalScore < 100) puts(Easy_q[nRand]);
	else if (totalScore >= 100) puts(quotes[nRand]); return nRand;
}

void inputStr()
{
	settextcol(DARK_SKYBLUE); puts("���ڿ��� �Է��Ͻÿ�.:::\n");
	settextcol(WHITE); gets_s(strin, STR_MAX - 1);
	settextcol(RED); puts("�Էµ�::");
	settextcol(BLUE); puts(strin);
}

int calcScore(int nRand)
{
	settextcol(GREEN);	int score = 0;
	if (totalScore < 100) {
		int nRandLen = (int)strlen(Easy_q[nRand]); // ���� ���ڿ��� ����
		int nInputlen = (int)strlen(strin); // �Է� ���ڿ��� ����
		int nMinLen = (nRandLen < nInputlen) ? nRandLen : nInputlen;
		for (int i = 0; i < nMinLen; i++)
		{
			if (Easy_q[nRand][i] == strin[i])score++;
			else { ExitScore -= 1; score -= 1; } // (nRandLen - nInputlen);
		}
		totalScore += score; // totalScore = totalScore + score
		printf("����::: %d\n", score);
		printf("�� ����::: %d\n", totalScore); return score; settextcol(BLUE);
	}
	else if (totalScore >= 100)
	{
		int nRandLen = (int)strlen(quotes[nRand]);
		int nInputlen = (int)strlen(strin); // �Է� ���ڿ��� ����
		int nMinLen = (nRandLen < nInputlen) ? nRandLen : nInputlen;
		for (int i = 0; i < nMinLen; i++)
		{
			if (quotes[nRand][i] == strin[i]) score++;
			else { ExitScore -= 1; score -= 1; } // (nRandLen - nInputlen);
		}
		totalScore += score; // totalScore = totalScore + score
		printf("����::: %d\n", score);
		printf("�� ����::: %d\n", totalScore); return score; settextcol(BLUE);
	}
}
void printTypeScore(int score, double type_T)
{
	double typeSpeed = score / type_T * 60; // score/type_T*60 = �д� Ÿ��
	printf("�д� Ÿ��: %g\n\n", typeSpeed);
}
void GameStartScene()
{
	settextcol(VIOLET);
	puts("====================================================");
	puts("==    ����� Ÿ�� ���ӿ� ���� ���� ȯ���մϴ�.    ==");
	puts("====================================================");
	puts("==                      Rule                      ==");
	puts("==     Ÿ�ڸ� ��Ȯ�� �Է��� ������ ��������!!     ==");
	puts("==               500���� ������ Ŭ����!!          ==");
	puts("==     5ȸ �Ǵ� 5���� �̻� Ʋ������ �ƿ��Դϴ�!!  ==");
	puts("==        ������ �ö󰥼��� �� ����� ���ϴ�!!    ==");
	puts("==                  ����� ���ϴ�!!               ==");
	puts("====================================================");
}
void LevelUP()
{
	settextcol(RED);
	puts("=====================Level Up!!=====================");
	puts("==        ���� ������ ���̵��� �ö󰩴ϴ�!!       ==");
	puts("====================================================");
}
