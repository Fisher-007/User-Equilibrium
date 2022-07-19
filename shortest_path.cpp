/*****************************************************************************
*  @file     shortest_path.cpp                                               *
*  @brief    最短路类                                                        *
*  @details  用于求解最短路                                                  *
*  @author   Dong Yu                                                         *
*  @email    213191838@seu.edu.cn                                            *
*  @version  1.1                                                             *
*  @date     2022/06/05                                                      *
*                                                                            *
*----------------------------------------------------------------------------*
*  Change History :                                                          *
*  <Date>     | <Version> | <Author>       | <Description>                   *
*----------------------------------------------------------------------------*
*  2022/06/02 | 1.0       | Dong Yu        | Create file                     *
*----------------------------------------------------------------------------*
*  2022/06/05 | 1.1       | Dong Yu        | Update Comment                  *
*----------------------------------------------------------------------------*
*                                                                            *
*****************************************************************************/

#include "shortest_path.h"
#include <limits.h>


/**
* @brief 使用 Dijkstra 算法，计算最短路
* @param origin 最短路起点
* @param network 当前网络的副本
* @return 返回 network 中 origin 到所有 destinations 的最短 path
* @retval 返回值包括到所有 destination 对应的 path（倒序存放的一系列 link） 
*/
map<string, vector<string>> GetShortestPath(string origin, const Network& network) {

	map<string, int> state; // 每个节点状态（0-1），0 代表已选中
	map<string, double> total_cost; // 当前迭代中 origin 到每个节点的花费
	map<string, string> previous_node; // 记录前一个节点
	map<string, vector<string>> shortest_path;

	set<string> nodes = network.get_all_nodes();
	set<string> next;
	string current_node;
	double new_cost, min_cost;

	// 初始化所有节点
	for (set<string>::iterator id = nodes.begin(); id != nodes.end(); id++) {
		state[*id] = 1;
		total_cost[*id] = INT_MAX;
		previous_node[*id] = "NULL";
	}

	// 设置起点 origin
	total_cost[origin] = 0;

	// 求解最短路
	for (int iter = 1; iter < network.get_all_nodes().size(); iter++) {

		// 寻找当前还未选过的节点中，total_cost 最小的节点
		min_cost = INT_MAX;
		for (auto iter : total_cost) {
			if (iter.second < min_cost && state[iter.first]) {
				current_node = iter.first;
				min_cost = iter.second;
			}
		}
		
		// 更新当前选中的节点状态
		next = network.get_node(current_node).get_next();
		state[current_node] = 0;

		// 遍历 current_node 相邻的节点
		for (set<string>::iterator id = next.begin(); id != next.end(); id++) {

			if (state[*id]) {
				new_cost = total_cost[current_node] + network.get_node(current_node).get_cost(*id);
				if (total_cost[*id] > new_cost) {
					total_cost[*id] = new_cost;
					previous_node[*id] = current_node;
				}
			}
		}
	}

	// 根据 previous_node 获取最短路对应的 link 序列（倒序）
	string temp_node;
	for (set<string>::iterator id = nodes.begin(); id != nodes.end(); id++) {
		temp_node = *id;
		while (temp_node != "NULL") {
			shortest_path[*id].push_back(temp_node);
			temp_node = previous_node[temp_node];
		}
	}

	return shortest_path;
}
