/*   $Source: bitbucket.org:berkeleylab/gasnet.git/tests/delay.c $
 * Description: 
 * Copyright 2004, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */
#include <float.h>
#include <gasnet.h>
#include <gasnet_tools.h>

static volatile float x, y;
static volatile float z = (1.00001);

float test_bogus(void) { /* ensure the values escape (otherwise x is dead) */
 return x+y+z;
}

                                                                                                              
void test_delay (int64_t n, int pollcnt)
{
  int64_t i,j;


  y = z;
  x = 1.0;
  if (pollcnt) { /* include pollcnt AMPolls, evenly interspersed */
    int64_t n_chunk = n / (pollcnt+1);
    for (j=0; j<pollcnt; j++) {
      for (i=0; i<n_chunk; i++) { x *= y; }
      gasnet_AMPoll();
    }
    for (i=0; i<n_chunk; i++) { x *= y; }
  } else {
    for (i=0; i<n; i++) { x *= y; }
  }
}
