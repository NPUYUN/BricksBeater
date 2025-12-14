#pragma once 
#include "graphics.h"
#include "ini.hpp"

// 圆周率
#define PI					3.1415926f

// 程序界面属性
#define STATE_BAR_HEIGHT		150		// 状态栏高度
#define WINDOW_BOTTOM_MARGIN	187		// 窗口底部边距（留白）

// 方块总边长
#define SIDE_LEN	(g_nInnerSideLen + 2 * g_nInnerInterval)

// 挡板属性
#define BOARD_HALF_WIDTH		40		// 挡板半宽
#define BOARD_HALF_THICKNESS	3		// 挡板半厚度
#define BOARD_Y_DISTACE			100		// 挡板比地图底部低多少
#define BOARD_BALL_DISTANCE		16		// 待发射小球离挡板多远
#define BOARD_Y					(g_nMapH * SIDE_LEN + BOARD_Y_DISTACE)	// 挡板 y 坐标

// 小球属性
#define BALL_MAX_NUM		1024	// 最大小球数量
#define BALL_LIFE_NUM		4		// 球有几条命
#define BALL_SPEED			2.4f	// 小球速度

// 算法设置
#define PROCESS_REPEAT		3		// 在一帧内计算几次游戏状态

// 方块颜色
#define COLOR_WALL			RGB(163, 163, 162)	// 墙：灰色
#define COLOR_BRICK_R		RGB(214, 63, 37)	// 红 R
#define COLOR_BRICK_Y		RGB(228, 174, 31)	// 黄 Y
#define COLOR_BRICK_G		RGB(15, 216, 15)	// 绿 G
#define COLOR_BRICK_C		RGB(39, 253, 251)	// 青 C
#define COLOR_BRICK_B		RGB(68, 112, 202)	// 蓝 B
#define COLOR_BRICK_W		RGB(234, 234, 234)	// 白 W

// 方块类型标记
#define EMPTY				0
#define WALL				1
#define BRICK				2

// 道具类型标记
#define PROP_3X				0	// 每个小球向外发射三个小球
#define PROP_SHOOT_3		1	// 发射三个小球
#define PROP_HEART			2	// 加一条命

#define PROP_MAX_NUM		1024	// 最大道具数量

// 道具下落速度
#define PROP_DROP_SPEED			0.2f

// 碰撞结果
#define HIT_NONE			0	// 未发生任何碰撞
#define HIT_NORMAL			1	// 发生一次普通碰撞（与矩形水平边或竖直边的碰撞）
#define HIT_VERTEX			2	// 与矩形顶点发生了碰撞

// 游戏状态
#define GAME_WIN			1		// 赢
#define GAME_LOSE			2		// 输

// 资源路径
#define SETTINGS_PATH		_T("./BricksBeater.ini")	// 配置文件
#define LEVEL_FOLDER_PATH	("./res/level")				// 关卡文件存储路径（文件夹）

#define MUSIC_PING_PATH		_T("./res/music/ping.mp3")	// 碰撞声 1
#define MUSIC_PONG_PATH		_T("./res/music/pong.mp3")	// 碰撞声 2
#define MUSIC_PROP_PATH		_T("./res/music/prop.mp3")	// 领道具的声音

#define IMAGE_RESTART		_T("./res/pic/restart.gif")
#define IMAGE_3X_PATH		_T("./res/pic/prop_3x.gif")
#define IMAGE_SHOOT_3_PATH	_T("./res/pic/prop_shoot_3.gif")
#define IMAGE_HEART_PATH	_T("./res/pic/prop_heart.gif")

class Paint {
	public:
		int g_nMapW;								// 地图宽（单位：方块）
		int g_nMapH;								// 地图高（单位：方块）
		int g_nLevelNum;							// 关卡数
		int g_nCurrentLevel;						// 当前是第几关（从 0 开始数）
		char*** g_pppszOriginalLevelMap;			// 各关卡原始地图
		int g_nBoardCenterX;	// 挡板中心 x 坐标

		// 调试模式配置
		bool g_bDebugMode;					// 是否处于调试模式（在配置文件中设置的）
		bool g_bOnlyVertexCollisionSound;	// 只有小球与矩形顶点发生碰撞时才发出声音
		bool g_bBigScene;					// 大场景模式

		// 大场景模式配置
		int g_nInnerSideLen_BigScene;			// 大场景模式下的方块内填充矩形边长
		int g_nBallRadius_BigScene;			// 大场景模式下的小球半径

		// 方块大小属性
		int g_nInnerSideLen;		// 方块内填充矩形边长
		int g_nInnerInterval;		// 方块外周留空边距

		// 小球半径
		int g_nBallRadius;

		// 道具出现概率（百分比）
		int g_nProbability_3X;
		int g_nProbability_Shoot_3;
		int g_nProbability_Heart;

		int g_nGeneratedBallNum;		// 已经生成出的小球数量（小球出界不减数）
		bool g_bBallWait;				// 是否正在等待发射小球
		int g_nRemainingBallNum;	// 还剩几个球
		int g_nGeneratedPropNum;		// 已经生成的道具数量

		// 图像资源
		IMAGE g_imgRestart;
		IMAGE g_img3X;
		IMAGE g_imgShoot3;
		IMAGE g_imgHeart;

		// “重新开始” 按钮区域
		RECT g_rctRestartBtn;

		bool g_bInMenu = true;	// 是否还在主菜单

		// 游戏状态
		enum GameState { Gaming, GameWin, GameFail };
		GameState g_enuGameState = Gaming;

		// 道具
		struct Prop
		{
			int type;		// 种类
			bool used;		// 是否已被使用
			float x;
			float y;
		}g_pstProps[PROP_MAX_NUM];

		// 砖块
		struct Brick
		{
			int type;		// 方块类型（砖块被打掉之后会被改为 EMPTY）
			COLORREF color;	// 方块颜色
			RECT rct;		// 砖块碰撞箱位置
		}*g_pstMap;

		// 小球
		struct Ball
		{
			bool alive;		// 该小球是否已经出界了
			float x;
			float y;
			float vx;
			float vy;
		}g_pstBalls[BALL_MAX_NUM];

		void InitResource();
		void DrawProp();
		void DrawMap();
		void DrawBall();
		void DrawBoard();
		void DrawStateBar();
		void DrawTitle(LPCTSTR text);
		void DrawSmallText(LPCTSTR text);
		void InitLevel();
		void FreeLevel();
		void FreeResource();
		void Win();
		void Fail();
		void DrawMenu();

		// 构造函数
		Paint();

		// 拷贝构造函数
		Paint(const Paint& other);

		// 拷贝赋值运算符
		Paint& operator=(const Paint& other);

		// 移动构造函数
		Paint(Paint&& other) noexcept;

		// 移动赋值运算符
		Paint& operator=(Paint&& other) noexcept;

		// 析构函数
		~Paint();

	private:
		void DeepCopy(const Paint& other);
		void FreeResourceInternal();
		void FreeLevelInternal();
};