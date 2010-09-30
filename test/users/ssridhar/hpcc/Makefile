CHPL = chpl
RNG = $(USERNG)RARandomStream
FLAGS += --conditional-dynamic-dispatch-limit 100

ifeq ($(CHPL_DEVELOPER), true)

FLAGS += $(F) --devel --gen-ids -g --print-commands --print-passes

else

FLAGS += $(F) --no-devel --fast --print-commands --print-passes

endif

TARGETS = \
	ra \
	raSTM \
	raMLA \
	raSLA \
	raSDA \
	testLCG \
	testRAR \
	ra2 \
	ra2STM \
	ra2MLA \
	ra2SLA \
	ra2SDA \
	aswap 

default: all

all: $(TARGETS)

clean: FORCE
	rm -rf $(TARGETS) *_real c_* 

ra: HPCCProblemSize.chpl $(RNG).chpl ra.chpl
	$(CHPL) -o $@ $(FLAGS) --savec c_$@ $^

raSTM: HPCCProblemSize.chpl $(RNG).chpl ra.chpl dummystm.h
	$(CHPL) -o $@ $(FLAGS) --savec c_$@ $^
	sed -i -f dummystm.sed c_$@/ra.c
	grep LOAD_REF c_$@/ra.c
	make -f c_$@/Makefile

raMLA: HPCCProblemSize.chpl $(RNG).chpl MutexLock.chpl MutexLock.h ra_mutex_lock_array.chpl
	$(CHPL) -o $@ $(FLAGS) --savec c_$@ $^ 

raSLA: HPCCProblemSize.chpl $(RNG).chpl ra_sync_lock_array.chpl
	$(CHPL) -o $@ $(FLAGS) --savec c_$@ $^ 

raSDA: HPCCProblemSize.chpl $(RNG).chpl ra_sync_data_array.chpl
	$(CHPL) -o $@ $(FLAGS) --savec c_$@ $^ 

ra2: $(RNG).chpl ra2.chpl
	$(CHPL) -o $@ $(FLAGS) --savec c_$@ $^

ra2RAR: $(RNG).chpl ra2.chpl
	$(CHPL) -o $@ $(FLAGS) --savec c_$@ $^

ra2STM: $(RNG).chpl ra2.chpl dummystm.h
	$(CHPL) -o $@ $(FLAGS) --savec c_$@ $^
	sed -i -f dummystm.sed c_$@/ra2.c
	grep -e LOAD_REF -e STM_COMM_WIDE_GET c_$@/ra2.c
	make -f c_$@/Makefile

ra2MLA: HPCCProblemSize.chpl $(RNG).chpl MutexLock.chpl MutexLock.h ra2_mutex_lock_array.chpl
	$(CHPL) -o $@ $(FLAGS) --savec c_$@ $^

ra2SLA: $(RNG).chpl ra2_sync_lock_array.chpl
	$(CHPL) -o $(CHPLSVN)$@ $(FLAGS) --savec c_$@ $^

ra2SDA: $(RNG).chpl ra2_sync_data_array.chpl
	$(CHPL) -o $(CHPLSVN)$@ $(FLAGS) --savec c_$@ $^

testLCG: HPCCProblemSize.chpl $(RNG).chpl testLCG.chpl
	$(CHPL) -o $@ $(FLAGS) --savec c_$@ $^

testRAR: HPCCProblemSize.chpl $(RNG).chpl testLCG.chpl
	$(CHPL) -o $@ -suseLCG=false $(FLAGS) --savec c_$@ $^

aswap: HPCCProblemSize.chpl $(RNG).chpl atomicswap.chpl
	$(CHPL) -o $(CHPLSVN)$@ $(FLAGS) --savec c_$@ $^

FORCE:
