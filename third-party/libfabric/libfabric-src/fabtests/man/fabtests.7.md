---
layout: page
title: fabtests(7)
tagline: Fabtests Programmer's Manual
---
{% include JB/setup %}

# NAME

Fabtests

# SYNOPSIS

Fabtests is a set of examples for fabric providers that demonstrates
various features of libfabric- high-performance fabric software library.

# OVERVIEW

Libfabric defines sets of interface that fabric providers can support.
The purpose of Fabtests examples is to demonstrate some of the major features.
The goal is to familiarize users with different functionalities libfabric
offers and how to use them.  Although most tests report performance numbers,
they are designed to test functionality and not performance.  The exception
are the benchmarks and ubertest.

The tests are divided into the following categories. Except the unit tests
all of them are client-server tests.  Not all providers will support each test.

The test names try to indicate the type of functionality each test is
verifying.  Although some tests work with any endpoint type, many are
restricted to verifying a single endpoint type.  These tests typically
include the endpoint type as part of the test name, such as dgram, msg, or
rdm.

# Functional

These tests are a mix of very basic functionality tests that show major
features of libfabric.

*fi_av_xfer*
: Tests communication for connectionless endpoints, as addresses
  are inserted and removed from the local address vector.

*fi_cm_data*
: Verifies exchanging CM data as part of connecting endpoints.

*fi_cq_data*
: Tranfers messages with CQ data.

*fi_dgram*
: A basic datagram endpoint example.

*fi_dgram_waitset*
: Transfers datagrams using waitsets for completion notification.

*fi_inj_complete*
: Sends messages using the FI_INJECT_COMPLETE operation flag.

*fi_mcast*
: A simple multicast test.

*fi_msg*
: A basic message endpoint example.

*fi_msg_epoll*
: Transfers messages with completion queues configured to use file
  descriptors as wait objects.  The file descriptors are retrieved
  by the program and used directly with the Linux epoll API.

*fi_msg_sockets*
: Verifies that the address assigned to a passive endpoint can be
  transitioned to an active endpoint.  This is required applications
  that need socket API semantics over RDMA implementations (e.g. rsockets).

*fi_multi_ep*
: Performs data transfers over multiple endpoints in parallel.

*fi_multi_mr*
: Issues RMA write operations to multiple memory regions, using
  completion counters of inbound writes as the notification
  mechanism.

*fi_poll*
: Exchanges data over RDM endpoints using poll sets to drive
  completion notifications.

*fi_rdm*
: A basic RDM endpoint example.

*fi_rdm_atomic*
: Test and verifies atomic operations over an RDM endpoint.

*fi_rdm_deferred_wq*
: Test triggered operations and deferred work queue support.

*fi_rdm_multi_domain*
: Performs data transfers over multiple endpoints, with each
  endpoint belonging to a different opened domain.

*fi_rdm_multi_recv*
: Transfers multiple messages over an RDM endpoint that are received
  into a single buffer, posted using the FI_MULTI_RECV flag.

*fi_rdm_rma_event*
: An RMA write example over an RDM endpoint that uses RMA events
  to notify the peer that the RMA transfer has completed.

*fi_rdm_rma_trigger*
: A basic example of queuing an RMA write operation that is initiated
  upon the firing of a triggering completion. Works with RDM endpoints.

*fi_rdm_shared_av*
: Spawns child processes to verify basic functionality of using a shared
  address vector with RDM endpoints.

*fi_rdm_tagged_peek*
: Basic test of using the FI_PEEK operation flag with tagged messages.
  Works with RDM endpoints.

*fi_recv_cancel*
: Tests canceling posted receives for tagged messages.

*fi_resmgmt_test*
: Tests the resource management enabled feature.  This verifies that the
  provider prevents applications from overrunning local and remote command
  queues and completion queues.  This corresponds to setting the domain
  attribute resource_mgmt to FI_RM_ENABLED.

*fi_scalable_ep*
: Performs data transfers over scalable endpoints, endpoints associated
  with multiple transmit and receive contexts.

*fi_shared_ctx*
: Performs data transfers between multiple endpoints, where the endpoints
  share transmit and/or receive contexts.

*fi_unexpected_msg*
: Tests the send and receive handling of unexpected tagged messages.

*fi_unmap_mem*
: Tests data transfers where the transmit buffer is mmapped and
  unmapped between each transfer, but the virtual address of the transmit
  buffer tries to remain the same.  This test is used to validate the
  correct behavior of memory registration caches.

*fi_bw*
: Performs a one-sided bandwidth test with an option for data verification.
  A sleep time on the receiving side can be enabled in order to allow
  the sender to get ahead of the receiver.

# Benchmarks

The client and the server exchange messages in either a ping-pong manner,
for pingpong named tests, or transfer messages one-way, for bw named tests.
These tests can transfer various messages sizes, with controls over which
features are used by the test, and report performance numbers.  The tests
are structured based on the benchmarks provided by OSU MPI.  They are not
guaranteed to provide the best latency or bandwidth performance numbers a
given provider or system may achieve.

*fi_dgram_pingpong*
: Latency test for datagram endpoints

*fi_msg_bw*
: Message transfer bandwidth test for connected (MSG) endpoints.

*fi_msg_pingpong*
: Message transfer latency test for connected (MSG) endpoints.

*fi_rdm_cntr_pingpong*
: Message transfer latency test for reliable-datagram (RDM) endpoints
  that uses counters as the completion mechanism.

*fi_rdm_pingpong*
: Message transfer latency test for reliable-datagram (RDM) endpoints.

*fi_rdm_tagged_bw*
: Tagged message bandwidth test for reliable-datagram (RDM) endpoints.

*fi_rdm_tagged_pingpong*
: Tagged message latency test for reliable-datagram (RDM) endpoints.

*fi_rma_bw*
: An RMA read and write bandwidth test for reliable (MSG and RDM) endpoints.

# Unit

These are simple one-sided unit tests that validate basic behavior of the API.
Because these are single system tests that do not perform data transfers their
testing scope is limited.

*fi_av_test*
: Verify address vector interfaces.

*fi_cntr_test*
: Tests counter creation and destruction.

*fi_cq_test*
: Tests completion queue creation and destruction.

*fi_dom_test*
: Tests domain creation and destruction.

*fi_eq_test*
: Tests event queue creation, destruction, and capabilities.

*fi_getinfo_test*
: Tests provider response to fi_getinfo calls with varying hints.

*fi_mr_test*
: Tests memory registration.

*fi_mr_cache_evict*
: Tests provider MR cache eviction capabilities.

# Multinode

This test runs a series of tests over multiple formats and patterns to help
validate at scale. The patterns are an all to all, one to all, all to one and
a ring. The tests also run across multiple capabilities, such as messages, rma,
atomics, and tagged messages. Currently, there is no option to run these
capabilities and patterns independently, however the test is short enough to be
all run at once.

# Ubertest

This is a comprehensive latency, bandwidth, and functionality test that can
handle a variety of test configurations.  The test is able to run a large
number of tests by iterating over a large number of test variables.  As a
result, a full ubertest run can take a significant amount of time.  Because
ubertest iterates over input variables, it relies on a test configuration
file for control, rather than extensive command line options that are used
by other fabtests.  A configuration file must be constructed for each
provider.  Example test configurations are at test_configs.

*fi_ubertest*
: This test takes a configure file as input.  The file contains a list of
  variables and their values to iterate over.  The test will run a set of
  latency, bandwidth, and functionality tests over a given provider.  It
  will perform one execution for every possible combination of all variables.
  For example, if there are 8 test variables, with 6 having 2 possible
  values and 2 having 3 possible values, ubertest will execute 576 total
  iterations of each test.

### Config file options

The following keys and respective key values may be used in the config file.

*prov_name*
: Identify the provider(s) to test.  E.g. udp, tcp, verbs,
  ofi_rxm;verbs; ofi_rxd;udp.

*test_type*
: FT_TEST_LATENCY, FT_TEST_BANDWIDTH, FT_TEST_UNIT

*test_class*
: FT_CAP_MSG, FT_CAP_TAGGED, FT_CAP_RMA, FT_CAP_ATOMIC

*class_function*
: For FT_CAP_MSG and FT_CAP_TAGGED: FT_FUNC_SEND, FT_FUNC_SENDV, FT_FUNC_SENDMSG,
  FT_FUNC_INJECT, FT_FUNC_INJECTDATA, FT_FUNC_SENDDATA

  For FT_CAP_RMA: FT_FUNC_WRITE, FT_FUNC_WRITEV, FT_FUNC_WRITEMSG,
  FT_FUNC_WRITEDATA, FT_FUNC_INJECT_WRITE, FT_FUNC_INJECT_WRITEDATA,
  FT_FUNC_READ, FT_FUNC_READV, FT_FUNC_READMSG

  For FT_CAP_ATOMIC: FT_FUNC_ATOMIC, FT_FUNC_ATOMICV, FT_FUNC_ATOMICMSG,
  FT_FUNC_INJECT_ATOMIC, FT_FUNC_FETCH_ATOMIC, FT_FUNC_FETCH_ATOMICV,
  FT_FUNC_FETCH_ATOMICMSG, FT_FUNC_COMPARE_ATOMIC, FT_FUNC_COMPARE_ATOMICV,
  FT_FUNC_COMPARE_ATOMICMSG

*constant_caps - values OR'ed together*
: FI_RMA, FI_MSG, FI_SEND, FI_RECV, FI_READ,
  FI_WRITE, FI_REMOTE_READ, FI_REMOTE_WRITE, FI_TAGGED, FI_DIRECTED_RECV

*mode - values OR'ed together*
: FI_CONTEXT, FI_RX_CQ_DATA

*ep_type*
: FI_EP_MSG, FI_EP_DGRAM, FI_EP_RDM

*comp_type*
: FT_COMP_QUEUE, FT_COMP_CNTR, FT_COMP_ALL

*av_type*
: FI_AV_MAP, FI_AV_TABLE

*eq_wait_obj*
: FI_WAIT_NONE, FI_WAIT_UNSPEC, FI_WAIT_FD, FI_WAIT_MUTEX_COND

*cq_wait_obj*
: FI_WAIT_NONE, FI_WAIT_UNSPEC, FI_WAIT_FD, FI_WAIT_MUTEX_COND

*cntr_wait_obj*
: FI_WAIT_NONE, FI_WAIT_UNSPEC, FI_WAIT_FD, FI_WAIT_MUTEX_COND

*threading*
: FI_THREAD_UNSPEC, FI_THREAD_SAFE, FI_THREAD_FID, FI_THREAD_DOMAIN,
  FI_THREAD_COMPLETION, FI_THREAD_ENDPOINT

*progress*
: FI_PROGRESS_MANUAL, FI_PROGRESS_AUTO, FI_PROGRESS_UNSPEC

*mr_mode*
: (Values OR'ed together) FI_MR_LOCAL, FI_MR_VIRT_ADDR, FI_MR_ALLOCATED,
  FI_MR_PROV_KEY

*op*
: For FT_CAP_ATOMIC: FI_MIN, FI_MAX, FI_SUM, FI_PROD, FI_LOR, FI_LAND, FI_BOR,
  FI_BAND, FI_LXOR, FI_BXOR, FI_ATOMIC_READ, FI_ATOMIC_WRITE, FI_CSWAP,
  FI_CSWAP_NE, FI_CSWAP_LE, FI_CSWAP_LT, FI_CSWAP_GE, FI_CSWAP_GT, FI_MSWAP

*datatype*
: For FT_CAP_ATOMIC: FI_INT8, FI_UINT8, FI_INT16, FI_UINT16, FI_INT32,
  FI_UINT32, FI_INT64, FI_UINT64, FI_FLOAT, FI_DOUBLE, FI_FLOAT_COMPLEX,
  FI_DOUBLE_COMPLEX, FI_LONG_DOUBLE, FI_LONG_DOUBLE_COMPLEX

*msg_flags - values OR'ed together*
: For FT_FUNC_[SEND,WRITE,READ,ATOMIC]MSG: FI_REMOTE_CQ_DATA, FI_COMPLETION

*rx_cq_bind_flags - values OR'ed together*
: FI_SELECTIVE_COMPLETION

*tx_cq_bind_flags - values OR'ed together*
: FI_SELECTIVE_COMPLETION

*rx_op_flags - values OR'ed together*
: FI_COMPLETION

*tx_op_flags - values OR'ed together*
: FI_COMPLETION

*test_flags - values OR'ed together*
: FT_FLAG_QUICKTEST

# HOW TO RUN TESTS

(1) Fabtests requires that libfabric be installed on the system, and at least one provider be usable.

(2) Install fabtests on the system. By default all the test executables are installed in /usr/bin directory unless specified otherwise.

(3) All the client-server tests have the following usage model:

	fi_<testname> [OPTIONS]		start server
	fi_<testname> <host>		connect to server

# COMMAND LINE OPTIONS

Tests share command line options where appropriate.  The following
command line options are available for one or more test.  To see which
options apply for a given test, you can use the '-h' help option to see
the list available for that test.

*-h*
: Displays help output for the test.

*-f <fabric>*
: Restrict test to the specified fabric name.

*-d <domain>*
: Restrict test to the specified domain name.

*-p <provider>*
: Restrict test to the specified provider name.

*-e <ep_type>*
: Use the specified endpoint type for the test.  Valid options are msg,
  dgram, and rdm.  The default endpoint type is rdm.

*-D <device_name>*
: Allocate data buffers on the specified device, rather than in host
  memory.  Valid options are ze and cuda.

*-a <address vector name>*
: The name of a shared address vector.  This option only applies to tests
  that support shared address vectors.

*-B <src_port>*
: Specifies the port number of the local endpoint, overriding the default.

*-P <dst_port>*
: Specifies the port number of the peer endpoint, overriding the default.

*-s <address>*
: Specifies the address of the local endpoint.

*-F <address_format>
: Specifies the address format.

*-K
: Fork a child process after initializing endpoint.

*-b[=oob_port]*
: Enables out-of-band (via sockets) address exchange and test
  synchronization.  A port for the out-of-band connection may be specified
  as part of this option to override the default.

*-E[=oob_port]*
: Enables out-of-band (via sockets) address exchange only. A port for the
  out-of-band connection may be specified as part of this option to override
  the default. Cannot be used together with the '-b' option.

*-U*
: Run fabtests with FI_DELIVERY_COMPLETE.

*-I <number>*
: Number of data transfer iterations.

*-Q*
: Associated any EQ with the domain, rather than directly with the EP.

*-w <number>*
: Number of warm-up data transfer iterations.

*-S <size>*
: Data transfer size or 'all' for a full range of sizes.  By default a
  select number of sizes will be tested.

*-l*
: If specified, the starting address of transmit and receive buffers will
  be aligned along a page boundary.

*-m*
: Use machine readable output.  This is useful for post-processing the test
  output with scripts.

*-t <comp_type>*
: Specify the type of completion mechanism to use.  Valid values are queue
  and counter.  The default is to use completion queues.

*-c <comp_method>*
: Indicate the type of processing to use checking for completed operations.
  Valid values are spin, sread, and fd.  The default is to busy wait (spin)
  until the desired operation has completed.  The sread option indicates that
  the application will invoke a blocking read call in libfabric, such as
  fi_cq_sread.  Fd indicates that the application will retrieve the native
  operating system wait object (file descriptor) and use either poll() or
  select() to block until the fd has been signaled, prior to checking for
  completions.

*-o <rma_op>*
: For RMA based tests, specify the type of RMA operation to perform.  Valid
  values are read, write, and writedata.  Write operations are the default.

*-M <mcast_addr>*
: For multicast tests, specifies the address of the multicast group to join.

*-v*
: Add data verification check to data transfers.

# USAGE EXAMPLES

## A simple example

	run server: <test_name> -p <provider_name> -s <source_addr>
		e.g.	fi_msg_rma -p sockets -s 192.168.0.123
	run client: <test_name> <server_addr> -p <provider_name>
		e.g.	fi_msg_rma 192.168.0.123 -p sockets

## An example with various options

	run server: fi_rdm_atomic -p psm -s 192.168.0.123 -I 1000 -S 1024
	run client: fi_rdm_atomic 192.168.0.123 -p psm -I 1000 -S 1024

This will run "fi_rdm_atomic" for all atomic operations with

	- PSM provider
	- 1000 iterations
	- 1024 bytes message size
	- server node as 123.168.0.123

## Run multinode tests

	Server and clients are invoked with the same command:
		fi_multinode -n <number of processes> -s <server_addr> -C <mode>

	A process on the server must be started before any of the clients can be started
	succesfully. -C lists the mode that the tests will run in. Currently the options are
  for rma and msg. If not provided, the test will default to msg.

## Run fi_ubertest

	run server: fi_ubertest
	run client: fi_ubertest -u /usr/share/fabtests/test_configs/sockets/quick.test 192.168.0.123

This will run "fi_ubertest" with

	- sockets provider
	- configurations defined in /usr/share/fabtests/test_configs/sockets/quick.test
	- server node as 192.168.0.123

The config files are provided in /test_configs for sockets, verbs, udp,
and usnic providers and distributed with fabtests installation.

For more usage options: fi_ubertest -h

## Run the whole fabtests suite

A runscript scripts/runfabtests.sh is provided that runs all the tests
in fabtests and reports the number of pass/fail/notrun.

	Usage: runfabtests.sh [OPTIONS] [provider] [host] [client]

By default if none of the options are provided, it runs all the tests using

	- sockets provider
	- 127.0.0.1 as both server and client address
	- for small number of optiond and iterations

Various options can be used to choose provider, subset tests to run,
level of verbosity etc.

	runfabtests.sh -vvv -t all psm 192.168.0.123 192.168.0.124

This will run all fabtests using

	- psm provider
	- for different options and larger iterations
	- server node as 192.168.0.123 and client node as 192.168.0.124
	- print test output for all the tests

For detailed usage options: runfabtests.sh -h
