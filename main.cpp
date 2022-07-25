/*****************************************************************************
*  @file     main.cpp                                                        *
*  @brief    主程序                                                          *
*  @details  使用时，需要先新建一个 Network 对象，并完成初始化               *
*            初始化后即 可对Network对象 调用 Frank Wolfe 函数求解 UE         *
*            求解完成后可调用 Network 对象的 get_flow() 成员函数获取分配结果 *
*            具体可以参考下面小网络的例子，更详细内容需查看对应的.h与.cpp文件*
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

	// 部分子模块测试
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

	// 小网络测试结果正确性
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

	// 测试 1：网络共一对OD、三条路径，测试结果如下：
	// 1->2->5：流量 1947；花费 1098.8
	// 1->3->5：流量 26；花费 1132
	// 1->4->5：流量 586；花费 1099
	// ×总流量不守恒，可能是数据类型int导致

    // 测试 2：int 转换为 double：
	// 1->2->5：流量 2292；花费 1236.8
	// 1->3->5：流量 28；花费 1242
	// 1->4->5：流量 680；花费 1240

	// 测试 3：改变performance-function：
	// cost过大会导致计算溢出
	// obj没有及时清零
	// 误差累积，导致无法收敛，或者收敛很慢，注意数据类型之间的转换（alpha）
	// 1->2->5：流量 3462；花费 3185.43
	// 1->3->5：流量 2558；花费 3191.84
	// 1->4->5：流量 1981；花费 3188.15

	// 修改flow的初始化方法等细节后再进行 测试 3
	// 1->2->5：流量 3888；花费 569.220
	// 1->3->5：流量 2425；花费 569.372
	// 1->4->5：流量 1688；花费 569.055

    // Sioux Falls 网络测试
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

	// 测试 4：

	// 测试read_file类
	//ReadFile file("./data/Anaheim_net.tntp", "./data/Anaheim_trips.tntp");
	// 总流量差0.4？

	// Anaheim 网络测试
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

	// 测试 5：
}