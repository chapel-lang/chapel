---
layout: page
title: fi_strerror(1)
tagline: Libfabric Programmer's Manual
---
{% include JB/setup %}

# NAME

fi_strerror \- display libfabric error strings

# SYNOPSIS

```
fi_strerror FI_ERROR_CODE
```

# DESCRIPTION

Display the error string for the given numeric `FI_ERROR_CODE`.
`FI_ERROR_CODE` may be a hexadecimal, octal, or decimal constant.  Although
the [`fi_strerror`(3)](fi_errno.3.html) library function only accepts positive
error values, for convenience this utility accepts both positive and negative
error values.

This is primarily a convenience tool for developers.

# SEE ALSO

[`fabric`(7)](fabric.7.html)
[`fi_errno`(3)](fi_errno.3.html)
