#include "stdafx.h"
#include "boost/thread.hpp"
#include "boost/thread/condition_variable.hpp"
#include "boost/thread/mutex.hpp"
#include <thread>
#include <iostream>
#include <windows.h>

boost::condition_variable _g_cv;
boost::mutex _g_cv_mutex;
bool _g_is_quit = false;

void ThreadFunc_1()
{
	boost::mutex::scoped_lock lock(_g_cv_mutex);
	
	while(!_g_is_quit)
	{
		_g_cv.wait(lock);
		printf("ThreadFunc_1.\n");
		//Sleep(10);
	}
}

void ThreadFunc_2()
{
	boost::mutex::scoped_lock lock(_g_cv_mutex);
	
	while(!_g_is_quit)
	{
		_g_cv.wait(lock);
		printf("ThreadFunc_2.\n");
		//Sleep(10);
	}
}

void ThreadFunc_3()
{
	boost::mutex::scoped_lock lock(_g_cv_mutex);
	
	while(!_g_is_quit)
	{
		_g_cv.wait(lock);
		printf("ThreadFunc_3.\n");
		//Sleep(10);
	}
}

void ThreadFunc_4()
{
	boost::mutex::scoped_lock lock(_g_cv_mutex);
	
	while(!_g_is_quit)
	{
		_g_cv.wait(lock);
		printf("ThreadFunc_4.\n");
		//Sleep(10);
	}
}

void ThreadControl()
{
	#define THREADCOUNT 4
	
	typedef void(*FUNC)();
	FUNC threadArray[4] = {ThreadFunc_1, ThreadFunc_2, ThreadFunc_3, ThreadFunc_4};
	
	for(size_t i=0; i<THREADCOUNT; i++)
	{
		boost::thread th(threadArray[i]);
	}
}

int main()
{
	ThreadControl();
	
	using namespace std;
	int value = 0;
	while(true)
	{
		printf("enter 1 to notify one thread.\n");
		printf("enter 2 to notify thread one by one.\n");
		printf("enter 3 to notify all threads.\n");
		printf("enter -1 to quit.\n");
		
		cin >> value;
		switch (value)
		{
			case -1:
				_g_is_quit = true;
				break;
			case 1:
				_g_cv.notify_one();
				break;
			case 2:
				for(size_t i=0; i<THREADCOUNT; i++)
				{
					_g_cv.notify_one();
				}
				break;
			case 3:
				_g_cv.notify_all();
				break;
			default:
				printf("enter again.\n");
				break;
		}
		
		if (_g_is_quit)
		{
			break;
		}
	}
	
	system("pause");
}


































