/*****************************************************************************
*  @file     user_equilibrium_solver.h                                       *
*  @brief    �û����������                                                  *
*  @details  Ŀǰ��ʵ���� Frank Wolfe �㷨���                               *
*  @author   Dong Yu                                                         *
*  @email    213191838@seu.edu.cn                                            *
*  @version  2.0                                                             *
*  @date     2022/08/18                                                      *
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
*  2022/07/30 | 1.6       | Dong Yu        | Code optimization               *
*----------------------------------------------------------------------------*
*  2022/08/18 | 2.0       | Dong Yu        | Modified for MPI                *
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


extern int myid, numprocs;


// ȫ��ȫ�޷���
map<string, map<string, double>> AllOrNothingAssignment(const Network& network);

// ʹ�� Frank Wolfe �㷨��� UE
void FrankWolfe(Network& network, const string& criteria = "flow");


#endif

