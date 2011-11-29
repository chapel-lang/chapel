#ifndef _MODULEFINDER_H_
#define _MODULEFINDER_H_

extern int depth;
extern int filemodule;

void registerModule(char* modName, int depth);
void registerTopLevelCode(void);

#endif
