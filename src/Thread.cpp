#include "Thread.h"


Thread::Thread( void ) : _thread(0), thread_id(0), want_stop(false), auto_delete(true), exit_code(0)
{
	_thread = _beginthreadex( NULL, 0, (start_routine)_start_run, this, CREATE_SUSPENDED, &this->thread_id );
}

Thread::~Thread( void )
{
	this->close();
}

void Thread::start( void )
{
	ResumeThread( (HANDLE)_thread );
}

void Thread::pause( void )
{
	SuspendThread( (HANDLE)_thread );
}

void Thread::close( void )
{
	if ( _thread )
	{
		CloseHandle( (HANDLE)_thread );
		_thread = 0;
	}
}

void Thread::wait( unsigned long ms /*= INFINITE */ )
{
	WaitForSingleObject( (HANDLE)_thread, ms );
}

unsigned __stdcall Thread::_start_run( Thread * thread_ptr )
{
	thread_ptr->run();
	unsigned exit_code = thread_ptr->exit_code;

	if ( thread_ptr->auto_delete )
		delete thread_ptr;

	return exit_code;
}

void Thread::forceStop( void )
{
	TerminateThread( (HANDLE)_thread, exit_code );
}
