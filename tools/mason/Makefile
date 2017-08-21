

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

registry = $(CHPL_MASON_HOME)/.mason/registry/


all: mason install


mason:
	@mkdir -p $(CHPL_MASON_HOME)/.mason
	@mkdir -p $(CHPL_MASON_HOME)/.mason/src
ifneq ($(wildcard $(registry)),)
	@echo "Removing old registry..."
	@rm -rf $(CHPL_MASON_HOME)/.mason/registry
endif
	@echo "Cloning Registry..."
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
	@echo "Removing mason..."
	@rm -f $(shell pwd)/mason
ifneq ($(wildcard $(link)),)
	@echo "Removing old symbolic link..."
	rm -f $(link)
	@echo
endif
