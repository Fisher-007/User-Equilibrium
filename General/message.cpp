/*****************************************************************************
*  @file     message.cpp                                                       *
*  @brief    ��Ϣ������                                                      *
*  @details  ���ڴ�ӡ�������й�������Ҫչʾ����Ϣ����������ͱ���            *
*  @author   Dong Yu                                                         *
*  @email    213191838@seu.edu.cn                                            *
*  @version  2.0                                                             *
*  @date     2022/07/30                                                      *
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
*  2022/07/30 | 2.0       | Dong Yu        | Code optimization               *
*----------------------------------------------------------------------------*
*                                                                            *
*****************************************************************************/
#include "message.h"


void StatusMessage(const string& message) {
	// �����ڲ������ʽ������Լ�����չ
	cout << message << endl;
}


void StatusIter(const int& num, const int& len, const string& fill) {
	// cout << "iter = " << setfill(fill) << setw(len) << num << ": ";
	cout << "iter = " << num << ": ";
}


void StatusMessageB(const string& message) {
	cout << message << "...";
}


void StatusMessageA(const string& message) {
	if (message == "")
		cout << "Succeed!" << endl;
	else
		cout << message << endl;
}


void Warning(const string& message) {
	cout << "Warning: " << message << endl;
}


void ExitMessage(const string& message) {
	cout << "Error: " << message << endl;
	exit(EXIT_FAILURE);
}