---
layout: page
title: fi_hook(7)
tagline: Libfabric Programmer's Manual
---
{% include JB/setup %}

# NAME

fi_hook \- The Hook Fabric Provider Utility

# OVERVIEW

The hooking provider is a utility function that can intercept calls to any
provider.  The hook provider is always available, but has zero impact on
calls unless enabled.  It is useful for providing performance data on
selected calls or debugging information.

# SUPPORTED FEATURES

Hooking support is enabled through the FI_HOOK environment variable.  To
enable hooking, FI_HOOK must be set to the name of one or more of the
available hooking providers.  When multiple hooks are specified, the
names must be separated by a semi-colon.  To obtain a list of hooking
providers available on the current system, one can use the fi_info
utility with the '--env' command line option.  Hooking providers are
usually identified by 'hook' appearing in the provider name.

Known hooking providers include the following:

*ofi_hook_perf*
: This hooks 'fast path' data operation calls.  Performance data is
  captured on call entrance and exit, in order to provide an average of
  how long each call takes to complete.  See the PERFORMANCE HOOKS section
  for available performance data.

# PERFORMANCE HOOKS

The hook provider allows capturing inline performance data by accessing the
CPU Performance Management Unit (PMU).  PMU data is only available on Linux
systems.  Additionally, access to PMU data may be restricted to privileged
(super-user) applications.

Performance data is captured for critical data transfer calls:
fi_msg, fi_rma, fi_tagged, fi_cq, and fi_cntr.  Captured data is displayed
as logged data using the FI_LOG_LEVEL trace level.  Performance data is
logged when the associated fabric is destroyed.

The environment variable FI_PERF_CNTR is used to identify which performance
counter is tracked.  The following counters are available:

*cpu_cycles*
: Counts the number of CPU cycles each function takes to complete.

*cpu_instr*
: Counts the number of CPU instructions each function takes to complete.
  This is the default performance counter if none is specified.

# LIMITATIONS

Hooking functionality is not available for providers built using the
FI_FABRIC_DIRECT feature.  That is, directly linking to a provider prevents
hooking.

The hooking provider does not work with triggered operations.  Application
that use FI_TRIGGER operations that attempt to hook calls will likely crash.

# SEE ALSO

[`fabric`(7)](fabric.7.html),
[`fi_provider`(7)](fi_provider.7.html)
