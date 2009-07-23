export CHPL_COMM=mpi
CHAPEL_ROOT = $(shell pwd)/../..
CHPL_MAKE_HOST_TARGET = --target
include $(CHAPEL_ROOT)/make/Makefile.base

OPENMPI_INSTALL_DIR = $(OPENMPI_BASE_DIR)/install/$(CHPL_MAKE_TARGET_PLATFORM)-$(CHPL_MAKE_TARGET_COMPILER)
OPENMPI_BUILD_DIR = $(OPENMPI_BASE_DIR)/build/$(CHPL_MAKE_TARGET_PLATFORM)-$(CHPL_MAKE_TARGET_COMPILER)
OPENMPI_SRC_DIR = $(OPENMPI_BASE_DIR)/openmpi-$(OPENMPI_VERSION)


OPENMPI_CFG_SCRIPT = $(OPENMPI_SRC_DIR)/configure
OPENMPI_CFG_OPTIONS += --prefix=$(OPENMPI_INSTALL_DIR) --enable-mpi-threads --with-threads=posix --enable-mpi-f77=no --enable-orterun-prefix-by-default 


default: all

all: openmpi

clean: FORCE
	cd $(OPENMPI_BUILD_DIR) && make clean

clobber: FORCE
	rm -fr $(OPENMPI_BUILD_DIR) $(OPENMPI_INSTALL_DIR)

openmpi-config: FORCE
	mkdir -p $(OPENMPI_BUILD_DIR)
	cd $(OPENMPI_BUILD_DIR) && $(OPENMPI_CFG_SCRIPT) $(OPENMPI_CFG_OPTIONS)

openmpi-build: FORCE
	cd $(OPENMPI_BUILD_DIR) && $(MAKE) all

openmpi-install: FORCE
	cd $(OPENMPI_BUILD_DIR) && $(MAKE) install

openmpi: openmpi-config openmpi-build openmpi-install

FORCE:
