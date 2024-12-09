#pragma once
#include <stdlib.h>
#include <time.h>
#define DU2SU (1000./1750.) //다이얼로그 단위 / 화면단위 
#define SCREEN_TIMER    (1)
#define FPS (60) // frame per second

// 난수 발생을 초기화
inline void randseed()
{
	time_t nTime = time(NULL);
	srand((unsigned int)llabs(nTime * nTime));
}

// rmin, rmin+1, ..., rmax-1 사이의 정수 출력
inline int randrange(int rmin, int rmax)
{
	if (rmin < rmax)
	{
		int nRand = rand() % (rmax - rmin);
		return nRand + rmin;
	}
	else if (rmin == rmax) return rmin;
	else return 0;
}