#pragma once
#include<iostream>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include "dataBase.h"
#define OK 999
#define NO 888 //平局也算作失败
#define SIZE 3
#define WINLEN 3
#define Null 2   //棋盘该点未落子
#define x 1
#define o 0
using namespace std;
class CDL {


public://结构体
	typedef struct OpenTree {
		int index;//索引码
		int deep;//深度
		double winProb;//胜率
		int childNum;//孩子数量
		int tag;//该层属于x还是属于o
		int dataBaseIndex;

		//结构体指针
		OpenTree* child;//孩子
		OpenTree* father;//父亲
		OpenTree* brother;//兄弟
	}OT;

public://结构体变量
	OT* oth;//open头
	double a;//学习率
private:
	int** broad;
	void init();

private://成员函数
	void initHead();
	OT* insOTnode(int index, double winProb, OT* father);//插入open树节点
	void structNode(OT* father);


private://工具函数
	int transToIndex(int** maxtri);//将一个棋盘矩阵转换为index
	void transToMaxtri(int** maxtri, int index);//将一个index转换为棋盘矩阵
	int toPower(int number, int Binary);//获得幂数 powr：幂
	int toNum(int power, int Binary);//获得数
	int ifWin(int I, int J);//判断输赢
	void showBroad(int I, int J);//显示输出
	void SumNode(OT* node);//统计总计节点数
	int sumNode;
	string transToStr(double num,int ifIndex);
	double transToDouble(string str);
public:
	CDL();

public:
	OT* nowNode;//当前所处棋盘状态节点
	void fallSon(int* I, int* J);//落子，交出决策
	void upDate(int I, int J, int round);

public:
	void backWard(int winORdraw);//一方胜利反向迭代

	//void structDataBase(OT* node);//构建数据库节点
	//void updateDataBase();//更新数据库节点信息
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
	init();//初始化棋盘
	oth->index = transToIndex(broad);//将初始状态放入open树中
}

/*
	 函数：插入一个节点到Open树中
	 传参：节点所需参数
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

	//如果父节点没有孩子节点，则该新节点为孩子节点；若有，则为孩子节点的兄弟
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
	参数：递归构造open树节点
	传参：当前待拓展节点指针
*/
void CDL::structNode(OT* father)
{
	//获取当前落子I J，判断输赢
	if (father!= oth)
	{
		int I, J;
		int deffIndex = father->father->index - father->index;//index差值
		//得出这个差值第几位
		int power = toPower(deffIndex, 3);
		I = power / SIZE;
		J = power % SIZE;
		if (ifWin(I, J) == OK)
			return;

	}

	//遍历棋盘，找到可桌子点
	for (int i = 0; i < SIZE; i++)
		for (int j = 0; j < SIZE; j++)
		{
			//恢复棋盘至父节点棋盘样子
			transToMaxtri(broad, father->index);

			//如果该点可以着子
			if (broad[i][j] == Null)
			{
				//判断着x或者o，并着子
				if (father->deep % 2 == 0)
					broad[i][j] = x;
				else
					broad[i][j] = o;
				int index = transToIndex(broad);

				//递归
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

	//初始化数据库
	/*initDataBase();
	string qury = "use xo";
	MySqlQury(qury);
	qury = " CREATE TABLE tb_base(dbIndex INT, dbWP  DOUBLE)ENGINE = InnoDB DEFAULT CHARSET = utf8; ";
	MySqlQury(qury);*/


	initHead();
	structNode(oth);
	nowNode = oth;//初始化当前节点为根
	a = 0.1;
	
	cout << "初始化完毕！" << endl;
	
}


/*
 已查
	   函数：根据当前状态节点选择决策节点
	   返回值： i和j，回合round
*/
void CDL::fallSon(int* I, int* J)
{
	
	//求出概率总和
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
		//生成随机数
		int sum_int = (int)sum_double;
		if (sum_int != 0)
			sum_int = rand() % sum_int;//整数部分
		sum_double = (int)rand() % 100;//小数部分
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

	
	//找到属于哪个区间
	ptr = nowNode->child;
	for (int j = 0; j < nowNode->childNum; j++)
	{
		if (num <= ptr->winProb)
		{
			//找出i，和j
			int deffIndex = nowNode->index - ptr->index;//index差值
			  //得出这个差值第几位
			int power = toPower(deffIndex, 3);
			*I = power / SIZE;
			*J = power % SIZE;
			
			//更新当前状态
			nowNode = ptr;
			return;
		}
		num = num - ptr->winProb;
		ptr = ptr->brother;
	}
	cout << "fallSon函数错误!" ;
	
}

void CDL::backWard(int winORdraw)
{
	if (winORdraw == OK)
	{
       nowNode->winProb += a * (100 - nowNode->winProb);
	   nowNode->father->winProb += a * (0 - nowNode->father->winProb);
	}//给定初始化奖励(惩罚)
	else
	{
		//先手平局给30，后手平局给50
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
		//隔层传递
		ptr->father->father->winProb += a * (ptr->winProb - ptr->father->father->winProb);
		ptr = ptr->father->father;
	}

	ptr = nowNode->father;
	while (ptr->deep > 2)
	{
		//隔层传递
		ptr->father->father->winProb += a * (ptr->winProb - ptr->father->father->winProb);
		ptr = ptr->father->father;
	}

	//初始化当前所处节点状态
	nowNode = oth;//初始化当前节点为根
}


/*
已查
	 函数：更新当前所处棋盘状态节点
	 参数：i和j,谁的回合round
*/
void CDL::upDate(int I, int J, int round)
{
	int newIndex;
	if (round == 1)
		newIndex = nowNode->index + (x - Null) * toNum(I * SIZE + J, 3);
	else
		newIndex = nowNode->index + (o - Null) * toNum(I * SIZE + J, 3);

	//开始寻找该index的child
	OT* ptr = nowNode->child;
	while (ptr)
	{
		//如果找到该节点，则更新当前节点状态为该节点
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
		cout << "upDate函数出错。" << endl;
		while (1);
	}
}

//初始化棋盘
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
	 *已查
	 函数：将一个棋盘矩阵转换为index
	 传参：棋盘矩阵
	 返回：index值
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
	 函数：将一个index转换为棋盘矩阵
	 传参：1.棋盘矩阵容器 2.index值
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
   *已查
	 函数：将一个数按照进制获得幂
	 传参：1.数  2.进制
	 返回：幂
*/
int CDL::toPower(int number, int Binary)
{

	int t = 1;
	const int maxPower = 10;//最大幂
	for (int i = 0; i < maxPower; i++)
	{
		if (number / t < Binary)
			return i;
		t = t * Binary;
	}
	return NO;
}

/*
*已查
	 函数：
	 传参：1.幂数  2.进制
	 返回：数
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
	函数：判断是否赢棋
	传参：当前落子点
	返回：WIN或者FAIL
*/
int CDL::ifWin(int I, int J)
{
	int nCh = broad[I][J];
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
		if (i<0 || i>(SIZE - 1) || broad[i][j] != nCh)
		{
			nowLen = 0;
			i++;
			continue;  //如果当前i或者j超出边界或者中断，则跳过
		}


		if (broad[i][j] == nCh)
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

		if (j<0 || j>(SIZE - 1) || broad[i][j] != nCh)
		{
			nowLen = 0;
			j++;
			continue;  //如果当前i或者j超出边界或者中断，则跳过
		}


		if (broad[i][j] == nCh)
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

		if (j<0 || j>(SIZE - 1) || i<0 || i>(SIZE - 1) || broad[i][j] != nCh)
		{
			nowLen = 0;
			i++;
			j++;
			continue;  //如果当前i或者j超出边界或者中断，则跳过
		}

		if (broad[i][j] == nCh)
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

		if (j<0 || j>(SIZE - 1) || i < 0 || i >(SIZE - 1) || broad[i][j] != nCh)
		{
			nowLen = 0;
			i--;
			j++;
			continue;  //如果当前i或者j超出边界或者中断，则跳过
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
		//保留两位
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
