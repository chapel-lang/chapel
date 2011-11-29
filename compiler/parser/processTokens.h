#ifndef _PROCESSTOKENS_H_
#define _PROCESSTOKENS_H_

/* BLC: This file contains routines that help chapel.lex process
   tokens */

void processNewline(void);
char* eatStringLiteral(const char* c);
void processSingleLineComment(void);
void processMultiLineComment(void);
void processWhitespace(const char* tabOrSpace);
void processInvalidToken(void);

#endif

