#include "UpTime.h"
#include <sstream>
#include <iomanip>


// Global static pointer used to ensure a single instance of the class
UpTime* UpTime::m_pInstance = NULL;

/** This function is called to create an instance of the class.
Calling the constructor publicly is no allowed. The constructor
is private and is only called by this Instance function.
*/
UpTime* UpTime::Instance()
{
	return m_pInstance ? m_pInstance : (m_pInstance = new UpTime);
}



void UpTime::StartClock()
{
	m_pInstance ? NULL : (m_pInstance = new UpTime);
}



std::string UpTime::getUpTimeStr()
{
	steady_clock::time_point now = steady_clock::now();

	int days = getDayField();
	int hours = getHourField();
	int mins = getMinField();
	int secs = getSecField();

	std::ostringstream stream;
	stream <<
		std::setfill('0') << std::setw(2) << days << ':' <<
		std::setfill('0') << std::setw(2) << hours << ':' <<
		std::setfill('0') << std::setw(2) << mins << ':' <<
		std::setfill('0') << std::setw(2) << secs;
	return stream.str();
}



long int UpTime::getElapsedMs()
{
	return static_cast<long int>(duration_cast<milliseconds>(steady_clock::now() - m_startTime).count());
}



int UpTime::getSecField()
{
	return duration_cast<seconds>(steady_clock::now() - m_startTime).count() % 60;
}

int UpTime::getMinField()
{
	return duration_cast<minutes>(steady_clock::now() - m_startTime).count() % 60;
}



int UpTime::getHourField()
{
	return duration_cast<hours>(steady_clock::now() - m_startTime).count() % 24;
}



int UpTime::getDayField()
{
	return duration_cast<days>(steady_clock::now() - m_startTime).count();
}
