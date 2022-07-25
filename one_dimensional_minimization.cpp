/*****************************************************************************
*  @file     one_dimensional_minimization.cpp                                *
*  @brief    һά���Ż����������                                            *
*  @details  Ŀǰ��ʵ�������� Frank Wolfe �㷨�Ķ��ַ�                       *
*  @author   Dong Yu                                                         *
*  @email    213191838@seu.edu.cn                                            *
*  @version  1.5                                                             *
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
double BisectionMethod(set<string> all_nodes, map<string, map<string, map<string, double>>> cost, map<string, map<string, double>> xn, map<string, map<string, double>> yn) {
	double alpha = 0, a = 0, b = 1, obj;
	map<string, map<string, double>> flow = xn;

	while (abs(b - a) >= 1e-4) {
		obj = 0;
		alpha = (a + b) / 2;

		// ���� alpha �µ����� xn+1
		for (auto i : flow)
			for (auto j : i.second)
				flow[i.first][j.first] = (double)(xn[i.first][j.first] + alpha * (yn[i.first][j.first] - xn[i.first][j.first]));

		// ����Ŀ�꺯���� alpha ���ĵ���
		for (set<string>::iterator origin = all_nodes.begin(); origin != all_nodes.end(); origin++)
			for (auto i : cost[*origin])
				// obj += (yn[*origin][i.first] - xn[*origin][i.first]) * (i.second["c"] + i.second["b"] * flow[*origin][i.first] + i.second["a"] * flow[*origin][i.first] * flow[*origin][i.first]);
				obj += (yn[*origin][i.first] - xn[*origin][i.first]) * i.second["t0"] * (1 + ALPHA * pow(flow[*origin][i.first] / i.second["c"], BETA));

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