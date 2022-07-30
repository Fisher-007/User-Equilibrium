/*****************************************************************************
*  @file     read_file.h                                                     *
*  @brief    文件读取类                                                      *
*  @details  用于读取路网信息、路阻信息和 OD 数据                            *
*  @author   Dong Yu                                                         *
*  @email    213191838@seu.edu.cn                                            *
*  @version  2.0                                                             *
*  @date     2022/07/30                                                      *
*                                                                            *
*----------------------------------------------------------------------------*
*  Change History :                                                          *
*  <Date>     | <Version> | <Author>       | <Description>                   *
*----------------------------------------------------------------------------*
*  2022/07/22 | 1.0       | Dong Yu        | Create File                     *
*----------------------------------------------------------------------------*
*  2022/07/23 | 1.1       | Dong Yu        | Update Comment                  *
*----------------------------------------------------------------------------*
*  2022/07/30 | 2.0       | Dong Yu        | Code optimization               *
*----------------------------------------------------------------------------*
*                                                                            *
*****************************************************************************/
#pragma once
#ifndef READFILE_H
#define READFILE_H


#include <set>
#include <string>
#include <map>
using namespace std;


class ReadFile {

private:
	string network_file; // 网络文件地址
	string od_file;      // OD文件地址
	set<string> all_nodes;                                       // 储存网络中所有点节点
	map<string, set<string>> next_nodes;                         // 储存每一个节点的相邻节点
	map<string, map<string, double>> od_matrix;                  // 储存OD数据
	map<string, map<string, map<string, double>>> cost_parm;     // 储存每一个link的相关参数

public:
	ReadFile(const string& network, const string& od);
	// 通过metadata检查读取的数据结果有无问题
	void CheckData(const int& zones, const int& nodes, const int& links, const int& first_thru_node, const double& total_flow);
	// 将line中信息转化为网络数据并进行储存
	void set_network(const string& line);
	set<string> get_all_nodes();
	map<string, set<string>> get_next_nodes();
	map<string, map<string, double>> get_od_matrix();
	map<string, map<string, map<string, double>>> get_cost_parm();
};

#endif