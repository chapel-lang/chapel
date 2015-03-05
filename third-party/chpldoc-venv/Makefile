ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/../..
endif

CHPL_MAKE_HOST_TARGET = --host
include $(CHPL_MAKE_HOME)/make/Makefile.base

PYTHON = $(shell which python)
EASY_INSTALL = $(shell which easy_install)

default: all

all: $(CHPLDOC_VENV_SPHINX_BUILD)

clean: FORCE

cleanall: FORCE

clobber: FORCE
	rm -rf install

$(PYTHON):
ifeq ($(wildcard $(PYTHON)),)
	$(error python and easy_install are required to install chpldoc \
	        dependencies. See https://www.python.org/)
endif

$(EASY_INSTALL):
ifeq ($(wildcard $(EASY_INSTALL)),)
	$(error python and easy_install are required to install chpldoc \
	        dependencies. See https://www.python.org/)
endif

check-exes: $(PYTHON) $(EASY_INSTALL)

$(CHPLDOC_VENV_INSTALL_DIR):
	mkdir -p $@

# Install virtualenv program.
$(CHPLDOC_VENV_VIRTUALENV): $(CHPLDOC_VENV_INSTALL_DIR) $(PYTHON) $(EASY_INSTALL)
	export PYTHONPATH=$(CHPLDOC_VENV_INSTALL_DIR):$$PYTHONPATH && \
	$(EASY_INSTALL) --install-dir=$(CHPLDOC_VENV_INSTALL_DIR) $(shell cat virtualenv.txt)

# Phony convenience target for installing virtualenv.
virtualenv: $(CHPLDOC_VENV_VIRTUALENV)

# Create the virtualenv.
$(CHPLDOC_VENV_VIRTUALENV_DIR): $(CHPLDOC_VENV_VIRTUALENV)
	$(CHPLDOC_VENV_VIRTUALENV) --python=$(PYTHON) $@

# Phony convenience target for creating virtualenv.
create-virtualenv: $(CHPLDOC_VENV_VIRTUALENV_DIR)

# Install requirements.txt inside virtualenv. Use sphinx-build executable to
# indicate success or failure. Call virtualenv --relocatable <virtualenv_dir>
# after installing, which takes the hardcoded paths out of the installed
# "python binaries".
$(CHPLDOC_VENV_SPHINX_BUILD): $(CHPLDOC_VENV_VIRTUALENV_DIR)
	export PATH=$(CHPLDOC_VENV_VIRTUALENV_BIN):$$PATH && \
	export VIRTUAL_ENV=$(CHPLDOC_VENV_VIRTUALENV_DIR) && \
	pip install -U --force-reinstall -r requirements.txt && \
	$(CHPLDOC_VENV_VIRTUALENV) --relocatable $(CHPLDOC_VENV_VIRTUALENV_DIR)

# Phony convenience target for install python packages.
install-requirements: rm-sphinx-build $(CHPLDOC_VENV_SPHINX_BUILD)

# Remove sphinx-build, forcing install-requirements to be rebuilt.
rm-sphinx-build: FORCE
	@rm -f $(CHPLDOC_VENV_SPHINX_BUILD)

FORCE:

.PHONY: install-requirements create-virtualenv virtualenv check-exes rm-sphinx-build

.NOTPARALLEL:
