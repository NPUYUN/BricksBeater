#include "main.h"

Paint paint;
Game game;
Sound sound;

int main()
{
	paint.InitResource();

	initgraph(
		paint.g_nMapW * (paint.g_nInnerSideLen + 2 * paint.g_nInnerInterval),	// 地图宽
		STATE_BAR_HEIGHT + paint.g_nMapH * (paint.g_nInnerSideLen + 2 * paint.g_nInnerInterval) + BOARD_Y_DISTACE + BOARD_HALF_THICKNESS + WINDOW_BOTTOM_MARGIN /* 留白 */
	);

	paint.InitLevel();
	srand((UINT)time(nullptr));

	setbkcolor(RGB(19, 19, 92));
	setbkmode(TRANSPARENT);
	BeginBatchDraw();

	while (true)
	{

		for (int i = 0; i < PROCESS_REPEAT; i++) {
			game.GameProcess(sound, paint); // 传递引用
		}

		cleardevice();

		paint.DrawStateBar();

		setorigin(0, STATE_BAR_HEIGHT);
		{
			paint.DrawProp();
			paint.DrawMap();
			paint.DrawBoard();
			paint.DrawBall();
			paint.DrawMenu();
		}
		setorigin(0, 0);

		FlushBatchDraw();

		game.UserInputProcess(paint , paint.g_rctRestartBtn);

		game.DelayFPS();
	}

	EndBatchDraw();

	paint.FreeResource();

	closegraph();
	return 0;
}

