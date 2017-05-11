#pragma once

#include "Thread.h"
#include <assert.h>
#include <vector>


/* 线程管理器 */
template <typename _Thread> class ThreadManager
{
public:
	typedef typename std::vector<_Thread *> thread_array;

	ThreadManager( void ) {}
	~ThreadManager( void )
	{
		thread_array::iterator it;
		for ( it = _thread_arr.begin(); it != _thread_arr.end(); it++ )
		{
			delete *it;
		}
		_thread_arr.clear();
	}
	template <typename _ARG_T1> void create( int count, _ARG_T1 arg1 )
	{
		int i;
		for ( i = 0; i < count; i++ )
		{
			_Thread * th_ptr = new _Thread(arg1);
			th_ptr->auto_delete = false;
			_thread_arr.push_back(th_ptr);
		}
	}
	template < typename _ARG_T1, typename _ARG_T2 > void create( int count, _ARG_T1 arg1, _ARG_T2 arg2 )
	{
		int i;
		for ( i = 0; i < count; i++ )
		{
			_Thread * th_ptr = new _Thread( arg1, arg2 );
			th_ptr->auto_delete = false;
			_thread_arr.push_back(th_ptr);
		}
	}
	template < typename _ARG_T1, typename _ARG_T2, typename _ARG_T3> void create( int count, _ARG_T1 arg1, _ARG_T2 arg2, _ARG_T3 arg3 )
	{
		int i;
		for ( i = 0; i < count; i++ )
		{
			_Thread * th_ptr = new _Thread( arg1, arg2, arg3 );
			th_ptr->auto_delete = false;
			_thread_arr.push_back(th_ptr);
		}
	}
	void create( int count )
	{
		int i;
		for ( i = 0; i < count; i++ )
		{
			_Thread * th_ptr = new _Thread();
			th_ptr->auto_delete = false;
			_thread_arr.push_back(th_ptr);
		}
	}
	void start( void )
	{
		thread_array::iterator it;
		for ( it = _thread_arr.begin(); it != _thread_arr.end(); it++ )
		{
			(*it)->start();
		}
	}
	void pause( void )
	{
		thread_array::iterator it;
		for ( it = _thread_arr.begin(); it != _thread_arr.end(); it++ )
		{
			(*it)->pause();
		}
	}
	void close( void )
	{
		thread_array::iterator it;
		for ( it = _thread_arr.begin(); it != _thread_arr.end(); it++ )
		{
			(*it)->close();
		}
	}
	void wait( unsigned long ms = INFINITE )
	{
		int count = _thread_arr.size();
		assert( count <= MAXIMUM_WAIT_OBJECTS );
		HANDLE * th_hs = new HANDLE[count];
		int i;
		for ( i = 0; i < count; ++i )
		{
			th_hs[i] = (HANDLE)_thread_arr[i]->handle();
		}
		WaitForMultipleObjects( count, th_hs, TRUE, ms );
		delete [] th_hs;
	}
private:
	thread_array _thread_arr;

	ThreadManager( ThreadManager const & );
	ThreadManager & operator = ( ThreadManager const & );
};