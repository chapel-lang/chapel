#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <stdio.h>

#include "net/net.h"
#include "net/driver.h"

#include "qthread_innards.h"

static void
net_cleanup(void)
{
    fprintf(stderr, "begin net_fini\n");
    qthread_internal_net_driver_finalize();
    fprintf(stderr, "end net_fini\n");
}

int
qthread_internal_net_initialize(void)
{
    int ret;

    fprintf(stderr, "begin net_init\n");

    /* initialize the network driver */
    ret = qthread_internal_net_driver_initialize();
    if (0 != ret)  {
        fprintf(stderr, "qthread_internal_net_driver_init failed: %d\n", ret);
        return ret;
    }

    /* make sure we can clean up */
    qthread_internal_cleanup(net_cleanup);
    fprintf(stderr, "end net_init\n");

    return 0;
}

int
qthread_internal_net_get_rank(void)
{
    return qthread_internal_net_driver_get_rank();
}


int
qthread_internal_net_get_size(void)
{
    return qthread_internal_net_driver_get_size();
}
