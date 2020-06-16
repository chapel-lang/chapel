#
# Copyright (c) 2016 Cisco Systems, Inc.  All rights reserved.
# Copyright (c) 2017-2018 Intel Corporation, Inc. All right reserved.
# Copyright (c) 2018 Amazon.com, Inc. or its affiliates. All rights reserved.
#
# Makefile.am for libfabric

AM_CPPFLAGS = \
	-I$(srcdir)/include \
	-D_GNU_SOURCE -D__USE_XOPEN2K8 \
	-DSYSCONFDIR=\"$(sysconfdir)\" \
	-DRDMADIR=\"@rdmadir@\" \
	-DPROVDLDIR=\"$(pkglibdir)\"

noinst_LTLIBRARIES =
lib_LTLIBRARIES =

if EMBEDDED
noinst_LTLIBRARIES += src/libfabric.la
else
lib_LTLIBRARIES += src/libfabric.la
endif

pkglib_LTLIBRARIES = $(DL_PROVIDERS)


ACLOCAL_AMFLAGS = -I config
AM_CFLAGS = -Wall

if HAVE_LD_VERSION_SCRIPT
    libfabric_version_script = -Wl,--version-script=$(builddir)/libfabric.map
else !HAVE_LD_VERSION_SCRIPT
    libfabric_version_script =
endif !HAVE_LD_VERSION_SCRIPT

rdmaincludedir = $(includedir)/rdma

rdmainclude_HEADERS =

# internal utility functions shared by in-tree providers:
common_srcs =				\
	src/common.c			\
	src/enosys.c			\
	src/rbtree.c			\
	src/tree.c			\
	src/fasthash.c			\
	src/indexer.c			\
	src/mem.c			\
	src/iov.c			\
	src/shared/ofi_str.c		\
	prov/util/src/util_atomic.c	\
	prov/util/src/util_attr.c	\
	prov/util/src/util_av.c		\
	prov/util/src/util_cq.c		\
	prov/util/src/util_cntr.c	\
	prov/util/src/util_domain.c	\
	prov/util/src/util_ep.c		\
	prov/util/src/util_pep.c	\
	prov/util/src/util_eq.c		\
	prov/util/src/util_fabric.c	\
	prov/util/src/util_main.c	\
	prov/util/src/util_poll.c	\
	prov/util/src/util_wait.c	\
	prov/util/src/util_buf.c	\
	prov/util/src/util_mr_map.c	\
	prov/util/src/util_ns.c		\
	prov/util/src/util_shm.c	\
	prov/util/src/util_mem_monitor.c\
	prov/util/src/util_mem_hooks.c	\
	prov/util/src/util_mr_cache.c	\
	prov/util/src/util_coll.c


if MACOS
common_srcs += src/unix/osd.c
common_srcs += include/osx/osd.h
common_srcs += include/unix/osd.h
endif

if FREEBSD
common_srcs += src/unix/osd.c
common_srcs += include/freebsd/osd.h
common_srcs += include/unix/osd.h
endif

if LINUX
common_srcs += src/unix/osd.c
common_srcs += src/linux/osd.c
if HAVE_LINUX_PERF_RDPMC
common_srcs += src/linux/rdpmc.c
endif
common_srcs += include/linux/rdpmc.h
common_srcs += include/linux/osd.h
common_srcs += include/unix/osd.h
endif

# ensure dl-built providers link back to libfabric
linkback = src/libfabric.la

bin_PROGRAMS = \
	util/fi_info \
	util/fi_strerror \
	util/fi_pingpong

bin_SCRIPTS =

util_fi_info_SOURCES = \
	util/info.c
util_fi_info_LDADD = $(linkback)

util_fi_strerror_SOURCES = \
	util/strerror.c
util_fi_strerror_LDADD = $(linkback)

util_fi_pingpong_SOURCES = \
	util/pingpong.c
util_fi_pingpong_LDADD = $(linkback)

nodist_src_libfabric_la_SOURCES =
src_libfabric_la_SOURCES =			\
	include/ofi.h				\
	include/ofi_abi.h			\
	include/ofi_atom.h			\
	include/ofi_enosys.h			\
	include/ofi_file.h			\
	include/ofi_hook.h			\
	include/ofi_indexer.h			\
	include/ofi_iov.h			\
	include/ofi_list.h			\
	include/ofi_bitmask.h			\
	include/shared/ofi_str.h		\
	include/ofi_lock.h			\
	include/ofi_mem.h			\
	include/ofi_osd.h			\
	include/ofi_proto.h			\
	include/ofi_recvwin.h			\
	include/ofi_rbuf.h			\
	include/ofi_shm.h			\
	include/ofi_signal.h			\
	include/ofi_epoll.h			\
	include/ofi_tree.h			\
	include/ofi_util.h			\
	include/ofi_atomic.h			\
	include/ofi_mr.h			\
	include/ofi_net.h			\
	include/ofi_perf.h			\
	include/ofi_coll.h			\
	include/fasthash.h			\
	include/rbtree.h			\
	include/uthash.h			\
	include/ofi_prov.h			\
	include/rdma/providers/fi_log.h		\
	include/rdma/providers/fi_prov.h	\
	src/fabric.c				\
	src/fi_tostr.c				\
	src/perf.c				\
	src/log.c				\
	src/var.c				\
	src/abi_1_0.c				\
	prov/hook/src/hook.c			\
	prov/hook/src/hook_av.c			\
	prov/hook/src/hook_cm.c			\
	prov/hook/src/hook_cntr.c		\
	prov/hook/src/hook_cq.c			\
	prov/hook/src/hook_domain.c		\
	prov/hook/src/hook_ep.c			\
	prov/hook/src/hook_eq.c			\
	prov/hook/src/hook_wait.c		\
	prov/hook/src/hook_xfer.c		\
	$(common_srcs)

src_libfabric_la_CPPFLAGS = $(AM_CPPFLAGS)
src_libfabric_la_LDFLAGS =
src_libfabric_la_LIBADD =
src_libfabric_la_DEPENDENCIES = libfabric.map

if !EMBEDDED
src_libfabric_la_LDFLAGS += -version-info 14:1:13
endif
src_libfabric_la_LDFLAGS += -export-dynamic \
			   $(libfabric_version_script)
rdmainclude_HEADERS += \
	$(top_srcdir)/include/rdma/fabric.h \
	$(top_srcdir)/include/rdma/fi_atomic.h \
	$(top_srcdir)/include/rdma/fi_cm.h \
	$(top_srcdir)/include/rdma/fi_collective.h \
	$(top_srcdir)/include/rdma/fi_domain.h \
	$(top_srcdir)/include/rdma/fi_eq.h \
	$(top_srcdir)/include/rdma/fi_rma.h \
	$(top_srcdir)/include/rdma/fi_endpoint.h \
	$(top_srcdir)/include/rdma/fi_errno.h \
	$(top_srcdir)/include/rdma/fi_tagged.h \
	$(top_srcdir)/include/rdma/fi_trigger.h

if HAVE_DIRECT
nodist_rdmainclude_HEADERS = \
	$(top_srcdir)/prov/$(PROVIDER_DIRECT)/include/rdma/fi_direct_domain.h \
	$(top_srcdir)/prov/$(PROVIDER_DIRECT)/include/rdma/fi_direct_endpoint.h \
	$(top_srcdir)/prov/$(PROVIDER_DIRECT)/include/rdma/fi_direct_tagged.h \
	$(top_srcdir)/prov/$(PROVIDER_DIRECT)/include/rdma/fi_direct_rma.h \
	$(top_srcdir)/prov/$(PROVIDER_DIRECT)/include/rdma/fi_direct_atomic_def.h \
	$(top_srcdir)/prov/$(PROVIDER_DIRECT)/include/rdma/fi_direct_atomic.h \
	$(top_srcdir)/prov/$(PROVIDER_DIRECT)/include/rdma/fi_direct_cm.h \
	$(top_srcdir)/prov/$(PROVIDER_DIRECT)/include/rdma/fi_direct_eq.h \
	$(top_srcdir)/prov/$(PROVIDER_DIRECT)/include/rdma/fi_direct_trigger.h
if FI_DIRECT_H_IN
nodist_rdmainclude_HEADERS += $(top_builddir)/prov/$(PROVIDER_DIRECT)/include/rdma/fi_direct.h
else
nodist_rdmainclude_HEADERS += $(top_srcdir)/prov/$(PROVIDER_DIRECT)/include/rdma/fi_direct.h
endif
endif HAVE_DIRECT

real_man_pages = \
        man/man1/fi_info.1 \
        man/man1/fi_pingpong.1 \
        man/man1/fi_strerror.1 \
        man/man3/fi_av.3 \
        man/man3/fi_av_set.3 \
        man/man3/fi_cm.3 \
        man/man3/fi_cntr.3 \
        man/man3/fi_collective.3 \
        man/man3/fi_control.3 \
        man/man3/fi_cq.3 \
        man/man3/fi_domain.3 \
        man/man3/fi_endpoint.3 \
        man/man3/fi_errno.3 \
        man/man3/fi_eq.3 \
        man/man3/fi_fabric.3 \
        man/man3/fi_getinfo.3 \
        man/man3/fi_mr.3 \
        man/man3/fi_msg.3 \
	man/man3/fi_nic.3 \
        man/man3/fi_poll.3 \
        man/man3/fi_rma.3 \
        man/man3/fi_tagged.3 \
        man/man3/fi_trigger.3 \
        man/man3/fi_version.3 \
        man/man7/fabric.7 \
	man/man7/fi_hook.7 \
        man/man7/fi_provider.7 \
        man/man7/fi_direct.7

dummy_man_pages = \
        man/man3/fi_accept.3 \
        man/man3/fi_alias.3 \
        man/man3/fi_allgather.3 \
        man/man3/fi_allreduce.3 \
        man/man3/fi_alltoall.3 \
        man/man3/fi_atomic.3 \
        man/man3/fi_atomic_valid.3 \
        man/man3/fi_atomicmsg.3 \
        man/man3/fi_atomicv.3 \
        man/man3/fi_av_bind.3 \
        man/man3/fi_av_insert.3 \
        man/man3/fi_av_insertsvc.3 \
        man/man3/fi_av_lookup.3 \
        man/man3/fi_av_open.3 \
        man/man3/fi_av_remove.3 \
        man/man3/fi_av_set_diff.3 \
        man/man3/fi_av_set_insert.3 \
        man/man3/fi_av_set_intersect.3 \
        man/man3/fi_av_set_remove.3 \
        man/man3/fi_av_set_union.3 \
        man/man3/fi_av_straddr.3 \
        man/man3/fi_barrier.3 \
        man/man3/fi_broadcast.3 \
        man/man3/fi_cancel.3 \
        man/man3/fi_close.3 \
        man/man3/fi_cntr_add.3 \
        man/man3/fi_cntr_open.3 \
        man/man3/fi_cntr_read.3 \
        man/man3/fi_cntr_set.3 \
        man/man3/fi_cntr_wait.3 \
        man/man3/fi_compare_atomic.3 \
        man/man3/fi_compare_atomic_valid.3 \
        man/man3/fi_compare_atomicmsg.3 \
        man/man3/fi_compare_atomicv.3 \
        man/man3/fi_connect.3 \
        man/man3/fi_cq_open.3 \
        man/man3/fi_cq_read.3 \
        man/man3/fi_cq_readerr.3 \
        man/man3/fi_cq_readfrom.3 \
        man/man3/fi_cq_sread.3 \
        man/man3/fi_cq_sreadfrom.3 \
        man/man3/fi_cq_strerror.3 \
        man/man3/fi_cq_signal.3 \
        man/man3/fi_domain_bind.3 \
        man/man3/fi_domain_query.3 \
        man/man3/fi_dupinfo.3 \
        man/man3/fi_enable.3 \
        man/man3/fi_ep.3 \
        man/man3/fi_ep_bind.3 \
        man/man3/fi_eq_open.3 \
        man/man3/fi_eq_read.3 \
        man/man3/fi_eq_readerr.3 \
        man/man3/fi_eq_sread.3 \
        man/man3/fi_eq_strerror.3 \
        man/man3/fi_eq_write.3 \
        man/man3/fi_fetch_atomic.3 \
        man/man3/fi_fetch_atomic_valid.3 \
        man/man3/fi_fetch_atomicmsg.3 \
        man/man3/fi_fetch_atomicv.3 \
        man/man3/fi_freeinfo.3 \
        man/man3/fi_getname.3 \
        man/man3/fi_getopt.3 \
        man/man3/fi_getpeer.3 \
        man/man3/fi_inject.3 \
        man/man3/fi_injectdata.3 \
        man/man3/fi_inject_atomic.3 \
        man/man3/fi_inject_write.3 \
        man/man3/fi_inject_writedata.3 \
        man/man3/fi_join.3 \
        man/man3/fi_join_collective.3 \
        man/man3/fi_leave.3 \
        man/man3/fi_listen.3 \
        man/man3/fi_mr_bind.3 \
        man/man3/fi_mr_desc.3 \
        man/man3/fi_mr_key.3 \
        man/man3/fi_mr_reg.3 \
        man/man3/fi_mr_regattr.3 \
        man/man3/fi_mr_regv.3 \
        man/man3/fi_open.3 \
        man/man3/fi_open_ops.3 \
        man/man3/fi_passive_ep.3 \
        man/man3/fi_pep_bind.3 \
        man/man3/fi_poll_add.3 \
        man/man3/fi_poll_del.3 \
        man/man3/fi_poll_open.3 \
        man/man3/fi_query_atomic.3 \
        man/man3/fi_query_collective.3 \
        man/man3/fi_read.3 \
        man/man3/fi_readmsg.3 \
        man/man3/fi_readv.3 \
        man/man3/fi_recv.3 \
        man/man3/fi_recvmsg.3 \
        man/man3/fi_recvv.3 \
        man/man3/fi_reduce_scatter.3 \
        man/man3/fi_reject.3 \
        man/man3/fi_rx_addr.3 \
        man/man3/fi_rx_size_left.3 \
        man/man3/fi_scalable_ep_bind.3 \
        man/man3/fi_send.3 \
        man/man3/fi_senddata.3 \
        man/man3/fi_sendmsg.3 \
        man/man3/fi_sendv.3 \
        man/man3/fi_setname.3 \
        man/man3/fi_setopt.3 \
        man/man3/fi_shutdown.3 \
        man/man3/fi_strerror.3 \
        man/man3/fi_tinject.3 \
        man/man3/fi_tinjectdata.3 \
        man/man3/fi_tostr.3 \
        man/man3/fi_trecv.3 \
        man/man3/fi_trecvmsg.3 \
        man/man3/fi_trecvv.3 \
        man/man3/fi_trywait.3 \
        man/man3/fi_tsend.3 \
        man/man3/fi_tsenddata.3 \
        man/man3/fi_tsendmsg.3 \
        man/man3/fi_tsendv.3 \
        man/man3/fi_tx_size_left.3 \
        man/man3/fi_wait.3 \
        man/man3/fi_wait_open.3 \
        man/man3/fi_write.3 \
        man/man3/fi_writedata.3 \
        man/man3/fi_writemsg.3 \
        man/man3/fi_writev.3

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libfabric.pc

nroff:
	@for file in $(real_man_pages) $(prov_install_man_pages); do \
	    source=`echo $$file | sed -e 's@/man[0-9]@@'`; \
	    perl $(top_srcdir)/config/md2nroff.pl --source=$(top_srcdir)/$$source.md; \
	done

dist-hook: libfabric.spec
	cp libfabric.spec $(distdir)
	perl $(top_srcdir)/config/distscript.pl "$(distdir)" "$(PACKAGE_VERSION)"

TESTS = \
	util/fi_info

test:
	./util/fi_info

rpm: dist
	LDFLAGS=-Wl,--build-id rpmbuild -ta libfabric-$(PACKAGE_VERSION).tar.bz2

prov_install_man_pages=
prov_dist_man_pages=
EXTRA_DIST=

include prov/sockets/Makefile.include
include prov/udp/Makefile.include
include prov/verbs/Makefile.include
include prov/efa/Makefile.include
include prov/usnic/Makefile.include
include prov/psm/Makefile.include
include prov/psm2/Makefile.include
include prov/gni/Makefile.include
include prov/rxm/Makefile.include
include prov/mrail/Makefile.include
include prov/rxd/Makefile.include
include prov/bgq/Makefile.include
include prov/shm/Makefile.include
include prov/tcp/Makefile.include
include prov/rstream/Makefile.include
include prov/hook/Makefile.include
include prov/hook/perf/Makefile.include
include prov/hook/hook_debug/Makefile.include

man_MANS = $(real_man_pages) $(prov_install_man_pages) $(dummy_man_pages)

EXTRA_DIST += \
        NEWS.md \
        libfabric.spec.in \
        config/distscript.pl \
        $(real_man_pages) $(prov_dist_man_pages) $(dummy_man_pages)
