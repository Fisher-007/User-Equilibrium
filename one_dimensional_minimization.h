/*****************************************************************************
*  @file     one_dimensional_minimization.h                                  *
*  @brief    һά���Ż����������                                            *
*  @details  Ŀǰ��ʵ�������� Frank Wolfe �㷨�Ķ��ַ�                       *
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

// ���ַ�(�贫��network���ɸĽ�)
double BisectionMethod(const Network& network, const map<string, map<string, double>>& xn, const map<string, map<string, double>>& yn);

// �����Լ������������ķ�����
//float GoldenSectionMethod();
//float NewtonMethod();


#endif

