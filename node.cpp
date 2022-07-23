/*****************************************************************************
*  @file     node.cpp                                                        *
*  @brief    节点类                                                          *
*  @details  用于储存单个网络节点及其与相连节点的关系                        *
*            不单独使用，其实例主要作为 Network 类的成员函数                 *
*  @author   Dong Yu                                                         *
*  @email    213191838@seu.edu.cn                                            *
*  @version  2.3                                                             *
*  @date     2022/07/02                                                      *
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

#include "node.h"
#include <limits.h>
#include <math.h>


Node::Node() :degree(0) {}

Node::Node(string id) {
    this->id = id;
    this->degree = 0;
}

//void Node::set_cost_parm(string id, double c, double b, double a) {
//    this->cost_parm[id]["c"] = c;
//    this->cost_parm[id]["b"] = b;
//    this->cost_parm[id]["a"] = a;
//}

void Node::set_cost_parm(string id, double t0, double c) {
    this->cost_parm[id]["t0"] = t0;
    this->cost_parm[id]["c"] = c;
}

//void Node::UpdateNext(string id, double c, double b, double a) {
//    this->next.insert(id);
//    this->cost[id] = INT_MAX;
//    set_cost_parm(id, c, b, a);
//    this->degree++;
//}

void Node::UpdateNext(string id, double t0, double c) {
    this->next.insert(id);
    this->cost[id] = INT_MAX;
    set_cost_parm(id, t0, c);
    this->degree++;
}

set<string> Node::get_next() {
    return this->next;
}

double Node::get_cost(string id) {
    return this->cost[id];
}

void Node::UpdateCost(string id, double flow) {
    // this->cost[id] = this->cost_parm[id]["c"] + this->cost_parm[id]["b"] * flow + this->cost_parm[id]["a"] * flow * flow;
    this->cost[id] = this->cost_parm[id]["t0"] * (1 + ALPHA * pow(flow / this->cost_parm[id]["c"], BETA));
}

map<string, map<string, double>> Node::get_cost_parm() {
    return this->cost_parm;
}