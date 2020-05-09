#pragma once
#include<iostream>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include "dataBase.h"
#define OK 999
#define NO 888 //ƽ��Ҳ����ʧ��
#define SIZE 3
#define WINLEN 3
#define Null 2   //���̸õ�δ����
#define x 1
#define o 0
using namespace std;
class CDL {


public://�ṹ��
	typedef struct OpenTree {
		int index;//������
		int deep;//���
		double winProb;//ʤ��
		int childNum;//��������
		int tag;//�ò�����x��������o
		int dataBaseIndex;

		//�ṹ��ָ��
		OpenTree* child;//����
		OpenTree* father;//����
		OpenTree* brother;//�ֵ�
	}OT;

public://�ṹ�����
	OT* oth;//openͷ
	double a;//ѧϰ��
private:
	int** broad;
	void init();

private://��Ա����
	void initHead();
	OT* insOTnode(int index, double winProb, OT* father);//����open���ڵ�
	void structNode(OT* father);


private://���ߺ���
	int transToIndex(int** maxtri);//��һ�����̾���ת��Ϊindex
	void transToMaxtri(int** maxtri, int index);//��һ��indexת��Ϊ���̾���
	int toPower(int number, int Binary);//������� powr����
	int toNum(int power, int Binary);//�����
	int ifWin(int I, int J);//�ж���Ӯ
	void showBroad(int I, int J);//��ʾ���
	void SumNode(OT* node);//ͳ���ܼƽڵ���
	int sumNode;
	string transToStr(double num,int ifIndex);
	double transToDouble(string str);
public:
	CDL();

public:
	OT* nowNode;//��ǰ��������״̬�ڵ�
	void fallSon(int* I, int* J);//���ӣ���������
	void upDate(int I, int J, int round);

public:
	void backWard(int winORdraw);//һ��ʤ���������

	//void structDataBase(OT* node);//�������ݿ�ڵ�
	//void updateDataBase();//�������ݿ�ڵ���Ϣ
};

void CDL::initHead()
{
	oth = (OT*)malloc(sizeof(OT));
	oth->deep = 0;
	oth->winProb = -1;
	oth->child = NULL;
	oth->brother = NULL;
	oth->father = NULL;
	oth->childNum = 0;
	oth->tag = -1;
	oth->dataBaseIndex= 0;
	init();//��ʼ������
	oth->index = transToIndex(broad);//����ʼ״̬����open����
}

/*
	 ����������һ���ڵ㵽Open����
	 ���Σ��ڵ��������
*/
CDL::OT* CDL::insOTnode(int index, double winProb, OT* father)
{
	OT* newNode = (OT*)malloc(sizeof(OT));
	newNode->index = index;
	newNode->deep = father->deep + 1;
	newNode->winProb = winProb;
	newNode->father = father;
	newNode->child = NULL;
	newNode->childNum = 0;
	newNode->dataBaseIndex = father->dataBaseIndex * 10 + father->childNum+1;
	if (father->tag == -1)
		newNode->tag = 1;
	else if (father->tag == 0)
		newNode->tag = 1;
	else
		newNode->tag = 0;

	//������ڵ�û�к��ӽڵ㣬����½ڵ�Ϊ���ӽڵ㣻���У���Ϊ���ӽڵ���ֵ�
	if (father->child == NULL)
	{
		father->child = newNode;
		newNode->brother = NULL;
	}
	else
	{
		newNode->brother = father->child->brother;
		father->child->brother = newNode;
	}

	father->childNum++;
	return newNode;
}
int ddd = 0;
/*
	�������ݹ鹹��open���ڵ�
	���Σ���ǰ����չ�ڵ�ָ��
*/
void CDL::structNode(OT* father)
{
	//��ȡ��ǰ����I J���ж���Ӯ
	if (father!= oth)
	{
		int I, J;
		int deffIndex = father->father->index - father->index;//index��ֵ
		//�ó������ֵ�ڼ�λ
		int power = toPower(deffIndex, 3);
		I = power / SIZE;
		J = power % SIZE;
		if (ifWin(I, J) == OK)
			return;

	}

	//�������̣��ҵ������ӵ�
	for (int i = 0; i < SIZE; i++)
		for (int j = 0; j < SIZE; j++)
		{
			//�ָ����������ڵ���������
			transToMaxtri(broad, father->index);

			//����õ��������
			if (broad[i][j] == Null)
			{
				//�ж���x����o��������
				if (father->deep % 2 == 0)
					broad[i][j] = x;
				else
					broad[i][j] = o;
				int index = transToIndex(broad);

				//�ݹ�
				OT* nowNode = insOTnode(index, 50.01, father);
				structNode(nowNode);

			}
		}


}

void CDL::SumNode(OT* node)
{
	sumNode += node->childNum;

	OT* ptr = node->child;
	while (ptr != NULL)
	{
		SumNode(ptr);
		ptr = ptr->brother;
	}
}
CDL::CDL()
{
	srand((int)time(0));

	//��ʼ�����ݿ�
	/*initDataBase();
	string qury = "use xo";
	MySqlQury(qury);
	qury = " CREATE TABLE tb_base(dbIndex INT, dbWP  DOUBLE)ENGINE = InnoDB DEFAULT CHARSET = utf8; ";
	MySqlQury(qury);*/


	initHead();
	structNode(oth);
	nowNode = oth;//��ʼ����ǰ�ڵ�Ϊ��
	a = 0.1;
	
	cout << "��ʼ����ϣ�" << endl;
	
}


/*
 �Ѳ�
	   ���������ݵ�ǰ״̬�ڵ�ѡ����߽ڵ�
	   ����ֵ�� i��j���غ�round
*/
void CDL::fallSon(int* I, int* J)
{
	
	//��������ܺ�
	double sum_double = 0;
	OT* ptr = nowNode->child;
	for (int i = 0; i <nowNode->childNum; i++)
	{
		sum_double += ptr->winProb;
		ptr = ptr->brother;
	}
	double test = sum_double;
	

	double num=0;
	if (sum_double >=1)
	{
		//���������
		int sum_int = (int)sum_double;
		if (sum_int != 0)
			sum_int = rand() % sum_int;//��������
		sum_double = (int)rand() % 100;//С������
		sum_double = sum_double / 100;
		num = (double)sum_int + sum_double;

		
	}
	else
	{
		int dec = sum_double * 100;
		if (dec != 0)
		{
			dec = rand() % dec;
			sum_double = dec;
			sum_double = sum_double / 100;
		}
		else
			sum_double = 0;
	}

	
	//�ҵ������ĸ�����
	ptr = nowNode->child;
	for (int j = 0; j < nowNode->childNum; j++)
	{
		if (num <= ptr->winProb)
		{
			//�ҳ�i����j
			int deffIndex = nowNode->index - ptr->index;//index��ֵ
			  //�ó������ֵ�ڼ�λ
			int power = toPower(deffIndex, 3);
			*I = power / SIZE;
			*J = power % SIZE;
			
			//���µ�ǰ״̬
			nowNode = ptr;
			return;
		}
		num = num - ptr->winProb;
		ptr = ptr->brother;
	}
	cout << "fallSon��������!" ;
	
}

void CDL::backWard(int winORdraw)
{
	if (winORdraw == OK)
	{
       nowNode->winProb += a * (100 - nowNode->winProb);
	   nowNode->father->winProb += a * (0 - nowNode->father->winProb);
	}//������ʼ������(�ͷ�)
	else
	{
		//����ƽ�ָ�30������ƽ�ָ�50
		if (nowNode->tag == x)
		{
           nowNode->winProb += a * (30 - nowNode->winProb);
		   nowNode->father->winProb += a * (50 - nowNode->father->winProb);
		}
		else
		{
			nowNode->winProb += a * (50 - nowNode->winProb);
			nowNode->father->winProb += a * (30 - nowNode->father->winProb);
		}
		
	}

	OT* ptr = nowNode;
	while (ptr->deep > 2)
	{
		//���㴫��
		ptr->father->father->winProb += a * (ptr->winProb - ptr->father->father->winProb);
		ptr = ptr->father->father;
	}

	ptr = nowNode->father;
	while (ptr->deep > 2)
	{
		//���㴫��
		ptr->father->father->winProb += a * (ptr->winProb - ptr->father->father->winProb);
		ptr = ptr->father->father;
	}

	//��ʼ����ǰ�����ڵ�״̬
	nowNode = oth;//��ʼ����ǰ�ڵ�Ϊ��
}


/*
�Ѳ�
	 ���������µ�ǰ��������״̬�ڵ�
	 ������i��j,˭�Ļغ�round
*/
void CDL::upDate(int I, int J, int round)
{
	int newIndex;
	if (round == 1)
		newIndex = nowNode->index + (x - Null) * toNum(I * SIZE + J, 3);
	else
		newIndex = nowNode->index + (o - Null) * toNum(I * SIZE + J, 3);

	//��ʼѰ�Ҹ�index��child
	OT* ptr = nowNode->child;
	while (ptr)
	{
		//����ҵ��ýڵ㣬����µ�ǰ�ڵ�״̬Ϊ�ýڵ�
		if (ptr->index == newIndex)
		{
			nowNode = ptr;
			cout << "1" << endl;
			return;
		}
		ptr = ptr->brother;
	}

	if (ptr == NULL)
	{
		cout << "upDate��������" << endl;
		while (1);
	}
}

//��ʼ������
void CDL::init()
{
	broad = (int**)malloc(sizeof(int*) * SIZE);

	for (int i = 0; i < SIZE; i++)
		broad[i] = (int*)malloc(sizeof(int) * SIZE);

	for (int i = 0; i < SIZE; i++)
		for (int j = 0; j < SIZE; j++)
			broad[i][j] = Null;

}

/*
	 *�Ѳ�
	 ��������һ�����̾���ת��Ϊindex
	 ���Σ����̾���
	 ���أ�indexֵ
*/
int CDL::transToIndex(int** maxtri)
{
	int index = 0;
	for (int i = 0; i < SIZE; i++)
		for (int j = 0; j < SIZE; j++)
		{
			index += maxtri[i][j] * toNum(i * SIZE + j, 3);
		}
	return index;
}

/*
	 ��������һ��indexת��Ϊ���̾���
	 ���Σ�1.���̾������� 2.indexֵ
*/
void CDL::transToMaxtri(int** maxtri, int index)
{
	for (int i = SIZE - 1; i > -1; i--)
		for (int j = SIZE - 1; j > -1; j--)
		{

			int t = toNum(i * SIZE + j, 3);
			maxtri[i][j] = index / t;
			index = index - t * maxtri[i][j];
		}

}

/*
   *�Ѳ�
	 ��������һ�������ս��ƻ����
	 ���Σ�1.��  2.����
	 ���أ���
*/
int CDL::toPower(int number, int Binary)
{

	int t = 1;
	const int maxPower = 10;//�����
	for (int i = 0; i < maxPower; i++)
	{
		if (number / t < Binary)
			return i;
		t = t * Binary;
	}
	return NO;
}

/*
*�Ѳ�
	 ������
	 ���Σ�1.����  2.����
	 ���أ���
*/
int CDL::toNum(int power, int Binary)
{
	int t = 1;
	for (int i = 0; i < power; i++)
	{
		t = t * Binary;
	}
	return t;
}

/*
	�������ж��Ƿ�Ӯ��
	���Σ���ǰ���ӵ�
	���أ�WIN����FAIL
*/
int CDL::ifWin(int I, int J)
{
	int nCh = broad[I][J];
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
		if (i<0 || i>(SIZE - 1) || broad[i][j] != nCh)
		{
			nowLen = 0;
			i++;
			continue;  //�����ǰi����j�����߽�����жϣ�������
		}


		if (broad[i][j] == nCh)
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

		if (j<0 || j>(SIZE - 1) || broad[i][j] != nCh)
		{
			nowLen = 0;
			j++;
			continue;  //�����ǰi����j�����߽�����жϣ�������
		}


		if (broad[i][j] == nCh)
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

		if (j<0 || j>(SIZE - 1) || i<0 || i>(SIZE - 1) || broad[i][j] != nCh)
		{
			nowLen = 0;
			i++;
			j++;
			continue;  //�����ǰi����j�����߽�����жϣ�������
		}

		if (broad[i][j] == nCh)
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

		if (j<0 || j>(SIZE - 1) || i < 0 || i >(SIZE - 1) || broad[i][j] != nCh)
		{
			nowLen = 0;
			i--;
			j++;
			continue;  //�����ǰi����j�����߽�����жϣ�������
		}

		if (broad[i][j] == nCh)
			nowLen++;
		i--;
		j++;
	}
	return NO;
}

void CDL::showBroad(int I, int J)
{
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{

			if (broad[i][j] == 1)
			{
				if (i == I && j == J)
					cout << 'X' << " ";
				else
					cout << 'x' << " ";
			}
			else if (broad[i][j] == 0)
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
		cout << endl;
	}

}




string CDL::transToStr(double num,int ifIndex)
{
	string str;
	int num_int = num;
	while (1)
	{
		str += num_int - (num_int / 10 * 10) + 48;
		num_int = num_int / 10;
		if (num_int == 0)
			break;
	}
	char tC;
	for (int i = 0; i < str.size() / 2; i++)
	{
		tC = str[i];
		str[i] = str[str.size() - i - 1];
		str[str.size() - i - 1] = tC;

	}
	if (!ifIndex)
	{
		str += ".";

		num = num - (int)num;
		//������λ
		num_int = num * 100;
		str += num_int / 10 + 48;
		str += num_int % 10 + 48;
	}

	return str;
}

double CDL::transToDouble(string str)
{
	int num_int = 0;
	for (int i = 0; str[i] != '.'; i++)
	{

		num_int *= 10;
		num_int += str[i] - 48;

	}
	double num_double;
	for (int i = 0; i < str.size(); i++)
	{
		if (str[i] == '.')
		{
			num_double = (str[i + 1] - 48) * 10;
			num_double += str[i + 2] - 48;
			num_double /= 100;
			num_double += num_int;
			return num_double;
		}
	}
}
