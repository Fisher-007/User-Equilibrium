/*****************************************************************************
*  @file     shortest_path.h                                                 *
*  @brief    ���·��                                                        *
*  @details  ����������·                                                  *
*  @author   Dong Yu                                                         *
*  @email    213191838@seu.edu.cn                                            *
*  @version  1.2                                                             *
*  @date     2022/07/30                                                      *
*                                                                            *
*----------------------------------------------------------------------------*
*  Change History :                                                          *
*  <Date>     | <Version> | <Author>       | <Description>                   *
*----------------------------------------------------------------------------*
*  2022/06/02 | 1.0       | Dong Yu        | Create file                     *
*----------------------------------------------------------------------------*
*  2022/06/05 | 1.1       | Dong Yu        | Update Comment                  *
*----------------------------------------------------------------------------*
*  2022/07/30 | 1.2       | Dong Yu        | Code optimization               *
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
map<string, vector<string>> GetShortestPath(const string& origin, const Network& network);


#endif

