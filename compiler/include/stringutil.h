#ifndef _stringutil_H_
#define _stringutil_H_

char *dupstr(const char *s, const char *e = 0);
int stringlen(const char* s1);
int stringlen(const char* s1, const char* s2, const char* s3=NULL, 
              const char* s4 = NULL, const char* s5 = NULL);


char* stringcpy(const char* s1);
char* stringcat(const char* s1, const char* s2 = "", const char* s3 = "", 
                const char* s4 = "", const char* s5 = "");
char* stringinsert(const char* origstr, const char* insertpt, const char* newstr);

char* intstring(int i);

#endif
