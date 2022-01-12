## EFA Libfabric Provider Documentation

The EFA Libfabric provider supports the Amazon Elastic Fabric Adapter (EFA), an
OS bypass network interface available on Amazon EC2 instances. The EFA device
supports both reliable and unreliable datagram send and receive semantics, the
EFA Libfabric provider adds additional functionality in software such as tag
matching, reordering, and software emulation for features the hardware does not
support natively. EFA provides lower and more consistent latency and higher
throughput compared to TCP transports which provides better application
performance for HPC and Machine Learning applications on Amazon EC2.

Please see the [fi_efa(7) man
page](https://ofiwg.github.io/libfabric/master/man/fi_efa.7.html) for more
information on the features and capabilities of the EFA Libfabric provider.

### Background information

The EFA developer documentation assumes a working knowledge of OS bypass
networking and the Libfabric API. The [OFI Programmer's
Guide](https://github.com/ofiwg/ofi-guide/blob/master/OFIGuide.md) provides
motivation for Libfabric and defines the API and structures used by Libfabric
applications.

For more information on EFA, SRD and the [AWS Nitro
System](https://aws.amazon.com/ec2/nitro/), please refer to these resources:

* [A Cloud-Optimized Transport Protocol for Elastic and Scalable
  HPC](https://ieeexplore.ieee.org/document/91673990) whitepaper
* [AWS re:Invent 2019 - Monday Night Live with Peter
  DeSantis](https://www.youtube.com/watch?v=GPUWATKe15E&feature=youtu.be&t=228)
 keynote
* [HPC Application Scaling with Elastic Fabric Adapter (EFA) and Scalable
  Reliable Datagram
  (SRD)](https://pages.awscloud.com/HPC-Application-Scaling-with-Elastic-Fabric-Adapter-EFA-and-Scalable-Reliable-Datagram-SRD_2020_0004-CMP_OD.html)
  tech talk

### Getting started with EFA Libfabric provider development

You will need an Amazon EC2 instance which has EFA support. The [EFA getting
started guide](https://docs.aws.amazon.com/AWSEC2/latest/UserGuide/efa.html) in
the AWS documentation is a good reference on which Amazon EC2 instances support
EFA and how to setup an EFA enabled instance.

To get started with EFA Libfabric provider development, you will need to either
install a pre-packaged version or build the:

* [EFA kernel
  driver](https://github.com/amzn/amzn-drivers/tree/master/kernel/linux/efa) - The
  driver is required to enable the EFA device and is utilized by libibverbs and
  Libfabric to setup and teardown device resources such as queue pairs,
  completion queues, memory registration, and address handles. Some OS
  distributions provide an up-to-date version of the EFA kernel driver such as
  Amazon Linux 2 and Ubuntu.
* [rdma-core](https://github.com/linux-rdma/rdma-core) - The EFA Libfabric
  provider utilizes the libibverbs library which provides an abstraction layer
  for the Linux kernel verbs interface. This avoids tightly coupling the
  Libfabric provider to the EFA kernel driver and simplifies the Libfabric
  provider. Similar to the driver, there are OS distributions that pre-package
  rdma-core. EFA device support was added to rdma-core version 24.0. However,
  it's best to use the latest rdma-core release for bugfixes and to support the
  latest device features.
* Ensure you have configured your instance to increase the locked memory limits
  (unlimited is fine) and set aside [huge
  pages](https://www.kernel.org/doc/Documentation/vm/hugetlbpage.txt) for the best
  performance. The EFA Libfabric provider will utilize these for the bounce
  buffers used for sends and matching receives.
* `FI_HMEM` and CUDA support for NVIDIA GPUDirect + EFA is available, see the
  `--with-cuda` configure flag. GPUDirect will be enabled by default if CUDA is
  installed in the default system paths.
