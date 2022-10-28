/*****************************************************************************
*  @file     user_equilibrium_solver.cpp                                     *
*  @brief    用户均衡求解类                                                  *
*  @details  目前仅实现了 Frank Wolfe 算法求解                               *
*  @author   Dong Yu                                                         *
*  @email    213191838@seu.edu.cn                                            *
*  @version  2.1                                                             *
*  @date     2022/07/30                                                      *
*                                                                            *
*----------------------------------------------------------------------------*
*  Change History :                                                          *
*  <Date>     | <Version> | <Author>       | <Description>                   *
*----------------------------------------------------------------------------*
*  2022/06/02 | 1.0       | Dong Yu        | Create File                     *
*----------------------------------------------------------------------------*
*  2022/06/05 | 1.1       | Dong Yu        | Update Comment                  *
*----------------------------------------------------------------------------*
*  2022/07/02 | 1.2       | Dong Yu        | Change Flow Type to Double      *
*----------------------------------------------------------------------------*
*  2022/07/03 | 1.3       | Dong Yu        | Add Some Output                 *
*----------------------------------------------------------------------------*
*  2022/07/03 | 1.4       | Dong Yu        | Change Step Type to Double      *
*----------------------------------------------------------------------------*
*  2022/07/04 | 1.5       | Dong Yu        | Add a New Convergence Judgment  *
*----------------------------------------------------------------------------*
*  2022/07/25 | 1.6       | Dong Yu        | Modify flow Initialize method   *
*----------------------------------------------------------------------------*
*  2022/07/25 | 1.7       | Dong Yu        | Modify the obj. calculation     *
*----------------------------------------------------------------------------*
*  2022/07/25 | 1.8       | Dong Yu        | Modify a convergence judgment   *
*----------------------------------------------------------------------------*
*  2022/07/25 | 1.9       | Dong Yu        | Modify cost handling method     *
*----------------------------------------------------------------------------*
*  2022/07/25 | 2.0       | Dong Yu        | Modify the message output method*
*----------------------------------------------------------------------------*
*  2022/07/30 | 2.1       | Dong Yu        | Code optimization               *
*----------------------------------------------------------------------------*
*                                                                            *
*****************************************************************************/

#include "message.h"
#include "user_equilibrium_solver.h"
#include "one_dimensional_minimization.h"
#include "shortest_path.h"
#include <stdlib.h>
#include <iostream>


/**
* @brief 全由全无分配
* @param network 当前网络的副本
* @return 返回分配后的结果
* @retval 返回值为 link 流量矩阵
*/
map<string, map<string, double>> AllOrNothingAssignment(const Network& network) {

	map<string, map<string, double>> od_matrix = network.get_od_matrix();
	map<string, map<string, double>> flow = network.get_flow();
	map<string, vector<string>> shortest_path;
	vector<string> path;
	set<string> all_nodes = network.get_all_nodes();

	// 初始化 link 流量为 0
	for (auto i : flow)
		for (auto j : i.second)
			flow[i.first][j.first] = 0;

	// 流量分配
	for (auto origin : od_matrix) {

		// 计算 origin 到其他 destination 的最短路（link序列）
		shortest_path = GetShortestPath(origin.first, network);

		for (auto destination : origin.second) {
			path = shortest_path[destination.first];

			// 当序列长度不为 1（origin本身也在序列中）时，将 origin-destination 的流量全部加载到 path 中的每一条 link 上
			if (path.size() > 1)
				for (int i = 1; i < path.size(); i++)
					flow[path[i]][path[i - 1]] += od_matrix[origin.first][destination.first];
		}
	}

	//cout << "------------------All or Nothing------------------" << endl;
	//for (auto i : flow)
	//	for (auto j : i.second)
	//		cout << i.first << " --> " << j.first << " is " << round(j.second) << endl;
			//if (j.second != 0)
			//    cout << i.first << " --> " << j.first << " is " << round(j.second) << endl;

	return flow;
}


/**
* @brief 依据全由全无分配，初始化网络流量
* @param network 当前网络的引用
*/
void Initialization(Network& network) {
	network.set_flow(AllOrNothingAssignment(network));
}


/**
* @brief 依据当前的流量，更新网络中所有 link 的花费
* @param network 当前网络的引用
*/
void Update(Network& network) {
	network.UpdateCost();
}


/**
* @brief 更新网络流量
* @param network 当前网络的引用
* @return 返回更新后的流量
* @retval 返回值为 link 流量矩阵
*/
map<string, map<string, double>> NetworkLoading(Network& network) {

	map<string, map<string, double>> new_flow, xn, yn;
	set<string> all_nodes = network.get_all_nodes();
	map<string, map<string, map<string, double>>> cost;
	map<string, map<string, double>> od_matrix = network.get_od_matrix();

	// 记录网络中所有 link 的 performance function
	for (set<string>::iterator id = all_nodes.begin(); id != all_nodes.end(); id++)
		cost[*id] = network.get_node(*id).get_cost_parm();

	xn = network.get_flow();
	yn = AllOrNothingAssignment(network);

	// 二分法计算最优步长 \alpha
	double alpha = BisectionMethod(network, xn, yn);

	// 计算网络更新后的流量
	for (auto i : xn)
		for (auto j : i.second)
			new_flow[i.first][j.first] = (double)(xn[i.first][j.first] + alpha * (yn[i.first][j.first] - xn[i.first][j.first]));
	
	// 更新网络流量并返回
	network.set_flow(new_flow);
	return new_flow;
}


/**
* @brief 算法收敛判断（依据流量变化量）
* @param all_nodes 网络的中所有节点
* @param flow 上一轮迭代后网络的流量
* @param flow 当前一轮迭代后网络的流量
* @return 返回判断结果
* @retval 
          - 1 收敛
		  - 0 不收敛
*/
bool IsConverge(const set<string>& all_nodes, const map<string, map<string, double>>& flow, const map<string, map<string, double>>& new_flow) {
	int count = 0;

	// 计算两次迭代结果的总变化量
	for (auto i : flow)
		for (auto j : i.second)
			if (abs(new_flow.at(i.first).at(j.first) - flow.at(i.first).at(j.first)) >= 1)
				count += abs(new_flow.at(i.first).at(j.first) - flow.at(i.first).at(j.first));

	// 依据总变化量判断是否收敛（精度过高，则无法收敛）
	if (count < 1)
		return 1;
	else
		return 0;
}


/**
* @brief 算法收敛判断（依据目标函数值变化）
* @param obj1 上一轮迭代后的目标函数值
* @param obj2 最新一轮迭代后的目标函数值
* @return 返回判断结果
* @retval
		  - 1 收敛
		  - 0 不收敛
*/
bool IsConverge(const long double& obj1, const long double& obj2) {
	if (abs(obj1 - obj2) / obj1 < 1e-7)
		return 1;
	else
		return 0;
}


/**
* @brief 计算目标函数值
* @param network 当前网络的副本
* @param delta 积分步长
* @return 目标函数值
*/
//long double CalculateObj(const Network& network, double delta = 1) {
//	map<string, map<string, double>> parm;
//	set<string> next, all_nodes = network.get_all_nodes();
//	map<string, map<string, double>> flows = network.get_flow();
//	long double obj = 0;
//	double flow, f1, f2;
//	int n;
//	for (set<string>::iterator id_1 = all_nodes.begin(); id_1 != all_nodes.end(); id_1++) {
//		parm = network.get_node(*id_1).get_cost_parm();
//		next = network.get_node(*id_1).get_next();
//		for (set<string>::iterator id_2 = next.begin(); id_2 != next.end(); id_2++) {
//			n = (int)(flows[*id_1][*id_2] / delta);
//			// f1 = network.get_node(*id_1).CalculateCost(*id_2, 0);
//			f1 = CalculateCost(parm, *id_2, 0);
//			for (int i = 1; i < n; i++) {
//				flow = delta * i;
//				// f2 = network.get_node(*id_1).CalculateCost(*id_2, flow);
//				f2 = CalculateCost(parm, *id_2, flow);
//				obj += (f1 + f2) * delta;
//				f1 = f2;
//			}
//		}
//	}
//	return obj / 2;
//}
long double CalculateObj(const Network& network) {
	long double obj = 0.0;
	double link_integral = 0.0;
	map<string, map<string, double>> parm;
	map<string, map<string, double>> flow = network.get_flow();

	for (auto i : flow) {
		parm = network.get_node(i.first).get_cost_parm();
		for (auto j : i.second) {
			link_integral = parm.at(j.first).at("t0") * flow.at(i.first).at(j.first)
				+ parm.at(j.first).at("t0") * ALPHA / pow(parm.at(j.first).at("c"), BETA)
				* pow(flow.at(i.first).at(j.first), 5) / 5;
			obj += link_integral;
		}
	}

	return obj;
}


/**
* @brief Frank Wolfe 算法
* @param network 当前网络的引用
*/
void FrankWolfe(Network& network, const string& criteria) {

	if (criteria == "flow") {
		map<string, map<string, double>> flow, new_flow;
		int i = 0;

		StatusMessageB("Initialization");
		Initialization(network);           // 初始化
		StatusMessageA();

		do {
			StatusIter(++i);

			StatusMessageB("Update");
			Update(network); // 更新花费

			flow = network.get_flow(); // 记录上一轮迭代结果
			StatusMessageB("Network Loading");
			new_flow = NetworkLoading(network); // 重新分配并记录

			StatusMessageA();

		} while (!IsConverge(network.get_all_nodes(), flow, new_flow)); // 收敛判断
	}
	else if (criteria == "obj") { // 需要多方面权衡（计算速度，结果准确性，目标函数值收敛性）
		map<string, map<string, double>> flow, new_flow;
		long double obj1, obj2;
		// Network pre_network;
		int i = 0;

		StatusMessageB("Initialization");
		Initialization(network);           // 初始化
		StatusMessageA();

		Update(network); // 更新花费
		obj2 = CalculateObj(network);

		do {
			StatusIter(++i);
			obj1 = obj2;

			StatusMessageB("Network Loading");
			NetworkLoading(network);        // 重新分配并记录

			StatusMessageB("Update");
			Update(network);                // 更新花费

			obj2 = CalculateObj(network);
			StatusMessageA("delta = " + to_string((obj1 - obj2) / obj1));

		} while (!IsConverge(obj1, obj2) || i < 10); // 收敛判断
	}
}