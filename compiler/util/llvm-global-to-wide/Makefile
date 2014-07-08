##===- projects/sample/Makefile ----------------------------*- Makefile -*-===##
#
# This is a sample Makefile for a project that uses LLVM.
#
##===----------------------------------------------------------------------===##

#
# Indicates our relative path to the top of the project's root directory.
#
LEVEL = .
EXTRA_DIST = include
#
# Give the name of a library.  This will build a dynamic version.
#
LIBRARYNAME=global-to-wide
SHARED_LIBRARY=1

#
# Include the Master Makefile that knows how to build all.
#
include $(LEVEL)/Makefile.common
CXXFLAGS += -DHAVE_LLVM

