/*****************************************************************************
*  @file     shortest_path.h                                                 *
*  @brief    ���·��                                                        *
*  @details  ����������·                                                  *
*  @author   Dong Yu                                                         *
*  @email    213191838@seu.edu.cn                                            *
*  @version  1.1                                                             *
*  @date     2022/06/05                                                      *
*                                                                            *
*----------------------------------------------------------------------------*
*  Change History :                                                          *
*  <Date>     | <Version> | <Author>       | <Description>                   *
*----------------------------------------------------------------------------*
*  2022/06/02 | 1.0       | Dong Yu        | Create file                     *
*----------------------------------------------------------------------------*
*  2022/06/05 | 1.1       | Dong Yu        | Update Comment                  *
*----------------------------------------------------------------------------*
*                                                                            *
*****************************************************************************/

#pragma once
#ifndef SORTEST_PATH
#define SORTEST_PATH

#include "network.h"
#include <vector>
#include <string>
using namespace std;


// ʹ�� Dijkstra �㷨������ Network �� origin ������ destinations ����� path
map<string, vector<string>> GetShortestPath(string origin, const Network& network);


#endif

