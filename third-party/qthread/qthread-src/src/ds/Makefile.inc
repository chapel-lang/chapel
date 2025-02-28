# -*- Makefile -*-
# vim:ft=automake
#
# Copyright (c) 2010 Sandia Corporation
#

libqthread_la_SOURCES += \
			 ds/qarray.c \
			 ds/qdqueue.c \
			 ds/qlfqueue.c \
			 ds/qswsrqueue.c \
			 ds/qpool.c \
			 ds/dictionary/hash.c \
			 ds/dictionary/dictionary_@with_dict@.c

EXTRA_DIST += \
			 ds/dictionary/dictionary_shavit.c \
			 ds/dictionary/dictionary_trie.c \
			 ds/dictionary/dictionary_simple.c
