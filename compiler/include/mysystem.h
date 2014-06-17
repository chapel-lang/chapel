/**************************************************************************
  Copyright (c) 2004-2013, Cray Inc.  (See LICENSE file for more details)
**************************************************************************/


#ifndef _mysystem_H_
#define _mysystem_H_

extern bool printSystemCommands;

int mysystem(const char* command, const char* description, int ignorestatus=0);

#endif
