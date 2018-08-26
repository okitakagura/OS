//贪吃蛇游戏 
//snake.c
//编译命令：gcc snake.c -lcurses -o snake
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<curses.h>
#include<signal.h>
#include<unistd.h>
#include<sys/time.h>
#include<time.h>


//纵横轴方向
typedef struct direct
{
	int cx;
	int cy;
}direct;

//蛇身结点
typedef struct node
{
	int cx;
	int cy;
	struct node *last;
	struct node *next;
}node;

//函数声明 
void initGame();                   //初始化游戏
void pauseGame();                  //暂停游戏 
int setTicker(int i);              //设置计时器
void show();                       //画面设置
void showInfo();                   //显示信息
void showSnake();                  //显示蛇
void getInput();                   //获取方向键输入信息
void overGame(int i);              //游戏结束
void creatLink();
void insertNode(int x, int y);
void deleteNode();
void deleteLink();

//全局变量 
int ch;                            //输入的命令
int hour, minute, second;          //时分秒
int length;                        //蛇的长度
int totTime, level;                //计时，等级
chtype chsnake;                    //蛇的符号
direct dir;                        //蛇前进方向
direct food;                       //食物的位置
node *head, *tail;                 //链表的头尾结点

int main()
{
	initscr();                     //curses程序开始
	cbreak();                      //打开终端的CBREAK模式
	noecho();                      //关闭回显

	move(6, 28);
	attron(A_BOLD);
	addstr("Welcome to the GreedySnake!");
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
	addstr("'KEY_LEFT' :Control snake to go left.");
	move(13, 23);
	addstr("'KEY_RIGHT':Control snake to go right.");
	move(14, 23);
	addstr("'KEY_UP'   :Control snake to go up.");
	move(15, 23);
	addstr("'KEY_DOWN' :Control snake to go down.");
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
		addstr("Game will start! Are you read?");
		refresh();
		sleep(2);
		clear();
		attroff(A_BLINK);          //关闭A_BLINK属性
		initGame();
		signal(SIGALRM, show);
		getInput();
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

	endwin();                      //curses程序结束
	return 0;
}

//初始化游戏
void initGame()
{
	curs_set(0);                   //光标置为不可见
	keypad(stdscr, true);          //为标准屏幕激活功能键
	if (start_color() == OK)       //设置颜色
	{
		attron(A_BOLD);
		init_pair(1, COLOR_YELLOW, COLOR_BLACK);
		attron(COLOR_PAIR(1));
	}
	srand(time(0));                //设置随机数种子
	hour = minute = second = 0;    //初始化数据
	totTime = 0;
	length = 1;
	dir.cx = 1;
	dir.cy = 0;
	ch = 'A';
	food.cx = rand() % COLS;
	food.cy = rand() % (LINES - 2) + 2;    //前两行显示信息
	creatLink();
	setTicker(20);
}

//设置计时器
int setTicker(int i)
{
	struct itimerval itv;
	long  n_sec, n_usecs;
	n_sec = i / 1000;
	n_usecs = (i % 1000) * 1000L;
	//间隔时间
	itv.it_interval.tv_sec = n_sec;
	itv.it_interval.tv_usec = n_usecs;
	i = 1;
	n_sec = i / 1000;
	n_usecs = (i % 1000) * 1000L;
	//初始启动时间
	itv.it_value.tv_sec = n_sec;
	itv.it_value.tv_usec = n_usecs;
	return setitimer(ITIMER_REAL, &itv, NULL);
}

//暂停游戏
void pauseGame()
{
	struct  itimerval stop = { 0 }, older; //创建一个暂停时钟
	setitimer(ITIMER_REAL, &stop, &older);
	show();
	int key;
	while(1)
	{
		key = getch();
		if (key == ' ')
			break;
	}
	setitimer(ITIMER_REAL, &older, NULL);
} 

//设置画面
void show()
{
	signal(SIGALRM, show);         //设置中断信号
	showInfo();
	showSnake();
	refresh();                     //刷新真实屏幕
}

//显示上方两行的游戏信息
void showInfo()
{
	totTime++;
	if (totTime >= 1000000)
		totTime = 0;
	if (1 != totTime % 50)
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
	move(0, COLS / 2 + 2);
	addch('*');
	move(0, COLS / 2 + 2);
	chsnake = inch();
	move(1, 0);
	int i;
	for (i = 0; i < COLS; i++)
		addstr("-");
	move(0, COLS / 2 - 5);
	printw("length: %d", length);         //显示蛇的长度
	move(0, COLS - 10);
	level = length / 3 + 1;
	printw("level: %d", level);           //显示等级

}

//显示蛇
void showSnake()
{
	if (1 != totTime % (20 - level))
		return;
	bool lengCh = false;
	move(food.cy, food.cx);
	addch('@');                            //显示食物
										   //当蛇碰到墙，则游戏结束
	if ((COLS - 1 == head->next->cx && 1 == dir.cx)
		|| (0 == head->next->cx && -1 == dir.cx)
		|| (LINES - 1 == head->next->cy && 1 == dir.cy)
		|| (2 == head->next->cy && -1 == dir.cy))
	{
		overGame(1);
		return;
	}
	//当蛇吃到自己身体，则游戏结束
	if (chsnake == mvinch(head->next->cy + dir.cy, head->next->cx + dir.cx))
	{
		overGame(2);
		return;
	}
	insertNode(head->next->cx + dir.cx, head->next->cy + dir.cy);
	//当蛇吃了食物，则蛇身变长
	if (head->next->cx == food.cx && head->next->cy == food.cy)
	{
		beep();
		lengCh = true;
		length++;
		//当成功吃完50个食物时，过关
		if (length >= 50)
		{
			overGame(3);
			return;
		}
		//重新设置下一个食物的位置
		food.cx = rand() % COLS;
		food.cy = rand() % (LINES - 2) + 2;
	}
	//蛇正常前进，删除尾部结点，增加头部结点
	if (!lengCh)
	{
		move(tail->last->cy, tail->last->cx);
		addch(' ');
		deleteNode();
	}
	move(head->next->cy, head->next->cx);
	addch('*');
}

//游戏结束
void overGame(int i)
{
	setTicker(0);                 //关闭计时器
	deleteLink();                 //释放整个链表
	clear();
	move(10, 25);
	//显示结束理由
	if (i == 1)
		addstr("Crash the wall. Game over!");
	else if (i == 2)
		addstr("Crash itself. Game over!");
	else if (i == 3)
		addstr("Pass the Game!");
}

//读取键盘输入的命令
void getInput()
{
	while (1)
	{
		ch = getch();
		if (ch == KEY_RIGHT)
		{
			dir.cx = 1;
			dir.cy = 0;
		}
		if (ch == KEY_LEFT)
		{
			dir.cx = -1;
			dir.cy = 0;
		}
		if (ch == KEY_UP)
		{
			dir.cx = 0;
			dir.cy = -1;
		}
		if (ch == KEY_DOWN)
		{
			dir.cx = 0;
			dir.cy = 1;
		}
		if (ch == ' ')
		{
			pauseGame();
		}
		setTicker(20);
	}
}

//创建蛇身，即一个双向链表
void creatLink()
{
	node * temp = (node *)malloc(sizeof(node));
	head = (node *)malloc(sizeof(node));
	tail = (node *)malloc(sizeof(node));
	temp->cx = 5;
	temp->cy = 10;
	head->last = tail->next = NULL;
	head->next = temp;
	temp->next = tail;
	tail->last = temp;
	temp->last = head;
}

//在链表头部插入一个结点
void insertNode(int x, int y)
{
	node * temp = (node *)malloc(sizeof(node));
	temp->cx = x;
	temp->cy = y;
	temp->next = head->next;
	head->next = temp;
	temp->last = head;
	temp->next->last = temp;
}

//在链表尾部删除一个结点
void deleteNode()
{
	node *temp = tail->last;
	node *btemp = temp->last;
	btemp->next = tail;
	tail->last = btemp;
	temp->next = temp->last = NULL;
	free(temp);
	temp = NULL;
}

//删除整个链表
void deleteLink()
{
	while (head->next != tail)
		deleteNode();
	head->next = tail->last = NULL;
	free(head);
	free(tail);
}

