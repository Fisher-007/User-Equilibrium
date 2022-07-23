/*****************************************************************************
*  @file     network.h                                                       *
*  @brief    网络类                                                          *
*  @details  以临界目录表的方式存储网络信息                                  *
*            描述网络的数据需要从csv文件中读取，即需要先生成指定格式的csv文件*
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
        int max_nodes; // 网络最大节点数
        map<string, Node> nodes; // 储存所有节点
        set<string> all_nodes; // 记录所有节点的 id
        map<string, map<string, double>> od_matrix; // 网络 OD 矩阵
        map<string, map<string, double>> flow; // 网络当前流量
        
    public:
        Network(int max_nodes = 1000);
        // 初始化网络（节点、边、performance-function、OD）
        void Init(string network = "./data/tri_link.csv", string od = "./data/tri_od.csv", string criteria = "simplified");
        // 新增一个节点
        int AddNode(string id);
        // 获取网络中所有节点的 id 集合
        set<string> get_all_nodes()const;
        // 获取编号为 id 的节点
        Node get_node(string id)const;
        // 设置网络流量
        void set_flow(map<string, map<string, double>> flow);
        // 获取网络当前流量
        map<string, map<string, double>> get_flow()const;
        // 获取网络 OD 矩阵
        map<string, map<string, double>> get_od_matrix()const;
        // 更新网络中所有 link 的花费
        void UpdateCost();
};

#endif