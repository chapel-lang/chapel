#ifndef _error_H_
#define _error_H_


extern char* chpl_input_filename;
extern int   chpl_input_lineno;

void printError(char* message);
void printWarning(char* message);
void printInternalError(char* message);


#endif
