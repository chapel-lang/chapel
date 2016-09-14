#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <qthread/qthread.h>
#include "argparsing.h"

#define MAX(a,b) a >= b ? a : b

static const unsigned int non_team_id     = 0;
static const unsigned int default_team_id = 1;

static aligned_t ping(void *args_)
{
    int *distance = (int *)args_;

    unsigned int id = qt_team_id();
    unsigned int parent_id = qt_team_parent_id();

    iprintf("Ping from %lu to %lu at distance %d\n", (unsigned long)parent_id,
        (unsigned long)id, *distance);
    assert(id != non_team_id && id != default_team_id);
    assert(parent_id != non_team_id);

    *distance -= 1;
    if (*distance > 0) {
        aligned_t pong;
        qthread_fork_new_subteam(ping, distance, &pong);
        qthread_readFF(NULL, &pong);
    }
   
    return 0;
}

/******************************************************************************/
int main(int   argc,
         char *argv[])
{
    int distance = 1;
    NUMARG(distance, "DISTANCE");
    assert(distance > 0);

    assert(qthread_initialize() == 0);

    CHECK_VERBOSE();

    iprintf("Main executing in team %lu (w/ parent %lu)\n", 
        (unsigned long)qt_team_id(), (unsigned long)qt_team_parent_id());
    assert(qt_team_id() == default_team_id);
    assert(qt_team_parent_id() == non_team_id);


    // Spawn a task into a new subteam, no eureka
    {
        aligned_t pong;
        qthread_fork_new_subteam(ping, &distance, &pong);
        qthread_readFF(NULL, &pong);
    }

    if (distance == 0) {
        iprintf("SUCCEEDED\n");
        return 0;
    } else {
        iprintf("FAILED with distance %d\n", distance);
        return 1;
    }
}

/* vim:set expandtab */
