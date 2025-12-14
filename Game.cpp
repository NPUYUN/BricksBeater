#include "Game.h"

// 获取点到矩形的最小距离
float Game::GetDistance_PointToRect(float x, float y, RECT rct)
{
	float x_rct, y_rct;	// 保存矩形内到目标点最近的点
	if (x >= rct.left && x <= rct.right)
		x_rct = x;
	else
		x_rct = (float)(fabsf(x - rct.left) < fabsf(x - rct.right) ? rct.left : rct.right);
	if (y >= rct.top && y <= rct.bottom)
		y_rct = y;
	else
		y_rct = (float)(fabsf(y - rct.top) < fabsf(y - rct.bottom) ? rct.top : rct.bottom);

	float dx = x - x_rct;
	float dy = y - y_rct;

	return sqrtf(dx * dx + dy * dy);
}

// 返回是否存在相切
bool GetTangentCirclePoint(
	float x0,		// 切点坐标
	float y0,
	float x1,		// 圆心轨迹直线上的一点（更早运动到的点）
	float y1,
	float x2,		// 圆心轨迹直线上的另一点（其实运动不到的点）
	float y2,
	float r,		// 圆半径
	float* p_out_x,	// 输出圆心坐标
	float* p_out_y
)
{
	// 斜率不存在时
	if (fabsf(x1 - x2) < 0.00001f)
	{
		// 计算相切时圆心与切点的竖直距离
		float d2 = r * r - (x0 - x1) * (x0 - x1);
		if (d2 < 0)
			return false;
		float d = sqrtf(d2);

		// 求出两组解
		float _y1 = y0 + d;
		float _y2 = y0 - d;

		// 保留离 (x1, y1) 更近的解
		float _y_closer = fabsf(y1 - _y1) < fabsf(y1 - _y2) ? _y1 : _y2;

		*p_out_x = x1;
		*p_out_y = _y_closer;

		return true;
	}

	// 圆心轨迹直线方程：y - y1 = (y2 - y1) / (x2 - x1) * (x - x1)
	// 即：y = kx - kx1 + y1
	// 圆的方程：(x - x0) ^ 2 + (y - y0) ^ 2 = r ^ 2
	// 联立得二次函数，如下。

	float k = (y2 - y1) / (x2 - x1);			// 直线斜率
	float m = -k * x1 + y1 - y0;				// 部分常数
	float a = k * k + 1;						// 二次函数的 abc 系数
	float b = 2 * (k * m - x0);
	float c = x0 * x0 + m * m - r * r;
	float delta = b * b - 4 * a * c;			// 判别式
	if (delta < 0)								// 无解
		return false;
	float sqrt_delta = sqrtf(delta);			// 判别式开根号
	float _x1 = (-b + sqrt_delta) / (2 * a);	// 两个根
	float _x2 = (-b - sqrt_delta) / (2 * a);

	// 保留离 (x1, y1) 更近的解
	float _x_closer = fabsf(x1 - _x1) < fabsf(x1 - _x2) ? _x1 : _x2;
	float _y = k * _x_closer - k * x1 + y1;

	*p_out_x = _x_closer;
	*p_out_y = _y;

	return true;
}

// 单个碰撞箱和单个小球的碰撞处理
int Game::SingleRect_BallHit(Sound& sound, Paint& paint, Paint::Ball* ball, RECT rct, bool is_board, bool left, bool up, bool right, bool down)
{
	float last_x = (ball->x - ball->vx);
	float last_y = (ball->y - ball->vy);

	if (ball->x >= rct.left && ball->x <= rct.right
		&& ball->y >= rct.top && ball->y <= rct.bottom)
	{
		ball->x = last_x;
		ball->y = last_y;
		if (ball->x >= rct.left && ball->x <= rct.right
			&& ball->y >= rct.top && ball->y <= rct.bottom)
		{
			if (ball->vx > 0 && left)
				ball->x = (float)(rct.left - paint.g_nBallRadius);
			else if (ball->vx < 0 && right)
				ball->x = (float)(rct.right + paint.g_nBallRadius);
			if (ball->vy > 0 && up)
				ball->y = (float)(rct.top - paint.g_nBallRadius);
			else if (ball->vy < 0 && down)
				ball->y = (float)(rct.bottom + paint.g_nBallRadius);
		}
		ball->vx *= -1 + rand() % 10 / 1000.f;
		ball->vy *= -1 + rand() % 10 / 1000.f;
		if (is_board)
		{
			ball->y = (float)(paint.g_nMapH * (paint.g_nInnerSideLen + 2 * paint.g_nInnerInterval) + BOARD_Y_DISTACE - BOARD_HALF_THICKNESS - paint.g_nBallRadius - 1);
		}
		return HIT_NORMAL;
	}

	if (!(GetDistance_PointToRect(ball->x, ball->y, rct) <= paint.g_nBallRadius * 0.98f))
		return HIT_NONE;

	int return_flag = HIT_NONE;
	bool cross_left = rct.left > ball->x && fabsf(ball->x - rct.left) <= paint.g_nBallRadius && left;
	bool cross_right = ball->x > rct.right && fabsf(ball->x - rct.right) <= paint.g_nBallRadius && right;
	bool cross_top = rct.top > ball->y && fabsf(ball->y - rct.top) <= paint.g_nBallRadius && up;
	bool cross_bottom = ball->y > rct.bottom && fabsf(ball->y - rct.bottom) <= paint.g_nBallRadius && down;

	bool vertex_judge_flag = true;
	float fVertex_X = 0, fVertex_Y = 0;
	if (cross_left && cross_top) { fVertex_X = (float)rct.left; fVertex_Y = (float)rct.top; }
	else if (cross_right && cross_top) { fVertex_X = (float)rct.right; fVertex_Y = (float)rct.top; }
	else if (cross_left && cross_bottom) { fVertex_X = (float)rct.left; fVertex_Y = (float)rct.bottom; }
	else if (cross_right && cross_bottom) { fVertex_X = (float)rct.right; fVertex_Y = (float)rct.bottom; }
	else {
		if (cross_top || cross_bottom) {
			fVertex_Y = cross_top ? (float)rct.top : (float)rct.bottom;
			if (left && right)
				fVertex_X = (fabsf(ball->x - rct.left) < fabsf(ball->x - rct.right)) ? (float)rct.left : (float)rct.right;
			else if (left) fVertex_X = (float)rct.left;
			else if (right) fVertex_X = (float)rct.right;
			else vertex_judge_flag = false;
		}
		else if (cross_left || cross_right) {
			fVertex_X = cross_left ? (float)rct.left : (float)rct.right;
			if (up && down)
				fVertex_Y = (fabsf(ball->y - rct.top) < fabsf(ball->y - rct.bottom)) ? (float)rct.top : (float)rct.bottom;
			else if (up) fVertex_Y = (float)rct.top;
			else if (down) fVertex_Y = (float)rct.bottom;
			else vertex_judge_flag = false;
		}
		else vertex_judge_flag = false;
	}

	bool isVertexCollision = false;
	if (vertex_judge_flag)
	{
		float fCollisionX, fCollisionY;
		if (!GetTangentCirclePoint(
			fVertex_X, fVertex_Y, last_x, last_y, ball->x, ball->y, (float)paint.g_nBallRadius, &fCollisionX, &fCollisionY))
			goto tag_after_vertex_colision;
		if (GetDistance_PointToRect(fCollisionX, fCollisionY, rct) < paint.g_nBallRadius * 0.98f)
			goto tag_after_vertex_colision;
		float f_dx = fCollisionX - fVertex_X;
		float f_dy = fCollisionY - fVertex_Y;
		float f_radianReflectingSurface = atan2f(f_dy, f_dx);
		float f_radianNormal = f_radianReflectingSurface + PI / 2;
		float f_radianIncidence = atan2f(ball->vy, ball->vx);
		float f_radianReflection = 2 * f_radianNormal - f_radianIncidence;
		ball->vx = cosf(f_radianReflection) * BALL_SPEED;
		ball->vy = sinf(f_radianReflection) * BALL_SPEED;
		ball->x = fCollisionX;
		ball->y = fCollisionY;
		isVertexCollision = true;
	}
tag_after_vertex_colision:
	if (isVertexCollision)
		return_flag = HIT_VERTEX;
	else
	{
		if (cross_left || cross_right)
		{
			ball->vx = -ball->vx;
			return_flag = HIT_NORMAL;
		}
		if (cross_top || cross_bottom)
		{
			if (is_board)
			{
				ball->vx = (float)(ball->x - paint.g_nBoardCenterX) / BOARD_HALF_WIDTH * BALL_SPEED;
				if (fabsf(ball->vx) > BALL_SPEED)
				{
					ball->vx = ball->vx > 0 ? BALL_SPEED : -BALL_SPEED;
					ball->vy = 0;
				}
				else
				{
					ball->vy = (ball->vy < 0 ? 1 : -1) * sqrtf(BALL_SPEED * BALL_SPEED - ball->vx * ball->vx);
				}
			}
			else
			{
				ball->vy = -ball->vy;
			}
			return_flag = HIT_NORMAL;
		}
	}
	if (!paint.g_bOnlyVertexCollisionSound && return_flag != HIT_NONE)
		sound.PlayHitSound(false);
	else if (return_flag == HIT_VERTEX)
		sound.PlayHitSound(false);
	if (!isVertexCollision && return_flag != HIT_NONE)
	{
		ball->x = last_x;
		ball->y = last_y;
	}
	return return_flag;
}

// 消除砖块后，调用此函数幸运创建道具
void Game::GeneratePropByLuck(Paint& paint, float x, float y)
{
	if (paint.g_nGeneratedPropNum == PROP_MAX_NUM)
		return;
	int pnProps[3][2] = {
		{PROP_3X,paint.g_nProbability_3X},
		{PROP_SHOOT_3,paint.g_nProbability_Shoot_3},
		{PROP_HEART,paint.g_nProbability_Heart},
	};
	int choice = rand() % 3;
	if (rand() % 100 < pnProps[choice][1])
	{
		Paint::Prop prop;
		prop.type = pnProps[choice][0];
		prop.used = false;
		prop.x = x;
		prop.y = y;
		paint.g_pstProps[paint.g_nGeneratedPropNum] = prop;
		paint.g_nGeneratedPropNum++;
	}
}

// 某索引位置是否为空白区域（可活动区域）
bool Game::IsEmptyBlock(Paint& paint , int id)
{
	return id < 0 || paint.g_pstMap[id].type == EMPTY;
}

// 对单个小球的碰撞处理
void Game::SingleBallHit(Sound& sound, Paint& paint, Paint::Ball* ball)
{
	int border_thickness = 100;
	RECT rctLeft = { -border_thickness,0,1,getheight() };
	RECT rctRight = { paint.g_nMapW * (paint.g_nInnerSideLen + 2 * paint.g_nInnerInterval) - 1,0,paint.g_nMapW * (paint.g_nInnerSideLen + 2 * paint.g_nInnerInterval) + border_thickness,getheight() };
	RECT rctTop = { -border_thickness,-border_thickness,paint.g_nMapW * (paint.g_nInnerSideLen + 2 * paint.g_nInnerInterval) + border_thickness,1 };
	SingleRect_BallHit(sound, paint, ball, rctLeft, false);
	SingleRect_BallHit(sound, paint, ball, rctRight, false);
	SingleRect_BallHit(sound, paint, ball, rctTop, false);

	if (ball->y > getheight() - STATE_BAR_HEIGHT)
	{
		ball->alive = false;
		return;
	}

	int brick_x = (int)(ball->x / (paint.g_nInnerSideLen + 2 * paint.g_nInnerInterval));
	int brick_y = (int)(ball->y / (paint.g_nInnerSideLen + 2 * paint.g_nInnerInterval));
	int pnOffset[9][2] = {
		{-1,-1}, {0,-1}, {1,-1},
		{-1, 0}, {0, 0}, {1, 0},
		{-1, 1}, {0, 1}, {1, 1},
	};
	for (int i = 0; i < 9; i++)
	{
		int current_x = brick_x + pnOffset[i][0];
		int current_y = brick_y + pnOffset[i][1];
		if (current_x < 0 || current_x >= paint.g_nMapW
			|| current_y < 0 || current_y >= paint.g_nMapH)
			continue;
		int current = current_x + current_y * paint.g_nMapW;
		int left = -1, right = -1, up = -1, down = -1;
		if (current_x > 0) left = current_x - 1 + current_y * paint.g_nMapW;
		if (current_x < paint.g_nMapW - 1) right = current_x + 1 + current_y * paint.g_nMapW;
		if (current_y > 0) up = current_x + (current_y - 1) * paint.g_nMapW;
		if (current_y < paint.g_nMapH - 1) down = current_x + (current_y + 1) * paint.g_nMapW;
		if (paint.g_pstMap[current].type != EMPTY)
		{
			RECT rct = paint.g_pstMap[current].rct;
			int hit_flag = SingleRect_BallHit(
				sound,
				paint,
				ball,
				rct,
				false,
				IsEmptyBlock(paint , left),
				IsEmptyBlock(paint , up),
				IsEmptyBlock(paint , right),
				IsEmptyBlock(paint , down)
			);
			if (hit_flag == HIT_NONE)
				continue;
			else if (paint.g_pstMap[current].type == BRICK)
			{
				paint.g_pstMap[current].type = EMPTY;
				GeneratePropByLuck(paint, (float)rct.left, (float)rct.top);
			}
			break;
		}
	}
	RECT rctBoard = {
		paint.g_nBoardCenterX - BOARD_HALF_WIDTH,
		paint.g_nMapH * (paint.g_nInnerSideLen + 2 * paint.g_nInnerInterval) + BOARD_Y_DISTACE - BOARD_HALF_THICKNESS,
		paint.g_nBoardCenterX + BOARD_HALF_WIDTH,
		paint.g_nMapH * (paint.g_nInnerSideLen + 2 * paint.g_nInnerInterval) + BOARD_Y_DISTACE + BOARD_HALF_THICKNESS
	};
	SingleRect_BallHit(sound, paint, ball, rctBoard, true);
}

// 所有小球的处理
void Game::AllBallProcess(Sound& sound, Paint& paint)
{
	if (paint.g_bBallWait)
		return;
	bool isAnyBall = false;
	for (int i = 0; i < paint.g_nGeneratedBallNum; i++)
	{
		if (paint.g_pstBalls[i].alive)
		{
			isAnyBall = true;
			SingleBallHit(sound, paint, &paint.g_pstBalls[i]);
			paint.g_pstBalls[i].x += paint.g_pstBalls[i].vx + rand() % 10 / 100.f;
			paint.g_pstBalls[i].y += paint.g_pstBalls[i].vy + rand() % 10 / 100.f;
			if (paint.g_pstBalls[i].x < paint.g_nBallRadius)
				paint.g_pstBalls[i].x = paint.g_nBallRadius;
			else if (paint.g_pstBalls[i].x > getwidth() - paint.g_nBallRadius)
				paint.g_pstBalls[i].x = getwidth() - paint.g_nBallRadius;
			if (paint.g_pstBalls[i].y < paint.g_nBallRadius)
				paint.g_pstBalls[i].y = paint.g_nBallRadius;
		}
	}
	if (!isAnyBall)
	{
		if (paint.g_nRemainingBallNum)
			paint.g_bBallWait = true;
		else
			paint.g_enuGameState = paint.GameFail;
	}
}

// 判断是否已经胜利
void Game::CheckSuccess(Paint& paint)
{
	bool isAnyBrick = false;
	for (int i = 0; i < paint.g_nMapW * paint.g_nMapH; i++)
	{
		if (paint.g_pstMap[i].type == BRICK)
		{
			isAnyBrick = true;
			break;
		}
	}
	if (!isAnyBrick)
		paint.g_enuGameState = paint.GameWin;
}

// 获取随机 Vx 和 Vy
void Game::RandSpeedXY(float* vx, float* vy)
{
	*vx = fmodf((float)rand(), BALL_SPEED - rand() % 10 / 10000.f) * (rand() % 2 ? 1 : -1);
	*vy = sqrtf(BALL_SPEED * BALL_SPEED - *vx * *vx) * (rand() % 2 ? 1 : -1);
}

// 生成新的球
void Game::GenerateBall(Paint& paint, bool is_board_launch, float x, float y, bool up)
{
	if (paint.g_nGeneratedBallNum == BALL_MAX_NUM)
		return;
	Paint::Ball new_ball;
	new_ball.alive = true;
	if (is_board_launch)
	{
		int g_nInnerSideLen = paint.g_nInnerSideLen;
		int g_nInnerInterval = paint.g_nInnerInterval;
		int g_nMapH = paint.g_nMapH;
		new_ball.x = (float)paint.g_nBoardCenterX;
		new_ball.y = (float)(paint.g_nMapH * (paint.g_nInnerSideLen + 2 * paint.g_nInnerInterval) + BOARD_Y_DISTACE - BOARD_BALL_DISTANCE);
		new_ball.vx = 0;
		new_ball.vy = -BALL_SPEED;
	}
	else
	{
		new_ball.x = x;
		new_ball.y = y;
		RandSpeedXY(&new_ball.vx, &new_ball.vy);
		if (up && new_ball.vy > 0)
			new_ball.vy = -new_ball.vy;
	}
	paint.g_pstBalls[paint.g_nGeneratedBallNum] = new_ball;
	paint.g_nGeneratedBallNum++;
}

// 发射小球
void Game::LaunchBall(Paint& paint)
{
	if (!paint.g_bBallWait)
		return;
	if (paint.g_nRemainingBallNum)
	{
		GenerateBall(paint, true);
		paint.g_nRemainingBallNum--;
		paint.g_bBallWait = false;
	}
}

// 应用一个道具
void Game::ApplyProp(Paint& paint, Paint::Prop prop)
{
	switch (prop.type)
	{
	case PROP_3X:
	{
		int old_num = paint.g_nGeneratedBallNum;
		for (int i = 0; i < old_num; i++)
		{
			if (!paint.g_pstBalls[i].alive)
				continue;
			for (int j = 0; j < 3; j++)
				GenerateBall(paint, false, paint.g_pstBalls[i].x, paint.g_pstBalls[i].y);
		}
		break;
	}
	case PROP_SHOOT_3:
		for (int i = 0; i < 3; i++) {
			int g_nInnerSideLen = paint.g_nInnerSideLen;
			int g_nInnerInterval = paint.g_nInnerInterval;
			int g_nMapH = paint.g_nMapH;
			GenerateBall(paint, false, prop.x, (float)(paint.g_nMapH * (paint.g_nInnerSideLen + 2 * paint.g_nInnerInterval) - BOARD_Y_DISTACE), true);
		}
		break;
	case PROP_HEART:
		paint.g_nRemainingBallNum++;
		break;
	}
}

// 道具单点碰撞判定
bool Game::SinglePointPropHit(Paint& paint, int index, float x, float y)
{
	return (y >= paint.g_nMapH * (paint.g_nInnerSideLen + 2 * paint.g_nInnerInterval) + BOARD_Y_DISTACE - BOARD_HALF_THICKNESS
		&& y <= paint.g_nMapH * (paint.g_nInnerSideLen + 2 * paint.g_nInnerInterval) + BOARD_Y_DISTACE + BOARD_HALF_THICKNESS
		&& x >= paint.g_nBoardCenterX - BOARD_HALF_WIDTH
		&& x <= paint.g_nBoardCenterX + BOARD_HALF_WIDTH);
}

// 处理道具相关
void Game::PropProcess(Sound& sound, Paint& paint)
{
	for (int i = 0; i < paint.g_nGeneratedPropNum; i++)
	{
		if (paint.g_pstProps[i].used)
			continue;

		float y1 = paint.g_pstProps[i].y;
		float y2 = paint.g_pstProps[i].y + paint.g_img3X.getheight();
		float x1 = paint.g_pstProps[i].x;
		float x2 = paint.g_pstProps[i].x + paint.g_img3X.getwidth();

		if (SinglePointPropHit(paint, i, x1, y1)
			|| SinglePointPropHit(paint, i, x2, y2)
			|| SinglePointPropHit(paint, i, (x1 + x2) / 2, (y1 + y2) / 2))
		{
			ApplyProp(paint, paint.g_pstProps[i]);
			paint.g_pstProps[i].used = true;
			sound.PlayHitSound(true);
			continue;
		}
		paint.g_pstProps[i].y += PROP_DROP_SPEED;
	}
}

// 用户输入处理
void Game::UserInputProcess(Paint& paint, RECT g_rctRestartBtn)
{
	if (paint.g_bInMenu)
	{
		while (!getmessage(EX_MOUSE).lbutton);
		LaunchBall(paint);
		paint.g_bInMenu = false;
	}
	ExMessage msg;
	while (peekmessage(&msg, EX_MOUSE))
	{
		if (paint.g_bBallWait)
		{
			if (msg.lbutton)
			{
				LaunchBall(paint);
			}
		}
		else
		{
			paint.g_nBoardCenterX = msg.x;
		}
		if (msg.message == WM_LBUTTONUP
			&& msg.x >= g_rctRestartBtn.left && msg.x <= g_rctRestartBtn.right
			&& msg.y >= g_rctRestartBtn.top && msg.y <= g_rctRestartBtn.bottom)
		{
			paint.FreeLevel();
			paint.InitLevel();
		}
	}
}

// 游戏过程函数
void Game::GameProcess(Sound& sound, Paint& paint)
{
	AllBallProcess(sound, paint);
	PropProcess(sound, paint);
	CheckSuccess(paint);
}

// 延时以稳定帧率
void Game::DelayFPS()
{
	static clock_t cRecord = clock();
	clock_t consumption = clock() - cRecord;
	clock_t sleep = 30 - consumption;
	if (sleep > 0)
		Sleep(sleep);
	cRecord = clock();
}