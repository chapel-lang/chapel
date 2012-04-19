default:
	@echo "Use \$$CHPL_HOME/util/start_test to run the testing system"

clean:
	../util/start_test --clean-only > /dev/null

#
# Set FILES on the make command line to a list of file or directory name
# (standard wildcard notation accepted)
#
RM_FILES = a.out a.out_real svn-commit.\* \*~ $(FILES)
FIND_FILES = $(foreach f, $(RM_FILES), -o -name $(f) -print)

#
# Set RM_FLAGS=-r on the make command line if you want to remove directories
#
RM_FLAGS += -v

clobber: clean
	find . -name .svn -prune $(FIND_FILES) | xargs --interactive rm $(RM_FLAGS)

