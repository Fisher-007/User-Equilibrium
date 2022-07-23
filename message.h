/*****************************************************************************
*  @file     message.h                                                       *
*  @brief    信息处理类                                                      *
*  @details  用于打印程序运行过程中需要展示的信息，包括警告和报错            *
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

// 打印一般文字信息
void StatusMessage(string message);
// 打印事件完成前信息，文字末尾加...
void StatusMessageB(string message);
// 打印事件完成后信息，无特殊需要默认为Succeed！
void StatusMessageA(string message = "");
// 打印警告信息
void Warning(string message);
// 打印Error信息并退出
void ExitMessage(string message);