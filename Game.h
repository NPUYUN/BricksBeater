#pragma once  
#include "Sound.h"  

class Game {  
	public:  
		float GetDistance_PointToRect(float x, float y, RECT rct);  
		int SingleRect_BallHit(Sound& sound , Paint& paint, Paint::Ball* ball , RECT rct, bool is_board, bool left = true, bool up = true, bool right = true, bool down = true);
		void GeneratePropByLuck(Paint& paint , float x, float y);  
		bool IsEmptyBlock(Paint& paint , int id);  
		void SingleBallHit(Sound& sound, Paint& paint, Paint::Ball* ball);
		void AllBallProcess(Sound& sound , Paint& paint);  
		void CheckSuccess(Paint& paint);  
		void RandSpeedXY(float* vx, float* vy);  
		void GenerateBall(Paint& paint , bool is_board_launch, float x = 0, float y = 0, bool up = false);  
		void LaunchBall(Paint& paint);  
		void ApplyProp(Paint& paint , Paint::Prop prop);
		bool SinglePointPropHit(Paint& paint , int index, float x, float y);  
		void PropProcess(Sound& sound, Paint& paint);  
		void UserInputProcess(Paint& paint, RECT g_rctRestartBtn);  
		void GameProcess(Sound& sound , Paint& paint);  
		void DelayFPS();  
};
