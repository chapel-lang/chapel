include ../Makefile.include

%: %.chpl 
	$(CHPL) -o $@$(IA) $(FLAGS) --savec c_$@$(IA) $^ ../myParams.chpl ../HPCCProblemSize.chpl ../$(RNG).chpl
