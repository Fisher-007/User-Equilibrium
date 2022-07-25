/*****************************************************************************
*  @file     message.h                                                       *
*  @brief    ��Ϣ������                                                      *
*  @details  ���ڴ�ӡ�������й�������Ҫչʾ����Ϣ����������ͱ���            *
*  @author   Dong Yu                                                         *
*  @email    213191838@seu.edu.cn                                            *
*  @version  1.2                                                             *
*  @date     2022/07/25                                                      *
*                                                                            *
*----------------------------------------------------------------------------*
*  Change History :                                                          *
*  <Date>     | <Version> | <Author>       | <Description>                   *
*----------------------------------------------------------------------------*
*  2022/07/23 | 1.0       | Dong Yu        | Create File                     *
*----------------------------------------------------------------------------*
*  2022/07/23 | 1.1       | Dong Yu        | Update Comment                  *
*----------------------------------------------------------------------------*
*  2022/07/25 | 1.2       | Dong Yu        | Add iter message print          *
*----------------------------------------------------------------------------*
*                                                                            *
*****************************************************************************/
#pragma once
#ifndef MESSAGE_H
#define MESSAGE_H


#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

// ��ӡһ��������Ϣ
void StatusMessage(string message);
// ��ӡһ��������Ϣ
void StatusIter(int num, int len = 3, string fill = "0");
// ��ӡ�¼����ǰ��Ϣ������ĩβ��...
void StatusMessageB(string message);
// ��ӡ�¼���ɺ���Ϣ����������ҪĬ��ΪSucceed��
void StatusMessageA(string message = "");
// ��ӡ������Ϣ
void Warning(string message);
// ��ӡError��Ϣ���˳�
void ExitMessage(string message);


#endif