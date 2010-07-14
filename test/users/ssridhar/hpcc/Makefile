CHPL = chpl

ifeq ($(CHPL_DEVELOPER), true)

FLAGS = $(F) --devel --gen-ids -g --print-commands --print-passes

else

FLAGS = $(F) --no-devel --fast --print-commands --print-passes

endif

TARGETS = \
	ra \
	ra_mla \
	ra_sla \
	ra_sda \
	testLCG

default: all

all: $(TARGETS)

clean: FORCE
	rm -rf $(TARGETS) *_real c_* 

ra: HPCCProblemSize.chpl RARandomStream.chpl ra.chpl
	$(CHPL) -o $@ $(FLAGS) --savec c_$@ $^

ra_mla: HPCCProblemSize.chpl RARandomStream.chpl MutexLock.chpl MutexLock.h ra_mutex_lock_array.chpl
	$(CHPL) -o $@ $(FLAGS) --savec c_$@ $^ 

ra_sla: HPCCProblemSize.chpl RARandomStream.chpl ra_sync_lock_array.chpl
	$(CHPL) -o $@ $(FLAGS) --savec c_$@ $^ 

ra_sda: HPCCProblemSize.chpl RARandomStream.chpl ra_sync_data_array.chpl
	$(CHPL) -o $@ $(FLAGS) --savec c_$@ $^ 

testLCG: HPCCProblemSize.chpl RARandomStream.chpl testLCG.chpl
	$(CHPL) -o $@ $(FLAGS) --savec c_$@ $^

FORCE:
