#include"��ͷ.h"
#include <thread>



int Broad[SIZE][SIZE];//����

/*
   ����������
   ���Σ����ӵ㣬x��o
   ���أ����ӳɹ�ʧ��
*/
int fallPiece(int I, int J, int nCh)
{
	//1.����õ������ӣ��򷵻�NO
	if (Broad[I][J] != Null)
		return NO;
	//2.�ڸõ�����
	Broad[I][J] = nCh;

	return OK;
}


/*
	��������ʾ����
	���Σ���ǰ���ӵ�
*/
void ShowBroad(int I, int J)
{
	for (int i = 0; i < SIZE; i++)
	{
		cout << "����������" << endl;
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
	cout << "����������" << endl<< endl;
}

/*
	�������ж��Ƿ�Ӯ��
	���Σ���ǰ���ӵ�
	���أ�WIN����FAIL
*/
int IfWin(int I, int J)
{
	int nCh = Broad[I][J];
	int i, j, nowLen;  //nowLen ��ǰ�����᳤��
   //��
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
			continue;  //�����ǰi����j�����߽�����жϣ�������
		}


		if (Broad[i][j] == nCh)
			nowLen++;
		i++;
	}

	//��
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
			continue;  //�����ǰi����j�����߽�����жϣ�������
		}


		if (Broad[i][j] == nCh)
			nowLen++;
		j++;
	}

	//����
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
			continue;  //�����ǰi����j�����߽�����жϣ�������
		}

		if (Broad[i][j] == nCh)
			nowLen++;
		i++;
		j++;
	}

	//����
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
			continue;  //�����ǰi����j�����߽�����жϣ�������
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

		//��������
		ai.fallSon(&I, &J);

		//�������̺�round
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
			//��ʾ����
			ShowBroad(I, J);
			
		}
	    if (IfWin(I, J) == OK)
		{
			if(nnn< 18000000000000000000)
				nnn++;
			ai.backWard(OK);//������½ڵ�
			init();
			if (bbb)
			{
				cout << "�ڣ�"<<nnn<<"����WIN" << endl << endl;
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
			ai.backWard(NO);//������½ڵ�
			init();
			if (bbb)
			{
				cout << "�ڣ�" << nnn << "����DRAW" << endl << endl;
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


