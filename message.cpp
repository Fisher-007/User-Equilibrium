#include "message.h"


void StatusMessage(string message) {
	cout << message << endl;
}


void StatusMessageB(string message) {
	cout << message << "...";
}


void StatusMessageA(string message) {
	if (message == "")
		cout << "Succeed!" << endl;
	else
		cout << message << endl;
}


void Warning(string message) {
	cout << "Warning: " << message << endl;
}


void ExitMessage(string message) {
	cout << "Error: " << message << endl;
	exit(EXIT_FAILURE);
}