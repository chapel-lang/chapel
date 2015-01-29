#ifndef _mysystem_H_
#define _mysystem_H_

extern bool printSystemCommands;

int mysystem(const char* command, 
             const char* description, 
             bool        ignorestatus = false);

#endif
