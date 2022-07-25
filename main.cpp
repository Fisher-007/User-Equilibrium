/*****************************************************************************
*  @file     main.cpp                                                        *
*  @brief    ������                                                          *
*  @details  ʹ��ʱ����Ҫ���½�һ�� Network ���󣬲���ɳ�ʼ��               *
*            ��ʼ���� �ɶ�Network���� ���� Frank Wolfe ������� UE         *
*            �����ɺ�ɵ��� Network ����� get_flow() ��Ա������ȡ������ *
*            ������Բο�����С��������ӣ�����ϸ������鿴��Ӧ��.h��.cpp�ļ�*
*  @author   Dong Yu                                                         *
*  @email    213191838@seu.edu.cn                                            *
*  @version  4.0                                                             *
*  @date     2022/07/23                                                      *
*                                                                            *
*----------------------------------------------------------------------------*
*  Change History :                                                          *
*  <Date>     | <Version> | <Author>       | <Description>                   *
*----------------------------------------------------------------------------*
*  2022/05/31 | 1.0       | Dong Yu        | Create File                     *
*----------------------------------------------------------------------------*
*  2022/06/04 | 2.0       | Dong Yu        | Test the Full Program(T1)       *
*----------------------------------------------------------------------------*
*  2022/06/05 | 2.1       | Dong Yu        | Update Comment                  *
*----------------------------------------------------------------------------*
*  2022/07/02 | 2.2       | Dong Yu        | Change Flow Type to Double(T2)  *
*----------------------------------------------------------------------------*
*  2022/07/03 | 2.3       | Dong Yu        | Test 3                          *
*----------------------------------------------------------------------------*
*  2022/07/03 | 3.0       | Dong Yu        | Test on Sioux Falls Network(T4) *
*----------------------------------------------------------------------------*
*  2022/07/23 | 3.1       | Dong Yu        | Test the ReadFile Class         *
*----------------------------------------------------------------------------*
*  2022/07/23 | 4.0       | Dong Yu        | Test on Anaheim Network(T5)     *
*----------------------------------------------------------------------------*
*                                                                            *
*****************************************************************************/

#include "network.h"
#include "shortest_path.h"
#include "user_equilibrium_solver.h"
#include "read_file.h"
#include <iostream>
using namespace std;

int main() {

	// ������ģ�����
	//Network myNetwork = Network();
	//myNetwork.Init();
	//cout << myNetwork.get_node("1").get_cost("3") << endl;
	//cout << myNetwork.get_flow()["1"]["3"] << endl;

	//map<string, vector<string>> result = GetShortestPath("1", myNetwork);
	//for (auto i : result["20"])
	//{
	//	cout << i << endl;
	//}

	//map<string, map<string, int>> flow = AllOrNothingAssignment(myNetwork);
	//cout << flow["1"]["3"] << endl;

	//FrankWolfe(myNetwork);
	//flow = myNetwork.get_flow();
	//cout << flow["1"]["3"] << endl;

	// С������Խ����ȷ��
	Network small_network = Network();
	string network = "./data/tri_link_test3.csv", od = "./data/tri_od_test3.csv";
	small_network.Init(network, od);
	FrankWolfe(small_network);
	cout << "------------------restult------------------" << endl;
	map<string, map<string, double>> flow = small_network.get_flow();
	for (auto i : flow)
		for (auto j : i.second)
			cout << i.first << " --> " << j.first << " is " << round(j.second) << endl;
	Node node;
	for (auto i : small_network.get_all_nodes()) {
		node = small_network.get_node(i);
		for (auto j : node.get_next())
			cout << "Cost:" << i << " --> " << j << " is " << node.get_cost(j) << endl;
	}

	// ���� 1�����繲һ��OD������·�������Խ�����£�
	// 1->2->5������ 1947������ 1098.8
	// 1->3->5������ 26������ 1132
	// 1->4->5������ 586������ 1099
	// �����������غ㣬��������������int����

    // ���� 2��int ת��Ϊ double��
	// 1->2->5������ 2292������ 1236.8
	// 1->3->5������ 28������ 1242
	// 1->4->5������ 680������ 1240

	// ���� 3���ı�performance-function��
	// cost����ᵼ�¼������
	// objû�м�ʱ����
	// ����ۻ��������޷���������������������ע����������֮���ת����alpha��
	// 1->2->5������ 3462������ 3185.43
	// 1->3->5������ 2558������ 3191.84
	// 1->4->5������ 1981������ 3188.15

	// �޸�flow�ĳ�ʼ��������ϸ�ں��ٽ��� ���� 3
	// 1->2->5������ 3888������ 569.220
	// 1->3->5������ 2425������ 569.372
	// 1->4->5������ 1688������ 569.055

    // Sioux Falls �������
	//Network big_network = Network();
	//string network = "./data/tri_link.csv", od = "./data/tri_od.csv";
	//big_network.Init(network, od);
	//FrankWolfe(big_network, "obj");
	//cout << "------------------restult------------------" << endl;
	//map<string, map<string, double>> flow = big_network.get_flow();
	//for (auto i : flow)
	//	for (auto j : i.second)
	//		if (j.second != 0)
	//			cout << i.first << " --> " << j.first << " is " << round(j.second) << endl;
	//Node node;
	//for (auto i : big_network.get_all_nodes()) {
	//	node = big_network.get_node(i);
	//	for (auto j : node.get_next())
	//		cout << "Cost:" << i << " --> " << j << " is " << node.get_cost(j) << endl;
	//}

	// ���� 4��

	// ����read_file��
	//ReadFile file("./data/Anaheim_net.tntp", "./data/Anaheim_trips.tntp");
	// ��������0.4��

	// Anaheim �������
	//Network big_network = Network();
	//string network = "./data/Anaheim_net.tntp", od = "./data/Anaheim_trips.tntp";
	//big_network.Init(network, od, "tntp");
	//FrankWolfe(big_network, "obj");
	//cout << "------------------restult------------------" << endl;
	//map<string, map<string, double>> flow = big_network.get_flow();
	//for (auto i : flow)
	//	for (auto j : i.second)
	//		if (j.second != 0)
	//			cout << i.first << " --> " << j.first << " is " << round(j.second) << endl;
	//Node node;
	//for (auto i : big_network.get_all_nodes()) {
	//	node = big_network.get_node(i);
	//	for (auto j : node.get_next())
	//		cout << "Cost:" << i << " --> " << j << " is " << node.get_cost(j) << endl;
	//}

	// ���� 5��
}