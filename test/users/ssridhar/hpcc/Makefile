CHPL = chpl

USERNG = LCG
RNG = $(USERNG)RARandomStream

FLAGS += --conditional-dynamic-dispatch-limit 100
FLAGS += --print-commands --print-passes

ifeq ($(CHPL_DEVELOPER), true)

FLAGS += $(F) --devel --gen-ids -g

else

FLAGS += $(F) --no-devel --fast

endif

TARGET_SRCS = $(wildcard *.chpl)
TARGETS = $(TARGET_SRCS:%.chpl=%)

TARGETS1 = \
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

ra: myParams.chpl HPCCProblemSize.chpl $(RNG).chpl ra.chpl
	$(CHPL) -o $@ $(FLAGS) --savec c_$@ $^

raSTM: myParams.chpl HPCCProblemSize.chpl $(RNG).chpl ra.chpl dummystm.h
	$(CHPL) -o $@ $(FLAGS) --savec c_$@ $^
	rm raSTM
	sed -i -f dummystm.sed c_$@/ra.c
	grep -e LOAD_REF -e STM_COMM_WIDE_GET c_$@/ra.c
	make -f c_$@/Makefile

raMLA: myParams.chpl HPCCProblemSize.chpl $(RNG).chpl MutexLock.chpl MutexLock.h ra_mutex_lock_array.chpl
	$(CHPL) -o $@ $(FLAGS) --savec c_$@ $^ 

raSLA: myParams.chpl HPCCProblemSize.chpl $(RNG).chpl ra_sync_lock_array.chpl
	$(CHPL) -o $@ $(FLAGS) --savec c_$@ $^ 

raSDA: myParams.chpl HPCCProblemSize.chpl $(RNG).chpl ra_sync_data_array.chpl
	$(CHPL) -o $@ $(FLAGS) --savec c_$@ $^ 

ra2: $(RNG).chpl ra2.chpl
	$(CHPL) -o $@ $(FLAGS) --savec c_$@ $^

ra3: $(RNG).chpl ra3.chpl
	$(CHPL) -o $@ $(FLAGS) --savec c_$@ $^

ra4: $(RNG).chpl ra4.chpl
	$(CHPL) -o $@ $(FLAGS) --savec c_$@ $^

ra5: $(RNG).chpl ra5.chpl
	$(CHPL) -o $@ $(FLAGS) --savec c_$@ $^

ra2RAR: $(RNG).chpl ra2.chpl
	$(CHPL) -o $@ $(FLAGS) --savec c_$@ $^

ra2STM: $(RNG).chpl ra2.chpl dummystm.h
	$(CHPL) -o $@ $(FLAGS) --savec c_$@ $^
	rm ra2STM
	sed -i -f dummystm.sed c_$@/ra2.c
	grep -e LOAD_REF -e STM_COMM_WIDE_GET c_$@/ra2.c
	make -f c_$@/Makefile

ra2MLA: myParams.chpl HPCCProblemSize.chpl $(RNG).chpl MutexLock.chpl MutexLock.h ra2_mutex_lock_array.chpl
	$(CHPL) -o $@ $(FLAGS) --savec c_$@ $^

ra2SLA: myParams.chpl $(RNG).chpl ra2_sync_lock_array.chpl
	$(CHPL) -o $(CHPLSVN)$@ $(FLAGS) --savec c_$@ $^

ra2SDA: myParams.chpl $(RNG).chpl ra2_sync_data_array.chpl
	$(CHPL) -o $(CHPLSVN)$@ $(FLAGS) --savec c_$@ $^

testLCG: myParams.chpl HPCCProblemSize.chpl $(RNG).chpl testLCG.chpl
	$(CHPL) -o $@ $(FLAGS) --savec c_$@ $^

testRAR: myParams.chpl HPCCProblemSize.chpl $(RNG).chpl testLCG.chpl
	$(CHPL) -o $@ -suseLCG=false $(FLAGS) --savec c_$@ $^

atomicswap: myParams.chpl HPCCProblemSize.chpl $(RNG).chpl atomicswap.chpl
	$(CHPL) -o $(CHPLSVN)$@ $(FLAGS) --savec c_$@ $^

%: %.chpl
	$(CHPL) --savec c_$@$(IA) $(FLAGS) -o $@$(IA) $^

FORCE:
