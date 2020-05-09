#include"标头.h"
#include <thread>



int Broad[SIZE][SIZE];//棋盘

/*
   函数：落子
   传参：落子点，x或o
   返回：落子成功失败
*/
int fallPiece(int I, int J, int nCh)
{
	//1.如果该点已落子，则返回NO
	if (Broad[I][J] != Null)
		return NO;
	//2.在该点落子
	Broad[I][J] = nCh;

	return OK;
}


/*
	函数：显示棋盘
	传参：当前落子点
*/
void ShowBroad(int I, int J)
{
	for (int i = 0; i < SIZE; i++)
	{
		cout << "—————" << endl;
		for (int j = 0; j < SIZE; j++)
		{
			cout << "|" ;
			if (Broad[i][j] == x)
			{
				if (i == I && j == J)
					cout << 'X' << " ";
				else
					cout << 'x' << " ";
			}
			else if (Broad[i][j] == o)
			{
				if (i == I && j == J)
					cout << 'O' << " ";
				else
					cout << 'o' << " ";
			}
			else
			{
				cout << " " << " ";
			}
		}
		cout << "|"<<endl;
	}
	cout << "—————" << endl<< endl;
}

/*
	函数：判断是否赢棋
	传参：当前落子点
	返回：WIN或者FAIL
*/
int IfWin(int I, int J)
{
	int nCh = Broad[I][J];
	int i, j, nowLen;  //nowLen 当前已连结长度
   //上
	nowLen = 0;
	i = I - (WINLEN - 1);
	j = J;
	while (1)
	{
		if (nowLen == WINLEN)
			return OK;
		if (i > (I + (WINLEN - 1)))
			break;
		if (i<0 || i>(SIZE - 1) || Broad[i][j] != nCh)
		{
			nowLen = 0;
			i++;
			continue;  //如果当前i或者j超出边界或者中断，则跳过
		}


		if (Broad[i][j] == nCh)
			nowLen++;
		i++;
	}

	//左
	nowLen = 0;
	i = I;
	j = J - (WINLEN - 1);
	while (1)
	{
		if (nowLen == WINLEN)
			return OK;
		if (j > (J + (WINLEN - 1)))
			break;

		if (j<0 || j>(SIZE - 1) || Broad[i][j] != nCh)
		{
			nowLen = 0;
			j++;
			continue;  //如果当前i或者j超出边界或者中断，则跳过
		}


		if (Broad[i][j] == nCh)
			nowLen++;
		j++;
	}

	//左上
	nowLen = 0;
	i = I - (WINLEN - 1);
	j = J - (WINLEN - 1);
	while (1)
	{
		if (nowLen == WINLEN)
			return OK;

		if (i > (I + (WINLEN - 1)) || j > (J + (WINLEN - 1)))
			break;

		if (j<0 || j>(SIZE - 1) || i<0 || i>(SIZE - 1) || Broad[i][j] != nCh)
		{
			nowLen = 0;
			i++;
			j++;
			continue;  //如果当前i或者j超出边界或者中断，则跳过
		}

		if (Broad[i][j] == nCh)
			nowLen++;
		i++;
		j++;
	}

	//左下
	nowLen = 0;
	i = I + (WINLEN - 1);
	j = J - (WINLEN - 1);
	while (1)
	{
		if (nowLen == WINLEN)
			return OK;

		if (i < (I - (WINLEN - 1)) || j >(J + (WINLEN - 1)))
			break;

		if (j<0 || j>(SIZE - 1) || i < 0 || i >(SIZE - 1) || Broad[i][j] != nCh)
		{
			nowLen = 0;
			i--;
			j++;
			continue;  //如果当前i或者j超出边界或者中断，则跳过
		}

		if (Broad[i][j] == nCh)
			nowLen++;
		i--;
		j++;
	}
	return NO;
}
void init()
{
	for (int i = 0; i < SIZE; i++)
		for (int j = 0; j < SIZE; j++)
			Broad[i][j] = Null;
}

int ifFull()
{
	

	for (int i = 0; i < SIZE; i++)
		for (int j = 0; j < SIZE; j++)
		{
			if (Broad[i][j] == Null)
				return NO;
		}
	return OK;
}
CDL ai;

int bbb;
void chakan()
{
	string str;
	while (1)
	{
		cin >> str;
		if (bbb)
			bbb = 0;
		else
			bbb = 1;
	}

}

void people()
{

}
void main()
{
	init();
	int I, J, nCh;
	bool round = 0;
	bbb =0;
	unsigned long long int nnn = 0;
	thread t(chakan);

	while (1)
	{

		//给出决定
		ai.fallSon(&I, &J);

		//更新棋盘和round
		if (round)
		{
			Broad[I][J] = x;
			round = 0;
		}
		else
		{
			Broad[I][J] = o;
			round = 1;
		}


		if (bbb)
		{
			//显示棋盘
			ShowBroad(I, J);
			
		}
	    if (IfWin(I, J) == OK)
		{
			if(nnn< 18000000000000000000)
				nnn++;
			ai.backWard(OK);//反向更新节点
			init();
			if (bbb)
			{
				cout << "第："<<nnn<<"场，WIN" << endl << endl;
				if (1)
				{
					CDL::OT* PTR = ai.oth->child;
					for (int i = 0; i < 9; i++)
					{
						cout << PTR->winProb << "  ";
						PTR = PTR->brother;
					}
					cout << endl;
				}
				Sleep(1000);
			}
			continue;
		}
		if (ifFull() == OK)
		{
			if (nnn < 18000000000000000000)
			    nnn++;
			ai.backWard(NO);//反向更新节点
			init();
			if (bbb)
			{
				cout << "第：" << nnn << "场，DRAW" << endl << endl;
				if (1)
				{
					CDL::OT* PTR = ai.oth->child;
					for (int i = 0; i < 9; i++)
					{
						cout << PTR->winProb << "  ";
						PTR = PTR->brother;
					}
					cout << endl;
				}
				Sleep(1000);
			}

		}
	}
}


