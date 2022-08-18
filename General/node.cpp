/*****************************************************************************
*  @file     node.cpp                                                        *
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

#include "node.h"
#include <limits.h>
#include <math.h>


string criteria;


Node::Node() :degree(0) {}

Node::Node(const string& id) {
    this->id = id;
    this->degree = 0;
}

//void Node::set_cost_parm(string id, double c, double b, double a) {
//    this->cost_parm[id]["c"] = c;
//    this->cost_parm[id]["b"] = b;
//    this->cost_parm[id]["a"] = a;
//}

void Node::set_cost_parm(const string& id, const double& t0, const double& c) {
    this->cost_parm[id]["t0"] = t0;
    this->cost_parm[id]["c"] = c;
}

void Node::set_cost_parm(const string& id, const map<string, double>& parm) {
    this->cost_parm[id] = parm;
}

//void Node::UpdateNext(string id, double c, double b, double a) {
//    this->next.insert(id);
//    this->cost[id] = INT_MAX;
//    set_cost_parm(id, c, b, a);
//    this->degree++;
//}

void Node::UpdateNext(const string& id, const double& t0, const double& c) {
    this->next.insert(id);
    this->cost[id] = INT_MAX;
    set_cost_parm(id, t0, c);
    this->degree++;
}

void Node::UpdateNext(const string& id, const map<string, double>& parm) {
    this->next.insert(id);
    this->cost[id] = INT_MAX;
    set_cost_parm(id, parm);
    this->degree++;
}

set<string> Node::get_next() const {
    return this->next;
}

double Node::get_cost(const string& id) const {
    return this->cost.at(id);
}

void Node::UpdateCost(const string& id, const double& flow) {
    // this->cost[id] = this->cost_parm[id]["c"] + this->cost_parm[id]["b"] * flow + this->cost_parm[id]["a"] * flow * flow;
    if (criteria == "simplified")
        this->cost[id] = this->cost_parm[id]["t0"] * (1 + ALPHA * pow(flow / this->cost_parm[id]["c"], BETA));
    else if (criteria == "tntp")
        this->cost[id] = this->cost_parm[id]["free_flow_time"] * (1 + cost_parm[id]["b"] * pow(flow / cost_parm[id]["capacity"], cost_parm[id]["power"]));
}

//double Node::CalculateCost(const string& id, const double& flow) {
//    // this->cost[id] = this->cost_parm[id]["c"] + this->cost_parm[id]["b"] * flow + this->cost_parm[id]["a"] * flow * flow;
//    if (criteria == "simplified")
//        return this->cost_parm[id]["t0"] * (1 + ALPHA * pow(flow / this->cost_parm[id]["c"], BETA));
//    else if (criteria == "tntp")
//        return this->cost_parm[id]["free_flow_time"] * (1 + cost_parm[id]["b"] * pow(flow / cost_parm[id]["capacity"], cost_parm[id]["power"]));
//}

map<string, map<string, double>> Node::get_cost_parm() const {
    return this->cost_parm;
}