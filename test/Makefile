default:
	@echo "Use \$$CHPL_HOME/util/start_test to run the testing system"

clean:
	../util/start_test --clean-only > /dev/null

#
# Set FILES on the make command line to a list of file or directory name
# (standard wildcard notation accepted)
#
RM_FILES += a.out a.out_real svn-commit.\* \*~ $(FILES)
FIND_FILES = $(foreach f, $(RM_FILES), -o -name $(f) -print0)

#
# Set RM_FLAGS=-r on the make command line if you want to remove directories
#
RM_FLAGS += -rv
ifneq ($(NO_INTERACTIVE), 1)
	XARGS_FLAGS += --interactive
endif

clobber: clean
	rm -rf $(RM_FLAGS) ./release/examples/spec
	find . -name .svn -prune $(FIND_FILES) | xargs $(XARGS_FLAGS) --null rm $(RM_FLAGS)

