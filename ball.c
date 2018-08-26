//弹球游戏 
//ball.c
//编译命令：gcc ball.c -lcurses -o ball 
#include <curses.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>

#define RIGHT COLS-1     //屏幕最右
#define BOTTOM LINES-1   //屏幕最底
#define LEFT 0           //屏幕最左
#define TOP 1            //屏幕最顶
#define BOARD_LENGTH 10  //挡板长度

//函数声明
int set_ticker(int i);
void initGame();   //初始化游戏
void pauseGame();  //暂停游戏 
void showTime();   //显示时间 
void moveBall();   //球的运动
void control();    //用户输入的控制命令

//全局变量
char BALL = 'O';   //球的形状
char BLANK = ' ';  //空格

int left_board;    //挡板左座标
int right_board;   //挡板右坐标
int is_lose = 0;   //标志球是否掉落
int ntime = 100;   //计时器时间

int hdir;   //控制球水平运动的变量
int vdir;   //控制球垂直运动的变量
int pos_X;  //球的横坐标
int pos_Y;  //球的纵坐标

int totTime, hour, minute, second; //时分秒

int main()
{
	initscr(); //curses程序开始
	crmode();  //终端模式
	noecho();  //关闭回显

	move(6, 28);
	attron(A_BOLD);
	addstr("Welcome to the BallGame!");
	move(8, 20);
	attroff(A_BOLD);
	addstr("Help:");
	move(9, 23);
	addstr("'N':Start a new game.");
	move(10, 23);
	addstr("'Q':Quit game.");
	move(11, 23);
	addstr("'SPACE':Pause game.");
	move(12, 23);
	addstr("'KEY_LEFT' :Control baffle left  shift.");
	move(13, 23);
	addstr("'KEY_RIGHT':Control baffle right shift.");
	move(14, 23);
	addstr("'KEY_UP'   :Control of the ball speed.");
	move(15, 23);
	addstr("'KEY_DOWN' :Control of the ball reducer.");
	int flag = 1;
	char choice;
	move(17, 24);
	addstr("Please choose your choice!(n/q):");
	refresh();
	choice = getch();
	while (flag)
	{
		if (choice == 'q' || choice == 'Q' || choice == 'n' || choice == 'N')
		{
			flag = 0;
		}
		else
			choice = getch();
	}
	if (choice == 'n' || choice == 'N')  //开始游戏
	{
		clear();
		move(10, 25);
		addstr("BallGame will start! Are you read?");
		refresh();
		sleep(2);
		control();
	}
	else if (choice == 'q' || choice == 'Q') //退出游戏
	{
		clear();
		move(10, 25);
		addstr("You quit the game successfully!");
		refresh();
		sleep(2);
		endwin();
	}
	endwin();
	return 0;
}

//设置定时器
int set_ticker(int i)
{
	struct itimerval itv;
	long n_sec, n_usecs;
	n_sec = i / 1000;
	n_usecs = (i % 1000) * 1000L;
	//间隔时间
	itv.it_interval.tv_sec = n_sec;
	itv.it_interval.tv_usec = n_usecs;
	//初始启动时间
	itv.it_value.tv_sec = n_sec;
	itv.it_value.tv_usec = n_usecs;
	return setitimer(ITIMER_REAL, &itv, NULL);
}

//显示时间信息
void showTime()
{
	totTime++;
	if (totTime >= 1000000)
		totTime = 0;
	if (1 != totTime % (1000 / ntime))
		return;             //不到1秒
	move(0, 3);
	//显示时间
	printw("time: %d:%d:%d %c", hour, minute, second);
	second++;
	if (second > 60)
	{
		second = 0;
		minute++;
	}
	if (minute > 60)
	{
		minute = 0;
		hour++;
	}
	move(1, 0);
	int i;
	for (i = 0; i < COLS; i++)
		addstr("-");
}
//初始化游戏
void initGame()
{
	totTime = 0;
	hour = minute = second = 0;
	int i, j;
	clear();
	if (start_color() == OK) //设置颜色
	{
		attron(A_BOLD);
		init_pair(1, COLOR_CYAN, COLOR_BLACK);
		attron(COLOR_PAIR(1));
	}
	pos_X = 20;             //球初始的横坐标
	pos_Y = BOTTOM - 1;     //球初始的纵坐标
	hdir = 1;               //球初始的运动方向，往右上方
	vdir = -1;

	//初始化挡板
	left_board = 15;
	right_board = left_board + BOARD_LENGTH;
	for (i = left_board; i <= right_board; i++)
	{
		move(BOTTOM, i);
		addch('-');
	}

	signal(SIGALRM, moveBall);
	set_ticker(ntime);           //设置定时
	keypad(stdscr, TRUE);      //打开键盘属性
	curs_set(0);               //光标置为不可见
	attroff(A_BLINK);          //关闭A_BLINK属性

	is_lose = 0;
	move(pos_Y, pos_X);
	addch(BALL);
	move(LINES - 1, COLS - 1);
	refresh();
	usleep(100000);
	move(LINES - 1, COLS - 1);
	refresh();
}

//暂停游戏 
void pauseGame()
{
	struct  itimerval stop = { 0 }, older; //创建一个暂停时钟
	setitimer(ITIMER_REAL, &stop, &older);
	moveBall();
	int key;
	while(1)
	{
		key = getch();
		if (key == ' ')
			break;
	}
	setitimer(ITIMER_REAL, &older, NULL);
}

//球的运动
void moveBall()
{
	showTime();
	if (is_lose)     //球已掉落
		return;
	signal(SIGALRM, moveBall);
	move(pos_Y, pos_X);
	addch(BLANK);
	pos_X += hdir;
	pos_Y += vdir;
	//控制球的反弹运动
	if (pos_X >= RIGHT)
	{
		hdir = -1;
		beep();
	}
	if (pos_X <= LEFT)
	{
		hdir = 1;
		beep();
	}
	if (pos_Y <= TOP + 1)
	{
		vdir = 1;
		beep();
	}
	if (pos_Y >= BOTTOM - 1)
	{
		if (pos_X >= left_board && pos_X <= right_board) //球落在挡板上
			vdir = -1; //向上反弹
		else
		{
			is_lose = 1;
			move(pos_Y, pos_X);
			addch(BALL);
			move(LINES - 1, COLS - 1);
			refresh();
			usleep(100000);
			move(pos_Y, pos_X);
			addch(BLANK);
			pos_X += hdir;
			pos_Y += vdir;
			move(pos_Y, pos_X);
			addch(BALL);
			move(LINES - 1, COLS - 1);
			refresh();
		}
	}
	move(pos_Y, pos_X);
	addch(BALL);
	move(LINES - 1, COLS - 1);
	refresh();
}

//获取用户输入的控制命令
void control()
{
	initGame();
	int cmd;
	while (1)
	{
		if (!is_lose)
		{
			cmd = getch();
			if (cmd == 'q' || cmd == 'Q' || cmd == 27)   //退出游戏
				break;
			if (cmd == ' ')        //暂停游戏
			    pauseGame(); 
			if (cmd == KEY_LEFT)   //挡板左移
			{
				if (left_board > 0)//判断边界
				{
					move(BOTTOM, right_board);
					addch(' ');
					right_board--;
					left_board--;
					move(BOTTOM, left_board);
					addch('-');
					move(BOTTOM, RIGHT);
					refresh();
				}
			}
			else if (cmd == KEY_RIGHT)//挡板右移
			{
				if (right_board < RIGHT)//判断边界
				{
					move(BOTTOM, left_board);
					addch(' ');
					right_board++;
					left_board++;
					move(BOTTOM, right_board);
					addch('-');
					move(BOTTOM, RIGHT);
					refresh();
				}
			}
			else if (cmd == KEY_UP)  //球加速
			{
				ntime /= 2;
				set_ticker(ntime);
			}
			else if (cmd == KEY_DOWN)//球减速
			{
				ntime *= 2;
				set_ticker(ntime);
			}
		}

		else   //球掉落后的操作
		{
			int flag = 1;
			char choice;
			move(8, 15);
			addstr("Game Over!try again?(y/n):");
			curs_set(1);
			refresh();
			choice = getch();
			while (flag)
			{
				if (choice == 'y' || choice == 'Y' || choice == 'n' || choice == 'N')
				{
					flag = 0;
				}
				else
					choice = getch();
			}
			if (choice == 'y' || choice == 'Y') //开始新游戏
			{
				ntime = 100;
				initGame();
				continue;
			}
			else if (choice == 'n' || choice == 'N')//结束游戏
			{
				break;
			}
		}
	}
}

