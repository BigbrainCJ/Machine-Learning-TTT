#pragma once
#include "mysql.h"
#include <WinSock.h>  //һ��Ҫ�������������winsock2.h
#include <cstdio>
#include <string>
#include <iostream>
#pragma comment(lib,"wsock32.lib")
#pragma comment(lib,"libmysql.lib")
using namespace std;
#define OK 999
#define NO 888 //ƽ��Ҳ����ʧ��

MYSQL mysql;

//��ʼ��mysql
int initDataBase()
{
    //��ʼ�����ݿ�
    mysql_init(&mysql);
    //���ñ��뷽ʽ
    mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "gbk");
    if (mysql_real_connect(&mysql, "localhost", "root", "123456", "", 3306, NULL, 0) == NULL)
    {
        cout << "����ʧ�ܡ�" << endl;
        return NO;
    }
    return OK;
}


int MySqlQury(string sql)
{
    //MySQL����
    if (mysql_query(&mysql, sql.c_str()))//�ɹ�����0��ʧ�ܷ���1
    {
        cout <<"�������:"<<sql<< endl;

        return NO;
    }
    return OK;
}

//���շ�����Ϣ
void Receive()
{
    try
    {
        MYSQL_RES* result = mysql_store_result(&mysql);
        if (!result)
            throw string("MySQL not result!");

        //��ȡ�ֶ�����
        int num_fields = mysql_num_fields(result);
        if (0 == num_fields)
            throw string("MySQL fields number is 0!");

        //��ȡ�ֶ���
        MYSQL_FIELD* fields = mysql_fetch_fields(result);
        if (!fields)
            throw string("MySQL fields fetch is error!");

        //��ȡ����
        MYSQL_ROW column = mysql_fetch_row(result);
      /*  for (int i = 0; i < num_fields; i++)
        {
            cout << "field " << i << " name is " << fields[i].name << endl;
        }*/
        cout<<column[0]<<","<<column[1]<<endl;
       
        cout << "MySQL is OK." << endl;
    }
    catch (string & error_msg)
    {
        cout << error_msg << endl;
    }
    catch (...)
    {
        cout << "MySQL operation is error!" << endl;
    }

}

/*
void CDL::structDataBase(OT* node)
{
    ddd++;
    //�������ݿ�ڵ�
    int dataBaseIndex = node->dataBaseIndex;
    double dataBaseWinPro = node->winProb;


    string qury_2 = "INSERT INTO tb_base(dbIndex, dbWP)VALUES(";
    string data_dbIndex= transToStr(dataBaseIndex,1);
    string data_winPro=transToStr(dataBaseWinPro,0);
    qury_2 += data_dbIndex;
    qury_2 += ",";
    qury_2 += data_winPro;
    qury_2 += "); ";
    MySqlQury(qury_2);
    //Receive();
    return;
}
*/