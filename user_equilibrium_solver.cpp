/*****************************************************************************
*  @file     user_equilibrium_solver.cpp                                     *
*  @brief    �û����������                                                  *
*  @details  Ŀǰ��ʵ���� Frank Wolfe �㷨���                               *
*  @author   Dong Yu                                                         *
*  @email    213191838@seu.edu.cn                                            *
*  @version  1.1                                                             *
*  @date     2022/06/05                                                      *
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
*                                                                            *
*****************************************************************************/

#include "user_equilibrium_solver.h"
#include "one_dimensional_minimization.h"
#include "shortest_path.h"
#include <stdlib.h>
#include <iostream>


/**
* @brief ȫ��ȫ�޷���
* @param network ��ǰ����ĸ���
* @return ���ط����Ľ��
* @retval ����ֵΪ link ��������
*/
map<string, map<string, double>> AllOrNothingAssignment(const Network& network) {

	map<string, map<string, double>> od_matrix = network.get_od_matrix();
	map<string, map<string, double>> flow;
	//map<string, map<string, double>> flow = network.get_flow();
	map<string, vector<string>> shortest_path;
	vector<string> path;
	set<string> all_nodes = network.get_all_nodes();

	// ��ʼ�� link ����Ϊ 0
	for (set<string>::iterator id_1 = all_nodes.begin(); id_1 != all_nodes.end(); id_1++)
		for (set<string>::iterator id_2 = all_nodes.begin(); id_2 != all_nodes.end(); id_2++)
			flow[*id_1][*id_2] = 0;
	//for (auto i : flow)
	//	for (auto j : i.second)
	//		j.second = 0;

	// ��������
	for (set<string>::iterator origin = all_nodes.begin(); origin != all_nodes.end(); origin++) {

		// ���� origin ������ destination �����·��link���У�
		shortest_path = GetShortestPath(*origin, network);

		for (set<string>::iterator destination = all_nodes.begin(); destination != all_nodes.end(); destination++) {
			path = shortest_path[*destination];

			// �����г��Ȳ�Ϊ 1��origin����Ҳ�������У�ʱ���� origin-destination ������ȫ�����ص� path �е�ÿһ�� link ��
			if (path.size() > 1)
				for (int i = 1; i < path.size(); i++)
					flow[path[i]][path[i - 1]] += od_matrix[*origin][*destination];
		}
	}

	cout << "------------------All or Nothing------------------" << endl;
	for (auto i : flow)
		for (auto j : i.second)
			if (j.second != 0)
			    cout << i.first << " --> " << j.first << " is " << round(j.second) << endl;

	return flow;
}


/**
* @brief ����ȫ��ȫ�޷��䣬��ʼ����������
* @param network ��ǰ���������
*/
void Initialization(Network& network) {
	network.set_flow(AllOrNothingAssignment(network));
}


/**
* @brief ���ݵ�ǰ���������������������� link �Ļ���
* @param network ��ǰ���������
*/
void Update(Network& network) {
	network.UpdateCost();
}


/**
* @brief ������������
* @param network ��ǰ���������
* @return ���ظ��º������
* @retval ����ֵΪ link ��������
*/
map<string, map<string, double>> NetworkLoading(Network& network) {

	map<string, map<string, double>> new_flow, xn, yn;
	set<string> all_nodes = network.get_all_nodes();
	map<string, map<string, map<string, double>>> cost;

	// ��¼���������� link �� performance function
	for (set<string>::iterator id = all_nodes.begin(); id != all_nodes.end(); id++)
		cost[*id] = network.get_node(*id).get_cost_parm();

	xn = network.get_flow();
	yn = AllOrNothingAssignment(network);

	// ���ַ��������Ų��� \alpha
	double alpha = BisectionMethod(all_nodes, cost, xn, yn);
	cout << "------------------alpha = " << alpha << "------------------" << endl;

	// ����������º������
	for (set<string>::iterator id_1 = all_nodes.begin(); id_1 != all_nodes.end(); id_1++)
		for (set<string>::iterator id_2 = all_nodes.begin(); id_2 != all_nodes.end(); id_2++)
			new_flow[*id_1][*id_2] = (double)(xn[*id_1][*id_2] + alpha * (yn[*id_1][*id_2] - xn[*id_1][*id_2]));
	
	// ������������������
	network.set_flow(new_flow);
	return new_flow;
}


/**
* @brief �㷨�����жϣ����������仯����
* @param all_nodes ����������нڵ�
* @param flow ��һ�ֵ��������������
* @param flow ��ǰһ�ֵ��������������
* @return �����жϽ��
* @retval 
          - 1 ����
		  - 0 ������
*/
bool IsConverge(set<string> all_nodes, map<string, map<string, double>> flow, map<string, map<string, double>> new_flow) {
	int count = 0;

	// �������ε���������ܱ仯��
	for (set<string>::iterator id_1 = all_nodes.begin(); id_1 != all_nodes.end(); id_1++)
		for (set<string>::iterator id_2 = all_nodes.begin(); id_2 != all_nodes.end(); id_2++)
			if (abs(new_flow[*id_1][*id_2] - flow[*id_1][*id_2]) >= 1)
				count += abs(new_flow[*id_1][*id_2] - flow[*id_1][*id_2]);

	// �����ܱ仯���ж��Ƿ����������ȹ��ߣ����޷�������
	if (count < 1)
		return 1;
	else
		return 0;
}


/**
* @brief �㷨�����жϣ�����Ŀ�꺯��ֵ�仯��
* @param obj1 ��һ�ֵ������Ŀ�꺯��ֵ
* @param obj2 ����һ�ֵ������Ŀ�꺯��ֵ
* @return �����жϽ��
* @retval
		  - 1 ����
		  - 0 ������
*/
bool IsConverge(double obj1, double obj2) {
	cout << "------------------delta_obj=" << obj1 - obj2 << "------------------" << endl;
	if (abs(obj1 - obj2) < 0.1)
		return 1;
	else
		return 0;
}


/**
* @brief ����Ŀ�꺯��ֵ
* @param network ��ǰ����ĸ���
* @param delta ���ֲ���
* @return Ŀ�꺯��ֵ
*/
double CalculateObj(Network network, double delta = 1e1) {
	set<string> next, all_nodes;
	map<string, map<string, double>> parm;
	double obj = 0, flow1, flow2;
	int n;
	all_nodes = network.get_all_nodes();
	for (set<string>::iterator id_1 = all_nodes.begin(); id_1 != all_nodes.end(); id_1++) {
		next = network.get_node(*id_1).get_next();
		for (set<string>::iterator id_2 = next.begin(); id_2 != next.end(); id_2++) {
			n = (int)(network.get_node(*id_1).get_cost(*id_2) / delta);
			// cout << n << endl;
			parm = network.get_node(*id_1).get_cost_parm();
			for (int i = 1; i < n; i++) {
				flow1 = delta * (i - 1);
				flow2 = delta * i;
				obj += (parm[*id_2]["t0"] * (1 + ALPHA * pow(flow1 / parm[*id_2]["c"], BETA)) + parm[*id_2]["t0"] * (1 + ALPHA * pow(flow2 / parm[*id_2]["c"], BETA))) / 2 * delta;
			}
		}
	}
	return obj;
}


/**
* @brief Frank Wolfe �㷨
* @param network ��ǰ���������
*/
void FrankWolfe(Network& network, string criteria) {

	if (criteria == "flow") {
		map<string, map<string, double>> flow, new_flow;
		int i = 0;
		cout << "------------------Initialization------------------" << endl;
		Initialization(network); // ��ʼ��
		do {
			Update(network); // ���»���
			flow = network.get_flow(); // ��¼��һ�ֵ������

			cout << "------------------flow------------------" << endl;
			for (auto i : flow)
				for (auto j : i.second)
					if (j.second != 0)
						cout << i.first << " --> " << j.first << " is " << round(j.second) << endl;
			Node node;
			cout << "------------------cost------------------" << endl;
			for (auto i : network.get_all_nodes()) {
				node = network.get_node(i);
				for (auto j : node.get_next())
					cout << "Cost:" << i << " --> " << j << " is " << node.get_cost(j) << endl;
			}
			cout << "------------------iter = " << ++i << "------------------" << endl;

			new_flow = NetworkLoading(network); // ���·��䲢��¼
		} while (!IsConverge(network.get_all_nodes(), flow, new_flow)); // �����ж�
	}
	else if (criteria == "obj") { // ��Ҫ�෽��Ȩ�⣨�����ٶȣ����׼ȷ�ԣ�Ŀ�꺯��ֵ�����ԣ�
		map<string, map<string, double>> flow, new_flow;
		double obj1, obj2;
		// Network pre_network;
		int i = 0;
		cout << "------------------Initialization------------------" << endl;
		Initialization(network); // ��ʼ��
		Update(network); // ���»���
		obj2 = CalculateObj(network);

		do {
			obj1 = obj2;
			flow = network.get_flow(); // ��¼��һ�ֵ������

			cout << "------------------flow------------------" << endl;
			for (auto i : flow)
				for (auto j : i.second)
					if (j.second != 0)
						cout << i.first << " --> " << j.first << " is " << round(j.second) << endl;
			Node node;
			cout << "------------------cost------------------" << endl;
			for (auto i : network.get_all_nodes()) {
				node = network.get_node(i);
				for (auto j : node.get_next())
					cout << "Cost:" << i << " --> " << j << " is " << node.get_cost(j) << endl;
			}
			cout << "------------------iter = " << ++i << "------------------" << endl;

			new_flow = NetworkLoading(network); // ���·��䲢��¼

			Update(network); // ���»���
			obj2 = CalculateObj(network);

		} while (!IsConverge(obj1, obj2)); // �����ж�
	}
}