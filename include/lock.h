#include <Windows.h>

class CriticalSection
{
public:
	CriticalSection( void );
	~CriticalSection( void );
	void lock( void );
	bool trylock( void );
	void unlock( void );
private:
	CRITICAL_SECTION _cs;

	CriticalSection( CriticalSection const & );
	CriticalSection & operator = ( CriticalSection const & );
};