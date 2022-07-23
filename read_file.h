/*****************************************************************************
*  @file     read_file.h                                                     *
*  @brief    文件读取类                                                      *
*  @details  用于读取路网信息、路阻信息和 OD 数据                            *
*  @author   Dong Yu                                                         *
*  @email    213191838@seu.edu.cn                                            *
*  @version  1.0                                                             *
*  @date     2022/07/22                                                      *
*                                                                            *
*----------------------------------------------------------------------------*
*  Change History :                                                          *
*  <Date>     | <Version> | <Author>       | <Description>                   *
*----------------------------------------------------------------------------*
*  2022/07/22 | 1.0       | Dong Yu        | Create File                     *
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
	string network_file;
	string od_file;
	set<string> all_nodes;
	map<string, set<string>> next_nodes;
	map<string, map<string, double>> od_matrix;
	map<string, map<string, map<string, double>>> cost_parm;

public:
	ReadFile(string network, string od);
	void CheckData(int zones, int nodes, int links, int first_thru_node, double total_flow);
	void set_network(string line);
	set<string> get_all_nodes();
	map<string, set<string>> get_next_nodes();
	map<string, map<string, double>> get_od_matrix();
	map<string, map<string, map<string, double>>> get_cost_parm();
};

#endif