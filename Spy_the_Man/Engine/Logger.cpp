/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Logger.cpp
Project: CS230
Author: Kevin Wright, Dong-A Choi
Creation date: 2/10/2021, 9/29/2021
-----------------------------------------------------------------*/
#include <iostream>    // cout.rdbuf
#include "Logger.h"

PM::Logger::Logger(Logger::Severity severity, bool useConsole, std::chrono::system_clock::time_point startTime, std::string name) :
	minLevel(severity), outStream(name), startTime(startTime) {
	std::cout << "Hello";
	if (useConsole == true) {
		outStream.set_rdbuf(std::cout.rdbuf());
	}
	std::cout << "Hello2";
}


PM::Logger::~Logger() {
	outStream.flush();
	outStream.close();
}

void PM::Logger::Log(PM::Logger::Severity severity, std::string message) {
	
	//std::ofstream otherStream(fileName, std::ios::app);
	if (outStream.is_open()) {
		if (severity >= minLevel) {
			outStream.precision(4);
			outStream << '[' << std::fixed << GetSecondsSinceStart() << "]\t";

			switch (severity) {
			case Severity::Verbose:
				outStream << "Verb \t";
				break;
			case Severity::Debug:
				outStream << "Debug\t";
				break;
			case Severity::Error:
				outStream << "Error\t";
				break;
			case Severity::Event:
				outStream << "EVent\t";
				break;
			}
			outStream << message;
#ifdef _DEBUG
			outStream << std::endl;
#else
			outStream << '\n';
#endif
		}
	}
}

double PM::Logger::GetSecondsSinceStart() {
	return std::chrono::duration<double>(std::chrono::system_clock::now() - startTime).count();
}