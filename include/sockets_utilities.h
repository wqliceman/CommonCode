#ifndef DLIB_SOCKETS_UTILITIES_H
#define DLB_SOCKETS_UTILITIES_H

#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))

#include <windows.h>

namespace dlib
{

	typedef DWORD thread_id_type;

inline thread_id_type get_thread_id(
	)
{
	return GetCurrentThreadId();
}

// ----------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------
// mutex object
// ----------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------

class mutex
{
public:

	mutex(
		)
	{
		InitializeCriticalSection(&cs);
	}

	~mutex(
		) {
		DeleteCriticalSection(&cs);
	}

	void lock(
		) const {
		EnterCriticalSection(&cs);
	}

	void unlock(
		) const {
		LeaveCriticalSection(&cs);
	}

private:

	mutable CRITICAL_SECTION cs;

	// restricted functions
	mutex(mutex&);        // copy constructor
	mutex& operator=(mutex&);    // assignment operator
};

// ----------------------------------------------------------------------------------------

class noncopyable
{
	/*!
	This class makes it easier to declare a class as non-copyable.
	If you want to make an object that can't be copied just inherit
	from this object.
	!*/

protected:
	noncopyable() {}
	~noncopyable() {}
private:  // emphasize the following members are private
	noncopyable(const noncopyable&);
	const noncopyable& operator=(const noncopyable&);

};

template <typename T>
struct default_deleter
{
	void operator() (T* item) const
	{
		delete item;
	}
};

#if _MSC_VER > 1200

template <typename T>
struct default_deleter<T[]>
{
	void operator() (T* item) const
	{
		delete[] item;
	}
};

#endif

// ----------------------------------------------------------------------------------------

template <
	typename T,
	typename deleter = default_deleter<T>
>
class scoped_ptr : noncopyable
{
	/*!
	CONVENTION
	- get() == ptr
	!*/

public:
	typedef T element_type;
	typedef deleter deleter_type;

	explicit scoped_ptr(
		T* p = 0
		) : ptr(p) { }

	~scoped_ptr()
	{
		if (ptr)
		{
			deleter del;
			del(ptr);
		}
	}

	void reset(
		T* p = 0
		)
	{
		if (ptr)
		{
			deleter del;
			del(ptr);
		}

		ptr = p;
	}

	T& operator*() const
	{
		return *ptr;
	}

	T* operator->() const
	{
		return ptr;
	}

	T* get() const
	{
		return ptr;
	}

	operator bool() const
	{
		return (ptr != 0);
	}

	void swap(
		scoped_ptr& b
		)
	{
		std::swap(ptr, b.ptr);
	}

private:

	T* ptr;
};

// ----------------------------------------------------------------------------------------

#if _MSC_VER > 1200
template <
	typename T,
	typename deleter
>
class scoped_ptr<T[], deleter> : noncopyable
{
	/*!
	CONVENTION
	- get() == ptr
	!*/

public:
	typedef T element_type;

	explicit scoped_ptr(
		T* p = 0
		) : ptr(p) { }

	~scoped_ptr()
	{
		if (ptr)
		{
			deleter del;
			del(ptr);
		}
	}

	void reset(
		T* p = 0
		)
	{
		if (ptr)
		{
			deleter del;
			del(ptr);
		}
		ptr = p;
	}

	T& operator[] (
		unsigned long idx
		) const
	{

		return ptr[idx];
	}

	T* get() const
	{
		return ptr;
	}

	operator bool() const
	{
		return (ptr != 0);
	}

	void swap(
		scoped_ptr& b
		)
	{
		std::swap(ptr, b.ptr);
	}

private:

	T* ptr;
};
#endif

// ----------------------------------------------------------------------------------------

template <
	typename T,
	typename deleter
>
void swap(
scoped_ptr<T, deleter>& a,
scoped_ptr<T, deleter>& b
)
{
	a.swap(b);
}

}
#endif