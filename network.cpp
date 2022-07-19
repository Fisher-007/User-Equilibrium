/*****************************************************************************
*  @file     network.cpp                                                     *
*  @brief    网络类                                                          *
*  @details  以临界目录表的方式存储网络信息                                  *
*            描述网络的数据需要从csv文件中读取，即需要先生成指定格式的csv文件*
*  @author   Dong Yu                                                         *
*  @email    213191838@seu.edu.cn                                            *
*  @version  2.1                                                             *
*  @date     2022/06/05                                                      *
*                                                                            *
*----------------------------------------------------------------------------*
*  Change History :                                                          *
*  <Date>     | <Version> | <Author>       | <Description>                   *
*----------------------------------------------------------------------------*
*  2022/06/01 | 1.0       | Dong Yu        | Create File                     *
*----------------------------------------------------------------------------*
*  2022/06/03 | 2.0       | Dong Yu        | Refine Cost                     *
*----------------------------------------------------------------------------*
*  2022/06/05 | 2.1       | Dong Yu        | Update Comment                  *
*----------------------------------------------------------------------------*
*  2022/07/02 | 2.2       | Dong Yu        | Change Flow Type to Double      *
*----------------------------------------------------------------------------*
*  2022/07/02 | 2.3       | Dong Yu        | Change Performance-function     *
*----------------------------------------------------------------------------*
*                                                                            *
*****************************************************************************/

#include "network.h"


Network::Network(int max_nodes) {
    this->max_nodes = max_nodes;
}

/**
* @brief 新增一个节点
* @param id 新增节点的编号
* @return 返回操作状态
* @retval  
          - 1 新增成功
          - 0 节点已存在
*/
int Network::AddNode(string id) {
    if (!all_nodes.count(id)) {
        this->all_nodes.insert(id);
        this->nodes[id]=Node(id);
        return 1;
    }
    else
        return 0;
}

/**
* @brief 初始化网络（节点、边、performance-function、OD）
* @param network 存储网络节点、边、performance-function 文件（.csv）的地址，缺省默认为 ./data/tri_link.csv
* @param od 存储网络 OD 矩阵文件（.csv）的地址，缺省默认为 ./data/tri_od.csv
* @note 上述两个文件，格式有固定要求
       - network:
                node_id_1 | node_id_2 | t0     | c     
                string    | string    | double | double
       - od（需要记录网络中所有点到所有点的od，没有的记 0 ）:
                origin | destination | flow
                string | string      | double
* @see performance-function: t0 * (1 + alpha * (v / c) ^ beta)
*/
void Network::Init(string network, string od) {
    string line;
    ifstream network_data(network);
    // 读取 network 文件
    while (getline(network_data, line))
    {
        istringstream sin(line);
        // string id_1, id_2, c, b, a;
        string id_1, id_2, t0, c;

        getline(sin, id_1, ',');
        this->all_nodes.insert(id_1);
        AddNode(id_1);

        getline(sin, id_2, ',');
        this->all_nodes.insert(id_2);
        AddNode(id_2);

        //getline(sin, c, ',');
        //getline(sin, b, ',');
        //getline(sin, a, ',');
        //this->nodes[id_1].UpdateNext(id_2, stod(c), stod(b), stod(a));
        getline(sin, t0, ',');
        getline(sin, c, ',');
        this->nodes[id_1].UpdateNext(id_2, stod(t0), stod(c) * 2000);
    }

    ifstream od_data(od);
    // 读取 OD 文件
    while (getline(od_data, line))
    {
        istringstream sin(line);
        string origin, destination, flow;

        getline(sin, origin, ',');
        getline(sin, destination, ',');
        getline(sin, flow, ',');
        this->od_matrix[origin][destination] = stod(flow);
    }

    set<string> next;
    // 初始化 link 流量和当前的花费
    for (set<string>::iterator id_1 = this->all_nodes.begin(); id_1 != this->all_nodes.end(); id_1++) {
        for (set<string>::iterator id_2 = this->all_nodes.begin(); id_2 != this->all_nodes.end(); id_2++)
            this->flow[*id_1][*id_2] = 0;

        next = this->nodes[*id_1].get_next();
        for (set<string>::iterator id = next.begin(); id != next.end(); id++)
            this->nodes[*id_1].UpdateCost(*id, 0);
    }
}

set<string> Network::get_all_nodes() const {
    return this->all_nodes;
}

Node Network::get_node(string id) const {
    return this->nodes.find(id)->second;
}

void Network::set_flow(map<string, map<string, double>> flow) {
    this->flow = flow;
}

map<string, map<string, double>> Network::get_flow() const {
    return this->flow;
}

map<string, map<string, double>> Network::get_od_matrix() const {
    return this->od_matrix;
}

void Network::UpdateCost() {
    set<string> next;
    for (set<string>::iterator id_1 = this->all_nodes.begin(); id_1 != this->all_nodes.end(); id_1++) {
        next = this->nodes[*id_1].get_next();
        for (set<string>::iterator id_2 = next.begin(); id_2 != next.end(); id_2++) {
            this->nodes[*id_1].UpdateCost(*id_2, this->flow[*id_1][*id_2]);
        }
    }
}