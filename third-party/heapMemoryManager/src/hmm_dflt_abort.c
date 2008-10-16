/* This code is in the public domain.
** Version: 1.1  Author: Walt Karas
*/

/* The function in this file performs default actions if self-auditing
** finds heap corruption.  Don't rely my feeble attempt to handle the
** case where HMM is being used to implement the malloc and free standard
** library functions.  Rewrite the function if necessary to avoid using
** I/O and execution termination functions that call malloc or free.
** In Unix, for example, you would replace the fputs calls with calls
** to the write system call using file handle number 2.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static int entered = 0;

/* Print abort message, file and line.  Terminate execution.
*/
void HMM_dflt_abort(const char *file, const char *line)
  {
    /* Avoid use of printf(), which is more likely to use heap. */

    if (entered)
      /* The standard I/O functions called a heap function and caused
      ** an indirect recursive call to this function.  So we'll have
      ** to just exit without printing a message.  */
      _exit(1);

    entered = 1;

    fputs("\nABORT - Heap corruption\n" "File: ", stderr);
    fputs(file, stderr);
    fputs("  Line: ", stderr);
    fputs(line, stderr);
    fputs("\n\n", stderr);
    fflush(stderr);

    _exit(1);
  }
