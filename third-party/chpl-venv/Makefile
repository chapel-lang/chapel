ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/../..
endif

CHPL_MAKE_HOST_TARGET = --host
include $(CHPL_MAKE_HOME)/make/Makefile.base
include $(THIRD_PARTY_DIR)/chpl-venv/Makefile.include

# CHPL_PIP_INSTALL_PARAMS can be set to adjust the pip arguments,
# but if you want to build from source, set CHPL_PIP_FROM_SOURCE

ifdef CHPL_PIP_FROM_SOURCE
  CHPL_PIP_INSTALL_PARAMS=--no-binary :all:
endif

default: all

all: test-venv chpldoc-venv

clean: FORCE
	rm -rf build
	rm -rf $(CHPL_MAKE_HOME)/tools/chapel-py/build

cleanall: FORCE clean

clobber: FORCE clean
	rm -rf install

OLD_PYTHON_ERROR="python3 version 3.5 or later is required to install chpldoc and start_test dependencies. See https://www.python.org/"

$(CHPL_VENV_VIRTUALENV_DIR_DEPS1_OK):
	@# First check the python version is OK
	@if $(PYTHON) -c 'import sys; sys.exit(int(sys.version_info[:2] >= (3, 5)))'; then \
	  echo $(OLD_PYTHON_ERROR); \
	  exit 1; \
	fi

	@# Now create the venv to use to get the dependencies
	$(PYTHON) -m venv $(CHPL_VENV_VIRTUALENV_DIR)
	@# Now install wheel so we can pip install
	export PATH="$(CHPL_VENV_VIRTUALENV_BIN):$$PATH" && \
	export VIRTUAL_ENV=$(CHPL_VENV_VIRTUALENV_DIR) && \
	$(PIP) install --upgrade \
	  $(CHPL_PIP_INSTALL_PARAMS) $(LOCAL_PIP_FLAGS) wheel && \
	touch $(CHPL_VENV_VIRTUALENV_DIR_DEPS1_OK)

ifdef CHPL_PIP_FROM_SOURCE
$(CHPL_VENV_VIRTUALENV_DIR_DEPS2_OK): $(CHPL_VENV_VIRTUALENV_DIR_DEPS1_OK)
	@# Now install source dependencies so we can build from source
	export PATH="$(CHPL_VENV_VIRTUALENV_BIN):$$PATH" && \
	export VIRTUAL_ENV=$(CHPL_VENV_VIRTUALENV_DIR) && \
	$(PIP) install --upgrade \
	  $(CHPL_PIP_INSTALL_PARAMS) $(LOCAL_PIP_FLAGS) \
	  -r $(CHPL_VENV_CHPLDOC_REQUIREMENTS_FILE1) && \
	$(PIP) install --upgrade \
	   $(CHPL_PIP_INSTALL_PARAMS) $(LOCAL_PIP_FLAGS) \
	   -r $(CHPL_VENV_CHPLDOC_REQUIREMENTS_FILE2) && \
	touch $(CHPL_VENV_VIRTUALENV_DIR_DEPS2_OK)

else
$(CHPL_VENV_VIRTUALENV_DIR_DEPS2_OK): $(CHPL_VENV_VIRTUALENV_DIR_DEPS1_OK)
	touch $(CHPL_VENV_VIRTUALENV_DIR_DEPS2_OK)

endif

# Create the virtualenv to use during build.
#  (to allow for a different path to the system python3 in the future)
$(CHPL_VENV_VIRTUALENV_DIR_OK): $(CHPL_VENV_VIRTUALENV_DIR_DEPS1_OK) $(CHPL_VENV_VIRTUALENV_DIR_DEPS2_OK)
	touch $(CHPL_VENV_VIRTUALENV_DIR_OK)

# Phony convenience target for creating virtualenv.
create-virtualenv: $(CHPL_VENV_VIRTUALENV_DIR_OK)

$(CHPL_VENV_CHPLDEPS_MAIN): $(CHPL_VENV_VIRTUALENV_DIR_OK) $(CHPL_VENV_TEST_REQUIREMENTS_FILE) $(CHPL_VENV_CHPLDOC_REQUIREMENTS_FILE1) $(CHPL_VENV_CHPLDOC_REQUIREMENTS_FILE2) $(CHPL_VENV_CHPLDOC_REQUIREMENTS_FILE3) $(CHPL_VENV_C2CHAPEL_REQUIREMENTS_FILE) chpldeps-main.py
	@# Install dependencies to $(CHPL_VENV_CHPLDEPS)
	@# Rely on pip to create the directory
	export PATH="$(CHPL_VENV_VIRTUALENV_BIN):$$PATH" && \
	export VIRTUAL_ENV=$(CHPL_VENV_VIRTUALENV_DIR) && \
	$(PIP) install --upgrade $(CHPL_PIP_INSTALL_PARAMS) $(LOCAL_PIP_FLAGS) \
	  --target $(CHPL_VENV_CHPLDEPS) \
	  -r $(CHPL_VENV_TEST_REQUIREMENTS_FILE) \
	  -r $(CHPL_VENV_CHPLDOC_REQUIREMENTS_FILE1) \
	  -r $(CHPL_VENV_CHPLDOC_REQUIREMENTS_FILE2) \
	  -r $(CHPL_VENV_CHPLDOC_REQUIREMENTS_FILE3) \
	  -r $(CHPL_VENV_C2CHAPEL_REQUIREMENTS_FILE) && \
	cp chpldeps-main.py $(CHPL_VENV_CHPLDEPS_MAIN)

install-chpldeps: $(CHPL_VENV_CHPLDEPS_MAIN)

test-venv: install-chpldeps

chpldoc-venv: install-chpldeps

c2chapel-venv: install-chpldeps

# the Python bindings depend on libChplFrontendShared.so and some additional
# files listed in setup.py. The former is not consistently named (.so on UNIX,
# .dylib on macOS), making it harder to list as a dependency. The latter we
# could duplicate in the Makefile here, but that would be a possible 'gotcha':
# adding to setup.py but not here would break the build system. Thus, take
# the easy way out and mark this target as FORCE. This isn't too bad since setup.py
# generally skips re-compiling files that haven't changed.
#
# Since the target is always forced, no reason to keep around a sentinel file
# like ok1/ok2.
chapel-py-venv: FORCE $(CHPL_VENV_VIRTUALENV_DIR_OK)
	@# Install chapel-py's Python package into the Python Application
	@# directory structure at $(CHPL_VENV_CHPL_FRONTEND_PY_DEPS)
	@# this is python version specific
	export PATH="$(CHPL_VENV_VIRTUALENV_BIN):$$PATH" && \
	export VIRTUAL_ENV=$(CHPL_VENV_VIRTUALENV_DIR) && \
	CHPL_HOME=$(CHPL_MAKE_HOME) $(PIP) install --upgrade \
	  $(CHPL_PIP_INSTALL_PARAMS) $(LOCAL_PIP_FLAGS) \
	  --target $(CHPL_VENV_CHPL_FRONTEND_PY_DEPS) \
	  $(CHPL_MAKE_HOME)/tools/chapel-py \
	  -r $(CHPL_VENV_CHAPEL_PY_REQUIREMENTS_FILE)
	mkdir -p $(CHPL_MAKE_HOME)/tools/chapel-py/src/chapel/core
	CHPL_HOME=$(CHPL_MAKE_HOME) bash \
		$(CHPL_MAKE_HOME)/util/config/run-in-venv-with-python-bindings.bash \
		$(CHPL_MAKE_PYTHON) -c 'import chapel; print(chapel.Context()._get_pyi_file())' \
		>$(CHPL_MAKE_HOME)/tools/chapel-py/src/chapel/core/__init__.pyi && \
		cp $(CHPL_MAKE_HOME)/tools/chapel-py/src/chapel/core/__init__.pyi $(CHPL_VENV_CHPL_FRONTEND_PY_DEPS)/chapel/core.pyi

cls-test-venv: chapel-py-venv
	export PATH="$(CHPL_VENV_VIRTUALENV_BIN):$$PATH" && \
	export VIRTUAL_ENV=$(CHPL_VENV_VIRTUALENV_DIR) && \
	CHPL_HOME=$(CHPL_MAKE_HOME) $(PIP) install \
	  $(CHPL_PIP_INSTALL_PARAMS) $(LOCAL_PIP_FLAGS) \
	  --target $(CHPL_VENV_CHPLDEPS) \
	  $(CHPL_MAKE_HOME)/tools/chapel-py \
	  -r $(CHPL_VENV_CLS_TEST_REQUIREMENTS_FILE)
	@# Using --upgrade above breaks the test venv, because there's a bug
	@# in its interaction with --target. As a result, this command does
	@# not upgrade dependencies. Other commands are unaffected because
	@# they don't install executables into 'bin' (but pytest does).
	@echo "Warnings about non-empty directories above are expected."

chplcheck-venv: chapel-py-venv

chpl-language-server-venv: chapel-py-venv

install-requirements: install-chpldeps

$(CHPL_VENV_CHPLSPELL_REQS): $(CHPL_VENV_VIRTUALENV_DIR_OK) $(CHPL_VENV_CHPLSPELL_REQUIREMENTS_FILE)
	export PATH="$(CHPL_VENV_VIRTUALENV_BIN):$$PATH" && \
	export VIRTUAL_ENV=$(CHPL_VENV_VIRTUALENV_DIR) && \
	$(PIP) install --upgrade $(CHPL_PIP_INSTALL_PARAMS) $(LOCAL_PIP_FLAGS) \
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
