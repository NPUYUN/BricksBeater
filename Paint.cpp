#include "Paint.h"

// Paint.cpp
#include "Paint.h"
#include <algorithm> // for std::copy

// 默认构造函数
Paint::Paint()
	: g_pppszOriginalLevelMap(nullptr),
	g_pstMap(nullptr),
	g_nLevelNum(0),
	g_nMapW(0),
	g_nMapH(0)
{
	// 初始化其他成员变量
	g_nInnerSideLen = 7;
	g_nInnerInterval = 1;
	g_nBallRadius = 3;
	g_bInMenu = true;
	g_enuGameState = Gaming;
	g_nGeneratedBallNum = 0;
	g_bBallWait = true;
	g_nRemainingBallNum = BALL_LIFE_NUM;
	g_nGeneratedPropNum = 0;
	g_nBoardCenterX = 0;

	// 调试模式默认值
	g_bDebugMode = false;
	g_bOnlyVertexCollisionSound = false;
	g_bBigScene = false;
	g_nInnerSideLen_BigScene = 16;
	g_nBallRadius_BigScene = 10;
	g_nProbability_3X = 8;
	g_nProbability_Shoot_3 = 12;
	g_nProbability_Heart = 5;
}

// 拷贝构造函数
Paint::Paint(const Paint& other) {
	DeepCopy(other);
}

// 拷贝赋值运算符
Paint& Paint::operator=(const Paint& other) {
	if (this != &other) {
		// 先释放现有资源
		FreeResourceInternal();
		FreeLevelInternal();

		// 深拷贝
		DeepCopy(other);
	}
	return *this;
}

// 移动构造函数
Paint::Paint(Paint&& other) noexcept
	: g_pppszOriginalLevelMap(nullptr),
	g_pstMap(nullptr)
{
	// 转移资源所有权
	g_pppszOriginalLevelMap = other.g_pppszOriginalLevelMap;
	g_pstMap = other.g_pstMap;

	// 转移基本类型成员
	g_nLevelNum = other.g_nLevelNum;
	g_nMapW = other.g_nMapW;
	g_nMapH = other.g_nMapH;
	g_nCurrentLevel = other.g_nCurrentLevel;
	g_nBoardCenterX = other.g_nBoardCenterX;

	// 转移图像资源
	g_imgRestart = other.g_imgRestart;
	g_img3X = other.g_img3X;
	g_imgShoot3 = other.g_imgShoot3;
	g_imgHeart = other.g_imgHeart;

	// 转移游戏状态
	g_bInMenu = other.g_bInMenu;
	g_enuGameState = other.g_enuGameState;
	g_nGeneratedBallNum = other.g_nGeneratedBallNum;
	g_bBallWait = other.g_bBallWait;
	g_nRemainingBallNum = other.g_nRemainingBallNum;
	g_nGeneratedPropNum = other.g_nGeneratedPropNum;

	// 转移调试设置
	g_bDebugMode = other.g_bDebugMode;
	g_bOnlyVertexCollisionSound = other.g_bOnlyVertexCollisionSound;
	g_bBigScene = other.g_bBigScene;
	g_nInnerSideLen_BigScene = other.g_nInnerSideLen_BigScene;
	g_nBallRadius_BigScene = other.g_nBallRadius_BigScene;

	// 转移概率设置
	g_nProbability_3X = other.g_nProbability_3X;
	g_nProbability_Shoot_3 = other.g_nProbability_Shoot_3;
	g_nProbability_Heart = other.g_nProbability_Heart;

	// 转移重启按钮区域
	g_rctRestartBtn = other.g_rctRestartBtn;

	// 转移小球和道具状态
	for (int i = 0; i < BALL_MAX_NUM; i++) {
		g_pstBalls[i] = other.g_pstBalls[i];
	}
	for (int i = 0; i < PROP_MAX_NUM; i++) {
		g_pstProps[i] = other.g_pstProps[i];
	}

	// 置空源对象指针
	other.g_pppszOriginalLevelMap = nullptr;
	other.g_pstMap = nullptr;
	other.g_nLevelNum = 0;
}

// 移动赋值运算符
Paint& Paint::operator=(Paint&& other) noexcept {
	if (this != &other) {
		// 释放现有资源
		FreeResourceInternal();
		FreeLevelInternal();

		// 转移资源所有权
		g_pppszOriginalLevelMap = other.g_pppszOriginalLevelMap;
		g_pstMap = other.g_pstMap;

		// 转移基本类型成员
		g_nLevelNum = other.g_nLevelNum;
		g_nMapW = other.g_nMapW;
		g_nMapH = other.g_nMapH;
		g_nCurrentLevel = other.g_nCurrentLevel;
		g_nBoardCenterX = other.g_nBoardCenterX;

		// 转移图像资源
		g_imgRestart = other.g_imgRestart;
		g_img3X = other.g_img3X;
		g_imgShoot3 = other.g_imgShoot3;
		g_imgHeart = other.g_imgHeart;

		// 转移游戏状态
		g_bInMenu = other.g_bInMenu;
		g_enuGameState = other.g_enuGameState;
		g_nGeneratedBallNum = other.g_nGeneratedBallNum;
		g_bBallWait = other.g_bBallWait;
		g_nRemainingBallNum = other.g_nRemainingBallNum;
		g_nGeneratedPropNum = other.g_nGeneratedPropNum;

		// 转移调试设置
		g_bDebugMode = other.g_bDebugMode;
		g_bOnlyVertexCollisionSound = other.g_bOnlyVertexCollisionSound;
		g_bBigScene = other.g_bBigScene;
		g_nInnerSideLen_BigScene = other.g_nInnerSideLen_BigScene;
		g_nBallRadius_BigScene = other.g_nBallRadius_BigScene;

		// 转移概率设置
		g_nProbability_3X = other.g_nProbability_3X;
		g_nProbability_Shoot_3 = other.g_nProbability_Shoot_3;
		g_nProbability_Heart = other.g_nProbability_Heart;

		// 转移重启按钮区域
		g_rctRestartBtn = other.g_rctRestartBtn;

		// 转移小球和道具状态
		for (int i = 0; i < BALL_MAX_NUM; i++) {
			g_pstBalls[i] = other.g_pstBalls[i];
		}
		for (int i = 0; i < PROP_MAX_NUM; i++) {
			g_pstProps[i] = other.g_pstProps[i];
		}

		// 置空源对象指针
		other.g_pppszOriginalLevelMap = nullptr;
		other.g_pstMap = nullptr;
		other.g_nLevelNum = 0;
	}
	return *this;
}

// 析构函数
Paint::~Paint() {
	FreeResourceInternal();
	FreeLevelInternal();
}

// 深拷贝辅助函数
void Paint::DeepCopy(const Paint& other) {
	// 复制基本类型成员
	g_nLevelNum = other.g_nLevelNum;
	g_nMapW = other.g_nMapW;
	g_nMapH = other.g_nMapH;
	g_nCurrentLevel = other.g_nCurrentLevel;
	g_nBoardCenterX = other.g_nBoardCenterX;

	// 复制图像资源
	g_imgRestart = other.g_imgRestart;
	g_img3X = other.g_img3X;
	g_imgShoot3 = other.g_imgShoot3;
	g_imgHeart = other.g_imgHeart;

	// 复制游戏状态
	g_bInMenu = other.g_bInMenu;
	g_enuGameState = other.g_enuGameState;
	g_nGeneratedBallNum = other.g_nGeneratedBallNum;
	g_bBallWait = other.g_bBallWait;
	g_nRemainingBallNum = other.g_nRemainingBallNum;
	g_nGeneratedPropNum = other.g_nGeneratedPropNum;

	// 复制调试设置
	g_bDebugMode = other.g_bDebugMode;
	g_bOnlyVertexCollisionSound = other.g_bOnlyVertexCollisionSound;
	g_bBigScene = other.g_bBigScene;
	g_nInnerSideLen_BigScene = other.g_nInnerSideLen_BigScene;
	g_nBallRadius_BigScene = other.g_nBallRadius_BigScene;

	// 复制概率设置
	g_nProbability_3X = other.g_nProbability_3X;
	g_nProbability_Shoot_3 = other.g_nProbability_Shoot_3;
	g_nProbability_Heart = other.g_nProbability_Heart;

	// 复制重启按钮区域
	g_rctRestartBtn = other.g_rctRestartBtn;

	// 复制小球和道具状态
	for (int i = 0; i < BALL_MAX_NUM; i++) {
		g_pstBalls[i] = other.g_pstBalls[i];
	}
	for (int i = 0; i < PROP_MAX_NUM; i++) {
		g_pstProps[i] = other.g_pstProps[i];
	}

	// 深拷贝关卡数据
	if (other.g_pppszOriginalLevelMap && g_nLevelNum > 0) {
		g_pppszOriginalLevelMap = new char** [g_nLevelNum];
		for (int i = 0; i < g_nLevelNum; i++) {
			g_pppszOriginalLevelMap[i] = new char* [g_nMapH];
			for (int y = 0; y < g_nMapH; y++) {
				g_pppszOriginalLevelMap[i][y] = new char[g_nMapW + 1];
				memcpy(g_pppszOriginalLevelMap[i][y],
					other.g_pppszOriginalLevelMap[i][y],
					(g_nMapW + 1) * sizeof(char));
			}
		}
	}
	else {
		g_pppszOriginalLevelMap = nullptr;
	}

	// 深拷贝当前关卡地图
	if (other.g_pstMap && g_nMapW > 0 && g_nMapH > 0) {
		int mapSize = g_nMapW * g_nMapH;
		g_pstMap = new Brick[mapSize];
		for (int i = 0; i < mapSize; i++) {
			g_pstMap[i] = other.g_pstMap[i];
		}
	}
	else {
		g_pstMap = nullptr;
	}
}

// 释放资源内部实现
void Paint::FreeResourceInternal() {
	// 释放原始关卡数据
	if (g_pppszOriginalLevelMap) {
		for (int i = 0; i < g_nLevelNum; i++) {
			if (g_pppszOriginalLevelMap[i]) {
				for (int y = 0; y < g_nMapH; y++) {
					if (g_pppszOriginalLevelMap[i][y]) {
						delete[] g_pppszOriginalLevelMap[i][y];
					}
				}
				delete[] g_pppszOriginalLevelMap[i];
			}
		}
		delete[] g_pppszOriginalLevelMap;
		g_pppszOriginalLevelMap = nullptr;
	}
	g_nLevelNum = 0;

	// 停止并关闭音频
	mciSendString(_T("stop ping"), NULL, 0, NULL);
	mciSendString(_T("stop pong"), NULL, 0, NULL);
	mciSendString(_T("stop prop"), NULL, 0, NULL);
	mciSendString(_T("close ping"), NULL, 0, NULL);
	mciSendString(_T("close pong"), NULL, 0, NULL);
	mciSendString(_T("close prop"), NULL, 0, NULL);
}

// 释放当前关卡资源内部实现
void Paint::FreeLevelInternal() {
	if (g_pstMap) {
		delete[] g_pstMap;
		g_pstMap = nullptr;
	}
}

// 原有的FreeResource和FreeLevel函数修改为调用内部实现
void Paint::FreeResource() {
	FreeResourceInternal();
}

void Paint::FreeLevel() {
	FreeLevelInternal();
}

void Paint::InitResource()
{
	// 读取配置
	g_nMapW = GetIniFileInfoInt(SETTINGS_PATH, _T("game"), _T("width"), 0);
	g_nMapH = GetIniFileInfoInt(SETTINGS_PATH, _T("game"), _T("height"), 0);
	g_nLevelNum = GetIniFileInfoInt(SETTINGS_PATH, _T("game"), _T("level_num"), 0);
	g_nCurrentLevel = GetIniFileInfoInt(SETTINGS_PATH, _T("game"), _T("begin_level"), 1) - 1;

	g_bDebugMode = GetIniFileInfoInt(SETTINGS_PATH, _T("debug"), _T("enable_debug_mode"), 0);
	if (g_bDebugMode)
	{
		g_bOnlyVertexCollisionSound =
			GetIniFileInfoInt(SETTINGS_PATH, _T("debug"), _T("only_vertex_collision_sound"), 0);
		g_bBigScene =
			GetIniFileInfoInt(SETTINGS_PATH, _T("debug"), _T("enable_big_scene"), 0);
		g_nInnerSideLen_BigScene =
			GetIniFileInfoInt(SETTINGS_PATH, _T("debug"), _T("big_scene_brick_inner_side_len"), 0);
		g_nBallRadius_BigScene =
			GetIniFileInfoInt(SETTINGS_PATH, _T("debug"), _T("big_scene_ball_radius"), 0);

		// 简单模式
		if (GetIniFileInfoInt(SETTINGS_PATH, _T("debug"), _T("easy_mode"), 0))
		{
			g_nProbability_3X = 100;
		}
	}
	if (g_bBigScene)
	{
		g_nInnerSideLen = g_nInnerSideLen_BigScene;
		g_nBallRadius = g_nBallRadius_BigScene;
	}

	// 加载关卡
	// 分配每个关卡的内存
	g_pppszOriginalLevelMap = new char** [g_nLevelNum];

	for (int i = 0; i < g_nLevelNum; i++)
	{
		// 读取每个关卡文件
		FILE* fp;
		char pszPath[256] = {};
		sprintf_s(pszPath, "%s/level_%d.dat", LEVEL_FOLDER_PATH, i + 1);
		if (fopen_s(&fp, pszPath, "r"))
		{
			MessageBox(nullptr, _T("Error"), _T("加载关卡失败"), MB_OK);
			exit(-1);
		}

		// 分配每一行的内存
		g_pppszOriginalLevelMap[i] = new char* [g_nMapH];
		for (int y = 0; y < g_nMapH; y++)
		{
			// 分配每个字符的内存
			g_pppszOriginalLevelMap[i][y] = new char[g_nMapW + 1];
			memset(g_pppszOriginalLevelMap[i][y], 0, (g_nMapW + 1) * sizeof(char));

			// 读取一行
			fread_s(g_pppszOriginalLevelMap[i][y], g_nMapW + 1, g_nMapW, 1, fp);
			fseek(fp, 2, SEEK_CUR);	// 跳过换行符
		}

		fclose(fp);
	}

	// 加载音像资源

	mciSendString(_T("open ") MUSIC_PING_PATH _T(" alias ping"), NULL, 0, NULL);
	mciSendString(_T("open ") MUSIC_PONG_PATH _T(" alias pong"), NULL, 0, NULL);
	mciSendString(_T("open ") MUSIC_PROP_PATH _T(" alias prop"), NULL, 0, NULL);

	loadimage(&g_imgRestart, IMAGE_RESTART);
	loadimage(&g_img3X, IMAGE_3X_PATH);
	loadimage(&g_imgShoot3, IMAGE_SHOOT_3_PATH);
	loadimage(&g_imgHeart, IMAGE_HEART_PATH);
}

// 绘制道具
void Paint::DrawProp()
{
	for (int i = 0; i < g_nGeneratedPropNum; i++)
	{
		if (g_pstProps[i].used)
			continue;

		IMAGE* pimg = nullptr;
		switch (g_pstProps[i].type)
		{
		case PROP_3X:		pimg = &g_img3X;		break;
		case PROP_SHOOT_3:	pimg = &g_imgShoot3;	break;
		case PROP_HEART:	pimg = &g_imgHeart;		break;
		}

		if (pimg)
			putimage((int)g_pstProps[i].x, (int)g_pstProps[i].y, pimg);
	}
}

// 绘制地图
void Paint::DrawMap()
{
	for (int i = 0; i < g_nMapW * g_nMapH; i++)
	{
		if (g_pstMap[i].type != EMPTY)
		{
			RECT rct = g_pstMap[i].rct;

			setfillcolor(g_pstMap[i].color);
			solidrectangle(
				rct.left + g_nInnerInterval,
				rct.top + g_nInnerInterval,
				rct.right - g_nInnerInterval - 1,
				rct.bottom - g_nInnerInterval - 1
			);
		}
	}
}

// 绘制小球
void Paint::DrawBall()
{
	setfillcolor(WHITE);

	// 小球还没发射
	if (g_bBallWait)
	{
		solidcircle(g_nBoardCenterX, BOARD_Y - BOARD_BALL_DISTANCE, g_nBallRadius);
	}

	// 小球运动中
	else
	{
		for (int i = 0; i < g_nGeneratedBallNum; i++)
		{
			if (g_pstBalls[i].alive)
			{
				solidcircle((int)g_pstBalls[i].x, (int)g_pstBalls[i].y, g_nBallRadius);
			}
		}
	}
}

// 绘制挡板
void Paint::DrawBoard()
{
	setfillcolor(WHITE);
	solidrectangle(
		g_nBoardCenterX - BOARD_HALF_WIDTH,
		BOARD_Y - BOARD_HALF_THICKNESS,
		g_nBoardCenterX + BOARD_HALF_WIDTH,
		BOARD_Y + BOARD_HALF_THICKNESS
	);
}

// 绘制状态栏
void Paint::DrawStateBar()
{
	// 还没开始游戏
	if (g_bInMenu)
	{
		return;
	}

	int x = getwidth() / 2;

	setfillcolor(WHITE);
	solidcircle(x - 20, 28, 10);		// 画个球

	// 显示关卡信息
	TCHAR lpszLevel[64] = {};
	wsprintf(lpszLevel, _T("Level %d / %d"), g_nCurrentLevel + 1, g_nLevelNum);
	settextstyle(28, 0, _T("System"));
	outtextxy(10, 22, lpszLevel);

	// 显示一下还剩几个球
	TCHAR lpszBuf[12] = {};
	wsprintf(lpszBuf, _T("%02d"), g_nRemainingBallNum);
	settextstyle(24, 0, _T("System"));
	outtextxy(x, 22, lpszBuf);

	// “重新开始” 按钮
	putimage(g_rctRestartBtn.left, g_rctRestartBtn.top, &g_imgRestart);
}

// 在屏幕中央输出大字
void Paint::DrawTitle(LPCTSTR text)
{
	setfillcolor(BLACK);
	solidrectangle(0, 20, getwidth(), 140);

	settextstyle(48, 0, _T("System"));
	int w = textwidth(text);
	int h = textheight(text);
	outtextxy((getwidth() - w) / 2, 20 + (120 - h) / 2, text);
}

// 在屏幕中央输出小字
void Paint::DrawSmallText(LPCTSTR text)
{
	setfillcolor(BLACK);
	solidrectangle(0, 150, getwidth(), 200);

	settextstyle(24, 0, _T("System"));
	int w = textwidth(text);
	int h = textheight(text);
	outtextxy((getwidth() - w) / 2, 150 + (50 - h) / 2, text);
}

// 胜利界面
void Paint::Win()
{
	DrawTitle(_T("Clear"));
	if (g_nCurrentLevel == g_nLevelNum - 1)
		DrawSmallText(_T("Back to menu"));
	else
		DrawSmallText(_T("Next level"));
}

// 失败界面
void Paint::Fail()
{
	DrawSmallText(_T("Restart"));
}

// 绘制菜单
void Paint::DrawMenu()
{
	// 已开始游戏
	if (!g_bInMenu)
	{
		if (g_enuGameState == GameWin)
		{
			Win();
		}
		else if (g_enuGameState == GameFail)
		{
			Fail();
		}
		else
		{
			return;
		}

		FlushBatchDraw();

		// 等按下按键
		while (true)
		{
			ExMessage msg = getmessage(EX_MOUSE);
			if (msg.message == WM_LBUTTONUP
				&& (msg.y > 150 + STATE_BAR_HEIGHT && msg.y < 200 + STATE_BAR_HEIGHT))
			{
				break;
			}
		}

		FreeLevel();

		bool show_menu = false;	// 是否继续显示主菜单

		// 若是在胜利界面，则需要进入下一个关卡
		if (g_enuGameState == GameWin)
		{
			// 关卡打完了，回到主界面
			if (g_nCurrentLevel == g_nLevelNum - 1)
			{
				g_nCurrentLevel = 0;
				g_bInMenu = true;
				show_menu = true;
			}
			else
			{
				g_nCurrentLevel++;
			}
		}

		InitLevel();

		g_enuGameState = Gaming;

		if (!show_menu)
			return;
	}

	DrawTitle(_T("Bricks Beater"));

	// 致敬原游戏
	DrawSmallText(_T("Salute Many Bricks Breaker"));
	//DrawSmallText(_T("Salute                              "));
	//DrawSmallText(_T("Many Bricks Breaker"), true);

	settextstyle(20, 0, _T("System"));
	LPCTSTR lpszTip = _T("Click to start");
	int w = textwidth(lpszTip);
	outtextxy((getwidth() - w) / 2, BOARD_Y + BOARD_HALF_THICKNESS + 10, lpszTip);
}

// 初始化一个关卡
void Paint::InitLevel()
{
	g_pstMap = new Brick[g_nMapW * g_nMapH];
	for (int x = 0; x < g_nMapW; x++)
	{
		for (int y = 0; y < g_nMapH; y++)
		{
			Brick* pBrick = &g_pstMap[x + y * g_nMapW];

			int sx = x * SIDE_LEN;
			int sy = y * SIDE_LEN;

			// 设置碰撞箱
			pBrick->rct = { sx, sy, sx + SIDE_LEN,sy + SIDE_LEN };

			switch (g_pppszOriginalLevelMap[g_nCurrentLevel][y][x])
			{
				// 墙
			case '#':
				pBrick->type = WALL;
				pBrick->color = COLOR_WALL;
				break;

				// 各种砖块
			case 'R':
				pBrick->type = BRICK;
				pBrick->color = COLOR_BRICK_R;
				break;
			case 'Y':
				pBrick->type = BRICK;
				pBrick->color = COLOR_BRICK_Y;
				break;
			case 'G':
				pBrick->type = BRICK;
				pBrick->color = COLOR_BRICK_G;
				break;
			case 'C':
				pBrick->type = BRICK;
				pBrick->color = COLOR_BRICK_C;
				break;
			case 'B':
				pBrick->type = BRICK;
				pBrick->color = COLOR_BRICK_B;
				break;
			case 'W':
				pBrick->type = BRICK;
				pBrick->color = COLOR_BRICK_W;
				break;

				// 其它的当空气
			default:
				pBrick->type = EMPTY;
				pBrick->color = BLACK;	// 颜色没啥用
				break;
			}
		}
	}

	g_nBoardCenterX = getwidth() / 2;

	g_nGeneratedBallNum = 0;
	g_bBallWait = true;

	g_nRemainingBallNum = BALL_LIFE_NUM;

	int pos_x = 15;
	int pos_y = 15;
	g_rctRestartBtn = {
		getwidth() - g_imgRestart.getwidth() - pos_x,
		pos_y,
		getwidth() - pos_x,
		pos_y + g_imgRestart.getheight()
	};

	g_nGeneratedPropNum = 0;
}