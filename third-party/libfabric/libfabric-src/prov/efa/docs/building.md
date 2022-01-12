## Building the EFA Libfabric Provider

This document describes how to build the Libfabric provider once you've
followed the prerequisite steps to install required software, see the overview
doc if you are unsure what's needed.

An example of building and installing Libfabric and verifying that the EFA
device is available via libfabric:
```
$ ./autogen.sh
$ ./configure --enable-efa=<path to rdma-core install> --prefix=$PWD/install
$ make -j install
$ ./install/bin/fi_info -p efa
provider: efa
    fabric: EFA-fe80::df:57ff:fe1a:beb3
    domain: efa_0-rdm
    version: 112.0
    type: FI_EP_RDM
    protocol: FI_PROTO_EFA
provider: efa
    fabric: EFA-fe80::df:57ff:fe1a:beb3
    domain: efa_0-dgrm
    version: 112.0
    type: FI_EP_DGRAM
    protocol: FI_PROTO_EFA
```

Configure flags that may be useful in the context of the EFA provider:

* `--enable-debug`: will turn on `FI_LOG_LEVEL=debug`, add `-g` among others to
CFLAGS (see configure.ac for full list), and compile in some extra data
structures that may be helpful for debugging. Note that debug will likely
impact performance. See `ENABLE_DEBUG` in the code.
* `--enable-efa`: allows you to specify the rdma-core install path which is
needed if rdma-core is not in the default paths. Also allows you to compile the
provider as a shared library.
* `--enable-efa-mem-poisoning`: Write a poison value into memory structures after
they are freed. This has a performance overhead like debug. See
`ENABLE_EFA_POISONING` in the code.
* `--with-cuda`: Build Libfabric with cuda support (if cuda libraries are not in
the default path). The EFA provider supports sends/RDMA reads with GPUDirect
via FI_HMEM when Libfabric has CUDA support enabled.
* `--with-gdrcopy`: Build Libfabric with the NVIDIA GDRCopy library enabled. If
not enabled the EFA provider will have to utilize the EFA device (via a
loopback read) to copy receives in the bounce buffers (host memory) matched to
GPU memory.

CFLAGS that might be useful:

* `RXR_PERF_ENABLED`: enable the perf hooks to determine cycle/instruction count
for functions in the send/receive/completion paths. See fi_hook(7) and the
Linux perf documentation for more information.
* `ENABLE_RXR_PKT_DUMP`: turn on packet dump prints, very verbose. These
functions haven't been kept up to date with recent protocol changes so this
might not be useful until fixed.
