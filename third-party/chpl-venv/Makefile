ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/../..
endif

CHPL_MAKE_HOST_TARGET = --host
include $(CHPL_MAKE_HOME)/make/Makefile.base
include $(THIRD_PARTY_DIR)/chpl-venv/Makefile.include

default: all

all: test-venv chpldoc-venv

clean: FORCE

cleanall: FORCE

clobber: FORCE
	rm -rf install

check-python:
ifeq ($(wildcard $(PYTHON)),)
	$(error python and pip are required to install chpldoc \
	        and start_test dependencies. See https://www.python.org/)
endif

check-exes: check-python

$(CHPL_VENV_INSTALL_DIR):
	mkdir -p $@

# Install pip via get-pip.py to ensure we have an updated version
$(PIP): $(CHPL_VENV_INSTALL_DIR)
	@if [ -z "$$CHPL_PIP" ]; then \
	  echo "Installing local copy of pip with get-pip.py from $(GETPIP)"; \
	  (cd install && curl -O $(GETPIP) || \
	  { echo "Failed on command: curl -O $(GETPIP)."; \
	    echo "Try setting CHPL_PIP to $$(which pip) and trying again"; \
	    exit 1; } && \
	  $(GET_PIP_COMMAND) ) \
	fi

# Install virtualenv program.
$(CHPL_VENV_VIRTUALENV): check-exes $(PIP)
	export PYTHONPATH="$$CHPL_PYTHONPATH:$(PIPLIBS):$$PYTHONPATH" && \
	python $(PIP) install -U --force-reinstall --ignore-installed $(LOCAL_PIP_FLAGS) \
	 --prefix=$(CHPL_VENV_INSTALL_DIR) $(CHPL_PIP_INSTALL_PARAMS) $(shell cat virtualenv.txt)

# Phony convenience target for installing virtualenv.
virtualenv: $(CHPL_VENV_VIRTUALENV)

# Create the virtualenv.
$(CHPL_VENV_VIRTUALENV_DIR): $(CHPL_VENV_VIRTUALENV)
	export PYTHONPATH="$(PIPLIBS):$$PYTHONPATH" && \
	python $(CHPL_VENV_VIRTUALENV) --python=$(PYTHON) $@

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
	export PYTHONPATH="$(PIPLIBS):$$PYTHONPATH" && \
	export PATH="$(CHPL_VENV_VIRTUALENV_BIN):$$PATH" && \
	export VIRTUAL_ENV=$(CHPL_VENV_VIRTUALENV_DIR) && \
	python $(CHPL_VENV_VIRTUALENV_BIN)/pip install \
	-U --force-reinstall $(CHPL_PIP_INSTALL_PARAMS) $(LOCAL_PIP_FLAGS) -r test-requirements.txt && \
	python $(CHPL_VENV_VIRTUALENV) --relocatable $(CHPL_VENV_VIRTUALENV_DIR) && \
	touch $(CHPL_VENV_TEST_REQS)

$(CHPL_VENV_SPHINX_BUILD): $(CHPL_VENV_VIRTUALENV_DIR)
	export PYTHONPATH="$(PIPLIBS):$$PYTHONPATH" && \
	export PATH="$(CHPL_VENV_VIRTUALENV_BIN):$$PATH" && \
	export VIRTUAL_ENV=$(CHPL_VENV_VIRTUALENV_DIR) && \
	python $(CHPL_VENV_VIRTUALENV_BIN)/pip install \
	-U --force-reinstall $(CHPL_PIP_INSTALL_PARAMS) $(LOCAL_PIP_FLAGS) -r chpldoc-requirements.txt && \
	python $(CHPL_VENV_VIRTUALENV) --relocatable $(CHPL_VENV_VIRTUALENV_DIR)

$(CHPL_VENV_CHPLSPELL_REQS): $(CHPL_VENV_VIRTUALENV_DIR)
	export PYTHONPATH="$(PIPLIBS):$$PYTHONPATH" && \
	export PATH="$(CHPL_VENV_VIRTUALENV_BIN):$$PATH" && \
	export VIRTUAL_ENV=$(CHPL_VENV_VIRTUALENV_DIR) && \
	python $(CHPL_VENV_VIRTUALENV_BIN)/pip install \
	-U --force-reinstall $(CHPL_PIP_INSTALL_PARAMS) $(LOCAL_PIP_FLAGS) -r chplspell-requirements.txt && \
	python $(CHPL_VENV_VIRTUALENV) --relocatable $(CHPL_VENV_VIRTUALENV_DIR) && \
	touch $(CHPL_VENV_CHPLSPELL_REQS)

test-venv: $(CHPL_VENV_TEST_REQS)

chpldoc-venv: $(CHPL_VENV_SPHINX_BUILD)

chplspell-venv: $(CHPL_VENV_CHPLSPELL_REQS)

# Phony convenience target for install python packages.
install-requirements: rm-test-reqs rm-sphinx-build $(CHPL_VENV_TEST_REQS) $(CHPL_VENV_SPHINX_BUILD) $(CHPL_VENV_CHPLSPELL_REQS)

# Remove chpl-test-reqs, forcing install-requirements to be rebuilt.
rm-test-reqs: FORCE
	@rm -f $(CHPL_VENV_TEST_REQS)

# Remove sphinx-build, forcing install-requirements to be rebuilt.
rm-sphinx-build: FORCE
	@rm -f $(CHPL_VENV_SPHINX_BUILD)

# Remove chplspell-reqs, forcing install-requirements to be rebuilt.
rm-chplspell-reqs: FORCE
	@rm -f $(CHPL_VENV_CHPLSPELL_REQS)

# Fix up the chpl-venv installation in a Cray Chapel module that was built using a non-standard
# Python installation (ie, the Python used was not installed under "/usr/...").
# Only applicable when building a Cray Chapel module under certain conditions.
use-system-python: FORCE
	@./venv-use-sys-python.py $(CHPL_VENV_INSTALL_DIR)

FORCE:

.PHONY: install-requirements create-virtualenv virtualenv check-exes rm-sphinx-build check-pip check-python

.NOTPARALLEL:
