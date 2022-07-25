/*****************************************************************************
*  @file     one_dimensional_minimization.cpp                                *
*  @brief    һά���Ż����������                                            *
*  @details  Ŀǰ��ʵ�������� Frank Wolfe �㷨�Ķ��ַ�                       *
*  @author   Dong Yu                                                         *
*  @email    213191838@seu.edu.cn                                            *
*  @version  1.6                                                             *
*  @date     2022/07/25                                                      *
*                                                                            *
*----------------------------------------------------------------------------*
*  Change History :                                                          *
*  <Date>     | <Version> | <Author>       | <Description>                   *
*----------------------------------------------------------------------------*
*  2022/06/03 | 1.0       | Dong Yu        | Create File                     *
*----------------------------------------------------------------------------*
*  2022/06/05 | 1.1       | Dong Yu        | Update Comment                  *
*----------------------------------------------------------------------------*
*  2022/07/02 | 1.2       | Dong Yu        | Change Flow Type to Double      *
*----------------------------------------------------------------------------*
*  2022/07/03 | 1.3       | Dong Yu        | Change Performance-function     *
*----------------------------------------------------------------------------*
*  2022/07/03 | 1.4       | Dong Yu        | Change Step Type to Double      *
*----------------------------------------------------------------------------*
*  2022/07/25 | 1.5       | Dong Yu        | Modify flow Initialize method   *
*----------------------------------------------------------------------------*
*  2022/07/25 | 1.6       | Dong Yu        | Modify cost handling method     *
*----------------------------------------------------------------------------*
*                                                                            *
*****************************************************************************/

#include "one_dimensional_minimization.h"
#include "node.h"
#include<stdlib.h>


/**
* @brief ���� Frank Wolfe �Ķ��ַ�
* @param all_nodes ���������еĽڵ���
* @param cost ���統ǰ���� link �� performance function
* @return alpha
* @retval alpha \in [0, 1]
* @see xn+1 = xn + \alpha * (yn - xn)
*/
double BisectionMethod(Network network, map<string, map<string, double>> xn, map<string, map<string, double>> yn) {
	double alpha = 0, a = 0, b = 1, obj;
	map<string, map<string, double>> flow = xn;
	set<string> all_nodes = network.get_all_nodes();
	set<string> next;
	Node node;

	while (abs(b - a) >= 1e-4) {
		obj = 0;
		alpha = (a + b) / 2;

		// ���� alpha �µ����� xn+1
		for (auto i : flow)
			for (auto j : i.second)
				flow[i.first][j.first] = (double)(xn[i.first][j.first] + alpha * (yn[i.first][j.first] - xn[i.first][j.first]));

		// ����Ŀ�꺯���� alpha ���ĵ���
		for (set<string>::iterator id_1 = all_nodes.begin(); id_1 != all_nodes.end(); id_1++) {
			node = network.get_node(*id_1);
			for (auto id_2 : node.get_next())
				// obj += (yn[*origin][i.first] - xn[*origin][i.first]) * (i.second["c"] + i.second["b"] * flow[*origin][i.first] + i.second["a"] * flow[*origin][i.first] * flow[*origin][i.first]);
				// obj += (yn[*origin][i.first] - xn[*origin][i.first]) * i.second["t0"] * (1 + ALPHA * pow(flow[*origin][i.first] / i.second["c"], BETA));
				obj += (yn[*id_1][id_2] - xn[*id_1][id_2]) * node.CalculateCost(id_2, flow[*id_1][id_2]);
		}

		// ���ݵ����ķ�����С����
		if (obj > 0)
			b = alpha;
		else if (obj == 0)
			return alpha;
		else
			a = alpha;
	}
	return alpha;
}