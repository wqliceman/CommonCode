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
			std::cout << _name << " " << this->thread_id << " ��������...\n";
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

	// ����һ���̣߳�����create()���������̣߳�����ഫ3���������ù��캯��
	ThreadManager<MyThread> mgr;
	mgr.create( 5, &cs, std::string("Output") );
	mgr.start();

	system("pause");	
	return 0;
}