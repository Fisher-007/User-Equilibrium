/*****************************************************************************
*  @file     user_equilibrium_solver.h                                       *
*  @brief    �û����������                                                  *
*  @details  Ŀǰ��ʵ���� Frank Wolfe �㷨���                               *
*  @author   Dong Yu                                                         *
*  @email    213191838@seu.edu.cn                                            *
*  @version  1.1                                                             *
*  @date     2022/06/05                                                      *
*                                                                            *
*----------------------------------------------------------------------------*
*  Change History :                                                          *
*  <Date>     | <Version> | <Author>       | <Description>                   *
*----------------------------------------------------------------------------*
*  2022/06/02 | 1.0       | Dong Yu        | Create File                     *
*----------------------------------------------------------------------------*
*  2022/06/05 | 1.1       | Dong Yu        | Update Comment                  *
*----------------------------------------------------------------------------*
*  2022/07/02 | 1.2       | Dong Yu        | Change Flow Type to Double      *
*----------------------------------------------------------------------------*
*  2022/07/04 | 1.5       | Dong Yu        | Add a New Convergence Judgment  *
*----------------------------------------------------------------------------*
*                                                                            *
*****************************************************************************/

#pragma once
#ifndef UE_EQUILIBRIUM_SOLVER
#define UE_EQUILIBRIUM_SOLVER

#include "network.h"
#include <vector>
#include <set>
#include <map>
#include <string>
using namespace std;


// ȫ��ȫ�޷���
map<string, map<string, double>> AllOrNothingAssignment(const Network& network);

// ʹ�� Frank Wolfe �㷨��� UE
void FrankWolfe(Network& network, string criteria = "flow");


#endif

