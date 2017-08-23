

ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(realpath $(shell pwd)/../..)
endif

ifndef MASON_HOME
export MASON_HOME=$(HOME)
endif

include $(CHPL_MAKE_HOME)/make/Makefile.base

bdir=$(CHPL_BIN_DIR)
link=$(bdir)/mason

TOML=$(CHPL_HOME)/test/modules/packages/Toml/
REGISTRY=https://github.com/chapel-lang/mason-registry

registry = $(MASON_HOME)/.mason/registry/


all: install


mason:
	@mkdir -p $(MASON_HOME)/.mason
	@mkdir -p $(MASON_HOME)/.mason/src
ifneq ($(wildcard $(registry)),)
	@echo "Removing old registry..."
	@rm -rf $(MASON_HOME)/.mason/registry
endif
	@echo "Registry - $(registry)"
	@git -C $(MASON_HOME)/.mason/ clone $(REGISTRY) registry
	@echo "Building Mason..."
	@chpl mason.chpl -M $(TOML)

install: mason
ifneq ($(wildcard $(link)),)
	@echo "Removing old symbolic link..."
	rm -f $(link)
	@echo
endif
	@echo "Installing symbolic link..."
	@mkdir -p $(bdir)
	@ln -s $(shell pwd)/mason $(link)

update:
	@echo "Re-building Mason..."
	@chpl mason.chpl -M $(TOML)

clean:
	@echo "Removing Mason..."
	rm -f $(shell pwd)/mason
ifneq ($(wildcard $(link)),)
	@echo "Removing old symbolic link..."
	@rm -f $(link)
	@echo
endif


clobber: clean
	@echo "Removing Registry..."
	rm -rf $(MASON_HOME)/.mason/registry