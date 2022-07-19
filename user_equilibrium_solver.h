/*****************************************************************************
*  @file     user_equilibrium_solver.h                                       *
*  @brief    用户均衡求解类                                                  *
*  @details  目前仅实现了 Frank Wolfe 算法求解                               *
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


// 全由全无分配
map<string, map<string, double>> AllOrNothingAssignment(const Network& network);

// 使用 Frank Wolfe 算法求解 UE
void FrankWolfe(Network& network, string criteria = "flow");


#endif

