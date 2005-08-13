#ifndef _COUNTTOKENS_H_
#define _COUNTTOKENS_H_

/* BLC: This file contains routines that help keep track of statistics
   on numbers of tokens and to print out the code in a tokenized
   form */

extern bool countTokens;
extern bool printTokens;

void startCountingFileTokens(char* filename);
void stopCountingFileTokens(void);
void finishCountingTokens(void);

void countToken(char* tokentext);
void countNewline(void);
void countCommentLine(void);
void countSingleLineComment(char* comment);
void countMultiLineComment(char* comment);

#endif
