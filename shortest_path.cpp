/*****************************************************************************
*  @file     shortest_path.cpp                                               *
*  @brief    ���·��                                                        *
*  @details  ����������·                                                  *
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
* @brief ʹ�� Dijkstra �㷨���������·
* @param origin ���·���
* @param network ��ǰ����ĸ���
* @return ���� network �� origin ������ destinations ����� path
* @retval ����ֵ���������� destination ��Ӧ�� path�������ŵ�һϵ�� link�� 
*/
map<string, vector<string>> GetShortestPath(string origin, const Network& network) {

	map<string, int> state; // ÿ���ڵ�״̬��0-1����0 ������ѡ��
	map<string, double> total_cost; // ��ǰ������ origin ��ÿ���ڵ�Ļ���
	map<string, string> previous_node; // ��¼ǰһ���ڵ�
	map<string, vector<string>> shortest_path;

	set<string> nodes = network.get_all_nodes();
	set<string> next;
	string current_node;
	double new_cost, min_cost;

	// ��ʼ�����нڵ�
	for (set<string>::iterator id = nodes.begin(); id != nodes.end(); id++) {
		state[*id] = 1;
		total_cost[*id] = INT_MAX;
		previous_node[*id] = "NULL";
	}

	// ������� origin
	total_cost[origin] = 0;

	// ������·
	for (int iter = 1; iter < network.get_all_nodes().size(); iter++) {

		// Ѱ�ҵ�ǰ��δѡ���Ľڵ��У�total_cost ��С�Ľڵ�
		min_cost = INT_MAX;
		for (auto iter : total_cost) {
			if (iter.second < min_cost && state[iter.first]) {
				current_node = iter.first;
				min_cost = iter.second;
			}
		}
		
		// ���µ�ǰѡ�еĽڵ�״̬
		next = network.get_node(current_node).get_next();
		state[current_node] = 0;

		// ���� current_node ���ڵĽڵ�
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

	// ���� previous_node ��ȡ���·��Ӧ�� link ���У�����
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
