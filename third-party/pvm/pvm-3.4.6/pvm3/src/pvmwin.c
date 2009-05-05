
static char rcsid[] =
	"$Id: pvmwin.c,v 1.9 2003/02/04 20:35:47 pvmsrc Exp $";

/*
 *         PVM version 3.4:  Parallel Virtual Machine System
 *               University of Tennessee, Knoxville TN.
 *           Oak Ridge National Laboratory, Oak Ridge TN.
 *                   Emory University, Atlanta GA.
 *      Authors:  J. J. Dongarra, G. E. Fagg, M. Fischer
 *          G. A. Geist, J. A. Kohl, R. J. Manchek, P. Mucci,
 *         P. M. Papadopoulos, S. L. Scott, and V. S. Sunderam
 *                   (C) 1997 All Rights Reserved
 *
 *                              NOTICE
 *
 * Permission to use, copy, modify, and distribute this software and
 * its documentation for any purpose and without fee is hereby granted
 * provided that the above copyright notice appear in all copies and
 * that both the copyright notice and this permission notice appear in
 * supporting documentation.
 *
 * Neither the Institutions (Emory University, Oak Ridge National
 * Laboratory, and University of Tennessee) nor the Authors make any
 * representations about the suitability of this software for any
 * purpose.  This software is provided ``as is'' without express or
 * implied warranty.
 *
 * PVM version 3 was funded in part by the U.S. Department of Energy,
 * the National Science Foundation and the State of Tennessee.
 */

#include <pvm3.h>
#include "pvmalloc.h"
#include "pvmwin.h"
#include <winreg.h>
#include <fcntl.h>

extern int debugmask;

extern char *username;


char *
read_pvmregistry(token_call)
const char *token_call;
{
	long return_value;
	HKEY top=NULL;
	HKEY phkResult = NULL;
	static char subkey[]="SOFTWARE\\PVM";
	static char root[]="SOFTWARE\\PVM";
 	static char token[16];

	LPTSTR key_name ="";
	DWORD type = REG_EXPAND_SZ;

	int value_size=0;
	int i=0;
	char value[64];
	char *give_back = (char *) malloc (64 * sizeof(char));
	value_size=sizeof(value);

	top = HKEY_LOCAL_MACHINE;
	for (i=0; token_call[i];i++)
		token[i]=token_call[i];
	token[i++]=0;
	token[i++]=0;

	key_name=subkey;

	return_value = RegOpenKeyEx(top, 
				key_name, 
				0, 
				KEY_EXECUTE, 
				&phkResult);
    
	if ( return_value != ERROR_SUCCESS ) {
      		fprintf(stderr,"RegOpenKey failed for %s: %d\n",token,(int) return_value);
		return NULL;
	}
	
	return_value = RegQueryValueEx(phkResult,
					token,
					NULL,
					(LPDWORD) &type,
					(LPBYTE) &value,
					(LPDWORD)&value_size);
	if ( return_value != ERROR_SUCCESS ) {
	      fprintf(stderr,"RegQueryValueEx failed for %s: %d \n", token,(int) return_value);
	      return NULL;
	}
	strcpy(give_back,(char *)value);
	RegCloseKey(phkResult);
	return (char* ) give_back;
}


void
Wait(id, prv)
HANDLE id;
DWORD *prv;
{
	if (!WaitForSingleObject(id, INFINITE)) {
		*prv = 2;
	} else {
		if (!GetExitCodeThread(id, prv)) {
			*prv = 4;
		}
	}
}


int
win32_write_socket (s, tosend, size)
SOCKET s;
const char FAR *tosend;
int size;
{
	int nSend=0;

	int size2send=size;

	while (size2send > 0) {
		if ((nSend = send(s, tosend, size2send, 0)) == -1)
			return nSend;

		tosend += nSend;
		size2send -= nSend;
	}
/*
	pvmlogprintf("loclinput, to send: %d yes, send: %d\n",
			size,size-size2send);
*/
	return size-size2send;	/* this should be $size in the end */
}


int
win32_read_socket (s, torecv, size)
SOCKET s;
char FAR *torecv;
int size;
{
	int nReceived=0;

	int size2recv=size;

	while (size2recv > 0) {

		if ((nReceived = recv(s, torecv, size2recv, 0)) == -1)
			return nReceived;

		/* Nothing received - EOF? */
		/* return any bytes read */
		if (!nReceived)
			return size-size2recv;

		torecv += nReceived;
		size2recv -= nReceived;
	}
/*
	pvmlogprintf("loclinput, to read: %d yes, read: %d\n",
			size,size-size2recv);
*/
	return size-size2recv;
}


int
gettimeofday(t, tzp)
struct timeval *t;
struct timezone *tzp;
{
	struct _timeb timebuffer;

	/* this calls the time and returns sec ** msec */
	_ftime( &timebuffer );

	t->tv_sec=timebuffer.time;
	t->tv_usec=timebuffer.millitm*1000;
 	return 1;
}


char *
GetLastErrorToStr (error)
int error;
{
    switch(error)
    {
        case WSAENETDOWN:
            return("The network subsystem has failed.\n");
            break;
        case WSAEINTR:
            return("A blocking call was cancelled.  This can be caused \
		by\n1) a short response time, or\n2) \
		User interrupts the process.\n");
            break;
        case WSAEINPROGRESS:
            return("A blocking call is in progress.\n");
            break;
        case WSAENOBUFS:
            return("No buffer space is available.\n");
            break;
        case WSAENOTSOCK:
            return("Invalid socket descriptor.\n");
            break;
        case WSAEADDRINUSE:
            return("The specified address is already in use.\n");
            break;
        case WSAEADDRNOTAVAIL:
            return("The specified address is not available\nfrom the local machine.\n");
            break;
        case WSAECONNREFUSED:
            return("The connection attempt was refused.\n");
            break;
        case WSAEINVAL:
            return("The socket is not bound to an address.\n");
            break;
        case WSAEISCONN:
            return("The socket is already connected.\n");
            break;
        case WSAEMFILE:
            return("The maximum number of sockets has exceeded.\n");
            break;
        case WSAENETUNREACH:
            return("Network cannot be reached from this host at this time.\n");
            break;
        case WSAETIMEDOUT:
            return("Attempt to connect timed out without establishing a connection.\n");
            break;
        case WSAENOTCONN:
            return("The socket is not connected.\n");
            break;
        case WSAESHUTDOWN:
            return("The socket has been shut down.\n");
            break;
        case WSAECONNABORTED:
            return("The socket connection has been reset.\n");
            break;
        case WSAECONNRESET:
            return("Socket shutdown by remote.\n");
            break;
        case WSAEACCES:
            return("The requested address is a broadcast address.\n");
            break;
        case WSAENETRESET:
            return("The socket connection has been reset.\n");
            break;
        case WSAHOST_NOT_FOUND:
            return("Host could not found.\n");
            break;
        default:
	    return("could not match error tag");
            break;
    }
}

void 
nt_rpc_report(s)
char *s;
{
	printf("Error: %s: %d\n",s,GetLastError());
}


void
ErrorHandler(s)
char *s;
{
	printf("Error: %s: %d\n",s,GetLastError());
}


int 
win32_close_file(f)
HANDLE f;
{
	int success;
	success = CloseHandle(f);
	return success;
}


HANDLE 
win32_create_file(TMP_AREA, mode)
char *TMP_AREA;
int mode;
{
	HANDLE hFile;
	PSECURITY_DESCRIPTOR pSD;
	PACL pACLNew;
	DWORD cbACL = 1024;
	PSID pSID;
	DWORD cbSID = 1024;
	LPSTR szUserName;
	DWORD dwUserNameLength;
	LPSTR lpszDomain;
	DWORD cchDomainName = 80;
	PSID_NAME_USE psnuType;

  	/* begin of adding, by Jigen Zhou */

	LPSTR szUser = NULL;
	DWORD cchUser = 81;
	LPSTR szDomain = NULL;
	DWORD cchDomain = 81;
	HANDLE       hToken   = NULL;
	PTOKEN_USER  ptiUser  = NULL;
	DWORD        cbti     = 0;
	SID_NAME_USE snu;

	/* end of adding */

	/* Initialize a new security descriptor. */


	pSD = (PSECURITY_DESCRIPTOR) LocalAlloc(LPTR,
		SECURITY_DESCRIPTOR_MIN_LENGTH);	/* defined in WINNT.H */
	if (pSD == NULL) {
		ErrorHandler("LocalAlloc");
		return FALSE;
	}

	if (!InitializeSecurityDescriptor(pSD,
			SECURITY_DESCRIPTOR_REVISION)) {

		ErrorHandler("InitializeSecurityDescriptor");
		goto Cleanup;
	}

	/* Initialize a new ACL.				 */

	pACLNew = (PACL) LocalAlloc(LPTR, cbACL);
	if (pACLNew == NULL) {
		ErrorHandler("LocalAlloc");
		goto Cleanup;
	}

	if (!InitializeAcl(pACLNew, cbACL, ACL_REVISION2)) {
		if (GetLastError() == ERROR_CALL_NOT_IMPLEMENTED) {
			return (HANDLE) -2;
		}
		ErrorHandler("InitializeAcl");
		goto Cleanup;
	}

	/* Retrieve the SID for User		*/
	if (username)
		szUserName=username;
	else
 		szUserName=MyGetUserName();

	/* begin of changing, by Jigen Zhou */

	/* Get the calling thread's access token. */
	if (!OpenThreadToken(GetCurrentThread(), TOKEN_QUERY, TRUE, &hToken)) {

	    /* Retry against process token if no thread token exists. */
	   if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
	      fprintf(stderr,"Error to open thread/process token\n");
	}

	if (hToken) {

	   /* Obtain the size of the user information in the token. */
	   if (GetTokenInformation(hToken, TokenUser, NULL, 0, &cbti)) {
	      /* Call should have failed due to zero-length buffer. */
	      ErrorHandler("GetTokenInformation");
	      goto Cleanup;
	   }

	   /* Allocate buffer for user information in the token. */
	   ptiUser = (PTOKEN_USER)LocalAlloc(LPTR, cbti);
	   if (ptiUser== NULL) {
	      ErrorHandler("LocalAlloc");
	      goto Cleanup;
	   }

	   /* Retrieve the user information from the token. */
	   if (!GetTokenInformation(hToken, TokenUser, ptiUser, cbti, &cbti)) {
	      pvmlogprintf("Error while GetTokenInformation: %d\n",
				GetLastError());
	      pvmlogprintf("Continued by LookupAccountName\n");
	      hToken = NULL;
	   } else {
	      /* Retrieve user name and domain name based on user's SID. */
	      szUser   = (LPSTR)LocalAlloc(LPTR, cchUser);
	      szDomain = (LPSTR)LocalAlloc(LPTR, cchDomain);
	      if ((szUser == NULL) || (szDomain == NULL)) {
	         ErrorHandler("LocalAlloc");
	         goto Cleanup;
	      }
	      if (!LookupAccountSid(NULL, ptiUser->User.Sid, szUser, &cchUser,
	              szDomain, &cchDomain, &snu)) {
	         pvmlogprintf("Error while LookupAccountSid: %d\n",
				GetLastError());
	         pvmlogprintf("Continued by LookupAccountName\n");
	         hToken = NULL;
	      } else {
	         if (strcmp(szUser, szUserName)) {
	            pvmlogprintf("LookupAccountSid get %s does not match %s\n",
	                      szUser, szUserName);
	            pvmlogprintf("Continued by LookupAccountName\n");
	            hToken = NULL;
	         } else {
	            pSID = ptiUser->User.Sid;
	         }
	      }
	   }
	}

	if (hToken == NULL) {    /* This is the original block of code */
 
	   pSID = (PSID) LocalAlloc(LPTR, cbSID);
	   psnuType = (PSID_NAME_USE) LocalAlloc(LPTR, 1024);
	   lpszDomain = (LPSTR) LocalAlloc(LPTR, cchDomainName);
	   if (pSID == NULL || psnuType == NULL ||
		lpszDomain == NULL) {
		ErrorHandler("LocalAlloc");
		goto Cleanup;
	   }

	   if (!LookupAccountName((LPSTR) NULL, /* local name */
			szUserName,
			pSID,
			&cbSID,
			lpszDomain,
			&cchDomainName,
			psnuType)) {
		ErrorHandler("LookupAccountName");
		goto Cleanup;
	   }
	}

   /* end of changing, by Jigen Zhou */


	if (!IsValidSid(pSID))
		ErrorHandler("SID is not valid.\n");


	/* Allow read but not write access to the file. */

	if (!AddAccessAllowedAce(pACLNew,
			ACL_REVISION2,
			GENERIC_ALL,
			pSID)) {
		ErrorHandler("AddAccessAllowedAce");
		goto Cleanup;
	}

	/* Add a new ACL to the security descriptor. */

	if (!SetSecurityDescriptorDacl(pSD,
			TRUE,			/* fDaclPresent flag */
			pACLNew,
			FALSE)) {		/* not a default disc. ACL */
		ErrorHandler("SetSecurityDescriptorDacl");
		goto Cleanup;
	}

	hFile = CreateFile(TMP_AREA,
					GENERIC_WRITE | GENERIC_READ,
					// FILE_SHARE_READ | FILE_SHARE_WRITE,
					0,
					NULL,
					mode,
					FILE_ATTRIBUTE_NORMAL,
					NULL);

	if (GetLastError() == ERROR_ALREADY_EXISTS)
		if (mode == CREATE_NEW)
			return (HANDLE) -1;

	if (hFile == INVALID_HANDLE_VALUE)
	{
		return (HANDLE) -1;
	}

	/* Apply the new security descriptor to the file. */

	if (!SetFileSecurity(TMP_AREA,
		DACL_SECURITY_INFORMATION,
		pSD)) {
		ErrorHandler("SetFileSecurity");
		goto Cleanup;
	}

	/*
	printf("Successfully added access-allowed ACE to file's DACL.\n");
	*/

	return hFile;

Cleanup:
		FreeSid(pSID);
		if(pSD != NULL)
			LocalFree((HLOCAL) pSD);
		if(pACLNew != NULL)
			LocalFree((HLOCAL) pACLNew);
		if(psnuType != NULL)
			LocalFree((HLOCAL) psnuType);
		if(lpszDomain != NULL)
			LocalFree((HLOCAL) lpszDomain);
	return 0;
}


int 
win32_write_file(f, s, size)
HANDLE f;
char *s;
int size;
{
	int sizewritten=0;

	if (!WriteFile(f,s,size,&sizewritten,NULL)) {
		pvmlogprintf("Could not write to file :%d \n",GetLastError());
		return 0;
	}
	return sizewritten;

}


int
win32_read_file(f, s, size)
HANDLE f;
char *s;
int size;
{
	int sizeread=0;
	if (!ReadFile(f,s,size,&sizeread,NULL)) {
		pvmlogprintf("Could not read data from file :%d \n",
				GetLastError());
		return -1;
	}
	return sizeread;

}


HANDLE 
win32_open_file(TMP_AREA)
char *TMP_AREA;
{
	HANDLE hF;

	hF= win32_create_file(TMP_AREA,OPEN_EXISTING);
	if (hF == INVALID_HANDLE_VALUE)
		printf("Could not open file %s: %d \n",TMP_AREA,GetLastError());
	return hF;
}


int 
kill(pid, p_handle, signal)
int pid;
int p_handle;
int signal;
{
	HANDLE hProcess;

	// DebugBreak();
	if (p_handle)
	if (!TerminateProcess((HANDLE)p_handle,signal)) {
		pvmlogprintf("Unable to terminate process: %d \n",p_handle);
	} else return TRUE;

	hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,pid);

	if (hProcess) {
		if (!TerminateProcess(hProcess,signal)) {
			pvmlogprintf("Unable to terminate process: %d \n",hProcess);
		} else {CloseHandle (hProcess);return TRUE;}
	}
	CloseHandle (hProcess);
	return TRUE;
}


char *
MyGetUserName()
{
	char *thisname=0;
	char myuser[64];
	DWORD szuser=64;
	if (!GetUserName(myuser,&szuser)) {
		pvmlogprintf("You are not logged on to this machine: %d\n",
				GetLastError());
		pvmlogprintf("Continued by provided username\n");
		return NULL;
	}
	thisname = STRALLOC(myuser);
	return thisname;

}


int 
win32_delete_file(TMP_AREA)
char *TMP_AREA;
{
	return (!DeleteFile(TMP_AREA));
}

