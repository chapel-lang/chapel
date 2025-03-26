# Copyright © 2009-2024 Inria.  All rights reserved.
#
# See COPYING in top-level directory.

# This makefile is only reached when building in standalone mode

bashcompletionsdir = $(datadir)/bash-completion/completions
dist_bashcompletions_DATA = bash/hwloc

if HWLOC_HAVE_BASH
# no need to test everywhere, Linux is enough
if HWLOC_HAVE_LINUX
TESTS = test-bash-completion.sh
endif
endif
