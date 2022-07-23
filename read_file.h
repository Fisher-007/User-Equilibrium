/*****************************************************************************
*  @file     read_file.h                                                     *
*  @brief    �ļ���ȡ��                                                      *
*  @details  ���ڶ�ȡ·����Ϣ��·����Ϣ�� OD ����                            *
*  @author   Dong Yu                                                         *
*  @email    213191838@seu.edu.cn                                            *
*  @version  1.1                                                             *
*  @date     2022/07/23                                                      *
*                                                                            *
*----------------------------------------------------------------------------*
*  Change History :                                                          *
*  <Date>     | <Version> | <Author>       | <Description>                   *
*----------------------------------------------------------------------------*
*  2022/07/22 | 1.0       | Dong Yu        | Create File                     *
*----------------------------------------------------------------------------*
*  2022/07/23 | 1.1       | Dong Yu        | Update Comment                  *
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
	string network_file; // �����ļ���ַ
	string od_file;      // OD�ļ���ַ
	set<string> all_nodes;                                       // �������������е�ڵ�
	map<string, set<string>> next_nodes;                         // ����ÿһ���ڵ�����ڽڵ�
	map<string, map<string, double>> od_matrix;                  // ����OD����
	map<string, map<string, map<string, double>>> cost_parm;     // ����ÿһ��link����ز���

public:
	ReadFile(string network, string od);
	// ͨ��metadata����ȡ�����ݽ����������
	void CheckData(int zones, int nodes, int links, int first_thru_node, double total_flow);
	// ��line����Ϣת��Ϊ�������ݲ����д���
	void set_network(string line);
	set<string> get_all_nodes();
	map<string, set<string>> get_next_nodes();
	map<string, map<string, double>> get_od_matrix();
	map<string, map<string, map<string, double>>> get_cost_parm();
};

#endif