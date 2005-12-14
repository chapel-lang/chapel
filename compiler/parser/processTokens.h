#ifndef _PROCESSTOKENS_H_
#define _PROCESSTOKENS_H_

/* BLC: This file contains routines that help chapel.lex process
   tokens */

void processNewline(void);
char* eatStringLiteral(char* c);
void processSingleLineComment(void);
void processMultiLineComment(void);
void processWhitespace(char* tabOrSpace);
void processInvalidToken(void);

#endif

