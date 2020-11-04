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

OLD_PYTHON_ERROR="python3 version 3.5 or later is required to install chpldoc and start_test dependencies. See https://www.python.org/"

check-python:
	@case `$(PYTHON) --version` in \
	  *"Python 3.0"*) \
	    echo $(OLD_PYTHON_ERROR) ; \
            exit 1 ; \
	    ;; \
	  *"Python 3.1"*) \
	    echo $(OLD_PYTHON_ERROR) ; \
            exit 1 ; \
	    ;; \
	  *"Python 3.2"*) \
	    echo $(OLD_PYTHON_ERROR) ; \
            exit 1 ; \
	    ;; \
	  *"Python 3.3"*) \
	    echo $(OLD_PYTHON_ERROR) ; \
            exit 1 ; \
	    ;; \
	  *"Python 3.4"*) \
	    echo $(OLD_PYTHON_ERROR) ; \
            exit 1 ; \
	    ;; \
	  *"Python 3"*) \
	    ;; \
	  *) \
	    echo $(OLD_PYTHON_ERROR) ; \
            exit 1 ; \
	    ;; \
	esac
	@# Check to see if python3 -m pip succeeds and install pip if not
	@PIP_VERS=`$(PIP) --version` ; \
	if $(PIP) --version ; then \
	  : ; \
	else \
	  echo "Installing local copy of pip with get-pip.py from $(GETPIP)"; \
	  (mkdir -p install && \
	   cd install && \
	   ( (curl -O $(GETPIP) || wget $(GETPIP)) || \
	     { echo "Failed getpip download with" ; \
	       echo "  curl -O $(GETPIP) || wget $(GETPIP)" ; \
	       echo "Try adding pip to your Python installation" ; \
	       exit 1; }) && \
	   $(GET_PIP_COMMAND) ) ; \
	fi

check-exes: check-python

$(CHPL_VENV_INSTALL_DIR):
	mkdir -p $@

# Create the virtualenv.
$(CHPL_VENV_VIRTUALENV_DIR):
	$(PYTHON) -m venv $@
	export PYTHONPATH="$(PIPLIBS):$$PYTHONPATH" && \
	export PATH="$(CHPL_VENV_VIRTUALENV_BIN):$$PATH" && \
	export VIRTUAL_ENV=$(CHPL_VENV_VIRTUALENV_DIR) && \
	$(PIP) install \
	--upgrade $(CHPL_PIP_INSTALL_PARAMS) $(LOCAL_PIP_FLAGS) wheel && \
	rm  $(CHPL_VENV_VIRTUALENV_BIN)/python3
	cp python3-wrapper $(CHPL_VENV_VIRTUALENV_BIN)/python3
	touch $(CHPL_VENV_VIRTUALENV_DIR)

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
	$(PIP) install \
	--upgrade $(CHPL_PIP_INSTALL_PARAMS) $(LOCAL_PIP_FLAGS) -r test-requirements.txt && \
	touch $(CHPL_VENV_TEST_REQS)

$(CHPL_VENV_SPHINX_BUILD): $(CHPL_VENV_VIRTUALENV_DIR)
	export PYTHONPATH="$(PIPLIBS):$$PYTHONPATH" && \
	export PATH="$(CHPL_VENV_VIRTUALENV_BIN):$$PATH" && \
	export VIRTUAL_ENV=$(CHPL_VENV_VIRTUALENV_DIR) && \
	$(PIP) install \
	--upgrade $(CHPL_PIP_INSTALL_PARAMS) $(LOCAL_PIP_FLAGS) -r chpldoc-requirements.txt && \
	touch $(CHPL_VENV_SPHINX_BUILD)

$(CHPL_VENV_C2CHAPEL_REQS): $(CHPL_VENV_VIRTUALENV_DIR)
	export PYTHONPATH="$(PIPLIBS):$$PYTHONPATH" && \
	export PATH="$(CHPL_VENV_VIRTUALENV_BIN):$$PATH" && \
	export VIRTUAL_ENV=$(CHPL_VENV_VIRTUALENV_DIR) && \
	$(PIP) install \
	--upgrade $(CHPL_PIP_INSTALL_PARAMS) $(LOCAL_PIP_FLAGS) -r c2chapel-requirements.txt && \
	touch $(CHPL_VENV_C2CHAPEL_REQS)

$(CHPL_VENV_CHPLSPELL_REQS): $(CHPL_VENV_VIRTUALENV_DIR)
	export PYTHONPATH="$(PIPLIBS):$$PYTHONPATH" && \
	export PATH="$(CHPL_VENV_VIRTUALENV_BIN):$$PATH" && \
	export VIRTUAL_ENV=$(CHPL_VENV_VIRTUALENV_DIR) && \
	$(PIP) install \
	--upgrade $(CHPL_PIP_INSTALL_PARAMS) $(LOCAL_PIP_FLAGS) -r chplspell-requirements.txt && \
	touch $(CHPL_VENV_CHPLSPELL_REQS)

test-venv: $(CHPL_VENV_TEST_REQS)

chpldoc-venv: $(CHPL_VENV_SPHINX_BUILD)

c2chapel-venv: $(CHPL_VENV_C2CHAPEL_REQS)

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
	@echo ignoring use-system-python

FORCE:

.PHONY: install-requirements create-virtualenv virtualenv check-exes rm-sphinx-build check-pip check-python

.NOTPARALLEL:
