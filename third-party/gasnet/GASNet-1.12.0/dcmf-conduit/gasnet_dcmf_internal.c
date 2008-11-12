/*   $Source: /var/local/cvs/gasnet/dcmf-conduit/gasnet_dcmf_internal.c,v $
 *     $Date: 2008/10/28 05:43:39 $
 * $Revision: 1.2 $
 * Description: GASNet dcmf conduit Implementation
 * Copyright 2008, Rajesh Nishtala <rajeshn@cs.berkeley.edu>, 
                   Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnet_core_internal.h>

#define PRINT_NODE_INFO 1
/*preallocate request objects and tokens and put them on the free list*/

