

ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(realpath $(shell pwd)/../..)
endif

ifndef CHPL_MASON_HOME
export CHPL_MASON_HOME=$(HOME)
endif

include $(CHPL_MAKE_HOME)/make/Makefile.base

bdir=$(CHPL_BIN_DIR)
link=$(bdir)/mason

TOML=$(CHPL_HOME)/test/modules/packages/Toml/
REGISTRY=https://github.com/chapel-lang/mason-registry

all: mason install

mason:
	@mkdir -p $(CHPL_MASON_HOME)/.mason
	@mkdir -p $(CHPL_MASON_HOME)/.mason/src
	@git -C $(CHPL_MASON_HOME)/.mason/ clone $(REGISTRY) registry
	@echo "Building Mason..."
	@chpl mason.chpl -M $(TOML)

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