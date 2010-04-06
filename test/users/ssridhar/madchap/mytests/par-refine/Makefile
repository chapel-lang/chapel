FLAGS += $(F) --print-commands --print-passes --gen-ids -M ../.. 
FLAGS += --conditional-dynamic-dispatch-limit 100

ifeq ($(CHPL_DEVELOPER), true)

FLAGS += --gdb -g 

else 

FLAGS += -O --fast 

endif

clean:
	rm refine refine-ia

TARGETS: refine \
	refine-ia

refine: test_refine.chpl MRA.chpl
	chpl --savec gen_c $(FLAGS) -o $@ $^  

refine-l: test_refine.chpl MRA.chpl
	chpl --savec gen-l_c $(FLAGS) -o $@ $^ --local

refine-ia: test_refine.chpl MRA.chpl
	chpl --ignore-atomic --savec gen-ia_c $(FLAGS) -o $@ $^

refine-ial: test_refine.chpl MRA.chpl
	chpl --ignore-atomic --savec gen-ial_c $(FLAGS) -o $@ $^ --local
 
