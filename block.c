//俄罗斯方块 
//block.c
//编译命令：gcc block.c -o block 
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#define ROW 21     //游戏区域的行
#define COL 18     //游戏区域的列
#define NR   4     //方块个数
#define TYPE 7     //方块类型

//方块的数据
char blocks[TYPE][NR][NR][NR] = 
{
	{//第一种方块
		{//第一个方向
			{1,1,0,0},
			{1,1,0,0},
			{0,0,0,0},
			{0,0,0,0},
         },
		 {//第二个方向
			 { 1,1,0,0 },
			 { 1,1,0,0 },
			 { 0,0,0,0 },
			 { 0,0,0,0 },
		 },
		 {//第三个方向
			 { 1,1,0,0 },
			 { 1,1,0,0 },
			 { 0,0,0,0 },
			 { 0,0,0,0 },
		 },
		 {//第四个方向
			 { 1,1,0,0 },
			 { 1,1,0,0 },
			 { 0,0,0,0 },
			 { 0,0,0,0 },
		 },
	},
	{//第二种方块
		{//第一个方向
			{ 2,2,2,2 },
			{ 0,0,0,0 },
			{ 0,0,0,0 },
			{ 0,0,0,0 },
		},
		{//第二个方向
			{ 0,2,0,0 },
			{ 0,2,0,0 },
			{ 0,2,0,0 },
			{ 0,2,0,0 },
		},
		{//第三个方向
			{ 2,2,2,2 },
			{ 0,0,0,0 },
			{ 0,0,0,0 },
			{ 0,0,0,0 },
		},
		{//第四个方向
			{ 0,2,0,0 },
			{ 0,2,0,0 },
			{ 0,2,0,0 },
			{ 0,2,0,0 },
		},
	},
	{//第三种方块
		{//第一个方向
			{ 3,0,0,0 },
			{ 3,3,3,0 },
			{ 0,0,0,0 },
			{ 0,0,0,0 },
		},
		{//第二个方向
			{ 0,3,3,0 },
			{ 0,3,0,0 },
			{ 0,3,0,0 },
			{ 0,0,0,0 },
		},
		{//第三个方向
			{ 3,3,3,0 },
			{ 0,0,3,0 },
			{ 0,0,0,0 },
			{ 0,0,0,0 },
		},
		{//第四个方向
			{ 0,0,3,0 },
			{ 0,0,3,0 },
			{ 0,3,3,0 },
			{ 0,0,0,0 },
		},
	},
	{//第四种方块
		{//第一个方向
			{ 0,0,4,0 },
			{ 4,4,4,0 },
			{ 0,0,0,0 },
			{ 0,0,0,0 },
		},
		{//第二个方向
			{ 0,4,0,0 },
			{ 0,4,0,0 },
			{ 0,4,4,0 },
			{ 0,0,0,0 },
		},
		{//第三个方向
			{ 0,4,4,4 },
			{ 0,4,0,0 },
			{ 0,0,0,0 },
			{ 0,0,0,0 },
		},
		{//第四个方向
			{ 0,4,4,0 },
			{ 0,0,4,0 },
			{ 0,0,4,0 },
			{ 0,0,0,0 },
		},
	},
	{//第五种方块
		{//第一个方向
			{ 0,5,0,0 },
			{ 5,5,5,0 },
			{ 0,0,0,0 },
			{ 0,0,0,0 },
		},
		{//第二个方向
			{ 0,5,0,0 },
			{ 0,5,5,0 },
			{ 0,5,0,0 },
			{ 0,0,0,0 },
		},
		{//第三个方向
			{ 5,5,5,0 },
			{ 0,5,0,0 },
			{ 0,0,0,0 },
			{ 0,0,0,0 },
		},
		{//第四个方向
			{ 0,5,0,0 },
			{ 5,5,0,0 },
			{ 0,5,0,0 },
			{ 0,0,0,0 },
		},
	},
	{//第六种方块
		{//第一个方向
			{ 6,6,0,0 },
			{ 0,6,6,0 },
			{ 0,0,0,0 },
			{ 0,0,0,0 },
		},
		{//第二个方向
			{ 0,0,6,0 },
			{ 0,6,6,0 },
			{ 0,6,0,0 },
			{ 0,0,0,0 },
		},
		{//第三个方向
			{ 6,6,0,0 },
			{ 0,6,6,0 },
			{ 0,0,0,0 },
			{ 0,0,0,0 },
		},
		{//第四个方向
			{ 0,0,6,0 },
			{ 0,6,6,0 },
			{ 0,6,0,0 },
			{ 0,0,0,0 },
		},
	},
	{//第七种方块
		{//第一个方向
			{ 0,7,7,0 },
			{ 7,7,0,0 },
			{ 0,0,0,0 },
			{ 0,0,0,0 },
		},
		{//第二个方向
			{ 0,7,0,0 },
			{ 0,7,7,0 },
			{ 0,0,7,0 },
			{ 0,0,0,0 },
		},
		{//第三个方向
			{ 0,0,7,7 },
			{ 0,7,7,0 },
			{ 0,0,0,0 },
			{ 0,0,0,0 },
		},
		{//第四个方向
			{ 0,7,0,0 },
			{ 0,7,7,0 },
			{ 0,0,7,0 },
			{ 0,0,0,0 },
		},
	},
};

//键盘输入命令
enum key {
	DOWN,     //下
	FALL,     //直下到底
	LEFT,     //左
	RIGHT,    //右
	CHANGE,   //变化
	PAUSE,    //暂停
	EXIT,     //退出
	UNKNOW,   //未知
};

//函数声明
void initGame();                        //初始化游戏
void drawBlock(char bl[NR][NR]);        //画方块
void cleanBlock(char bl[NR][NR]);       //清除方块
void turnBlock(char bl[NR][NR]);        //旋转方块
void overGame();                        //结束游戏
void pauseGame();                       //暂停游戏
void show();                            //显示游戏画面
void gameRun(int signo);                //游戏运行
void checkLine();                       //检查是否满一行
void checkOver(char bl[NR][NR]);        //检查是否游戏结束
int  checkMove(char bl[NR][NR], int flag); //检查方块是否可移动
int  getInput(void);                       //获取输入

//全局变量
static char gameArea[ROW][COL] = { 0 };   //游戏区域
static int startX = 7, startY = 6;        //方块出现位置
static int type = 0;                      //当前方块类型
static int nextType = 0;                  //下个方块类型
static int direct = 0;                    //方块方向
char *gameState = "\033[33m Playing...\033[0m";//游戏状态
static unsigned int level = 0;            //等级
static unsigned int score = 0;            //分数
static unsigned int highScore = 0;        //最高分
static FILE *fp = NULL;                   //记录保存在文件中

int main()
{
	struct itimerval itiv = { {0,700000},{0,700000} }; //设置定时器
	setitimer(ITIMER_REAL, &itiv, NULL);
	initGame();
	signal(SIGALRM, gameRun);

	srand(time(0));                                    //设置随机数种子
	type = rand() % 7;                                 //初始化方块类型
	nextType = rand() % 7;

	fp = fopen("./record", "r+");                      //读取最高记录
	if (fp == NULL)
	{   //文件不存在时创建
		fp = fopen("./record", "w");
	}
	fscanf(fp, "%u", &highScore);

	int order;
	while (1)
	{
		order = getInput();                           //用户命令
		switch (order)
		{
		case RIGHT:checkMove(blocks[type][direct], RIGHT);
			break;
		case LEFT:checkMove(blocks[type][direct], LEFT);
			break;
		case FALL:checkMove(blocks[type][direct], FALL);
			break;
		case CHANGE:turnBlock(blocks[type][(direct + 1) % 4]);
			break;
		case PAUSE:pauseGame();
			break;
		case EXIT:overGame();
			break;
		case UNKNOW:continue;
		}

		drawBlock(blocks[type][direct]);             //画方块
		show();                                      //显示画面
		cleanBlock(blocks[type][direct]);            //清除方块
	}
	return 0;
}

//初始化游戏
void initGame()
{
	printf("\033[2J");            //清屏
	system("stty -icanon");       //关缓冲
	system("stty -echo");         //关回显
	fprintf(stdout, "\033[?25l"); //关闭鼠标显示
	//定义一些边界
	int i;
	for (i = 0; i < COL; i++) {
		gameArea[0][i] = 8;
		gameArea[5][i] = 8;
		gameArea[ROW - 1][i] = 8;
	}
	for (i = 0; i < ROW; i++)
	{
		gameArea[i][0] = 8;
		gameArea[i][COL - 1] = 8;
	}
	for (i = 1; i < 5; i++)
	{
		gameArea[i][6] = 8;
	}
}

//显示方块
void drawBlock(char block[NR][NR])
{
	int x, y;
	//当前方块
	for (y = 0; y < NR; y++)
	{
		for (x = 0; x < NR; x++)
		{
			if (block[y][x] != 0)
			{
				gameArea[startY + y][startX + x] = block[y][x];
			}
		}
	}

	//下一个方块
	for (x = 0; x < 2; x++)
	{
		for (y = 0; y < NR; y++)
		{
			if (blocks[nextType][0][x][y] != 0)
				gameArea[3 + x][2 + y] = blocks[nextType][0][x][y];
			else
				gameArea[3 + x][2 + y] = 0;
		}
	}
}

//清除方块
void cleanBlock(char block[NR][NR])
{
	int x, y;
	for (y = 0; y < NR; y++)
	{
		for (x = 0; x < NR; x++)
		{
			if (block[y][x] != 0)
			{
				gameArea[startY + y][startX + x] = 0;
			}
		}
	}
}

//旋转方块
void turnBlock(char block[NR][NR])
{
	int x, y;
	//检查是否越界
	for (y = 0; y < NR; y++)
	{
		for (x = 0; x < NR; x++)
		{
			if (block[y][x] != 0 && gameArea[startY + y][startX + x] != 0)
			{
				return;
			}
		}
	}
	direct = (direct + 1) % 4;    //改变方向
}

//显示游戏画面
void show()
{
	fprintf(stdout, "\033[1;1H");  //定位到第一行第一列
	fflush(stdout);
	int i, j;
	for (i = 0; i < ROW; i++)
	{
		for (j = 0; j < COL; j++)
		{
			if (gameArea[i][j] == 0)     //空白区域
			{
				fprintf(stdout, "  ");
			}
			else if (gameArea[i][j] == 8) //边界区域
			{
				fprintf(stdout, "\033[40m  \033[0m");
			}
			else                          //方块区域
			{
				fprintf(stdout, "\033[%dm[]\033[0m", gameArea[i][j] + 40);
			}
		}
		fputc('\n', stdout);
	}
	//游戏信息
	fprintf(stdout, "\033[2;3H\033[33m【NEXT】\033[0m\n");
	fprintf(stdout, "\033[2;15H Level:\033[36m%u\033[0m\n", level);
	fprintf(stdout, "\033[3;15H Score:\033[32m%u\033[0m\n", score);
	fprintf(stdout, "\033[4;15H HighScore:\033[35m%u\033[0m\n", highScore);
	fprintf(stdout, "\033[5;15H State:%s\n", gameState);
}

//方块自动下落
void gameRun(int signo)
{
	drawBlock(blocks[type][direct]);
	show();
	cleanBlock(blocks[type][direct]);
	//当方块已经到底时
	if (!checkMove(blocks[type][direct], DOWN))
	{
		checkOver(blocks[type][direct]);  //检查游戏是否结束
		drawBlock(blocks[type][direct]);  //保留已到底的方块
		show();                           //显示游戏结果
		checkLine();                      //检查是否可消除行
		//重新开始下一个方块
		struct itimerval itiv = { { 0,700000 },{ 0,700000 } }; //设置定时器
		setitimer(ITIMER_REAL, &itiv, NULL);
		startY = 6;
		startX = 7;
		type = nextType;
		nextType = rand() % 7;
		direct = 0;
	}
}

//检查是否可消去
void checkLine()
{
	int i, j, x, y;
	for (i = 3; i >= 0; i--)
	{
		for (j = 1; j < COL - 1; j++)
		{
			if (gameArea[startY + i][j] == 0) //方块是否满一行
				break;
			else if (gameArea[startY + i][j] == 8)//跳过边界区域
				break;
		}
		if (j == COL - 1) //方块满一行则需消除
		{
			for (j = 1; j < COL - 1; j++)
			{
				gameArea[startY + i][j] = 0;
			}

			score += 100; //分数累加
			if (score > highScore) //最高分
			{
				highScore = score;
				rewind(fp);
				fprintf(fp, "%u\n", highScore);
			}
			if (score % 300 == 0) //每300分升一级
			{
				level++;
			}

			for (x = 1; x < COL - 1; x++) //往下移动一行
			{
				for (y = startY + i; y >= 7; y--)
				{
					gameArea[y][x] = gameArea[y - 1][x];
				}
			}
			i++;
		}
	}
}

//检查游戏是否结束
void checkOver(char block[NR][NR])
{
	int i;
	for (i = 0; i < NR; i++)
	{   //方块碰到上方边界
		if (block[0][i] != 0 && gameArea[startY - 1][startX + i] == 8)
		{
			overGame();
		}
	}
}

//检查方块能否移动
int checkMove(char block[NR][NR], int flag)
{
	int x, y;//表示可移动的方向
	switch (flag)
	{
	case RIGHT:x = 1; y = 0; break;
	case LEFT:x = -1; y = 0; break;
	case DOWN:x = 0; y = 1; break;
	case FALL:x = 0; y = 1;
		struct itimerval itiv = { { 0,10000 },{ 0,10000 } }; //设置定时器
		setitimer(ITIMER_REAL, &itiv, NULL);
		break;
	}

	int i, j;
	for (i = 0; i < NR; i++)
	{
		for (j = 0; j < NR; j++)
		{   //会触到边界
			if (block[i][j] != 0 && gameArea[startY + y + i][startX + x + j] != 0)
			{
				return 0;
			}
		}
	}
	startY += y;
	startX += x;

	return 1;
}

//获取用户输入命令
int getInput()
{
	char key;
	key = fgetc(stdin);

	if (key == '\033' && fgetc(stdin) == '[')  //方向键
	{
		switch (fgetc(stdin))
		{
		case 'A':return CHANGE;
		case 'B':return FALL;
		case 'C':return RIGHT;
		case 'D':return LEFT;
		default:return UNKNOW;
		}
	}
	else if (key == ' ')   //空格键
	{
		return PAUSE;
	}
	else if (key == 'q')  //退出键
	{
		return EXIT;
	}
	else                 //其余键
	{
		return UNKNOW;
	}
}

//暂停游戏
void pauseGame()
{
	struct  itimerval stop = { 0 }, older; //创建一个暂停时钟
	setitimer(ITIMER_REAL, &stop, &older);

	gameState = "\033[32m Pause...\033[0m";
	startY--;   //防止按下暂停键后方块下滑一格
	drawBlock(blocks[type][direct]);
	show();
	cleanBlock(blocks[type][direct]);

	int key;
	while (1)
	{
		key = fgetc(stdin);
		if (key == ' ')
			break;
		else if (key == 'q')
			overGame();
	}
	//恢复游戏
	setitimer(ITIMER_REAL, &older, NULL);

	gameState = "\033[33m Playing...\033[0m";
}

//结束游戏
void overGame()
{
	gameState = "\033[32m Game Over...\033[0m";
	drawBlock(blocks[type][direct]);
	show();

	//恢复终端设置
	system("stty icanon");          //恢复缓冲
	system("stty echo");            //恢复回显
	fprintf(stdout, "\033[?25h");   //恢复鼠标显示

    //尾部处理
	fprintf(stdout, "\033[200;1H"); //定位光标到最后一行
	fclose(fp);                     //关闭文件
	exit(0);                        //退出程序
}
