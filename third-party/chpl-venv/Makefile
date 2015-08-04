ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/../..
endif

CHPL_MAKE_HOST_TARGET = --host
include $(CHPL_MAKE_HOME)/make/Makefile.base

PYTHON = $(shell which python)
EASY_INSTALL = $(shell which easy_install)

default: all

all: test-venv chpldoc-venv

clean: FORCE

cleanall: FORCE

clobber: FORCE
	rm -rf install

check-python:
ifeq ($(wildcard $(PYTHON)),)
	$(error python and easy_install are required to install chpldoc \
	        and start_test dependencies. See https://www.python.org/)
endif

check-easy-install:
ifeq ($(wildcard $(EASY_INSTALL)),)
	$(error python and easy_install are required to install chpldoc \
	        and start_test dependencies. See https://www.python.org/)
endif

check-exes: check-python check-easy-install

$(CHPL_VENV_INSTALL_DIR):
	mkdir -p $@

# Install virtualenv program.
$(CHPL_VENV_VIRTUALENV): $(CHPL_VENV_INSTALL_DIR) check-exes
	export PYTHONPATH=$(CHPL_VENV_INSTALL_DIR):$$PYTHONPATH && \
	$(EASY_INSTALL) --install-dir=$(CHPL_VENV_INSTALL_DIR) $(shell cat virtualenv.txt)

# Phony convenience target for installing virtualenv.
virtualenv: $(CHPL_VENV_VIRTUALENV)

# Create the virtualenv.
$(CHPL_VENV_VIRTUALENV_DIR): $(CHPL_VENV_VIRTUALENV)
	$(CHPL_VENV_VIRTUALENV) --python=$(PYTHON) $@

# Phony convenience target for creating virtualenv.
create-virtualenv: $(CHPL_VENV_VIRTUALENV_DIR)

# Install requirements.txt inside virtualenv. Use sphinx-build executable to
# indicate success or failure. Call virtualenv --relocatable <virtualenv_dir>
# after installing, which takes the hardcoded paths out of the installed
# "python binaries".
#
# In order to avoid issues with long hash bangs (#!) in the pip script, call it
# directly. For example, in some cases the hash bang can be longer than OS
# supports, so when pip is called the shell fails with an error like:
#
#   /bin/sh: .../path/to/bin/pip: <truncated hash bang>: bad interpreter: No such file or directory.
#
# By calling `python .../path/to/pip ...`, this issue is circumvented.
$(CHPL_VENV_TEST_REQS): $(CHPL_VENV_VIRTUALENV_DIR)
	export PATH=$(CHPL_VENV_VIRTUALENV_BIN):$$PATH && \
	export VIRTUAL_ENV=$(CHPL_VENV_VIRTUALENV_DIR) && \
	python $(CHPL_VENV_VIRTUALENV_BIN)/pip install \
	-U --force-reinstall -r test-requirements.txt && \
	$(CHPL_VENV_VIRTUALENV) --relocatable $(CHPL_VENV_VIRTUALENV_DIR) && \
	touch $(CHPL_VENV_TEST_REQS)

$(CHPL_VENV_SPHINX_BUILD): $(CHPL_VENV_VIRTUALENV_DIR)
	export PATH=$(CHPL_VENV_VIRTUALENV_BIN):$$PATH && \
	export VIRTUAL_ENV=$(CHPL_VENV_VIRTUALENV_DIR) && \
	python $(CHPL_VENV_VIRTUALENV_BIN)/pip install \
	-U --force-reinstall -r chpldoc-requirements.txt && \
	$(CHPL_VENV_VIRTUALENV) --relocatable $(CHPL_VENV_VIRTUALENV_DIR)

test-venv: $(CHPL_VENV_TEST_REQS)

chpldoc-venv: $(CHPL_VENV_SPHINX_BUILD)

# Phony convenience target for install python packages.
install-requirements: rm-test-reqs rm-sphinx-build $(CHPL_VENV_TEST_REQS) $(CHPL_VENV_SPHINX_BUILD)

# Remove chpl-test-reqs, forcing install-requirements to be rebuilt.
rm-test-reqs: FORCE
	@rm -f $(CHPL_VENV_TEST_REQS)

# Remove sphinx-build, forcing install-requirements to be rebuilt.
rm-sphinx-build: FORCE
	@rm -f $(CHPL_VENV_SPHINX_BUILD)


FORCE:

.PHONY: install-requirements create-virtualenv virtualenv check-exes rm-sphinx-build check-easy-install check-python

.NOTPARALLEL:
