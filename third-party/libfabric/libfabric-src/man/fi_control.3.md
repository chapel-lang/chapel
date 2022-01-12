---
layout: page
title: fi_control(3)
tagline: Libfabric Programmer's Manual
---
{% include JB/setup %}

# NAME

fi_control \- Perform an operation on a fabric resource.

# SYNOPSIS

```c
#include <rdma/fabric.h>

int fi_control(struct fid *fid, int command, void *arg);
int fi_alias(struct fid *fid, struct fid **alias_fid, uint64_t flags);
int fi_get_val(struct fid *fid, int name, void *val);
int fi_set_val(struct fid *fid, int name, void *val);
```


# ARGUMENTS

*fid*
: Fabric resource

*command*
: Operation to perform

*arg*
: Optional argument to the command

# DESCRIPTION

The fi_control operation is used to perform one or more operations on a
fabric resource.  Conceptually, fi_control is similar to the POSIX fcntl
routine.  The exact behavior of using fi_control depends on the fabric
resource being operated on, the specified command, and any provided
arguments for the command.  For specific details, see the fabric resource
specific help pages noted below.

fi_alias, fi_get_val, and fi_set_val are wrappers for fi_control with
commands FI_ALIAS, FI_GET_VAL, FI_SET_VAL, respectively. fi_alias creates
an alias of the specified fabric resource. fi_get_val reads the value of
the named parameter associated with the fabric resource, while fi_set_val
updates that value. Available parameter names depend on the type of the
fabric resource and the provider in use. Providers may define provider
specific names in the provider extension header files ('rdma/fi_ext_*.h').
Please refer to the provider man pages for details. 

# SEE ALSO

[`fi_endpoint`(3)](fi_endpoint.3.html),
[`fi_cm`(3)](fi_cm.3.html),
[`fi_cntr`(3)](fi_cntr.3.html),
[`fi_cq`(3)](fi_cq.3.html),
[`fi_eq`(3)](fi_eq.3.html),
