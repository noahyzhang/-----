#include"hanshu.h"

void initialize(int x, int y, int m, int n)
{
	int i, j, oldx;
	oldx = x;
	for (j = 0; j < n; j++)
	{
		for (i = 0; i < m; i++)
		{
			Table_board[j][i].var = 0;
			Table_board[j][i].color = BgColor;
			line(x, y, x + BSIZE, y);
			line(x, y, x, y + BSIZE);
			line(x, y + BSIZE, x + BSIZE, y + BSIZE);
			line(x + BSIZE, y, x + BSIZE, y + BSIZE);
			x += BSIZE;
		}
		y += BSIZE;
		x = oldx;
	}
	Curbox_x = x;
	Curbox_y = y;
	flag_newbox = false;
	speed = 0;
	score = 0;
	ShowScore(score);
	ShowSpeed(speed);
}

void  newtimer(void)
{
	(*oldtimer)();
	TimerCounter++;
}

void SetTimer(void interrupt(*IntProc)(void))
{
	oldtimer = getvect(TIMER);
	disable();
	setvect(TIMER, IntProc);
	enable();
}

void KillTimer()
{
	disable();
	setvect(TIMER, oldtimer);
	enable;
}

void ShowScore(int score)
{
	int x, y;
	char score_str[5];
	setfillstyle(SOLID_FILL, BgColor);
	x = LeftWin_x;
	y = 100;
	bar(x - BSIZE, y, x + BSIZE * 3, y + BSIZE * 3);
	sprintf(score_str, "%3d", score);
	outtextxy(x, y, "SCORE");
	outtextxy(x, y + 10, score_str);
}

void ShowSpeed(int speed)
{
	int x, y;
	char speed_str[5];
	setfillstyle(SOLID_FILL, BgColor);
	x = LeftWin_x;
	y = 150;
	bar(x - BSIZE, y, x + BSIZE * 3, y + BSIZE * 3);
	sprintf(speed_str, "%3d", speed + 1);
	outtextxy(x, y, "Level");
	outtextxy(x, y + 10, speed_str);
	outtextxy(x, y + 50, "Nextbox");
}

void show_help(int xs, int ys)
{
	char stemp[50];
	setcolor(15);
	rectangle(xs, ys, xs + 239, ys + 100);
	sprintf(stemp, " -ROLL -Downwards");
	stemp[0] = 24;
	stemp[8] = 25;
	setcolor(14);
	outtextxy(xs + 40, ys + 30, stemp);
	sprintf(stemp, "-Turn Left    -Turn Right");
	stemp[0] = 27;
	stemp[13] = 26;
	outtextxy(xs + 40, ys + 45, stemp);
	outtextxy(xs + 40, ys + 60, "Esc-Exit");
	setcolor(FgColor);
}

int DelFullRow(int y)
{
	int n, top = 0;
	register m, totoal;
	for (n = y; n >= 0; n--)
	{
		totoal = 0;
		for (m = 0; m < Horizontal_boxs; m++)
		{
			if (!Table_board[n][m].var)
			{
				totoal++;
			}
			if (Table_board[n][m].var != Table_board[n - 1][m].var)
			{
				Table_board[n][m].var = Table_board[n-1][m].var;
				Table_board[n][m].color = Table_board[n - 1][m].color;
			}
		}
		if (totoal == Horizontal_boxs)
		{
			top = n; break;
		}
	}
	return(top);
}

void setFullRow(int t_boardy)
{
	int n, full_numb = 0, top = 0;
	register m;
	for (n = t_boardy + 3; n >= t_boardy; n--)
	{
		if (n<0 || n>Vertical_boxs)
		{
			continue;
		}
		for (m = 0; m < Horizontal_boxs; m++)
		{
			if (!Table_board[n + full_numb][m].var)
				break;
		}
		if (m == Horizontal_boxs)
		{
			if (n == t_boardy + 3)
			{
				top = DelFullRow(n + full_numb);
			}
			else
			{
				DelFullRow(n + full_numb);
			}
			full_numb++;
		}
	}
	if (full_numb)
	{
		int oldx, x = Sys_x, y = BSIZE*top + Sys_y;
		oldx = x;
		score = score + full_numb * 10;
		for (n = top; n < t_boardy + 4; n++)
		{
			if (n >= Vertical_boxs)
				continue;
			for (m = 0; m < Horizontal_boxs; m++)
			{
				if (Table_board[n][m].var)
					setfillstyle(SOLID_FILL, Table_board[n][m].color);
				else
					setfillstyle(SOLID_FILL, BgColor);
				bar(x, y, x + BSIZE, y + BSIZE);
				line(x, y, x + BSIZE,y);
				line(x, y, x, y + BSIZE);
				line(x, y + BSIZE, x + BSIZE, y + BSIZE);
				line(x + BSIZE, y, x + BSIZE, y + BSIZE);
				x += BSIZE;
			}
			y += BSIZE;
			x = oldx;
		}
		ShowScore(score);
		if (speed != score / speed_step)
		{
			speed = score / speed_step;
			ShowScore(speed);
		}
		else
		{
			ShowScore(speed);
		}
	}
}

void show_box(int x, int y, int box_numb, int color)
{
	int i, ii, ls_x = x;
	if (box_numb < 0 || box_numb >= MAX_BOX)
	{
		box_numb = MAX_BOX / 2;
	}
	setfillstyle(SOLID_FILL, color);
	for (ii = 0; ii < 2; ii++)
	{
		int mask = 128;
		for (i = 0; i < 8; i++)
		{
			if (i % 4 == 0 && i != 0)
			{
				y += BSIZE;
				x = ls_x;
			}
			if ((shapes[box_numb].box[ii])&mask)
			{
				bar(x, y, x + BSIZE, y + BSIZE);
				line(x, y, x + BSIZE, y);
				line(x, y, x, y + BSIZE);
				line(x, y + BSIZE, x + BSIZE, y + BSIZE);
				line(x + BSIZE, y, x + BSIZE, y + BSIZE);
			}
			x += BSIZE;
			mask /= 2;
		}
		y += BSIZE;
		x = ls_x;
	}
}

void EraseBox(int x, int y, int box_numb)
{
	int mask = 128, t_boardx, t_boardy, n, m;
	setfillstyle(SOLID_FILL, BgColor);
	for (n = 0; n < 4; n++)
	{
		for (m = 0; m < 4; m++)
		{
			if ((shapes[box_numb].box[n / 2])&mask)
			{
				bar(x + m*BSIZE, y + n*BSIZE, x + m*BSIZE + BSIZE, y + n*BSIZE + BSIZE);
				line(x + m*BSIZE, y + n*BSIZE, x + m*BSIZE, +BSIZE, y + n*BSIZE);
				line(x + m*BSIZE, y + n*BSIZE, x + m*BSIZE, y + n*BSIZE + BSIZE);
				line(x + m*BSIZE, y + n*BSIZE + BSIZE, x + m*BSIZE + BSIZE, y + n*BSIZE + BSIZE);
				line(x + m*BSIZE+BSIZE, y + n*BSIZE, x + m*BSIZE + BSIZE, y + n*BSIZE + BSIZE);
			}
			mask = mask / (2);
			if (mask == 0)
			{
				mask = 128;
			}
		}
	}
}

int MkNestBox(int box_numb)
{
	int mask = 128, t_boardx, t_boardy, n, m;
	t_boardx = (Curbox_x - Sys_x) / BSIZE;
	t_boardy = (Curbox_y - Sys_y) / BSIZE;
	for (n = 0; n < 4; n++)
	{
		for (m = 0; m < 4; m++)
		{
			if (((shapes[current_box_numb].box[n / 2])&mask))
			{
				Table_board[t_boardx + n][t_boardy + m].var = 1;
				Table_board[t_boardx + n][t_boardy + m].color = shapes[current_box_numb].color;
			}
			mask = mask / (2);
			if (mask == 0)
			{
				mask = 128;
			}
		}
	}
	setFullRow(t_boardy);
	Curbox_x = Sys_x + Begin_boxs_x*BSIZE, Curbox_y = Sys_y;
	if (box_numb == -1)
		box_numb = rand() % MAX_BOX;
	current_box_numb = box_numb;
	flag_newbox = false;
	return(rand() % MAX_BOX);
}

int MoveAble(int x, int y, int box_numb, int direction)
{
	int n, m, t_boardx, t_boardy;
	int mask;
	if (direction == MoveLeft)
	{
		mask = 128;
		x -= BSIZE;
		t_boardx = (x - Sys_x) / BSIZE;
		t_boardy = (y - Sys_y) / BSIZE;
		for (n = 0; n < 4; n++)
		{
			for (m = 0; m < 4; m++)
			{
				if ((shapes[box_numb].box[n / 2])&mask)
				{
					if ((x + BSIZE*m) < Sys_x)
						return(false);
					else if (Table_board[t_boardy + n][t_boardx + m].var)
					{
						return(false);
					}
				}
				mask = mask / (2);
				if (mask == 0)
					mask = 128;
			}
		}
		return(true);
	}
	else if (direction == MoveRight)
	{
		x += BSIZE;
		t_boardx = (x - Sys_x) / BSIZE;
		t_boardy = (y - Sys_y) / BSIZE;
		mask = 128;
		for (n = 0; n < 4; n++)
		{
			for (m = 0; m < 4; m++)
			{
				if ((shapes[box_numb].box[n / 2])&mask)
				{
					if ((x + BSIZE*m) >= (Sys_x + BSIZE*Horizontal_boxs))
						return(false);
					else if (Table_board[t_boardy + n][t_boardx + m].var)
					{
						return(false);
					}
				}
				mask = mask / (2);
				if (mask == 0)
					mask = 128;
			}
		}
		return(true);
	}
	else if (direction == MoveDown)
	{
		y += BSIZE;
		t_boardx = (x - Sys_x) / BSIZE;
		t_boardy = (y - Sys_y) / BSIZE;
		mask = 128;
		for (n = 0; n < 4; n++)
		{
			for (m = 0; m < 4; m++)
			{
				if ((shapes[box_numb].box[n / 2])&mask)
				{
					if ((y + BSIZE*n) >= (Sys_y + BSIZE*Vertical_boxs) || Table_board[t_boardy + n][t_boardx + m].var)
					{
						flag_newbox = true;
						break;
					}
				}
				mask = mask / (2);
				if (mask == 0)
					mask = 128;
			}
		}
		if (flag_newbox)
		{
			return(false);
		}
		else
			return(true);
	}
	else if (direction == MoveRoll)
	{
		t_boardx = (x - Sys_x) / BSIZE;
		t_boardy = (y - Sys_y) / BSIZE;
		mask = 128;
		for (n = 0; n < 4; n++)
		{
			for (m = 0; m < 4; m++)
			{
				if ((shapes[box_numb].box[n / 2])&mask)
				{
					if ((y + BSIZE*n) >= (Sys_y + BSIZE*Vertical_boxs))
						return(false);
					if ((x + BSIZE*n) >= (Sys_x + BSIZE*Horizontal_boxs))
						return(false);
					if ((x + BSIZE*m) >= (Sys_x + BSIZE*Horizontal_boxs))
						return(false);
					else if (Table_board[t_boardy + n][t_boardx + m].var)
					{
						return(false);
					}
				}
				mask = mask / (-2);
				if (mask == 0)
					mask = 128;
			}	
		}
		return(true);
	}
	else
	{
		return(false);
	}
}