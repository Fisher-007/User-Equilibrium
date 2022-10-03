/*****************************************************************************
*  @file     main.cpp                                                        *
*  @brief    主程序                                                          *
*  @details  使用时，需要先新建一个 Network 对象，并完成初始化               *
*            初始化后即 可对Network对象 调用 Frank Wolfe 函数求解 UE         *
*            求解完成后可调用 Network 对象的 get_flow() 成员函数获取分配结果 *
*            具体可以参考下面小网络的例子，更详细内容需查看对应的.h与.cpp文件*
*  @author   Dong Yu                                                         *
*  @email    213191838@seu.edu.cn                                            *
*  @version  5.0                                                             *
*  @date     2022/08/18                                                      *
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
*  2022/07/25 | 4.0       | Dong Yu        | Test on Anaheim Network(T5)     *
*----------------------------------------------------------------------------*
*  2022/08/18 | 5.0       | Dong Yu        | Test for MPI                    *
*----------------------------------------------------------------------------*
*                                                                            *
*****************************************************************************/

#include "network.h"
#include "message.h"
#include "shortest_path.h"
#include "user_equilibrium_solver.h"
#include "read_file.h"
#include <mpi.h>
#include <time.h>
#include <iostream>
using namespace std;


int myid, numprocs;


inline void PrintResult(const Network& network) {
	cout << "------------------restult------------------" << endl;
	map<string, map<string, double>> flow = network.get_flow();
	Node node;

	for (auto i : network.get_all_nodes()) {
		node = network.get_node(i);
		for (auto j : node.get_next())
			cout << i << " --> " << j << " is " << flow[i][j] << endl;
	}

	for (auto i : network.get_all_nodes()) {
		node = network.get_node(i);
		for (auto j : node.get_next())
			cout << "Cost:" << i << " --> " << j << " is " << node.get_cost(j) << endl;
	}
}


int main(int argc, char* argv[]) {

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

	time_t time_start, time_end;
	time(&time_start);

	Network big_network = Network();
	string network = "./data/Anaheim_net.tntp", od = "./data/Anaheim_trips.tntp";
	big_network.Init(network, od, "tntp");

	MPI_Barrier(MPI_COMM_WORLD);
	FrankWolfe(big_network, "obj");

	if (myid == 0) {
		PrintResult(big_network);
		time(&time_end);
		cout << "Total time used: " << time_end - time_start << endl;
	}

	MPI_Finalize();
}