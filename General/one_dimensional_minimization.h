/*****************************************************************************
*  @file     one_dimensional_minimization.h                                  *
*  @brief    一维最优化问题求解类                                            *
*  @details  目前仅实现了用于 Frank Wolfe 算法的二分法                       *
*  @author   Dong Yu                                                         *
*  @email    213191838@seu.edu.cn                                            *
*  @version  1.6                                                             *
*  @date     2022/07/30                                                      *
*                                                                            *
*----------------------------------------------------------------------------*
*  Change History :                                                          *
*  <Date>     | <Version> | <Author>       | <Description>                   *
*----------------------------------------------------------------------------*
*  2022/06/03 | 1.0       | Dong Yu        | Create File                     *
*----------------------------------------------------------------------------*
*  2022/06/05 | 1.1       | Dong Yu        | Update Comment                  *
*----------------------------------------------------------------------------*
*  2022/07/02 | 1.2       | Dong Yu        | Change Flow Type to Double      *
*----------------------------------------------------------------------------*
*  2022/07/03 | 1.4       | Dong Yu        | Change Step Type to Double      *
*----------------------------------------------------------------------------*
*  2022/07/25 | 1.5       | Dong Yu        | Modify cost handling method     *
*----------------------------------------------------------------------------*
*  2022/07/30 | 1.6       | Dong Yu        | Code optimization               *
*----------------------------------------------------------------------------*
*                                                                            *
*****************************************************************************/

#pragma once
#ifndef ONE_DIMENSIONAL_MINIMIZATION
#define ONE_DIMENSIONAL_MINIMIZATION

#include "network.h"
#include <vector>
#include <set>
#include <map>
#include <string>
using namespace std;

// 二分法(需传参network，可改进)
double BisectionMethod(const Network& network, const map<string, map<string, double>>& xn, const map<string, map<string, double>>& yn);

// 还可以继续完善其他的方法：
//float GoldenSectionMethod();
//float NewtonMethod();


#endif

