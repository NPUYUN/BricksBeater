#include "Sound.h"
// 播放碰撞声
// prop 是否播放道具声
void Sound::PlayHitSound(bool prop)
{
	static bool flag = true;			// 乒乓碰撞声交替标记
	static clock_t record = 0;			// 上次操作的记录

	if (prop)
	{
		mciSendString(_T("play prop from 0"), NULL, 0, NULL);
	}
	else
	{
		// 避免过快操作，否则会一直从 0 开始播放，发不出声音
		if (clock() - record > 30)
		{
			if (flag)
				mciSendString(_T("play ping from 0"), NULL, 0, NULL);
			else
				mciSendString(_T("play pong from 0"), NULL, 0, NULL);
			record = clock();
			flag = !flag;
		}
	}
}