#ifndef _mysystem_H_
#define _mysystem_H_

extern bool printSystemCommands;

int mysystem(const char* command, char* description, int ignorestatus=0);

#endif
