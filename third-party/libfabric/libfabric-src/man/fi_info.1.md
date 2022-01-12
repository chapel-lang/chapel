---
layout: page
title: fi_info(1)
tagline: Libfabric Programmer's Manual
---
{% include JB/setup %}


# NAME

fi_info  \- Simple utility to query for fabric interfaces


# SYNOPSIS
```
 fi_info [OPTIONS]
```

# DESCRIPTION

The fi_info utility can be used to query for available fabric interfaces. The
utility supports filtering based on a number of options such as endpoint type,
provider name, or supported modes. Additionally, fi_info can also be used to
discover the environment variables that can be used to tune provider specific
parameters. If no filters are specified, then all available fabric interfaces
for all providers and endpoint types will be returned.

# OPTIONS

## Filtering

*-n, --node=\<NAME\>*
: Node name or address used to filter interfaces. Only interfaces which can
reach the given node or address will respond.

*-P, --port=\<PORT\>*
: Port number used to filter interfaces.

*-c, --caps=\<CAP1|CAP2\>..*
: Pipe separated list of capabilities used to filter interfaces. Only
interfaces supporting all of the given capabilities will respond. For more
information on capabilities, see fi_getinfo(3).

*-m, --mode=\<MOD1|MOD2\>..*
: Pipe separated list of modes used to filter interfaces. Only interfaces
supporting all of the given modes will respond. For more information on, modes
see fi_getinfo(3).

*-t, --ep_type=\<EPTYPE\>*
: Specifies the type of fabric interface communication desired. For example,
specifying FI_EP_DGRAM would return only interfaces which support unreliable
datagram. For more information on endpoint types, see fi_endpoint(3).

*-a, --addr_format=\<FMT\>*
: Filter fabric interfaces by their address format. For example,
specifying FI_SOCKADDR_IN would return only interfaces which use sockaddr_in
structures for addressing. For more information on address formats, see
fi_getinfo(3).

*-p, --provider=\<PROV\>*
: Filter fabric interfaces by the provider implementation. For a list of
providers, see the `--list` option.

*-d, --domain=\<DOMAIN\>*
: Filter interfaces to only those with the given domain name.

*-f, --fabric=\<FABRIC\>*
: Filter interfaces to only those with the given fabric name.

## Discovery

*-e, --env*
: List libfabric related environment variables which can be used to enable extra
configuration or tuning.

*-g [filter]
: Same as -e option, with output limited to environment variables containing
filter as a substring.

*-l, --list*
: List available libfabric providers.

*-v, --verbose*
: By default, fi_info will display a summary of each of the interfaces
discovered. If the verbose option is enabled, then all of the contents of the
fi_info structure are displayed. For more information on the data contained in
the fi_info structure, see fi_getinfo(3).

*--version*
: Display versioning information.

# USAGE EXAMPLES

```
$ fi_info -n 30.0.11.1 -p usnic -t FI_EP_DGRAM
```

This will respond with all fabric interfaces that can reach address 30.0.11.1
using endpoint type FI_EP_DGRAM with the usNIC provider.

# OUTPUT

By default fi_info will output a summary of the fabric interfaces discovered:

```
$ ./fi_info -n 30.0.11.1 -p usnic -t FI_EP_DGRAM
provider: usnic
    fabric: 30.0.11.0/24
    domain: usnic_2
    version: 1.0
    type: FI_EP_DGRAM
    protocol: FI_PROTO_UDP
```

To see the full fi_info structure, specify the `-v` option.

# SEE ALSO

[`fi_getinfo(3)`](fi_getinfo.3.html),
[`fi_endpoint(3)`](fi_endpoint.3.html)
