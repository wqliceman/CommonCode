#include "bzcomnstr.h"


STDAPI_(LPSTR) BzLPWSTRToANSI(LPCWSTR pwszString)
{
	LPSTR psz;
	UINT cblen;

	CHECK_NULL_RETURN(pwszString, NULL);

	UINT iCodepage = 0;
	cblen = WideCharToMultiByte(0, 0, pwszString, -1, NULL, 0, NULL, NULL);

	if ((psz = (LPSTR)malloc(cblen + 1)) && 
		(0 == WideCharToMultiByte(iCodepage, 0, pwszString, -1, psz, cblen, NULL, NULL)))
	{
		SAFE_FREESTRING(psz); psz = NULL;
	}
	return psz;
}
STDAPI_(LPSTR) BzLPWSTRToANSIWithLen(LPCWSTR pwszString,UINT cchWideChar)
{
	LPSTR psz;
	UINT cblen;

	CHECK_NULL_RETURN(pwszString, NULL);

	if(0 == cchWideChar)
	{
		return NULL;
	}

	UINT iCodepage = 0;
	cblen = WideCharToMultiByte(iCodepage, 0, pwszString, cchWideChar, NULL, 0, NULL, NULL);

	if ((psz = (LPSTR)malloc(cblen + 1)) && 
		(0 == WideCharToMultiByte(iCodepage, 0, pwszString, cchWideChar, psz, cblen, NULL, NULL)))
	{
		SAFE_FREESTRING(psz); psz = NULL;
	}
	return psz;
}
STDAPI_(LPWSTR) BzANSIToLPWSTR(LPCSTR pszString)
{
	LPWSTR pwsz;
	UINT cblen;

	CHECK_NULL_RETURN(pszString, NULL);

	UINT iCodepage = 0;

	cblen = MultiByteToWideChar(iCodepage, MB_PRECOMPOSED, pszString, -1, NULL, 0);

	if ((pwsz = (LPWSTR)malloc((cblen+1)*sizeof(WCHAR))) && 
		(0 == MultiByteToWideChar(iCodepage, MB_PRECOMPOSED, pszString, -1, pwsz, cblen + 1)))
	{
		SAFE_FREESTRING(pwsz); pwsz = NULL;
	}
	return pwsz;
}
STDAPI_(LPWSTR) BzANSIToLPWSTRWithLen(LPCSTR pszString,UINT bytesLen)
{
	LPWSTR pwsz;
	UINT cblen;

	CHECK_NULL_RETURN(pszString, NULL);

	if(0 == bytesLen)
	{
		return NULL;
	}

	UINT iCodepage = 0; 

	cblen = MultiByteToWideChar(iCodepage, MB_PRECOMPOSED, pszString, bytesLen, NULL, 0);

	if ((pwsz = (LPWSTR)malloc((cblen+1)*sizeof(WCHAR))) && 
		(0 == MultiByteToWideChar(iCodepage, MB_PRECOMPOSED, pszString, bytesLen, pwsz, cblen + 1)))
	{
		SAFE_FREESTRING(pwsz); pwsz = NULL;
	}
	return pwsz;
}

STDAPI_(BSTR) BzANSIToBSTR(LPCSTR pszString)
{
	BSTR bstr = NULL;
	LPWSTR pwsz;

	if (pwsz = BzANSIToLPWSTR(pszString))
	{
		bstr = ::SysAllocString(pwsz);
		SAFE_FREESTRING(pwsz);
	}

	return bstr;
}
