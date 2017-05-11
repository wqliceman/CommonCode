#include "lock.h"


CriticalSection::CriticalSection( void )
{
	ZeroMemory( &_cs, sizeof(_cs) );
	InitializeCriticalSection(&_cs);
}

CriticalSection::~CriticalSection( void )
{
	DeleteCriticalSection(&_cs);
}

void CriticalSection::lock( void )
{
	EnterCriticalSection(&_cs);
}

void CriticalSection::unlock( void )
{
	LeaveCriticalSection(&_cs);
}

bool CriticalSection::trylock( void )
{
	return TryEnterCriticalSection(&_cs);
}