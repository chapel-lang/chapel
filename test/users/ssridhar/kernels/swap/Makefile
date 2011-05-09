include ../Makefile.include.head

TARGETS += swapMLAA swapMLAD swapMLAAD \
	swapSLAA swapSLAD swapSLAAD 

# MLA Baseline 
swapMLA: ../myParams.chpl ../HPCCProblemSize.chpl ../$(RNG).chpl ../MutexLock.chpl ../MutexLock.h swapMLA.chpl
	$(CHPL) -o $@ $(FLAGS) --savec c_$@ $^

# MLA Lock Affinity optimization
swapMLAA: ../myParams.chpl ../HPCCProblemSize.chpl ../$(RNG).chpl ../MutexLock.chpl ../MutexLock.h swapMLA.chpl
	$(CHPL) -o $@ -suseAffinity=true $(FLAGS) --savec c_$@ $^

# MLA Data locality optimization
swapMLAD: ../myParams.chpl ../HPCCProblemSize.chpl ../$(RNG).chpl ../MutexLock.chpl ../MutexLock.h swapMLA.chpl
	$(CHPL) -o $@ -suseOn=true $(FLAGS) --savec c_$@ $^

# MLA Lock Affinity + Data Locality optimization
swapMLAAD: ../myParams.chpl ../HPCCProblemSize.chpl ../$(RNG).chpl ../MutexLock.chpl ../MutexLock.h swapMLA.chpl
	$(CHPL) -o $@ -suseAffinity=true -suseOn=true $(FLAGS) --savec c_$@ $^

# SLA Baseline 
swapSLA: ../myParams.chpl ../HPCCProblemSize.chpl ../$(RNG).chpl ../MutexLock.chpl ../MutexLock.h swapSLA.chpl
	$(CHPL) -o $@ $(FLAGS) --savec c_$@ $^

# SLA Lock Affinity optimization
swapSLAA: ../myParams.chpl ../HPCCProblemSize.chpl ../$(RNG).chpl ../MutexLock.chpl ../MutexLock.h swapSLA.chpl
	$(CHPL) -o $@ -suseAffinity=true $(FLAGS) --savec c_$@ $^

# SLA Data locality optimization
swapSLAD: ../myParams.chpl ../HPCCProblemSize.chpl ../$(RNG).chpl ../MutexLock.chpl ../MutexLock.h swapSLA.chpl
	$(CHPL) -o $@ -suseOn=true $(FLAGS) --savec c_$@ $^

# SLA Lock Affinity + Data Locality optimization
swapSLAAD: ../myParams.chpl ../HPCCProblemSize.chpl ../$(RNG).chpl ../MutexLock.chpl ../MutexLock.h swapSLA.chpl
	$(CHPL) -o $@ -suseAffinity=true -suseOn=true $(FLAGS) --savec c_$@ $^

include ../Makefile.include.foot