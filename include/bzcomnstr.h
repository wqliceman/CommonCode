#ifndef __CMMSTR_H__
#define __CMMSTR_H__

#include <windows.h>

#define SAFE_FREESTRING(s) if(s){free(s); s = NULL;}
#define SAFE_FREEBSTRING(s) if(s){SysFreeString(s); s = NULL;}
#define CHECK_NULL_RETURN(var, err) if(NULL==(var)) return (err)



STDAPI_(LPSTR) BzLPWSTRToANSI(LPCWSTR pwszString);
STDAPI_(LPSTR) BzLPWSTRToANSIWithLen(LPCWSTR pwszString,UINT cchWideChar);

STDAPI_(LPWSTR) BzANSIToLPWSTR(LPCSTR pszString);
STDAPI_(LPWSTR) BzANSIToLPWSTRWithLen(LPCSTR pszString,UINT bytesLen);
STDAPI_(BSTR)	BzANSIToBSTR(LPCSTR pszString);


#endif //__CMMSTR_H__