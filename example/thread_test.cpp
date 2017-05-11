#include <iostream>
#include <string>

#include "lock.h"
#include "Thread.h"
#include "ThreadManager.h"

class MyThread : public Thread
{
public:
	MyThread( CriticalSection * cs_lock ) : _cs_lock(cs_lock)
	{
	}
	MyThread( CriticalSection * cs_lock, std::string name ) :  _cs_lock(cs_lock), _name(name)
	{
	}
	virtual void run()
	{
		int i = 0;
		while ( !this->want_stop && i++ < 100 )
		{
			_cs_lock->lock();
			std::cout << _name << " " << this->thread_id << " 正在运行...\n";
			_cs_lock->unlock();
		}
	}
private:
	CriticalSection * _cs_lock;
	std::string _name;
};


int main()
{
	std::string cmd;
	CriticalSection cs;

	// 管理一组线程，调用create()函数创建线程，可最多传3个参数调用构造函数
	ThreadManager<MyThread> mgr;
	mgr.create( 5, &cs, std::string("Output") );
	mgr.start();

	system("pause");	
	return 0;
}