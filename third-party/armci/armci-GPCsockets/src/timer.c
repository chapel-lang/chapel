/* $Id: timer.c,v 1.3 2004-04-09 22:03:51 manoj Exp $ */
#ifdef WIN32
   static double msec;             /* reference for timer */
#  include <windows.h>
#else
#  include <sys/types.h>
#  include <sys/time.h>
   static unsigned firstsec=0;     /* Reference for timer */
   static unsigned firstusec=0;    /* Reference for timer */
#endif
static int first_call=1;

double armci_timer()
{
#ifdef WIN32
    double t0 = (double)GetCurrentTime();
    if(first_call){
        first_call=0; msec=t0; return 0.0;
    }
    t0 -=msec;
    if(t0<0.0)t0 += (double)0xffffffff;
    return 0.01*t0;
#else
    double low, high;
    struct timeval tp;
    struct timezone tzp;
    (void) gettimeofday(&tp,&tzp);

    if (first_call) {
         firstsec = tp.tv_sec;
         firstusec = tp.tv_usec;
         first_call = 0;
    }
    low = (double)(tp.tv_usec>>1) - (double) (firstusec>>1);
    high = (double) (tp.tv_sec - firstsec);
    return high + 1.0e-6*(low+low);
#endif
}

