

ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(realpath $(shell pwd)/../..)
endif

include $(CHPL_MAKE_HOME)/make/Makefile.base

bdir=$(CHPL_BIN_DIR)
link=$(bdir)/mason


REGISTRY=https://github.com/Spartee/mason-registry

all: mason

mason:
	@echo "Building Mason..."
	#build
	#install

build:
	@mkdir -p ~/.mason
	@mkdir -p ~/.mason/src
	@git -C ~/.mason/ clone $(REGISTRY) registry
	@echo "Building Mason..."
	@chpl mason.chpl

install: 
ifneq ($(wildcard $(link)),)
	@echo "Removing old symbolic link..."
	rm -f $(link)
	@echo
endif
	@echo "Installing symbolic link..."
	mkdir -p $(bdir)
	ln -s $(shell pwd)/mason $(link)

clean:
	rm -f $(shell pwd)/mason
ifneq ($(wildcard $(link)),)
	@echo "Removing old symbolic link..."
	rm -f $(link)
	@echo
endif