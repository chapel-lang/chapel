#ifndef _error_H_
#define _error_H_


extern char* _chpl_input_filename;
extern int   _chpl_input_linenumber;

void printError(char* message);
void printWarning(char* message);
void printInternalError(char* message);


#endif
