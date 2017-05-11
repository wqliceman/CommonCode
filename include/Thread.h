#pragma once

#include <Windows.h>
#include <process.h>

/* Ïß³Ì·â×° */
class Thread
{
public:
	typedef unsigned long handle_t;
	typedef unsigned ( __stdcall * start_routine )( void * param );
	bool want_stop;
	bool auto_delete;
	unsigned thread_id;
	unsigned exit_code;

	Thread( void );
	virtual ~Thread( void );
	virtual void run() = 0;

	void start( void );
	void pause( void );
	void close( void );
	void wait( unsigned long ms = INFINITE );
	void forceStop( void );

	handle_t handle( void ) { return _thread; }
private:
	handle_t _thread;
	static unsigned __stdcall _start_run( Thread * thread_ptr );

	Thread( Thread const & );
	Thread & operator = ( Thread const & );
};