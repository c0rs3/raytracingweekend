#pragma once
#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <fstream>

class Logger {
private:
	std::string file_name;
	std::ofstream stream;
public:
	Logger(std::string file_name) :file_name(file_name), stream(file_name) {
		if (!stream.is_open()) {
			return;
		}
	}

	~Logger() {
		stream << "===============LOG END===============" << "\n";
		stream.close();
	}

	template <typename T, typename... Args>
	void log(const T& first, const Args&... args) {
		stream << first;
		(void)std::initializer_list<int>{(stream << ' ' << args, 0)...};
		stream << "\n";
		stream.flush();
	}


	void clear() {
		stream.close();
		stream.open(file_name, std::ios::trunc);
	}
};
#endif