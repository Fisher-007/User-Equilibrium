/*****************************************************************************
*  @file     message.h                                                       *
*  @brief    ��Ϣ������                                                      *
*  @details  ���ڴ�ӡ�������й�������Ҫչʾ����Ϣ����������ͱ���            *
*  @author   Dong Yu                                                         *
*  @email    213191838@seu.edu.cn                                            *
*  @version  1.1                                                             *
*  @date     2022/07/23                                                      *
*                                                                            *
*----------------------------------------------------------------------------*
*  Change History :                                                          *
*  <Date>     | <Version> | <Author>       | <Description>                   *
*----------------------------------------------------------------------------*
*  2022/07/23 | 1.0       | Dong Yu        | Create File                     *
*----------------------------------------------------------------------------*
*  2022/07/23 | 1.1       | Dong Yu        | Update Comment                  *
*----------------------------------------------------------------------------*
*                                                                            *
*****************************************************************************/
#include <iostream>
#include <string>
using namespace std;

// ��ӡһ��������Ϣ
void StatusMessage(string message);
// ��ӡ�¼����ǰ��Ϣ������ĩβ��...
void StatusMessageB(string message);
// ��ӡ�¼���ɺ���Ϣ����������ҪĬ��ΪSucceed��
void StatusMessageA(string message = "");
// ��ӡ������Ϣ
void Warning(string message);
// ��ӡError��Ϣ���˳�
void ExitMessage(string message);