// a logrithmic self-cleaning(???I hope it will be) arrive first detector
// -- requires the size to be knownn
// Follows a strategy based on the barrier lock - 5/25/09 akp

#include <stddef.h>		       // for size_t (C89)
#include <qthread/qthread-int.h>       // for int64_t
#include <stdlib.h>		       // for calloc()
#include <stdio.h>		       // for perror()

#include <qthread/qloop.h>
#include <qthread/barrier.h>
#include "qt_arrive_first.h"
#include "qt_asserts.h"
#include "qloop_innards.h"	       /* for cnbWorkers_ and cnbTimeMin_ */
#include "qt_subsystems.h"             /* for qthread_internal_cleanup() */
#include "qt_debug.h"	               // for qthread_debug()

static qt_arrive_first_t *qt_arrive_first_create(int size, qt_barrier_btype type, int debug);
static void qt_arrive_first_destroy(qt_arrive_first_t * b);
static void qtar_internal_initialize_fixed(qt_arrive_first_t * b, size_t size, int debug);
static int64_t qt_arrive_first_enter(qt_arrive_first_t * b, qthread_shepherd_id_t shep);

qt_arrive_first_t *MArrFirst = NULL;

void qtar_resize(aligned_t size){
  qt_global_arrive_first_destroy();
  qt_global_arrive_first_init(size-1, 0);  // the size to resize is the number of threads (1 based)
                 // the size to first init is for the max thread number(0 based) -- so subtract one
}

aligned_t qtar_size(void){
  if(MArrFirst) return MArrFirst->activeSize;
  else return 0;
}

// dump function for debugging -  print list of occupied data
void qtar_dump(qt_arrive_first_t * b)
{
  int notClean = 0;
  printf ("\tdump arrive first data \n");
  if (!b->present) printf("\tarray is not allocated\n");
  else {
    int i = -1;
    for (i = 0; i <= b->doneLevel; i++) {
      if (!b->present[i]) printf ("\tarray row %d expected but not allocated \n", i);
      else {
	int j = -1;
	int firstTime = 0;
	int count = 0;
	for (j = 0; j <= b->activeSize; j++) {
	  if (b->present[i][j]){
	    if (firstTime == 0){
	      // first found -- print identification
	      printf("\tlevel %d occupied --", i);
	      firstTime = 1;
	    }
	    count++;
	    notClean = 1; // something was printed
	    printf (" %d",j);
	    if (count > 20){
	      // found too many need new line
	      printf("\n");
	      count = 0;
	    }
	  }
	}
	if (count > 0) printf("\n");  // finish up last line
      }
    }
    if (notClean == 0) printf("\tarrive first data was ready for use\n");
  }
}

static void qtar_internal_initialize_fixed(
    qt_arrive_first_t * b,
    size_t size,
    int debug)
{				       /*{{{ */
    int i;
    int depth = 1;
    int temp = size;

    assert(b);
    b->activeSize = size;
    //    b->arriveFirstDebug = (char)debug;
    b->arriveFirstDebug = 0;
    b->nestLock = 0;

    if (size < 1) {
	return;
    }
    // compute size of barrier arrays
    temp >>= 1;
    while (temp) {
	temp >>= 1;
	depth++;		       // how many bits set
    }

    // need to watch when max depth completely full and increment it to one bigger
    if (size >= ((1<<depth)-2)) depth++;

    b->doneLevel = depth;
    b->allocatedSize = (2 << depth);

    // allocate and init upLock and downLock arrays
    b->present = calloc(depth+1, sizeof(int64_t));
    for (i = 0; i <= depth; i++) {
	b->present[i] = calloc(b->allocatedSize+1, sizeof(int64_t));
    }
}				       /*}}} */

static qt_arrive_first_t *qt_arrive_first_create(
    int size,
    qt_barrier_btype type,
    int debug)
{				       /*{{{ */
    qt_arrive_first_t *b = calloc(1, sizeof(qt_arrive_first_t));

    qthread_debug(XOMP_CALLS,
		  "qt_arrive_first_create:size(%i), type(%i), debug(%i): begin\n",
		  size, (int)type, debug);
    assert(b);
    if (b) {
	assert(type == REGION_BARRIER);
	switch (type) {
	    case REGION_BARRIER:
		qtar_internal_initialize_fixed(b, size, debug);
		break;
	    default:
		perror("qt_arrive_first must be of type REGION_BARRIER");
		break;
	}
    }
    return b;
}				       /*}}} */

static void qt_arrive_first_destroy(
    qt_arrive_first_t * b)
{				       /*{{{ */
    assert(b);
    if (b->present) {
	int i;
	for (i = 0; i <= b->doneLevel; i++) {
	    if (b->present[i]) {
		free((void *)(b->present[i]));
		b->present[i] = NULL;
	    }
	}
	free((void *)(b->present));
	b->present = NULL;
    }
    free(b);
}				       /*}}} */

// walk up the psuedo barrier -- waits if neighbor beat you and
// value has not arrived yet.

static int64_t qtar_internal_up(
    qt_arrive_first_t * b,
    int myLock,
    int level)
{				       /*{{{ */
    // compute neighbor node at this level
    int64_t t = 0;
    int mask = 1 << level;
    int pairedLock = myLock ^ mask;
    int nextLevelLock = (myLock < pairedLock) ? myLock : pairedLock;
    if (b->activeSize == 0) return 1; // only one thread -- It's first by definition
    assert(b->activeSize > 1);

    qthread_debug(XOMP_CALLS,
		  "on lock %d paired with %d level %d lock value %ld  paired %ld\n",
		  myLock, pairedLock, level, b->present[level][myLock],
		  b->present[level][pairedLock]);
		  
    int lk = 0;
    lk = qthread_incr(&b->present[level][nextLevelLock], 1);    // mark me as present

    if (lk == 0) {		       // I'm first continue up
      if (pairedLock > b->activeSize) {
	// continue up
	if (level != b->doneLevel) {
	  t = qtar_internal_up(b, nextLevelLock, level + 1);
	  b->present[level][nextLevelLock] = 0;
	  return t; 
	} else if (myLock == 0) {
	  qthread_debug(XOMP_CALLS, "First arrived %d\n", myLock);
	  b->present[level][nextLevelLock] = 0;
	  return 1;
	}
      }
      else if ((level + 1) <= b->doneLevel) {	// done? -- more to check 
	t = qtar_internal_up(b, nextLevelLock, level + 1);
	return t;
      }
    } 
    b->present[level][nextLevelLock] = 0;
    return 0;     // someone else is first
}				       /*}}} */

// actual arrive first entry point

static int64_t qt_arrive_first_enter(
    qt_arrive_first_t * b,
    qthread_shepherd_id_t shep)
{				       /*{{{ */
    int64_t t = qtar_internal_up(b, shep, 0);
    return t;
}				       /*}}} */

int64_t qt_global_arrive_first(
    const qthread_shepherd_id_t shep)
{				       /*{{{ */
    int64_t t;
    t = qt_arrive_first_enter(MArrFirst, shep);
    size_t myid = qthread_barrier_id();
    qt_barrier_enter_id(qt_thread_barrier(),myid);
    return t;
}				       /*}}} */


// allow initization from C
void qt_global_arrive_first_init(
    int size,
    int debug)
{				       /*{{{ */

    if (MArrFirst == NULL) {
	*cnbWorkers_() = qthread_num_workers();
	*cnbTimeMin_() = 1.0;
	MArrFirst = qt_arrive_first_create(size, REGION_BARRIER, debug);
	assert(MArrFirst);
	qthread_internal_cleanup(qt_global_arrive_first_destroy);
    }
}				       /*}}} */

void qt_global_arrive_first_destroy(
    void)
{				       /*{{{ */
    if (MArrFirst) {
	qt_arrive_first_destroy(MArrFirst);
	MArrFirst = NULL;
    }
}				       /*}}} */
