include ../Makefile.include.head

TARGETS += swapMLAA swapMLAOnA \
	swapSLAA swapSLAOnA 

# MLA and MLAOn Baseline 
swapMLA swapMLAOn: ../MutexLock.chpl ../MutexLock.h

# MLA Lock Affinity optimization
swapMLAA: ../myParams.chpl ../HPCCProblemSize.chpl ../$(RNG).chpl ../MutexLock.chpl ../MutexLock.h swapMLA.chpl
	$(CHPL) -o $@ -suseAffinity=true $(FLAGS) --savec c_$@ $^

# MLA Lock Affinity + Data Locality optimization
swapMLAOnA: ../myParams.chpl ../HPCCProblemSize.chpl ../$(RNG).chpl ../MutexLock.chpl ../MutexLock.h swapMLA.chpl
	$(CHPL) -o $@ -suseAffinity=true $(FLAGS) --savec c_$@ $^

# SLA and SLAOn Baseline 
swapSLA swapSLAOn: ../MutexLock.chpl ../MutexLock.h

# SLA Lock Affinity optimization
swapSLAA: ../myParams.chpl ../HPCCProblemSize.chpl ../$(RNG).chpl ../MutexLock.chpl ../MutexLock.h swapSLA.chpl
	$(CHPL) -o $@ -suseAffinity=true $(FLAGS) --savec c_$@ $^

# SLA Lock Affinity + Data Locality optimization
swapSLAOnA: ../myParams.chpl ../HPCCProblemSize.chpl ../$(RNG).chpl ../MutexLock.chpl ../MutexLock.h swapSLA.chpl
	$(CHPL) -o $@ -suseAffinity=true $(FLAGS) --savec c_$@ $^

include ../Makefile.include.foot