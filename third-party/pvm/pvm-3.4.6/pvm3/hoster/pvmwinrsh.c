
static char rcsid[] =
	"$Id: pvmwinrsh.c,v 1.5 1999/03/15 21:52:03 pvmsrc Exp $";

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

#include <pvmwin.h>
#include <winsock.h>

struct sockaddr_in anaddr; // local socket address structure 
struct sockaddr_in saddr; // server socket address structure

u_short rshPort; // the local rsh port; determined dynamically
u_short rshErrPort; // the local rsh port for client stderr output
u_short rshSPort; // the remote rsh port; basically, the 'shell' port from services
u_short rshProto; // the rsh protocol ("tcp") 

SOCKET rshClient    = INVALID_SOCKET; // rsh client socket for outgoing connections 
SOCKET rshClientErr = INVALID_SOCKET; // rsh client socket for stderr input

char ruserName[64]; // user name given with option 'lo'
char luserName[64]; // user logged onto the machine

char cmd[4096];

int stderrFlag=1; // set when a connection has been detected on the stderr channel

// socket options variables 
int on=1;
struct linger linger;
LPPROTOENT lpProto;

char *res; // our return string

int
rresvport (u_short *alport, int sProto)
{
    struct sockaddr_in sin;
    int s;

    sin.sin_family=AF_INET;
    sin.sin_addr.s_addr=INADDR_ANY;
    s=socket(AF_INET, SOCK_STREAM, sProto);
    if(s<0)
        return -1;

    for(*alport=IPPORT_RESERVED-1; *alport>IPPORT_RESERVED/2;
			(*alport)--)
    {
        sin.sin_port=htons((u_short)*alport);
        if(bind(s, (struct sockaddr*)&sin, sizeof(sin))==0)
            return s;

        if(WSAGetLastError()!=WSAEADDRINUSE)
            break;
    }
    closesocket(s);
    return -1;
}


void
sendsocket (const char* buff, int bufflen, SOCKET rshClient)
{
    if(send(rshClient, buff, bufflen, 0) < bufflen)
        fprintf(stdout,"error sending command.");
}


// receive ////////////////////////////////////////////////////////////
//
// receive a string from the given socket
//
int
receivesocket (SOCKET rshClient, char* buff2, int blen,char *hostname)
{
    int bufflen;
    int totallen=0;
    int noresult=0;
    int i=0;
    char *b= malloc(128 * sizeof(char));
    char *buff = malloc(512 * sizeof(char));
    memset( buff, 0, 512 );
    strcpy(buff,"");

    while ((bufflen=recv(rshClient, b,sizeof b, 0)) > 0) {
                b[bufflen]='\0';
	        strcat(buff,b);
                totallen+=bufflen;
        
                if (noresult = (strncmp(buff, "ddpro", 5) == 0)) {
                        sprintf(res,"stdout@%s: %s\n",hostname,buff);
                        if (strlen(buff)>=66) return 1;
                }
        
        }
                
    if(bufflen==SOCKET_ERROR) {
                fprintf(stdout,"error in receive on socket: %s\n",
                                GetLastErrorToStr(GetLastError()));
        return bufflen;
        }
        
    return 0;
}


int
verify_hostname (const char* hostname)
{
	memset(&saddr, 0, sizeof(saddr));
	saddr.sin_addr.s_addr=inet_addr(hostname);
	if(saddr.sin_addr.s_addr==(u_long)INADDR_NONE)
	{
		// must have gotten host name instead of IP address; resolve!
		struct hostent* hostInfo=gethostbyname(hostname);
		if(!hostInfo) {
			fprintf(stdout,"Invalid hostname!");
			return 0;
		}
		memcpy((void*)&saddr.sin_addr.s_addr, hostInfo->h_addr,
				hostInfo->h_length);
	}
	return 1;
}


// initSocket /////////////////////////////////////////////////////////
//
// standard socket initialization procedure 
//
void
initSocket ()
{
    // get port number for rsh
    struct servent FAR* sp=getservbyname("shell", "tcp");
   
    if(sp==NULL)
        fprintf(stdout,
				"Cannot determine port number for the rsh client.");
    rshSPort=htons(sp->s_port);

    lpProto=getprotobyname("tcp");
    
    rshProto=lpProto->p_proto;

    // create socket 
    rshClient=rresvport(&rshPort, rshProto);
    if(rshClient==INVALID_SOCKET)
	fprintf(stdout,"Cannot allocate socket for the rsh client.");

    if(setsockopt(rshClient, SOL_SOCKET, SO_KEEPALIVE, (char*)&on,
			sizeof(on))<0)
        fprintf(stdout,"Cannot set SO_KEEPALIVE!\n", 0);
    linger.l_onoff=1;
    linger.l_linger=60;
    if(setsockopt(rshClient, SOL_SOCKET, SO_LINGER, (char*)&linger,
			sizeof(linger))<0)
        fprintf(stdout,"Cannot set SO_LINGER!\n");
}


// openErrSocket //////////////////////////////////////////////////////
//
// an additional socket is created for stderr output
//
int
initErrSocket ()
{
    // create the new socket and bind it to the client stderr port
    rshErrPort=IPPORT_RESERVED-1;
    rshClientErr=rresvport(&rshErrPort,0);
    if(rshClientErr==INVALID_SOCKET)
    {
        fprintf(stdout,"Cannot create stderr socket!");
        return 1;
    }

    if(setsockopt(rshClientErr, SOL_SOCKET, SO_KEEPALIVE, (char*)&on,
			sizeof(on))<0)
        fprintf(stdout,"Cannot set SO_KEEPALIVE!");
    linger.l_onoff=0;
    linger.l_linger=60;
    setsockopt(rshClientErr, SOL_SOCKET, SO_LINGER, (char*)&linger,
			sizeof(linger));
	if(listen(rshClientErr, 5))
		fprintf(stdout,"Cannot listen!");
    return 1;
}


// command ////////////////////////////////////////////////////////////
//
// pass the command string to the rsh server and retrieve the results
//
int
do_command (const char* cmd,char *hostname)
{
	char cmdbuff[2048];
	char buff[2048];
	int retval=0;
	int cmdbufflen=0;
	int noresult=0;
	
	sprintf(cmdbuff+cmdbufflen, "%d", rshErrPort); // local stderr port
	cmdbufflen=strlen(cmdbuff)+1;

	strcpy(cmdbuff+cmdbufflen, luserName); // locuser
	cmdbufflen+=strlen(luserName)+1;

	strcpy(cmdbuff+cmdbufflen, ruserName); // remuser
	cmdbufflen+=strlen(ruserName)+1;

	strcpy(cmdbuff+cmdbufflen, cmd); // command
	cmdbufflen+=strlen(cmd)+1;

    sendsocket(cmdbuff, cmdbufflen, rshClient);

	if (receivesocket(rshClient, buff, 2047,hostname))
		return 1;

	return 0;
}


long
clientThread ()
{
    SOCKET rshServer;
    char buff[2048];
    struct sockaddr anaddr;
    int len=sizeof(anaddr);

    rshServer=accept(rshClientErr, (struct sockaddr FAR*)&anaddr, &len);
	if(rshServer==INVALID_SOCKET)
	{
		fprintf(stdout,"Error connecting to the stderr server port!");
		return 0;
	}

	stderrFlag=1; // mark connection made

	while(receivesocket(rshServer, buff, 2047,"")>0) 
		fprintf(stderr, buff);
    
        shutdown(rshClientErr, 2);
        closesocket(rshClientErr);

	return 1;
}


HANDLE threadHnd;

void
th_start (HANDLE threadHnd)
{

    initSocket();
    if(!initErrSocket())
	fprintf(stdout,"Cannot create error socket!");
	saddr.sin_family=AF_INET;
	saddr.sin_port=rshSPort;
	if(connect(rshClient, (struct sockaddr FAR*)&saddr, sizeof(saddr)))
	     fprintf(stdout,"Cannot connect to RSH port!\n");
	
	if(rshClientErr!=INVALID_SOCKET)
	{
		DWORD threadID;
		threadHnd= CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)clientThread,
			(LPVOID)0, 0, (LPDWORD)&threadID);
		if(!threadHnd)
			fprintf(stdout,"Cannot start client thread...");
	}
} 


int
th_stop(HANDLE threadHnd) 
{
    if (shutdown(rshClient, 2))
		fprintf(stdout,"could not shut down rsh client socket \n");
    closesocket(rshClient);
	if(threadHnd)
	{
		DWORD exitCode=0;
		GetExitCodeThread(threadHnd, &exitCode);
		while(exitCode==STILL_ACTIVE && stderrFlag)
		{
			Sleep(50);
			GetExitCodeThread(threadHnd, &exitCode);
		}
		CloseHandle(threadHnd);
	}
	return 0;
}


int
rsh (char *hostname, char *username, char *command,char *retresult)
{

	res=malloc(512*sizeof(char));
	strcpy(ruserName,username);
	strcpy(luserName,(char *)MyGetUserName());
	
	if (!verify_hostname(hostname)) {
		fprintf(stdout,"Could not verify hostname: No such host \n");
		return 0;
	}
	th_start(threadHnd);	
        if (!do_command(command,hostname)) {
		th_stop(threadHnd);
		return 0;
	}
	th_stop(threadHnd);
	strcpy(retresult,res);
    return(1);
}

