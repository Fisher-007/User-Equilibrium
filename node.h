/*****************************************************************************
*  @file     node.h                                                          *
*  @brief    �ڵ���                                                          *
*  @details  ���ڴ��浥������ڵ㼰���������ڵ�Ĺ�ϵ                        *
*            ������ʹ�ã���ʵ����Ҫ��Ϊ Network ��ĳ�Ա����                 *
*  @author   Dong Yu                                                         *
*  @email    213191838@seu.edu.cn                                            *
*  @version  2.1                                                             *
*  @date     2022/06/05                                                      *
*                                                                            *
*----------------------------------------------------------------------------*
*  Change History :                                                          *
*  <Date>     | <Version> | <Author>       | <Description>                   *
*----------------------------------------------------------------------------*
*  2022/05/31 | 1.0       | Dong Yu        | Create File                     *
*----------------------------------------------------------------------------*
*  2022/06/03 | 2.0       | Dong Yu        | Refine Cost                     *
*----------------------------------------------------------------------------*
*  2022/06/05 | 2.1       | Dong Yu        | Update Comment                  *
*----------------------------------------------------------------------------*
*  2022/07/02 | 2.2       | Dong Yu        | Change Flow Type to Double      *
*----------------------------------------------------------------------------*
*  2022/07/02 | 2.3       | Dong Yu        | Change Performance-function     *
*----------------------------------------------------------------------------*
*                                                                            *
*****************************************************************************/
#pragma once
#ifndef NODE_H
#define NODE_H

#include <set>
#include <string>
#include <map>
using namespace std;


#define ALPHA 0.15
#define BETA 4


class Node {

private:
    string id; // �ڵ���
    int degree; // �ڵ��
    set<string> next; // ��ýڵ����ڵ��������нڵ�
    map<string, double> cost; // �����ڽڵ�� link ��ǰ�Ļ��ѣ����ܻ��������������ı仯���ı�
    // map<string, map<string, double>> cost_parm; // ���������ڽڵ�� link �� performance-function: c + b * flow + c * flow ^ 2
    map<string, map<string, double>> cost_parm;  // t0 * (1 + alpha * (v / c) ^ beta)

public:
    Node();
    Node(string id);
    // �������ڽڵ�
    // void UpdateNext(string id, double c, double b = 0, double a = 0);
    void UpdateNext(string id, double t0, double c);
    // ��ȡ���ڽڵ�
    set<string> get_next();
    // ��ȡ��ڵ� id ֮�� link �Ļ���
    double get_cost(string id);
    // ������ڵ� id ֮�� link �� performance-function
    // void set_cost_parm(string id, double c, double b, double a);
    void set_cost_parm(string id, double t0, double c);
    // ��ȡ�����ڽڵ�� link �� performance-function
    map<string, map<string, double>> get_cost_parm();
    // ����������������ڵ� id ֮�� link �Ļ���
    void UpdateCost(string id, double flow);
};

#endif