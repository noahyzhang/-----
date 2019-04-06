#include<stdio.h>
#include<stdlib.h>
#include<dos.h>
#include<windows.h>
#include<graphics.h>  //图形函数库

//定义按键码
#define VK_LEFT 0x4b00
#define VK_RIGHT 0X4d00
#define VK_DOWN 0x5000
#define VK_UP 0x4800
#define VK_ESC 0x011b
#define TIMER 0x1c  //设置中断号

#define MAX_BOX 19  //总共19中形态的方块
#define BSIZE 20  //方块的边上是20个像素
#define Sys_x 160 //显示方块界面的左上角x坐标
#define Sys_y 25 //显示方块界面的左上角y坐标
#define Horizontal_boxs 10  //水平方向以方块为单位的长度
#define Vertical_boxs 15  //垂直的方向以方块为单位的长度，长15个方块
#define Begin_boxs_x  2 //产生第一个方块时出现的起始位置

#define FgColor 3  //背景颜色 
#define BgColor 0 //前景颜色
#define LeftWin_x Sys_x+Horizontal_boxs*BSIZE+46

#define false 0
#define true 1
//移动的方向
#define MoveLeft 1
#define MoveRight 2
#define MoveDown 3
#define MoveRoll 4
//以后坐标的每个方块可以看做像素点是BSIZE*BSIZE的正方形

int current_box_numb;  //保存当前方块编号
//x,y是保存方块的当前坐标的
int Curbox_x = Sys_x + Begin_boxs_x*BSIZE, Curbox_y = Sys_y;
int flag_newbox = false;  //是否产生新放块的标记
int speed = 0; //下落速度
int score = 0;  //总分 
int speed_step = 30;  //每等级所需要分数
//指向原来时钟中断处理过程入口的中断处理函数指针
void (*oldtimer)(void);

struct BOARD  //游戏底板结构，表示每个点所具有的属性
{
	int var;  //当前状态 只有0和1,1表示此点已被占用
	int color; //颜色，游戏底板的每个点可以拥有不同的颜色，增强美观性
}Table_board[Vertical_boxs][Horizontal_boxs];

//方块结构
struct SHAPE{
	char box[2]; //一个字节等于8位，每4位表示一个方块的一行
	int color;  //一个方块的颜色
	int next;  //下个方块的编号
};

struct SHAPE shapes[MAX_BOX] =
{
	{ 0x88, 0xc0, CYAN, 1 },
	{ 0xe8, 0x0, CYAN, 2 },
	{ 0xc4, 0x40, CYAN, 3 },
	{ 0x2e, 0x0, CYAN, 0 },

	{ 0x44, 0xc0, MAGENTA, 5 },
	{ 0x8e, 0x0, MAGENTA, 6 },
	{ 0xc8, 0x80, MAGENTA, 7 },
	{ 0xe2, 0x0, MAGENTA, 4 },

	{ 0x8c, 0x40, YELLOW, 9 },
	{ 0x6c, 0x0, YELLOW, 8 },

	{ 0x4c, 0x80, BROWN, 11 },
	{ 0xc6, 0x0, BROWN, 10 },

	{ 0x4e, 0x0, WHITE, 13 },
	{ 0x8c, 0x80, WHITE, 14 },
	{ 0xe4, 0x0, WHITE, 15 },
	{ 0x4c, 0x40, WHITE, 12 },

	{ 0x88, 0x88, RED, 17 },
	{ 0xf0, 0x0, RED, 16 },

	{ 0xcc, 0x0, BLUE, 18 },
};

