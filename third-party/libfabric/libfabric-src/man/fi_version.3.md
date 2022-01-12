---
layout: page
title: fi_version(3)
tagline: Libfabric Programmer's Manual
---
{% include JB/setup %}

# NAME

fi_version \- Version of the library interfaces

# SYNOPSIS

```c
#include <rdma/fabric.h>

uint32_t fi_version(void);

FI_MAJOR(version)

FI_MINOR(version)
```

# DESCRIPTION

This call returns the current version of the library interfaces.  The
version includes major and minor numbers.  These may be extracted
from the returned value using the FI_MAJOR() and FI_MINOR() macros.

# NOTES

The library may support older versions of the interfaces.

# RETURN VALUE

Returns the current library version.  The upper 16-bits of the version
correspond to the major number, and the lower 16-bits correspond with
the minor number.

# SEE ALSO

[`fabric`(7)](fabric.7.html),
[`fi_getinfo`(3)](fi_getinfo.3.html)
