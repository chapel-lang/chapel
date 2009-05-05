
static char rcsid[] =
	"$Id: pvmwinrexec.c,v 1.4 2003/02/04 20:06:31 pvmsrc Exp $";

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


#include <process.h>

#pragma warning(disable: 4699)

#define STRICT
#pragma warning(disable: 4201)
#include <windows.h>
#pragma warning(default: 4201)
#include <winsock.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <signal.h>

static DWORD WINAPI PassOutputThread(LPVOID);
static DWORD WINAPI PassErrorThread(LPVOID);

HANDLE hStdIn, hStdOut, hStdErr;

static char *GetPassword(void);
int MyOpenService(const char *remote_host);
static void Cleanup(void);
static HANDLE PassOutput(void);
static HANDLE PassError(void);

static BOOL SendZString(const char *);
static BOOL GetErrString(char *, size_t);
extern void Wait(HANDLE, DWORD *);

static SOCKET sIO = INVALID_SOCKET;
static SOCKET sErr = INVALID_SOCKET;
char *res;

char *hn;
int found_mess=0;
int errno;

int rexec(char *hostname, char *username, char *command,char *retresult)
{
	char *password;
	size_t cmdlen;
	int i;
	HANDLE idIn = 0;
	HANDLE idOut, idErr;
	DWORD rvIn, rvOut, rvErr;

	res=malloc(512*sizeof(char));

	hn=hostname;

	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	hStdErr = GetStdHandle(STD_ERROR_HANDLE);

	password = NULL;
	
	if (!username) {
		username = (char *)MyGetUserName();
	}

	if (!password) {
		password = GetPassword();
	}

	if (!MyOpenService(hostname))
		return 0;

	SendZString(username);
	SendZString(password);
	memset (password, '\0', strlen(password));
	SendZString(command);

	if (!GetErrString(command, sizeof command)) {
		fprintf(stderr, "Remote aborted connection without initiating protocol: %d.\n",
			WSAGetLastError());
		return 0;
	}

	if (*command != '\0') {
		char *p = command;
		if (*p == '\001') {
			p++;
		}
		fprintf(stderr, "Remote aborted connection: %s\n", p);
		return 0;
	}

	if (shutdown(sIO, 1) == SOCKET_ERROR) {
		fprintf(stderr, "Failed to shutdown from input socket: error = %d.\n",
			WSAGetLastError());
		return 0;
	}

	idOut = PassOutput();
	idErr = PassError();

	Wait(idOut, &rvOut);
	Wait(idErr, &rvErr);
	 
	
	strcpy(retresult,res);
		
	return (int)rvOut;
}


static char *GetPassword()
{
	static char password[30];
	char *p;
	HANDLE hConIn, hConOut;
	DWORD dwMode;
	DWORD cbRead, cbWritten;

	hConIn = CreateFile("CONIN$", GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);

	if (hConIn == INVALID_HANDLE_VALUE) {
		fprintf(stderr, "Can't open Console for input: %lu\n", GetLastError());
		exit(1);
	}

	hConOut = CreateFile("CONOUT$", GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);

	if (hConOut == INVALID_HANDLE_VALUE) {
		fprintf(stderr, "Can't open Console for output: %lu\n", GetLastError());
		exit(1);
	}

	(void) WriteFile(hConOut, "Password: ", 10, &cbWritten, 0);

	if (!GetConsoleMode(hConIn, &dwMode)) {
		fprintf(stderr, "Can't Console input mode: %lu\n", GetLastError());
		exit(1);
	}

	dwMode &= ~(ENABLE_ECHO_INPUT);

	(void) signal(SIGINT, SIG_IGN);

	(void)SetConsoleMode(hConIn, dwMode);

	(void)ReadFile(hConIn, password, sizeof password, &cbRead, 0);

	dwMode |= ENABLE_ECHO_INPUT;
	(void)SetConsoleMode(hConIn, dwMode);

	(void) signal(SIGINT, SIG_DFL);
	
	(void) WriteFile(hConOut, "\r\n", 2, &cbWritten, 0);


	(void) CloseHandle(hConIn);
	(void) CloseHandle(hConOut);

	if ((p=strchr(password, '\r')) != NULL) {
		*p = '\0';
	}

	if ((p=strchr(password, '\n')) != NULL) {
		*p = '\0';
	}

	return password;
}


static HANDLE PassOutput()
{
	HANDLE id;
	DWORD threadId;
	

	id = (HANDLE)CreateThread(NULL,0,PassOutputThread, NULL, 0, &threadId);
	if ((long)id == -1) {
		fprintf(stderr, "Could not start output passing thread: error = %lu\n", GetLastError());
		exit(1);
	}
	return id;
}

extern int debugmask;
static DWORD WINAPI PassOutputThread(LPVOID nothing)
{


	DWORD retval = 1;
	int count=0;
	char *buf;
	int noresult;
	int i=0;

	buf = malloc(512*sizeof (char));

	while ((count=recv(sIO, &buf[i], sizeof buf, 0)) > 0) {
	
		i+=count;
	}
	buf[i]=0;

	if (noresult = (strncmp(buf, "ddpro", 5) == 0)) {
		sprintf(res,"stdout@%s: %s\n",hn,buf);
		retval=0;
	}
	if (!noresult)
		retval =1;
	

	if (count == -1) {
		fprintf(stderr, "Error passing standard output from socket: error = %d.\n",
			WSAGetLastError());
		retval = 1;
	}

	if (count != -1) {
		if (shutdown(sIO, 0) == SOCKET_ERROR) {
			fprintf(stderr, "Failed to shutdown standard output socket: error = %d.\n",
				WSAGetLastError());
			retval = 1;
		}
	}
	
	free(buf);
final:
	ExitThread(retval);
	return retval;

}


static HANDLE PassError()
{
	HANDLE id;
	DWORD threadId;
	

	id = (HANDLE)CreateThread(NULL,0,PassErrorThread, NULL, 0, &threadId);
	if ((long)id == -1) {
		fprintf(stderr, "Could not start error passing thread: error = %lu\n", GetLastError());
		exit(1);
	}
	return id;
}

static DWORD WINAPI PassErrorThread(LPVOID nothing)
{
	
	DWORD retval = 0;
	int count=0;
	char *buf;

	buf=malloc(4096*sizeof(char));

	while ((count=recv(sErr, buf, sizeof buf, 0)) > 0) {
		if (!win32_write_file(hStdErr, buf, count)) {
			fprintf(stderr, "Error writing to standard error: error = %lu.\n", GetLastError());
			retval = 1;
			break;
		}
	}

	if (count == -1) {
		fprintf(stderr, "Error passing standard error from socket: error = %d.\n",
			WSAGetLastError());
		retval = 1;
	}

	if (count != -1) {
		if (shutdown(sErr, 0) == SOCKET_ERROR) {
			fprintf(stderr, "Failed to shutdown standard error socket: error = %d.\n",
				WSAGetLastError());
			retval = 1;
		}
	}


	ExitThread(retval);
	return retval;

}


int MyOpenService(const char *remote_host)
{
	WSADATA wsadata;

	struct sockaddr_in server_addr, my_err_addr, junk_addr;
	struct servent *sv;
	struct hostent *hent;
	static char portbuf[30];
	SOCKET sTmp;
	int addr_len;

	if (WSAStartup(MAKEWORD(1,1), &wsadata) != 0) {
		fprintf(stderr, "Failed to initialize TCP/IP: error=%d.\n", WSAGetLastError());
		return(0);
	}

	if (LOBYTE(wsadata.wVersion) != 1 || HIBYTE(wsadata.wVersion) != 1) {
		fprintf(stderr, "Old version of TCP/IP: error=%d.\n", WSAGetLastError());
		return (0);
	}
	if (atexit(Cleanup) != 0) {	

		WSACleanup();
		fprintf(stderr, "Could not register TCP/IP cleanup function.\n");
		return(0);
	}

	hent = gethostbyname(remote_host);
	if(!hent) {
		fprintf(stderr, "Lookup of server hostname failed: error=%d.\n",
			WSAGetLastError());
		return (0);
	}

	sv=getservbyname("exec", "tcp");
	if (!sv) {
		fprintf(stderr, "Lookup of port number for rexec service failed: error=%d.\n",
			WSAGetLastError());
		return(0);
	}

	memcpy((char *)&server_addr.sin_addr, hent->h_addr, hent->h_length);
	server_addr.sin_family = hent->h_addrtype;
	server_addr.sin_port = sv->s_port;

	if((sIO=socket(PF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		fprintf(stderr, "I/O socket creation failed: error=%d.\n",
			WSAGetLastError());
		return(0);
	}

	if(connect(sIO, (struct sockaddr *)&server_addr, sizeof server_addr) == SOCKET_ERROR) {
		fprintf(stderr, "I/O socket connection failed: error=%d.\n",
			WSAGetLastError());
		return(0);
	}

	memset(&my_err_addr, '\0', sizeof my_err_addr);
	my_err_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	my_err_addr.sin_family = AF_INET;
	my_err_addr.sin_port = 0;

	if ((sTmp=socket(PF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		fprintf(stderr, "Error socket creation failed: error=%d.\n",
			WSAGetLastError());
		return(0);
	}

	if (bind(sTmp, (struct sockaddr *)&my_err_addr, sizeof my_err_addr) == SOCKET_ERROR) {
		fprintf(stderr, "Error socket bind failed: error=%d.\n",
			WSAGetLastError());
		(void) closesocket(sTmp);
		return(0);
	}

	if (listen(sTmp, 1) == SOCKET_ERROR) {
		fprintf(stderr, "Error socket listen failed: error=%d.\n",
			WSAGetLastError());
		(void) closesocket(sTmp);
		return(0);
	}	

	addr_len = sizeof my_err_addr;
	if (getsockname(sTmp, (struct sockaddr *)&my_err_addr, &addr_len) == SOCKET_ERROR) {
		fprintf(stderr, "Error socket bind failed: error=%d.\n",
			WSAGetLastError());
		(void) closesocket(sTmp);
		return(0);
	}

	sprintf(portbuf, "%hu", ntohs(my_err_addr.sin_port));

	SendZString(portbuf);

	addr_len = sizeof junk_addr;
	if ((sErr = accept(sTmp, (struct sockaddr *)&junk_addr, &addr_len))
		== INVALID_SOCKET) {
		fprintf(stderr, "Error socket accept failed: error=%d.\n",
			WSAGetLastError());
		(void) closesocket(sTmp);
		return(0);
	}

	(void) closesocket(sTmp);
	return 1;
}

static void Cleanup(void)
{
	if (sIO != INVALID_SOCKET) {
		(void)closesocket(sIO);
	}

	if (sErr != INVALID_SOCKET) {
		(void)closesocket(sErr);
	}


	WSACleanup();
}


static BOOL SendZString(const char *str)
{
	return win32_write_socket(sIO, str, strlen(str)+1);
}


static BOOL GetErrString(char *str, size_t len)
{
	size_t pos = 0;

	while (pos < len) {
		char ch;
		if (recv(sIO, &ch, 1, 0) != 1) {
			return FALSE;
		}
		str[pos++] = ch;
		if (ch == '\0') {
			return TRUE;
		}
		if (ch == '\n') {
			return TRUE;
		}
	}
	return FALSE;
}

