# Bricks Beater
Bricks Beater 是一个基于 Windows 的打砖块小游戏，使用 EasyX 图形库与 WinMM MCI 接口播放音效。

## 目录结构
- `include/` 头文件
- `src/` 源文件
- `res/` 游戏资源（关卡、图片、音乐）
- `screenshot/` 游戏截图
- `BricksBeater.sln` 解决方案文件
- `BricksBeater.vcxproj` 工程文件
- `BricksBeater.ini` 配置文件
- 其他文件

## 运行环境
- Windows 10/11
- Visual Studio 2022（v143 工具链）
- 安装 EasyX 图形库（提供 `graphics.h` 与对应库）
  - 参考 EasyX 官方安装步骤，将头文件与库目录加入 VS 的包含目录与库目录

## 构建与运行
- 双击打开 `BricksBeater.sln`
- 确保 `include/` 已加入工程的包含目录（工程已配置 `$(ProjectDir)include`）
- 选择 `x64` + `Debug` 或 `Release`，点击“生成”并运行

## 游戏说明
- 控制：鼠标移动控制挡板，点击发球；胜利或失败后点击提示区域继续
- 目标：清除所有可破坏的砖块
- 道具：
  - `3X`：当前每个存活球额外生成 3 个球
  - `SHOOT_3`：向上发射 3 个球
  - `HEART`：增加 1 次生命

## 关卡文件格式
- 路径：`res/level/level_X.dat`
- 配置文件控制关卡尺寸与数量：`BricksBeater.ini`
- 文件字符含义：
  - `#` 灰墙
  - `R/Y/G/C/B/W` 不同颜色的砖块
  - 其他字符为空

## 配置项（BricksBeater.ini）
- `game.width` 地图宽（格数）
- `game.height` 地图高（格数）
- `game.level_num` 关卡总数
- `game.begin_level` 初始关卡（从 1 开始）
- `debug.enable_debug_mode` 是否开启调试模式
- `debug.only_vertex_collision_sound` 仅顶点碰撞播放音效
- `debug.enable_big_scene` 大场景模式（更大砖块与球半径）
- `debug.big_scene_brick_inner_side_len` 大场景砖块内边长
- `debug.big_scene_ball_radius` 大场景球半径
- `debug.easy_mode` 简易模式（提高掉落概率）

## 截图
- 见 `screenshot/` 目录

## 许可证
- 见 `LICENSE`
