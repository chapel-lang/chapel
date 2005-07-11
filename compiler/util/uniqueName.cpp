#include "uniqueName.h"

void UniqueName::inc()
{
  char* ns = (char*) malloc((strlen(newstr) + 2)*sizeof(char));
  ns = strcpy(ns, newstr);
  ns = strcat(ns, newchar);
  newstr = ns;
}
char* UniqueName::uniqueStr() {
  return newstr;
}

char* UniqueName::uniqueStr(char* another) {
  char* sub1 = strstr(another, newstr);
  char* sub2 = sub1;
  while (sub2){
    inc();
    sub2 = strstr(sub1 + 1, newstr);
  }
  return newstr;
}

void UniqueName::resetStr()
{
  newstr = strcpy(newstr, start);
}

char* UniqueName::newName() {
  char* nn = (char*) malloc((2*(strlen(newstr) + 1) + strlen(name) + 1)*sizeof(char));
  nn = strcpy(nn, newstr);
  nn = strcat(nn, name);
  nn = strcat(nn, newstr);
  return nn;
}
