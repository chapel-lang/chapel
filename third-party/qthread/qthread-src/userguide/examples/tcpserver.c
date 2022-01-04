#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <linux/sched.h>
#include <sched.h>
#include <qthread/qthread.h>

#define BUFSIZE 1024
#define PORT 5000

void error(char *msg) {
  perror(msg);
  exit(1);
}

int fib(int x){return x < 2 ? 1 : fib(x-1) + fib(x-2);}

typedef struct {
    int x; 
    int childfd;
} argtype;

aligned_t process(void* arg){
    argtype *a = (argtype*) arg;
    char buf[BUFSIZE];
    sprintf(buf, "fib %d: %d\n", a->x, fib(a->x));
    write(a->childfd, buf, strlen(buf));
    return 0;
}

static void handleRequest(int childfd){
    char buf[BUFSIZE];
    while(1){
        bzero(buf, BUFSIZE);
        if(read(childfd, buf, BUFSIZE) == 0) break;
        printf("Working on %s\n", buf);
        argtype *newArg = malloc(sizeof(argtype));
        newArg->x = atoi(buf);
        newArg->childfd = childfd;
        //if(fork() == 0) process(newArg);
        qthread_fork(process, (void*)newArg, NULL);
        qthread_flushsc();
    }
}

int main(int argc, char **argv) {
    qthread_initialize();
    int parentfd, childfd;
    struct sockaddr_in serveraddr; /* server's addr */
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(PORT);
     
    parentfd = socket(AF_INET, SOCK_STREAM, 0);
    int optval = 1;
    setsockopt(parentfd, SOL_SOCKET, SO_REUSEADDR, 
               (const void *)&optval , sizeof(int));

    if (parentfd < 0) 
        error("ERROR opening socket");

    if (bind(parentfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0) 
        error("ERROR on binding");

    if (listen(parentfd, 1024) < 0)
        error("ERROR on listen");

    while (1) {
        childfd = accept(parentfd, NULL, NULL);
        if (childfd < 0) error("ERROR on accept");
        handleRequest(childfd);
    }
    return 0;
}
