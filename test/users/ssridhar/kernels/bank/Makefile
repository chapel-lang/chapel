include ../Makefile.include

ra2STM: ../myParams.chpl ../HPCCProblemSize.chpl ../$(RNG).chpl ra2.chpl ../dummystm.h
	$(CHPL) -o $@ $(FLAGS) --savec c_$@ $^
	rm ra2STM
	sed -i -f ../dummystm.sed c_$@/ra2.c
	grep -e LOAD_REF -e STM_COMM_WIDE_GET c_$@/ra2.c
	make -f c_$@/Makefile

ra2MLA: ../myParams.chpl ../HPCCProblemSize.chpl ../$(RNG).chpl ../MutexLock.chpl ../MutexLock.h ra2MLA.chpl
	$(CHPL) -o $@ $(FLAGS) --savec c_$@ $^

%: %.chpl 
	$(CHPL) -o $@$(IA) $(FLAGS) --savec c_$@$(IA) $^ ../myParams.chpl ../HPCCProblemSize.chpl ../$(RNG).chpl
