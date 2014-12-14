
// UpTime.h
// The UpTime class is used to query server UpTime





#pragma once
#include <chrono>
#include <string>

using namespace std::chrono;

typedef duration<int, std::ratio<3600 * 24>> days;

class UpTime{
public:

	/** This function is called to create an instance of the class.
	Calling the constructor publicly is no allowed. The constructor
	is private and is only called by this Instance function.
	*/
	static UpTime * Instance();

	/** Initializes the timer **/
	static void StartClock();

	/** Returns the current UpTime in DD:HH:MM:SS format */
	std::string getUpTimeStr();


	long int getElapsedMs();

	/** Returns the second field DD:HH:MM:<SS> **/
	int getSecField();

	/** Returns the minute field DD:HH:<MM>:SS **/
	int getMinField();

	/** Returns the hour field DD:<HH>:MM:SS **/
	int getHourField();

	/** Returns the day field <DD>:HH:MM:SS **/
	int getDayField();


private:
	/** Prevent the class from being manually initialized/copied **/

	/** Start the clock on initialization **/
	UpTime(){ m_startTime = steady_clock::now(); };


	UpTime(UpTime const &){};


	UpTime& operator=(UpTime const&){};

	/** end initialization/copy prevention **/

	static UpTime* m_pInstance;


	steady_clock::time_point m_startTime;
};
