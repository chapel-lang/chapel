CHPL = chpl

FLAGS += --conditional-dynamic-dispatch-limit 100

ifeq ($(CHPL_DEVELOPER), true)

FLAGS += $(F) --devel --gen-ids -g --print-commands --print-passes

else

FLAGS += $(F) --no-devel --fast --print-commands --print-passes

endif

TARGETS = \
	ra \
	ra_mla \
	ra_sla \
	ra_sda \
	testLCG \
	testRAR \
	ra2 \
	ra2_sda \
	aswap \
	Svnra Svnra_mla Svnra_sla \
	SvntestLCG SvntestRAR \
	Svnra2

default: all

all: $(TARGETS)

clean: FORCE
	rm -rf $(TARGETS) *_real c_* 

ra: HPCCProblemSize.chpl RARandomStream.chpl ra.chpl
	$(CHPL) -o $(CHPLSVN)$@ $(FLAGS) --savec c_$@ $^

ra_mla: HPCCProblemSize.chpl RARandomStream.chpl MutexLock.chpl MutexLock.h ra_mutex_lock_array.chpl
	$(CHPL) -o $(CHPLSVN)$@ $(FLAGS) --savec c_$@ $^ 

ra_sla: HPCCProblemSize.chpl RARandomStream.chpl ra_sync_lock_array.chpl
	$(CHPL) -o $(CHPLSVN)$@ $(FLAGS) --savec c_$@ $^ 

ra_sda: HPCCProblemSize.chpl RARandomStream.chpl ra_sync_data_array.chpl
	$(CHPL) -o $(CHPLSVN)$@ $(FLAGS) --savec c_$@ $^ 

testLCG: HPCCProblemSize.chpl RARandomStream.chpl testLCG.chpl
	$(CHPL) -o $(CHPLSVN)$@ $(FLAGS) --savec c_$@ $^

testRAR: HPCCProblemSize.chpl RARandomStream.chpl testLCG.chpl
	$(CHPL) -o $(CHPLSVN)$@ -suseLCG=false $(FLAGS) --savec c_$@ $^

ra2: RARandomStream.chpl ra2.chpl
	$(CHPL) -o $(CHPLSVN)$@ $(FLAGS) --savec c_$@ $^

ra2_sla: RARandomStream.chpl ra2_sync_lock_array.chpl
	$(CHPL) -o $(CHPLSVN)$@ $(FLAGS) --savec c_$@ $^

aswap: HPCCProblemSize.chpl RARandomStream.chpl atomicswap.chpl
	$(CHPL) -o $(CHPLSVN)$@ $(FLAGS) --savec c_$@ $^

FORCE:
