/*****************************************************************************
*  @file     read_file.cpp                                                     *
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
#include "read_file.h"
#include "message.h"
#include<fstream>
#include<vector>


void RemoveSpacese(string& line) {
	//line.erase(0, line.find_first_not_of("	"));
	line.erase(line.find_last_not_of("	") + 1);
	//line.erase(0, line.find_first_not_of(" "));
	if (line.find_last_not_of(" ") < line.find_last_of(" "))
	    line.erase(line.find_last_not_of(" ") + 1);
}


void ReadLine(ifstream& file, string& line) {
	getline(file, line);
	RemoveSpacese(line);
}


void SplitMetaData(string line, string& head, int& num) {
	int data = 0, pos, len;
	len = line.length();
	pos = line.find_last_of(" ");
	head = line.substr(0, pos);
	num = stoi(line.substr(pos + 1));
}


map<string, double> GetBuffer(string line) {
	map<string, double> buffer;
	while (line.find(" ") != string::npos)
		line.erase(line.find(" "), 1);
	while (line.find("	") != string::npos)
		line.erase(line.find("	"), 1);
	int pos1 = line.find(":", 0), pos2 = -1;
	do {
		buffer[line.substr(pos2 + 1, pos1 - pos2 - 1)] = stod(line.substr(pos1 + 1, line.find(";", pos2 + 1) - pos1 - 1));
		pos2 = line.find(";", pos2 + 1);
		pos1 = line.find(":", pos1 + 1);
	} while (line.find(":", pos1) != string::npos);
	return buffer;
}


void ReadFile::set_network(string line) {
	int pos_last = line.find("	", 0), pos = line.find("	", pos_last + 1);
	string ori, des;
	vector<string> item = {"init_node", "term_node", "capacity", "length", "free_flow_time", "b", "power", "speed", "toll", "link_type"};

	ori = line.substr(pos_last + 1, pos - pos_last - 1);
	pos_last = line.find("	", pos_last + 1), pos = line.find("	", pos_last + 1);
	des = line.substr(pos_last + 1, pos - pos_last - 1);

	this->all_nodes.insert(ori);
	this->all_nodes.insert(des);
	this->next_nodes[ori].insert(des);
	for (int i = 2; i < 10; i++) {
		pos_last = line.find("	", pos_last + 1), pos = line.find("	", pos_last + 1);
		this->cost_parm[ori][des][item[i]] = stod(line.substr(pos_last + 1, pos - pos_last - 1));
	}
}


void ReadFile::CheckData(int zones, int nodes, int links, int first_thru_node, double total_flow) {
	if (this->od_matrix.size() != zones)
		ExitMessage("Wrong zones!");

	if (this->all_nodes.size() != nodes)
		ExitMessage("Wrong nodes!");

	int _links = 0;
	for (auto i : this->next_nodes)
		_links += i.second.size();
	if (_links != links)
		ExitMessage("Wrong links!");

	double _flow = 0.0;
	for (auto i : this->od_matrix)
		for (auto j : i.second)
			_flow += j.second;
	if ((int)_flow != (int)total_flow)
		ExitMessage("Wrong total flow!");

	// first_thru_node ??
}


ReadFile::ReadFile(string network, string od) {
	StatusMessage("读取文件: ");

	ifstream network_file;
	ifstream od_file;
	network_file.open(network);
	od_file.open(od);

	string line = "";
	string head;
	int num;
	int zones = 0, nodes = 0, links = 0, first_thru_node = 0;
	double total_flow = 0.0;
	
	// 读取meta数据
	StatusMessageB("读取meta数据");
	ReadLine(network_file, line);
	while (line.compare("<END OF METADATA>") != 0) {
		if (line.find("~") != string::npos) {
			ReadLine(network_file, line);
			continue;
		}
		else {
			SplitMetaData(line, head, num);
			if (head == "<NUMBER OF ZONES>")
				zones = num;
			else if (head == "<NUMBER OF NODES>")
				nodes = num;
			else if (head == "<FIRST THRU NODE>")
				first_thru_node = num;
			else if (head == "<NUMBER OF LINKS>")
				links = num;
			ReadLine(network_file, line);
		}
	}

	ReadLine(od_file, line);
	while (line.compare("<END OF METADATA>") != 0) {
		if (line.find("~") != string::npos) {
			ReadLine(od_file, line);
			continue;
		}
		else {
			SplitMetaData(line, head, num);
			if (head == "<NUMBER OF ZONES>") {
				if (zones != num)
					ExitMessage("Wrong in file!");
			}
			else if (head == "<TOTAL OD FLOW>") {
				total_flow = num;
			}
			ReadLine(od_file, line);
		}
	}
	if (zones * nodes * links * first_thru_node * total_flow == 0)
		ExitMessage("Wrong matadata input!");
	StatusMessageA();

	// 读取网络数据
	StatusMessageB("读取网络数据");
	while (!network_file.eof()) {
		ReadLine(network_file, line);
		if (line.empty() || line.find("~") != string::npos)
			continue;
		set_network(line);
	}
	StatusMessageA();

	// 读取OD数据
	StatusMessageB("读取OD数据");
	int origin = 0;
	map<string, double> buffer;
	while (!od_file.eof()) {
		ReadLine(od_file, line);
		if (line.empty())
			continue;
		if (line.find("Origin") != string::npos) {
			origin += 1;
			continue;
		}
		buffer = GetBuffer(line);
		for (auto des : buffer)
			this->od_matrix[to_string(origin)][des.first] = des.second;
	}
	StatusMessageA();

	network_file.close();
	od_file.close();

	CheckData(zones, nodes, links, first_thru_node, total_flow);
	StatusMessage("文件读取成功！");
}


set<string> ReadFile::get_all_nodes() {
	return this->all_nodes;
}


map<string, set<string>> ReadFile::get_next_nodes() {
	return this->next_nodes;
}


map<string, map<string, double>> ReadFile::get_od_matrix() {
	return this->od_matrix;
}


map<string, map<string, map<string, double>>> ReadFile::get_cost_parm() {
	return this->cost_parm;
}