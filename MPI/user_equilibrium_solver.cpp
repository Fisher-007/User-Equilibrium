/*****************************************************************************
*  @file     user_equilibrium_solver.cpp                                     *
*  @brief    �û����������                                                  *
*  @details  Ŀǰ��ʵ���� Frank Wolfe �㷨���                               *
*  @author   Dong Yu                                                         *
*  @email    213191838@seu.edu.cn                                            *
*  @version  3.0                                                             *
*  @date     2022/08/18                                                      *
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
*  2022/08/18 | 3.0       | Dong Yu        | Modified for MPI                *
*----------------------------------------------------------------------------*
*                                                                            *
*****************************************************************************/

#include "message.h"
#include "user_equilibrium_solver.h"
#include "one_dimensional_minimization.h"
#include "shortest_path.h"
#include <stdlib.h>
#include <mpi.h>
#include <iostream>


/**
* @brief ȫ��ȫ�޷���
* @param network ��ǰ����ĸ���
* @return ���ط����Ľ��
* @retval ����ֵΪ link ��������
*/
map<string, map<string, double>> AllOrNothingAssignment(const Network& network) {

	map<string, map<string, double>> od_matrix = network.get_od_matrix();
	map<string, map<string, double>> _flow = network.get_flow(), flow;
	map<string, vector<string>> shortest_path;
	vector<string> path, origin;
	set<string> all_nodes = network.get_all_nodes();

	// ��ʼ�� link ����Ϊ 0
	for (auto i : _flow)
		for (auto j : i.second) {
			_flow[i.first][j.first] = 0;
			flow[i.first][j.first] = 0;
		}

	// ��ȡorigin
	for (auto i : od_matrix)
		origin.push_back(i.first);

	// ��������
	for (int o = myid; o < origin.size(); o += numprocs) {

		// ���� origin ������ destination �����·��link���У�
		shortest_path = GetShortestPath(origin[o], network);

		for (auto destination : od_matrix[origin[o]]) {
			path = shortest_path[destination.first];

			// �����г��Ȳ�Ϊ 1��origin����Ҳ�������У�ʱ���� origin-destination ������ȫ�����ص� path �е�ÿһ�� link ��
			if (path.size() > 1)
				for (int i = 1; i < path.size(); i++)
					_flow[path[i]][path[i - 1]] += od_matrix[origin[o]][destination.first];
		}
	}
	for (auto i : _flow)
		for (auto j : i.second)
			MPI_Allreduce(&_flow[i.first][j.first], &flow[i.first][j.first], 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

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
	map<string, map<string, double>> od_matrix = network.get_od_matrix();

	// ��¼���������� link �� performance function
	for (set<string>::iterator id = all_nodes.begin(); id != all_nodes.end(); id++)
		cost[*id] = network.get_node(*id).get_cost_parm();

	xn = network.get_flow();
	yn = AllOrNothingAssignment(network);

	// ���ַ��������Ų��� \alpha
	double alpha = BisectionMethod(network, xn, yn);

	// ����������º������
	for (auto i : xn)
		for (auto j : i.second)
			new_flow[i.first][j.first] = (double)(xn[i.first][j.first] + alpha * (yn[i.first][j.first] - xn[i.first][j.first]));
	
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
bool IsConverge(const set<string>& all_nodes, const map<string, map<string, double>>& flow, const map<string, map<string, double>>& new_flow) {
	int count = 0;

	// �������ε���������ܱ仯��
	for (auto i : flow)
		for (auto j : i.second)
			if (abs(new_flow.at(i.first).at(j.first) - flow.at(i.first).at(j.first)) >= 1)
				count += abs(new_flow.at(i.first).at(j.first) - flow.at(i.first).at(j.first));

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
bool IsConverge(const long double& obj1, const long double& obj2) {
	if (abs(obj1 - obj2) / obj1 < 0.0001)
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
long double CalculateObj(const Network& network, double delta = 10) {
	map<string, map<string, double>> parm;
	set<string> next, nodes = network.get_all_nodes();
	vector<string> all_nodes;
	all_nodes.assign(nodes.begin(), nodes.end());
	map<string, map<string, double>> flows = network.get_flow();
	long double obj = 0;
	double flow, f1, f2;
	int n;
	for (int i = myid; i < all_nodes.size(); i += numprocs) {
		parm = network.get_node(all_nodes[i]).get_cost_parm();
		next = network.get_node(all_nodes[i]).get_next();
		for (set<string>::iterator id_2 = next.begin(); id_2 != next.end(); id_2++) {
			n = (int)(flows[all_nodes[i]][*id_2] / delta);
			f1 = CalculateCost(parm, *id_2, 0);
			for (int i = 1; i < n; i++) {
				flow = delta * i;
				f2 = CalculateCost(parm, *id_2, flow);
				obj += (f1 + f2) * delta;
				f1 = f2;
			}
		}
	}
	return obj / 2;
}


/**
* @brief Frank Wolfe �㷨
* @param network ��ǰ���������
*/
void FrankWolfe(Network& network, const string& criteria) {

	if (criteria == "flow") {
		map<string, map<string, double>> flow, new_flow;
		int i = 0;

		StatusMessageB("Initialization");
		Initialization(network);           // ��ʼ��
		StatusMessageA();

		do {
			StatusIter(++i);

			StatusMessageB("Update");
			Update(network); // ���»���

			flow = network.get_flow(); // ��¼��һ�ֵ������
			StatusMessageB("Network Loading");
			new_flow = NetworkLoading(network); // ���·��䲢��¼

			StatusMessageA();

		} while (!IsConverge(network.get_all_nodes(), flow, new_flow)); // �����ж�
	}
	else if (criteria == "obj") { // ��Ҫ�෽��Ȩ�⣨�����ٶȣ����׼ȷ�ԣ�Ŀ�꺯��ֵ�����ԣ�
		map<string, map<string, double>> flow, new_flow;
		long double obj, obj1, obj2;
		// Network pre_network;
		int i = 0;
		bool state = true;

		StatusMessageB("Initialization");
		Initialization(network);           // ��ʼ��
		StatusMessageA();

		Update(network); // ���»���
		obj = CalculateObj(network);
		MPI_Reduce(&obj, &obj2, 1, MPI_LONG_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

		do {
			StatusIter(++i);
			if (myid == 0)
				obj1 = obj2;

			StatusMessageB("Network Loading");
			NetworkLoading(network);        // ���·��䲢��¼

			StatusMessageB("Update");
			Update(network);                // ���»���

			obj = CalculateObj(network);
			MPI_Reduce(&obj, &obj2, 1, MPI_LONG_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

			if (myid == 0) {
				StatusMessageA("delta = " + to_string((obj1 - obj2) / obj1));

				if (IsConverge(obj1, obj2))
					state = false;
			}

			MPI_Bcast(&state, 1, MPI_C_BOOL, 0, MPI_COMM_WORLD);
		} while (state); // �����ж�
	}
}