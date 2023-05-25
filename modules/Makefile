# Copyright 2020-2023 Hewlett Packard Enterprise Development LP
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

SYS_MODULES_DIR=standard/gen/$(CHPL_MAKE_SYS_MODULES_SUBDIR)
SYS_CTYPES_MODULE_DOC=standard/ChapelSysCTypes.chpl
SYS_CTYPES_MODULE=$(SYS_MODULES_DIR)/ChapelSysCTypes.chpl

MODULE_SPHINX=${CHPL_MAKE_HOME}/modules/sphinx
DOC_BUILD=${CHPL_MAKE_HOME}/build/doc
DOC_RST=${CHPL_MAKE_HOME}/doc/rst
COPY_IF_DIFFERENT = $(CHPL_MAKE_PYTHON) $(CHPL_MAKE_HOME)/util/config/update-if-different --copy

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
	cd $(@D) && $(CHPL_MAKE_PYTHON) $(MAKE_SYS_BASIC_TYPES) $(@F)

$(SYS_CTYPES_MODULE_DOC): $(MAKE_SYS_BASIC_TYPES)
	mkdir -p $(@D)
	cd $(@D) && $(CHPL_MAKE_PYTHON) $(MAKE_SYS_BASIC_TYPES) --doc $(@F)

MODULES_TO_DOCUMENT = \
	standard/AutoMath.chpl \
	standard/Barriers.chpl \
	standard/BigInteger.chpl \
	standard/BitOps.chpl \
	standard/ChapelIO.chpl \
	standard/ChplConfig.chpl \
	standard/Collectives.chpl \
	standard/CommDiagnostics.chpl \
	standard/Communication.chpl \
	standard/GpuDiagnostics.chpl \
	standard/GPU.chpl \
	standard/CTypes.chpl \
	standard/DynamicIters.chpl \
	standard/Errors.chpl \
	standard/FileSystem.chpl \
	standard/GMP.chpl \
	standard/Heap.chpl \
	standard/Help.chpl \
	standard/IO.chpl \
	standard/List.chpl \
	standard/Map.chpl \
	standard/Math.chpl \
	standard/MemMove.chpl \
	standard/Memory.chpl \
	standard/MemDiagnostics.chpl \
	standard/Memory/Diagnostics.chpl \
	standard/Memory/Initialization.chpl \
	standard/OS.chpl \
	standard/Path.chpl \
	standard/Random.chpl \
	standard/Reflection.chpl \
	standard/Regex.chpl \
	standard/Set.chpl \
	standard/Subprocess.chpl \
	standard/Sys.chpl \
	standard/SysBasic.chpl \
	standard/Time.chpl \
	standard/Types.chpl \
	standard/VectorizingIterator.chpl \
	standard/Version.chpl

PACKAGES_TO_DOCUMENT = \
	packages/AllLocalesBarriers.chpl \
	packages/ArgumentParser.chpl \
	packages/AtomicObjects.chpl \
	packages/BLAS.chpl \
	packages/Buffers.chpl \
	packages/Channel.chpl \
	packages/Collection.chpl \
	packages/ConcurrentMap.chpl \
	packages/CopyAggregation.chpl \
	packages/Crypto.chpl \
	packages/Curl.chpl \
	packages/DistributedBag.chpl \
	packages/DistributedDeque.chpl \
	packages/DistributedIters.chpl \
	packages/EpochManager.chpl \
	packages/FFTW.chpl \
	packages/FunctionalOperations.chpl \
	packages/Futures.chpl \
	packages/HDF5.chpl \
	packages/HDFS.chpl \
	packages/LAPACK.chpl \
	packages/LinearAlgebra.chpl \
	packages/LinkedLists.chpl \
	packages/LockFreeQueue.chpl \
	packages/LockFreeStack.chpl \
	packages/MPI.chpl \
	packages/NetCDF.chpl \
	packages/ProtobufProtocolSupport.chpl \
	packages/PeekPoke.chpl \
	packages/RangeChunk.chpl \
	packages/RecordParser.chpl \
	packages/ReplicatedVar.chpl \
	packages/Search.chpl \
	packages/Socket.chpl \
	packages/Sort.chpl \
	packages/SortedMap.chpl \
	packages/SortedSet.chpl \
	packages/TOML.chpl \
	packages/URL.chpl \
	packages/UnitTest.chpl \
	packages/UnorderedAtomics.chpl \
	packages/UnorderedCopy.chpl \
	packages/UnrolledLinkedList.chpl \
	packages/VisualDebug.chpl \
	packages/ZMQ.chpl \


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


#
# use absolute paths for internal modules so that the compiler can
# determine that they're internal (and not just modules that happen to
# live in a path named 'modules/internal/' so that they don't get
# indexed like the user-facing modules.
#
INTERNAL_MODULES_TO_DOCUMENT =                        \
	$(CHPL_MAKE_HOME)/modules/internal/Atomics.chpl       \
	$(CHPL_MAKE_HOME)/modules/internal/Bytes.chpl         \
	$(CHPL_MAKE_HOME)/modules/internal/ChapelArray.chpl   \
	$(CHPL_MAKE_HOME)/modules/internal/ChapelDomain.chpl  \
	$(CHPL_MAKE_HOME)/modules/internal/ChapelLocale.chpl  \
	$(CHPL_MAKE_HOME)/modules/internal/ChapelRange.chpl   \
	$(CHPL_MAKE_HOME)/modules/internal/ChapelSyncvar.chpl \
	$(CHPL_MAKE_HOME)/modules/internal/ChapelTuple.chpl   \
	$(CHPL_MAKE_HOME)/modules/internal/OwnedObject.chpl   \
	$(CHPL_MAKE_HOME)/modules/internal/SharedObject.chpl  \
	$(CHPL_MAKE_HOME)/modules/internal/String.chpl        \


documentation: $(SYS_CTYPES_MODULE_DOC)
	@echo "Generating module documentation with chpldoc"
	@export CHPLDOC_AUTHOR='Hewlett Packard Enterprise Development LP' && \
	  $(CHPLDOC) --home $(CHPL_MAKE_HOME) --save-sphinx ${MODULE_SPHINX} --no-html $(MODULES_TO_DOCUMENT) $(DISTS_TO_DOCUMENT) $(PACKAGES_TO_DOCUMENT) $(INTERNAL_MODULES_TO_DOCUMENT) $(SYS_CTYPES_MODULE_DOC)
	@echo "Running post-processing scripts"
	@./internal/fixInternalDocs.sh ${MODULE_SPHINX}
	@./dists/fixDistDocs.perl      ${MODULE_SPHINX}
	@echo "Copying generated module documentation to ${DOC_BUILD}"
	@rm -rf {DOC_BUILD}/modules
	@rm -rf {DOC_BUILD}/builtins
	@mkdir -p ${DOC_BUILD}/modules
	@mkdir -p ${DOC_BUILD}/builtins
	@cp -rf ${MODULE_SPHINX}/source/modules/standard   ${DOC_BUILD}/modules
	@cp -rf ${MODULE_SPHINX}/source/modules/packages   ${DOC_BUILD}/modules
	@cp -rf ${MODULE_SPHINX}/source/modules/dists      ${DOC_BUILD}/modules
	@cp -rf ${MODULE_SPHINX}/source/modules/layouts    ${DOC_BUILD}/modules
	@cp -rf ${DOC_RST}/meta/modules/* ${DOC_BUILD}/modules
	@cp -rf ${MODULE_SPHINX}/source/modules/internal/* ${DOC_BUILD}/builtins
	@echo "Updating generated module documentation in ${DOC_RST}"
	@mkdir -p ${DOC_RST}/modules
	@mkdir -p ${DOC_RST}/builtins
	@${COPY_IF_DIFFERENT} ${DOC_BUILD}/modules ${DOC_RST}/modules
	@${COPY_IF_DIFFERENT} ${DOC_BUILD}/builtins ${DOC_RST}/builtins
	@echo "Removing generated files and directories in modules/"
	@rm -rf ./docs
	@rm -rf ${MODULE_SPHINX}
	@rm -f ${CHPL_MAKE_HOME}/modules/$(SYS_CTYPES_MODULE_DOC)

clean-documentation:
	rm -rf ./docs
	rm -rf $(MODULE_SPHINX)

FORCE:
