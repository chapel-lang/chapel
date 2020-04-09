# Copyright 2020 Hewlett Packard Enterprise Development LP
# Copyright 2004-2019 Cray Inc.
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

# Generate tags only if $TAGS turned on explicitly, or if $CHPL_DEVELOPER is on
ifdef CHPL_DEVELOPER
TAGS=1
endif

# Generate tags command, dependent on if Make variable, TAGS == 1
ifeq ($(TAGS), 1)
TAGS_COMMAND=-@($(CHPL_MAKE_HOME)/util/chpltags -r . > /dev/null 2>&1 && echo "Updating TAGS..." || echo "Tags utility not available.  Skipping tags generation.")
endif

CHPL_MAKE_HOST_TARGET = --target
include $(CHPL_MAKE_HOME)/make/Makefile.base

SYS_MODULES_DIR=standard/gen/$(CHPL_MAKE_PLATFORM)-$(CHPL_MAKE_ARCH)-$(CHPL_MAKE_COMPILER)
SYS_CTYPES_MODULE_DOC=standard/SysCTypes.chpl
SYS_CTYPES_MODULE=$(SYS_MODULES_DIR)/SysCTypes.chpl

MODULE_SPHINX=${CHPL_MAKE_HOME}/modules/sphinx
DOC=${CHPL_MAKE_HOME}/doc/rst

default: all

all: $(SYS_CTYPES_MODULE)
	$(TAGS_COMMAND)

clean: FORCE

cleanall: FORCE

clobber: FORCE clean-documentation
	rm -rf standard/gen
	rm -f TAGS tags

depend:

MAKE_SYS_BASIC_TYPES=$(CHPL_MAKE_HOME)/util/config/make_sys_basic_types.py

$(SYS_CTYPES_MODULE): $(MAKE_SYS_BASIC_TYPES)
	mkdir -p $(@D)
	cd $(@D) && $(MAKE_SYS_BASIC_TYPES) $(@F)

$(SYS_CTYPES_MODULE_DOC): $(MAKE_SYS_BASIC_TYPES)
	mkdir -p $(@D)
	cd $(@D) && $(MAKE_SYS_BASIC_TYPES) --doc $(@F)

MODULES_TO_DOCUMENT = \
	standard/Assert.chpl \
	standard/Barriers.chpl \
	standard/BigInteger.chpl \
	standard/BitOps.chpl \
	standard/CommDiagnostics.chpl \
	standard/DateTime.chpl \
	standard/DynamicIters.chpl \
	standard/FileSystem.chpl \
	standard/GMP.chpl \
	standard/Help.chpl \
	standard/IO.chpl \
	standard/LinkedLists.chpl \
	standard/List.chpl \
	standard/Map.chpl \
	standard/Math.chpl \
	standard/Memory.chpl \
	standard/Path.chpl \
	standard/Random.chpl \
	standard/Reflection.chpl \
	standard/Regexp.chpl \
	standard/Set.chpl \
	standard/Spawn.chpl \
	standard/Sys.chpl \
	standard/SysBasic.chpl \
	standard/SysError.chpl \
	standard/Time.chpl \
	standard/Types.chpl \
	$(SYS_CTYPES_MODULE_DOC)

PACKAGES_TO_DOCUMENT = \
	packages/AllLocalesBarriers.chpl \
	packages/AtomicObjects.chpl \
	packages/BLAS.chpl \
	packages/Buffers.chpl \
	packages/Crypto.chpl \
	packages/Curl.chpl \
	packages/EpochManager.chpl \
	packages/FFTW.chpl \
	packages/FunctionalOperations.chpl \
	packages/Futures.chpl \
	packages/HDF5.chpl \
	packages/HDFS.chpl \
	packages/LAPACK.chpl \
	packages/LinearAlgebra.chpl \
	packages/LockFreeQueue.chpl \
	packages/LockFreeStack.chpl \
	packages/MPI.chpl \
	packages/NetCDF.chpl \
	packages/Norm.chpl \
	packages/PeekPoke.chpl \
	packages/RangeChunk.chpl \
	packages/RecordParser.chpl \
	packages/ReplicatedVar.chpl \
	packages/Search.chpl \
	packages/Sort.chpl \
	packages/VisualDebug.chpl \
	packages/ZMQ.chpl \
	packages/Collection.chpl \
	packages/DistributedBag.chpl \
	packages/DistributedDeque.chpl \
	packages/DistributedIters.chpl \
	packages/TOML.chpl \
	packages/UnitTest.chpl \
	packages/UnorderedAtomics.chpl \
	packages/UnorderedCopy.chpl \
	packages/URL.chpl

DISTS_TO_DOCUMENT = \
	dists/BlockCycDist.chpl \
	dists/BlockDist.chpl \
	dists/CyclicDist.chpl \
	dists/DimensionalDist2D.chpl \
	dists/HashedDist.chpl \
	dists/PrivateDist.chpl \
	dists/ReplicatedDist.chpl \
	dists/StencilDist.chpl \
	dists/dims/BlockCycDim.chpl \
	dists/dims/BlockDim.chpl \
	dists/dims/ReplicatedDim.chpl \
	layouts/LayoutCS.chpl \

INTERNAL_MODULES_TO_DOCUMENT =                \
	internal/Atomics.chpl                 \
	internal/ChapelArray.chpl             \
	internal/ChapelComplex_forDocs.chpl   \
	internal/ChapelError.chpl             \
	internal/ChapelIO.chpl                \
	internal/ChapelIteratorSupport.chpl   \
	internal/ChapelLocale.chpl            \
	internal/UtilMisc_forDocs.chpl 	      \
	internal/ChapelRange.chpl             \
	internal/Bytes.chpl                   \
	internal/String.chpl                  \
	internal/ChapelSyncvar.chpl           \
	internal/ChapelTuple.chpl             \
	internal/ChapelEnv.chpl               \
	internal/CPtr.chpl                    \
	internal/OwnedObject.chpl             \
	internal/SharedObject.chpl


documentation: $(SYS_CTYPES_MODULE_DOC)
	@echo "Generating module documentation with chpldoc"
	export CHPLDOC_AUTHOR='Hewlett Packard Enterprise Development LP' && \
	$(CHPLDOC) --save-sphinx ${MODULE_SPHINX} --no-html $(MODULES_TO_DOCUMENT) $(DISTS_TO_DOCUMENT) $(PACKAGES_TO_DOCUMENT) $(INTERNAL_MODULES_TO_DOCUMENT)
	@echo "Running post-processing scripts"
	./internal/fixInternalDocs.sh ${MODULE_SPHINX}
	./dists/fixDistDocs.perl      ${MODULE_SPHINX}
	@echo "Copying generated module documentation to ${DOC}"
	mkdir -p ${DOC}/modules
	mkdir -p ${DOC}/builtins
	cp -rf ${MODULE_SPHINX}/source/modules/standard   ${DOC}/modules
	cp -rf ${MODULE_SPHINX}/source/modules/packages   ${DOC}/modules
	cp -rf ${MODULE_SPHINX}/source/modules/dists      ${DOC}/modules
	cp -rf ${MODULE_SPHINX}/source/modules/layouts    ${DOC}/modules
	cp -rf ${MODULE_SPHINX}/source/modules/internal/* ${DOC}/builtins
	@echo "Removing generated files and directories"
	rm -rf ./docs
	rm -rf ${MODULE_SPHINX}
	rm -f ${CHPL_MAKE_HOME}/modules/$(SYS_CTYPES_MODULE_DOC)

clean-documentation:
	rm -rf ./docs
	rm -rf $(MODULE_SPHINX)

FORCE:
