{
#include "g7.test.g.d_parser.h"
int myscanner(char **s, int *col, int *line, unsigned short *symbol, 
      int *term_priority, unsigned char *op_assoc, int *op_priority) 
{
  if (**s == 'a') {
    (*s)++;
    *symbol = A;
    return 1;
  } else if (**s == 'b') {
    (*s)++;
    *symbol = BB;
    return 1;
  } else if (**s == 'c') {
    (*s)++;
    *symbol = CCC;
    return 1;
  } else if (**s == 'd') {
    (*s)++;
    *symbol = DDDD;
    return 1;
  } else
    return 0;
}

}
${scanner myscanner}
${token A BB CCC DDDD}

S: A (BB CCC)+ SS;
SS: DDDD;
