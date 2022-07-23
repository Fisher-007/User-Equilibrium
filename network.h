/*****************************************************************************
*  @file     network.h                                                       *
*  @brief    ������                                                          *
*  @details  ���ٽ�Ŀ¼��ķ�ʽ�洢������Ϣ                                  *
*            ���������������Ҫ��csv�ļ��ж�ȡ������Ҫ������ָ����ʽ��csv�ļ�*
*  @author   Dong Yu                                                         *
*  @email    213191838@seu.edu.cn                                            *
*  @version  2.4                                                             *
*  @date     2022/07/22                                                      *
*                                                                            *
*----------------------------------------------------------------------------*
*  Change History :                                                          *
*  <Date>     | <Version> | <Author>       | <Description>                   *
*----------------------------------------------------------------------------*
*  2022/06/01 | 1.0       | Dong Yu        | Create File                     *
*----------------------------------------------------------------------------*
*  2022/06/03 | 2.0       | Dong Yu        | Refine Cost                     *
*----------------------------------------------------------------------------*
*  2022/06/05 | 2.1       | Dong Yu        | Update Comment                  *
*----------------------------------------------------------------------------*
*  2022/07/02 | 2.2       | Dong Yu        | Change Flow Type to Double      *
*----------------------------------------------------------------------------*
*  2022/07/22 | 2.4       | Dong Yu        | Add tntp file read              *
*----------------------------------------------------------------------------*
*                                                                            *
*****************************************************************************/
#pragma once
#ifndef NETWORK_H
#define NETWORK_H

#include "node.h"
#include "message.h"
#include <vector>
#include <set>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

class Network{

    private:
        int max_nodes; // �������ڵ���
        map<string, Node> nodes; // �������нڵ�
        set<string> all_nodes; // ��¼���нڵ�� id
        map<string, map<string, double>> od_matrix; // ���� OD ����
        map<string, map<string, double>> flow; // ���統ǰ����
        
    public:
        Network(int max_nodes = 1000);
        // ��ʼ�����磨�ڵ㡢�ߡ�performance-function��OD��
        void Init(string network = "./data/tri_link.csv", string od = "./data/tri_od.csv", string criteria = "simplified");
        // ����һ���ڵ�
        int AddNode(string id);
        // ��ȡ���������нڵ�� id ����
        set<string> get_all_nodes()const;
        // ��ȡ���Ϊ id �Ľڵ�
        Node get_node(string id)const;
        // ������������
        void set_flow(map<string, map<string, double>> flow);
        // ��ȡ���統ǰ����
        map<string, map<string, double>> get_flow()const;
        // ��ȡ���� OD ����
        map<string, map<string, double>> get_od_matrix()const;
        // �������������� link �Ļ���
        void UpdateCost();
};

#endif