# Copyright 2004-2015 Cray Inc.
# Other additional copyright holders may be indicated within.
# 
# The entirety of this work is licensed under the Apache License,
# Version 2.0 (the "License"); you may not use this file except
# in compliance with the License.
# 
# You may obtain a copy of the License at
# 
#     http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/..
endif

include $(CHPL_MAKE_HOME)/make/Makefile.base

all: third-party-pkgs all.helpme

depend:

clean: clean.helpme

cleanall: cleanall.helpme

cleandeps: cleandeps.helpme

clobber: clobber.helpme

%.helpme:
	@$(MAKE) -f Makefile.help MAKE_LAUNCHER=0 CHPL_MAKE_RULE=$* $*
ifneq ($(CHPL_MAKE_LAUNCHER),none)
	@$(MAKE) -f Makefile.help MAKE_LAUNCHER=1 CHPL_MAKE_RULE=$* $*
endif

THIRD_PARTY_PKGS = $(shell $(CHPL_MAKE_HOME)/util/chplenv/third-party-pkgs)
# NOTE: If any of these builds fail, we won't stop building
third-party-pkgs:
ifneq (, $(THIRD_PARTY_PKGS))
ifneq (, $(filter $(THIRD_PARTY_PKGS),hwloc))
	$(MAKE) -C $(THIRD_PARTY_DIR) hwloc
endif
	$(MAKE) -C $(THIRD_PARTY_DIR) $(THIRD_PARTY_PKGS)
endif

.NOTPARALLEL:
