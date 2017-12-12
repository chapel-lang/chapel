# Copyright 2004-2017 Cray Inc.
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

# This toplevel Makefile exists simply to make sure that we're not
# using BSD Make. It could also check a GNU Make version.
# Real toplevel rules are stored in Makefile.toplevel

# Don't want to include directories or other noisy output,
# want existence of this file to be transparent.
MAKEFLAGS = --no-print-directory

# MAKE_VERSION is e.g. 4.2.1 for GNU Make but e.g. 20100606
# for BSD Make.
# Additionally BSD Make does not support firstword, subst, etc,
# and so the following variable will be empty for BSD Make.
MY_MAKE_VERSION_MAJOR = $(firstword $(subst ., ,$(MAKE_VERSION)))

# Declare an action to run by default that checks
# for BSD Make. It does the check in a shell script because
# BSD Make and GNU Make differ in conditional syntax.
default-action : FORCE
	@if [ "$(MY_MAKE_VERSION_MAJOR)" = "" ] ; \
	then \
	  echo "Please use GNU make and not BSD make (try gmake)" ; \
	else \
	  $(MAKE) -f Makefile.toplevel; \
	fi

# This rule any other rule to Makefile.toplevel.
% : FORCE
	@if [ "$(MY_MAKE_VERSION_MAJOR)" = "" ] ; \
	then \
	  echo "Please use GNU make and not BSD make (try gmake)" ; \
	else \
	  $(MAKE) -f Makefile.toplevel $@; \
	fi


# Make sure these rules always run and don't try to make Makefile
.PHONY: FORCE Makefile
FORCE:
Makefile:
