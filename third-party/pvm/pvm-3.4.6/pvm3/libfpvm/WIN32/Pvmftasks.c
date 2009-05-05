
/* $Id: Pvmftasks.c,v 1.2 1998/09/21 20:27:47 pvmsrc Exp $ */

#ifdef WIN32
#include "..\..\include\pvm3.h"
#include "..\..\src\pvmwin.h"
#else 
#include "pvm3.h"
#endif

#include "pvm_consts.h"
#include "../../src/bfunc.h"

void 
#ifdef IMA_WIN32_WATCOM
__fortran PVMFTASKS
#else
pvmftasks
#endif
   (where,
      ntaskp,
      tidp,
      ptidp,
      hostp,
      flagp,
      a_out_ptr,a_out_len,
      infop)

   int  *where, *ntaskp, *tidp, *ptidp, *hostp, *flagp, *infop;
   char * a_out_ptr; int a_out_len;
{
   static struct pvmtaskinfo *tip = 0;
   static int ntask = 0;
   static int next = 0;

   int cc;
   struct pvmtaskinfo *tip2;

#ifndef WIN32
   char *malloc();
#endif
/* if user sets ntaskp to -1 then pvmftasks() will be reset */

    if (tip && (*ntaskp == -1)) {
        free((char*)tip);
        tip = 0;
    }

   if (!tip) {
      if ((cc = pvm_tasks(*where, &ntask, &tip2)) < 0) {
         *infop = cc;
         return;
      }
      tip = (struct pvmtaskinfo*)malloc(ntask * sizeof(struct pvmtaskinfo));
      BCOPY((char*)tip2, (char*)tip, ntask * sizeof(struct pvmtaskinfo));
      next = 0;
   }

   if (next < ntask) {
      *ntaskp = ntask;
      *tidp = tip[next].ti_tid;
      *ptidp = tip[next].ti_ptid;
      *hostp = tip[next].ti_host;
      *flagp = tip[next].ti_flag;
      ctofstr(a_out_ptr, a_out_len, tip[next].ti_a_out);
      *infop = 1;
      next++;
   }

/* can not return ti_pid without changing interface */
/* users needing this will have to use C            */

   if (next == ntask) {
      free((char*)tip);
      tip = 0;
   }
}

