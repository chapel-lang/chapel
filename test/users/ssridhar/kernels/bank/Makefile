include ../Makefile.include.head

TARGETS += ra2MLAA ra2MLOnA \
	ra2SLAA ra2SLAOnA 

# MLA and MLAOn Baseline 
ra2MLA ra2MLAOn: ../MutexLock.chpl ../MutexLock.h

# MLA Lock Affinity optimization
ra2MLAA: ../myParams.chpl ../HPCCProblemSize.chpl ../$(RNG).chpl ../MutexLock.chpl ../MutexLock.h ra2MLA.chpl
	$(CHPL) -o $@ -suseAffinity=true $(FLAGS) --savec c_$@ $^

# MLA Lock Affinity + Data Locality optimization
ra2MLAOnA: ../myParams.chpl ../HPCCProblemSize.chpl ../$(RNG).chpl ../MutexLock.chpl ../MutexLock.h ra2MLA.chpl
	$(CHPL) -o $@ -suseAffinity=true $(FLAGS) --savec c_$@ $^

# SLA and SLAOn Baseline 
ra2SLA ra2SLAOn: ../MutexLock.chpl ../MutexLock.h

# SLA Lock Affinity optimization
ra2SLAA: ../myParams.chpl ../HPCCProblemSize.chpl ../$(RNG).chpl ../MutexLock.chpl ../MutexLock.h ra2SLA.chpl
	$(CHPL) -o $@ -suseAffinity=true $(FLAGS) --savec c_$@ $^

# SLA Lock Affinity + Data Locality optimization
ra2SLAOnA: ../myParams.chpl ../HPCCProblemSize.chpl ../$(RNG).chpl ../MutexLock.chpl ../MutexLock.h ra2SLA.chpl
	$(CHPL) -o $@ -suseAffinity=true $(FLAGS) --savec c_$@ $^

ra2STM: ../myParams.chpl ../HPCCProblemSize.chpl ../$(RNG).chpl ra2.chpl ../dummystm.h
	$(CHPL) -o $@ $(FLAGS) --savec c_$@ $^
	rm ra2STM
	sed -i -f ../dummystm.sed c_$@/ra2.c
	grep -e LOAD_REF -e STM_COMM_WIDE_GET c_$@/ra2.c
	make -f c_$@/Makefile

include ../Makefile.include.foot
