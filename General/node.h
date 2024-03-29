/*****************************************************************************
*  @file     node.h                                                          *
*  @brief    节点类                                                          *
*  @details  用于储存单个网络节点及其与相连节点的关系                        *
*            不单独使用，其实例主要作为 Network 类的成员函数                 *
*  @author   Dong Yu                                                         *
*  @email    213191838@seu.edu.cn                                            *
*  @version  3.1                                                             *
*  @date     2022/07/30                                                      *
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
*  2022/07/25 | 3.0       | Dong Yu        | Modify cost handling method     *
*----------------------------------------------------------------------------*
*  2022/07/30 | 3.1       | Dong Yu        | Code optimization               *
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


extern string criteria;


class Node {

private:
    string id; // 节点编号
    int degree; // 节点度
    set<string> next; // 与该节点相邻的其他所有节点
    map<string, double> cost; // 与相邻节点间 link 当前的花费，可能会随着网络流量的变化而改变
    // map<string, map<string, double>> cost_parm; // 储存与相邻节点间 link 的 performance-function: c + b * flow + c * flow ^ 2
    map<string, map<string, double>> cost_parm;  // t0 * (1 + alpha * (v / c) ^ beta)

public:
    Node();
    Node(const string& id);
    // 更新相邻节点
    // void UpdateNext(string id, double c, double b = 0, double a = 0);
    void UpdateNext(const string& id, const double& t0, const double& c);
    void UpdateNext(const string& id, const map<string, double>& parm);
    // 获取相邻节点
    set<string> get_next()const;
    // 获取与节点 id 之间 link 的花费
    double get_cost(const string& id)const;
    // 设置与节点 id 之间 link 的 performance-function
    // void set_cost_parm(string id, double c, double b, double a);
    void set_cost_parm(const string& id, const double& t0, const double& c);
    void set_cost_parm(const string& id, const map<string, double>& parm);
    // 获取与相邻节点间 link 的 performance-function
    map<string, map<string, double>> get_cost_parm()const;
    // 根据流量，更新与节点 id 之间 link 的花费
    void UpdateCost(const string& id, const double& flow);
    // 根据流量，计算与节点 id 之间 link 的花费
    // double CalculateCost(const string& id, const double& flow);
};


inline double CalculateCost(const map<string, map<string, double>>& parm, const string& id, const double& flow) {
    if (criteria == "simplified")
        return parm.at(id).at("t0") * (1 + ALPHA * pow(flow / parm.at(id).at("c"), BETA));
    else if (criteria == "tntp")
        return parm.at(id).at("free_flow_time") * (1 + parm.at(id).at("b") * pow(flow / parm.at(id).at("capacity"), parm.at(id).at("power")));
}

#endif