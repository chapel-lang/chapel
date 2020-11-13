ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/../..
endif

CHPL_MAKE_HOST_TARGET = --host
include $(CHPL_MAKE_HOME)/make/Makefile.base
include $(THIRD_PARTY_DIR)/chpl-venv/Makefile.include

default: all

all: test-venv chpldoc-venv

clean: FORCE
	rm -rf build

cleanall: FORCE clean

clobber: FORCE clean
	rm -rf install

OLD_PYTHON_ERROR="python3 version 3.5 or later is required to install chpldoc and start_test dependencies. See https://www.python.org/"

# Create the virtualenv to use during build.
#  (to allow for a different path to the system python3 in the future)
$(CHPL_VENV_VIRTUALENV_DIR_OK):
	@# First check the python version is OK
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

	@# Now create the venv to use to get the dependencies
	$(PYTHON) -m venv $(CHPL_VENV_VIRTUALENV_DIR)
	export PATH="$(CHPL_VENV_VIRTUALENV_BIN):$$PATH" && \
	export VIRTUAL_ENV=$(CHPL_VENV_VIRTUALENV_DIR) && \
	$(PIP) install \
	--upgrade $(CHPL_PIP_INSTALL_PARAMS) $(LOCAL_PIP_FLAGS) wheel && \
	touch $(CHPL_VENV_VIRTUALENV_DIR_OK)

# Phony convenience target for creating virtualenv.
create-virtualenv: $(CHPL_VENV_VIRTUALENV_DIR_OK)

$(CHPL_VENV_CHPLDEPS_MAIN): $(CHPL_VENV_VIRTUALENV_DIR_OK) $(CHPL_VENV_TEST_REQUIREMENTS_FILE) $(CHPL_VENV_CHPLDOC_REQUIREMENTS_FILE) $(CHPL_VENV_C2CHAPEL_REQUIREMENTS_FILE) chpldeps-main.py
	@# Install dependencies to $(CHPL_VENV_CHPLDEPS)
	@# Rely on pip to create the directory
	export PATH="$(CHPL_VENV_VIRTUALENV_BIN):$$PATH" && \
	export VIRTUAL_ENV=$(CHPL_VENV_VIRTUALENV_DIR) && \
	$(PIP) install --upgrade $(CHPL_PIP_INSTALL_PARAMS) $(LOCAL_PIP_FLAGS) \
	  --target $(CHPL_VENV_CHPLDEPS) \
	  -r $(CHPL_VENV_TEST_REQUIREMENTS_FILE) \
	  -r $(CHPL_VENV_CHPLDOC_REQUIREMENTS_FILE) \
	  -r $(CHPL_VENV_C2CHAPEL_REQUIREMENTS_FILE) && \
	cp chpldeps-main.py $(CHPL_VENV_CHPLDEPS_MAIN)

install-chpldeps: $(CHPL_VENV_CHPLDEPS_MAIN)

test-venv: install-chpldeps

chpldoc-venv: install-chpldeps

c2chapel-venv: install-chpldeps

install-requirements: install-chpldeps

$(CHPL_VENV_CHPLSPELL_REQS): $(CHPL_VENV_VIRTUALENV_DIR_OK) $(CHPL_VENV_CHPLSPELL_REQUIREMENTS_FILE)
	export PATH="$(CHPL_VENV_VIRTUALENV_BIN):$$PATH" && \
	export VIRTUAL_ENV=$(CHPL_VENV_VIRTUALENV_DIR) && \
	$(PIP) install \
	  --upgrade $(CHPL_PIP_INSTALL_PARAMS) $(LOCAL_PIP_FLAGS) \
	  -r $(CHPL_VENV_CHPLSPELL_REQUIREMENTS_FILE) && \
	touch $(CHPL_VENV_CHPLSPELL_REQS)

chplspell-venv: $(CHPL_VENV_CHPLSPELL_REQS)

# Fix up the chpl-venv installation in a Cray Chapel module that was built using a non-standard
# Python installation (ie, the Python used was not installed under "/usr/...").
# Only applicable when building a Cray Chapel module under certain conditions.
use-system-python: FORCE
	@echo ignoring use-system-python

FORCE:

.PHONY: install-requirements create-virtualenv test-venv chpldoc-venv c2chapel-venv chplspell-venv install-chpldeps

.NOTPARALLEL:
