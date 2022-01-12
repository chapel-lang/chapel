configdir = $(datarootdir)/${PACKAGE_NAME}
AM_CFLAGS = -g -Wall -D_GNU_SOURCE -D 'CONFIG_PATH="${configdir}"' -I$(srcdir)/include
ACLOCAL_AMFLAGS = -I config

if MACOS
os_excludes = -f ./test_configs/osx.exclude
AM_CFLAGS += -I$(srcdir)/include/osx
endif

if FREEBSD
os_excludes = -f ./test_configs/freebsd.exclude
AM_CFLAGS += -I$(srcdir)/include/freebsd
endif

bin_PROGRAMS = \
	functional/fi_av_xfer \
	functional/fi_msg \
	functional/fi_stream_msg \
	functional/fi_msg_sockets \
	functional/fi_rdm \
	functional/fi_rdm_rma_event \
	functional/fi_rdm_rma_trigger \
	functional/fi_rdm_deferred_wq \
	functional/fi_dgram \
	functional/fi_mcast \
	functional/fi_dgram_waitset \
	functional/fi_rdm_tagged_peek \
	functional/fi_cq_data \
	functional/fi_poll \
	functional/fi_scalable_ep \
	functional/fi_shared_ctx \
	functional/fi_msg_epoll \
	functional/fi_rdm_shared_av \
	functional/fi_cm_data \
	functional/fi_multi_mr \
	functional/fi_rdm_multi_domain \
	functional/fi_multi_ep \
	functional/fi_recv_cancel \
	functional/fi_unexpected_msg \
	functional/fi_unmap_mem \
	functional/fi_inj_complete \
	functional/fi_resmgmt_test \
	functional/fi_rdm_atomic \
	functional/fi_multi_recv \
	functional/fi_bw \
	benchmarks/fi_msg_pingpong \
	benchmarks/fi_msg_bw \
	benchmarks/fi_rma_bw \
	benchmarks/fi_rdm_cntr_pingpong \
	benchmarks/fi_dgram_pingpong \
	benchmarks/fi_rdm_pingpong \
	benchmarks/fi_rdm_tagged_pingpong \
	benchmarks/fi_rdm_tagged_bw \
	unit/fi_eq_test \
	unit/fi_cq_test \
	unit/fi_mr_test \
	unit/fi_mr_cache_evict \
	unit/fi_cntr_test \
	unit/fi_av_test \
	unit/fi_dom_test \
	unit/fi_getinfo_test \
	ubertest/fi_ubertest	\
	multinode/fi_multinode	\
	multinode/fi_multinode_coll

dist_bin_SCRIPTS = \
	scripts/runfabtests.sh \
	scripts/rft_yaml_to_junit_xml

dist_noinst_SCRIPTS = \
	scripts/parseyaml.py

nobase_dist_config_DATA = \
	test_configs/osx.exclude \
        test_configs/eq_cq.test \
        test_configs/lat_bw.test \
        test_configs/sockets/all.test \
        test_configs/sockets/quick.test \
	test_configs/sockets/complete.test \
	test_configs/sockets/verify.test \
        test_configs/udp/all.test \
        test_configs/udp/lat_bw.test \
        test_configs/udp/quick.test \
        test_configs/udp/functional.test \
        test_configs/udp/udp.exclude \
        test_configs/tcp/tcp.exclude \
        test_configs/tcp/all.test \
        test_configs/verbs/all.test \
        test_configs/verbs/quick.test \
	test_configs/verbs/verbs.exclude \
	test_configs/usnic/all.test \
	test_configs/usnic/quick.test \
	test_configs/psm/all.test \
	test_configs/psm2/all.test \
	test_configs/psm2/verify.test \
	test_configs/psm2/psm2.exclude \
	test_configs/psm3/all.test \
	test_configs/psm3/verify.test \
	test_configs/psm3/psm3.exclude \
	test_configs/ofi_rxm/tcp.test \
	test_configs/ofi_rxm/verbs.test \
	test_configs/ofi_rxm/ofi_rxm.exclude \
	test_configs/ofi_rxd/udp.test \
	test_configs/ofi_rxd/verbs.test \
	test_configs/ofi_rxd/ofi_rxd.exclude \
	test_configs/shm/all.test \
	test_configs/shm/shm.exclude \
	test_configs/shm/quick.test \
	test_configs/shm/verify.test \
	test_configs/efa/efa.exclude

noinst_LTLIBRARIES = libfabtests.la

libfabtests_la_SOURCES = \
	common/shared.c \
	common/jsmn.c \
	common/hmem.c \
	common/hmem_cuda.c \
	common/hmem_rocr.c \
	common/hmem_ze.c \
	include/shared.h \
	include/hmem.h \
	include/jsmn.h \
	include/unix/osd.h \
	include/ft_osd.h

benchmarks_srcs = \
	benchmarks/benchmark_shared.h \
	benchmarks/benchmark_shared.c

unit_srcs = \
	include/unit_common.h \
	unit/common.c

if MACOS
if !HAVE_CLOCK_GETTIME
libfabtests_la_SOURCES += common/osx/osd.c
endif
endif

functional_fi_av_xfer_SOURCES = \
	functional/av_xfer.c
functional_fi_av_xfer_LDADD = libfabtests.la

functional_fi_msg_sockets_SOURCES = \
	functional/msg_sockets.c
functional_fi_msg_sockets_LDADD = libfabtests.la

functional_fi_msg_epoll_SOURCES = \
	functional/msg_epoll.c
functional_fi_msg_epoll_LDADD = libfabtests.la

functional_fi_msg_SOURCES = \
	functional/msg.c
functional_fi_msg_LDADD = libfabtests.la

functional_fi_stream_msg_SOURCES = \
	functional/stream_msg.c
functional_fi_stream_msg_LDADD = libfabtests.la

functional_fi_rdm_SOURCES = \
	functional/rdm.c
functional_fi_rdm_LDADD = libfabtests.la

functional_fi_rdm_shared_av_SOURCES = \
	functional/rdm_shared_av.c
functional_fi_rdm_shared_av_LDADD = libfabtests.la

functional_fi_rdm_rma_event_SOURCES = \
	functional/rdm_rma_event.c
functional_fi_rdm_rma_event_LDADD = libfabtests.la

functional_fi_rdm_rma_trigger_SOURCES = \
	functional/rdm_rma_trigger.c
functional_fi_rdm_rma_trigger_LDADD = libfabtests.la

functional_fi_rdm_deferred_wq_SOURCES = \
	functional/rdm_deferred_wq.c
functional_fi_rdm_deferred_wq_LDADD = libfabtests.la

functional_fi_dgram_SOURCES = \
	functional/dgram.c
functional_fi_dgram_LDADD = libfabtests.la

functional_fi_mcast_SOURCES = \
	functional/mcast.c
functional_fi_mcast_LDADD = libfabtests.la

functional_fi_dgram_waitset_SOURCES = \
	functional/dgram_waitset.c
functional_fi_dgram_waitset_LDADD = libfabtests.la

functional_fi_rdm_tagged_peek_SOURCES = \
	functional/rdm_tagged_peek.c
functional_fi_rdm_tagged_peek_LDADD = libfabtests.la

functional_fi_cq_data_SOURCES = \
	functional/cq_data.c
functional_fi_cq_data_LDADD = libfabtests.la

functional_fi_cm_data_SOURCES = \
	functional/cm_data.c
functional_fi_cm_data_LDADD = libfabtests.la

functional_fi_scalable_ep_SOURCES = \
	functional/scalable_ep.c
functional_fi_scalable_ep_LDADD = libfabtests.la

functional_fi_shared_ctx_SOURCES = \
	functional/shared_ctx.c
functional_fi_shared_ctx_LDADD = libfabtests.la

functional_fi_poll_SOURCES = \
	functional/poll.c
functional_fi_poll_LDADD = libfabtests.la

functional_fi_multi_ep_SOURCES = \
	functional/multi_ep.c
functional_fi_multi_ep_LDADD = libfabtests.la

functional_fi_multi_mr_SOURCES = \
	functional/multi_mr.c
functional_fi_multi_mr_LDADD = libfabtests.la

functional_fi_unexpected_msg_SOURCES = \
	functional/unexpected_msg.c
functional_fi_unexpected_msg_LDADD = libfabtests.la

functional_fi_unmap_mem_SOURCES = \
	functional/unmap_mem.c
functional_fi_unmap_mem_LDADD = libfabtests.la

functional_fi_rdm_multi_domain_SOURCES = \
	functional/rdm_multi_domain.c
functional_fi_rdm_multi_domain_LDADD = libfabtests.la

functional_fi_recv_cancel_SOURCES = \
	functional/recv_cancel.c
functional_fi_recv_cancel_LDADD = libfabtests.la

functional_fi_inj_complete_SOURCES = \
	functional/inj_complete.c
functional_fi_inj_complete_LDADD = libfabtests.la

functional_fi_resmgmt_test_SOURCES = \
	functional/resmgmt_test.c
functional_fi_resmgmt_test_LDADD = libfabtests.la

functional_fi_rdm_atomic_SOURCES = \
	functional/rdm_atomic.c
functional_fi_rdm_atomic_LDADD = libfabtests.la

functional_fi_multi_recv_SOURCES = \
	functional/multi_recv.c
functional_fi_multi_recv_LDADD = libfabtests.la

functional_fi_bw_SOURCES = \
	functional/bw.c
functional_fi_bw_LDADD = libfabtests.la

benchmarks_fi_msg_pingpong_SOURCES = \
	benchmarks/msg_pingpong.c \
	$(benchmarks_srcs)
benchmarks_fi_msg_pingpong_LDADD = libfabtests.la

benchmarks_fi_msg_bw_SOURCES = \
	benchmarks/msg_bw.c \
	$(benchmarks_srcs)
benchmarks_fi_msg_bw_LDADD = libfabtests.la

benchmarks_fi_rma_bw_SOURCES = \
	benchmarks/rma_bw.c \
	$(benchmarks_srcs)
benchmarks_fi_rma_bw_LDADD = libfabtests.la

benchmarks_fi_dgram_pingpong_SOURCES = \
	benchmarks/dgram_pingpong.c \
	$(benchmarks_srcs)
benchmarks_fi_dgram_pingpong_LDADD = libfabtests.la

benchmarks_fi_rdm_cntr_pingpong_SOURCES = \
	benchmarks/rdm_cntr_pingpong.c \
	$(benchmarks_srcs)
benchmarks_fi_rdm_cntr_pingpong_LDADD = libfabtests.la

benchmarks_fi_rdm_pingpong_SOURCES = \
	benchmarks/rdm_pingpong.c \
	$(benchmarks_srcs)
benchmarks_fi_rdm_pingpong_LDADD = libfabtests.la

benchmarks_fi_rdm_tagged_pingpong_SOURCES = \
	benchmarks/rdm_tagged_pingpong.c \
	$(benchmarks_srcs)
benchmarks_fi_rdm_tagged_pingpong_LDADD = libfabtests.la

benchmarks_fi_rdm_tagged_bw_SOURCES = \
	benchmarks/rdm_tagged_bw.c \
	$(benchmarks_srcs)
benchmarks_fi_rdm_tagged_bw_LDADD = libfabtests.la


unit_fi_eq_test_SOURCES = \
	unit/eq_test.c \
	$(unit_srcs)
unit_fi_eq_test_LDADD = libfabtests.la

unit_fi_cq_test_SOURCES = \
	unit/cq_test.c \
	$(unit_srcs)
unit_fi_cq_test_LDADD = libfabtests.la

unit_fi_mr_test_SOURCES = \
	unit/mr_test.c \
	$(unit_srcs)
unit_fi_mr_test_LDADD = libfabtests.la

unit_fi_mr_cache_evict_SOURCES = \
	unit/mr_cache_evict.c \
	$(unit_srcs)
unit_fi_mr_cache_evict_LDADD = libfabtests.la

unit_fi_cntr_test_SOURCES = \
	unit/cntr_test.c \
	$(unit_srcs)
unit_fi_cntr_test_LDADD = libfabtests.la

unit_fi_av_test_SOURCES = \
	unit/av_test.c \
	$(unit_srcs)
unit_fi_av_test_LDADD = libfabtests.la

unit_fi_dom_test_SOURCES = \
	unit/dom_test.c \
	$(unit_srcs)
unit_fi_dom_test_LDADD = libfabtests.la

unit_fi_getinfo_test_SOURCES = \
	unit/getinfo_test.c \
	$(unit_srcs)
unit_fi_getinfo_test_LDADD = libfabtests.la

ubertest_fi_ubertest_SOURCES = \
	ubertest/fabtest.h \
	ubertest/ofi_atomic.h \
	ubertest/ofi_atomic.c \
	ubertest/uber.c \
	ubertest/connect.c \
	ubertest/cq.c \
	ubertest/config.c \
	ubertest/domain.c \
	ubertest/ep.c \
	ubertest/xfer.c \
	ubertest/verify.c \
	ubertest/test_ctrl.c
ubertest_fi_ubertest_LDADD = libfabtests.la

multinode_fi_multinode_SOURCES = \
	multinode/src/harness.c \
	multinode/src/pattern.c \
	multinode/include/pattern.h \
	multinode/src/core.c \
	multinode/include/core.h

multinode_fi_multinode_LDADD = 	libfabtests.la

multinode_fi_multinode_CFLAGS = \
	$(AM_CFLAGS) \
	-I$(srcdir)/multinode/include

multinode_fi_multinode_coll_SOURCES = \
	multinode/src/harness.c \
	multinode/src/core_coll.c \
	multinode/include/coll_test.h \
	multinode/include/core.h

multinode_fi_multinode_coll_LDADD = 	libfabtests.la

multinode_fi_multinode_coll_CFLAGS = \
	$(AM_CFLAGS) \
	-I$(srcdir)/multinode/include

real_man_pages = \
	 man/man7/fabtests.7

dummy_man_pages = \
	man/man1/fi_av_xfer.1 \
	man/man1/fi_cm_data.1 \
	man/man1/fi_cq_data.1 \
	man/man1/fi_dgram.1 \
	man/man1/fi_dgram_waitset.1 \
	man/man1/fi_inj_complete.1 \
	man/man1/fi_mcast.1 \
	man/man1/fi_msg.1 \
	man/man1/fi_msg_epoll.1 \
	man/man1/fi_msg_sockets.1 \
	man/man1/fi_multi_ep.1 \
	man/man1/fi_multi_mr.1 \
	man/man1/fi_poll.1 \
	man/man1/fi_rdm.1 \
	man/man1/fi_rdm_atomic.1 \
	man/man1/fi_rdm_deferred_wq.1 \
	man/man1/fi_rdm_multi_domain.1 \
	man/man1/fi_multi_recv.1 \
	man/man1/fi_rdm_rma_event.1 \
	man/man1/fi_rdm_rma_trigger.1 \
	man/man1/fi_rdm_shared_av.1 \
	man/man1/fi_rdm_tagged_peek.1 \
	man/man1/fi_recv_cancel.1 \
	man/man1/fi_resmgmt_test.1 \
	man/man1/fi_scalable_ep.1 \
	man/man1/fi_shared_ctx.1 \
	man/man1/fi_unexpected_msg.1 \
	man/man1/fi_unmap_mem.1 \
	man/man1/fi_dgram_pingpong.1 \
	man/man1/fi_msg_bw.1 \
	man/man1/fi_msg_pingpong.1 \
	man/man1/fi_rdm_cntr_pingpong.1 \
	man/man1/fi_rdm_pingpong.1 \
	man/man1/fi_rdm_tagged_bw.1 \
	man/man1/fi_rdm_tagged_pingpong.1 \
	man/man1/fi_rma_bw.1 \
	man/man1/fi_av_test.1 \
	man/man1/fi_cntr_test.1 \
	man/man1/fi_cq_test.1 \
	man/man1/fi_dom_test.1 \
	man/man1/fi_eq_test.1 \
	man/man1/fi_getinfo_test.1 \
	man/man1/fi_mr_test.1 \
	man/man1/fi_bw.1 \
	man/man1/fi_ubertest.1

nroff:
	@for file in $(real_man_pages); do \
            source=`echo $$file | sed -e 's@/man[0-9]@@'`; \
            perl $(top_srcdir)/config/md2nroff.pl --source=$(top_srcdir)/$$source.md; \
        done


man_MANS = $(real_man_pages) $(dummy_man_pages)

EXTRA_DIST = \
	fabtests.spec.in $(real_man_pages) $(dummy_man_pages)

dist-hook: fabtests.spec
	cp fabtests.spec $(distdir)

test:
	./scripts/runfabtests.sh -vvv -S $(os_excludes) -f ./test_configs/sockets/sockets.exclude sockets
	./scripts/runfabtests.sh -vvv -S $(os_excludes) -f ./test_configs/udp/udp.exclude udp
	./scripts/runfabtests.sh -vvv -S $(os_excludes) -f ./test_configs/tcp/tcp.exclude tcp
