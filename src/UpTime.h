#pragma once
#include <chrono>
#include <string>

using namespace std::chrono;

typedef duration<int, std::ratio<3600 * 24>> days;

class UpTime{
public:
	static UpTime * Instance();
	static void StartClock();
	std::string getUpTimeStr();
	long int getElapsedMs();
	int getSecField();
	int getMinField();
	int getHourField();
	int getDayField();

private:
	UpTime(){ m_startTime = steady_clock::now(); };
	UpTime(UpTime const &){};
	UpTime& operator=(UpTime const&){};
	static UpTime* m_pInstance;
	steady_clock::time_point m_startTime;
};
