# The intermediate .o files allow us to use the auto-generated .d files.
# NB do not hand-create *.o and *.d - they are wiped out upon 'make clean'.
# NB the dependencies on *.patch are not traced.

INSTALL_DIR = ..

ifeq ($(DB),)
DBG=-DDBG=0
TEST_NAMES =       \
  01-error-global   \
  02-error-local     \
  03-error-in-fun     \
  04-error-in-var-iter \
  11-capture-in-begin   \
  12-no-capture-in-begin \
  21-capture-in-cobegin   \
  22-no-capture-in-cobegin \
  31-capture-in-coforall    \
  32-no-capture-in-coforall  \

else
# smaller tests, for debugging
DBG=-DDBG=1
TEST_NAMES =    \
  capture-begin  \
  capture-cobegin \
  capture-coforall \
  no-capture-begin  \
  no-capture-cobegin \
  no-capture-coforall \

endif

CHPL_FILES    = $(TEST_NAMES:%=$(INSTALL_DIR)/%.chpl)
INTERIM_CHPL  = $(TEST_NAMES:%=%.o)
SOURCE_CHPL   = $(TEST_NAMES:%=%.cpp)
SAMPLE_CHPL   = sample.o

GOOD_FILES    = $(TEST_NAMES:%=$(INSTALL_DIR)/%.good)
INTERIM_GOOD  = $(TEST_NAMES:%=%.good.o)
SOURCE_GOOD   = $(TEST_NAMES:%=%.good.c)
SAMPLE_GOOD   = sample.good.o

#------------------------------------------------------------------

# The default goal
all: $(CHPL_FILES) $(GOOD_FILES)

# run start_test
st: all
	start_test $(CHPL_FILES)

clean:
	rm -f *.d *.o

clobber: clean
	rm -f $(CHPL_FILES) $(GOOD_FILES)

# test the conversion .cpp -> .o
t1: $(SAMPLE_CHPL)
	 #-----------#
	@cat sample.cpp
	 #-----------#
	@cat sample.o
	 #-----------#

# test the conversion .good.c -> .good.o
t2: $(SAMPLE_GOOD)
	 #-----------#
	@cat sample.good.c
	 #-----------#
	@cat sample.good.o
	 #-----------#

# Do not remove automatically any intermediate files.
.SECONDARY:

-include $(TEST_NAMES:%=%.d) $(TEST_NAMES:%=%.good.d)

#------------------------------------------------------------------

# run the C preprocessor on .good
#  $(INTERIM_GOOD) $(SAMPLE_GOOD):
%.good.o: %.good.c
	gcc -MMD -E $< | grep -v '^#' > $@
	[ -r $<.patch ] && patch < $<.patch || true

# install Chapel files
#  $(GOOD_FILES):
$(INSTALL_DIR)/%.good: %.good.o
	cp $< $@

# run the C preprocessor on Chapel code
#  $(INTERIM_CHPL) $(SAMPLE_FILES):
%.o: %.cpp
	gcc -MMD -E -C -CC -P $(DBG) $< | ./removeEmptyLines > $@
	[ -r $<.patch ] && patch < $<.patch || true

# install Chapel files
#  $(CHPL_FILES):
$(INSTALL_DIR)/%.chpl: %.o
	cp $< $@
