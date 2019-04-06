#include<stdio.h>
#include<stdlib.h>
#include<dos.h>
#include<windows.h>
#include<graphics.h>  //ͼ�κ�����

//���尴����
#define VK_LEFT 0x4b00
#define VK_RIGHT 0X4d00
#define VK_DOWN 0x5000
#define VK_UP 0x4800
#define VK_ESC 0x011b
#define TIMER 0x1c  //�����жϺ�

#define MAX_BOX 19  //�ܹ�19����̬�ķ���
#define BSIZE 20  //����ı�����20������
#define Sys_x 160 //��ʾ�����������Ͻ�x����
#define Sys_y 25 //��ʾ�����������Ͻ�y����
#define Horizontal_boxs 10  //ˮƽ�����Է���Ϊ��λ�ĳ���
#define Vertical_boxs 15  //��ֱ�ķ����Է���Ϊ��λ�ĳ��ȣ���15������
#define Begin_boxs_x  2 //������һ������ʱ���ֵ���ʼλ��

#define FgColor 3  //������ɫ 
#define BgColor 0 //ǰ����ɫ
#define LeftWin_x Sys_x+Horizontal_boxs*BSIZE+46

#define false 0
#define true 1
//�ƶ��ķ���
#define MoveLeft 1
#define MoveRight 2
#define MoveDown 3
#define MoveRoll 4
//�Ժ������ÿ��������Կ������ص���BSIZE*BSIZE��������

int current_box_numb;  //���浱ǰ������
//x,y�Ǳ��淽��ĵ�ǰ�����
int Curbox_x = Sys_x + Begin_boxs_x*BSIZE, Curbox_y = Sys_y;
int flag_newbox = false;  //�Ƿ�����·ſ�ı��
int speed = 0; //�����ٶ�
int score = 0;  //�ܷ� 
int speed_step = 30;  //ÿ�ȼ�����Ҫ����
//ָ��ԭ��ʱ���жϴ��������ڵ��жϴ�����ָ��
void (*oldtimer)(void);

struct BOARD  //��Ϸ�װ�ṹ����ʾÿ���������е�����
{
	int var;  //��ǰ״̬ ֻ��0��1,1��ʾ�˵��ѱ�ռ��
	int color; //��ɫ����Ϸ�װ��ÿ�������ӵ�в�ͬ����ɫ����ǿ������
}Table_board[Vertical_boxs][Horizontal_boxs];

//����ṹ
struct SHAPE{
	char box[2]; //һ���ֽڵ���8λ��ÿ4λ��ʾһ�������һ��
	int color;  //һ���������ɫ
	int next;  //�¸�����ı��
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

