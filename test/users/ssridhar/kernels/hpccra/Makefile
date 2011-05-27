include ../Makefile.include.head

raSTM: ../myParams.chpl ../HPCCProblemSize.chpl ../$(RNG).chpl ra.chpl ../dummystm.h
	$(CHPL) -o $@ $(FLAGS) --savec c_$@ $^
	rm raSTM
	sed -i -f ../dummystm.sed c_$@/ra.c
	grep -e LOAD_REF -e STM_COMM_WIDE_GET c_$@/ra.c
	make -f c_$@/Makefile

raMLA: ../myParams.chpl ../HPCCProblemSize.chpl ../$(RNG).chpl ../MutexLock.chpl ../MutexLock.h raMLA.chpl
	$(CHPL) -o $@ $(FLAGS) --savec c_$@ $^ 

raMLA-alternate: ../myParams.chpl ../HPCCProblemSize.chpl ../$(RNG).chpl ../MutexLock.chpl ../MutexLock.h raMLA.chpl
	$(CHPL) -o $@ $(FLAGS) --savec c_$@ $^ 

include ../Makefile.include.foot
