FILES += analyze_torus_graphs_array_rep.chpl
#FILES += analyze_torus_graphs_stencil_rep_v1.chpl
#FILES += analyze_torus_graphs_stencil_rep_v2.chpl
#FILES += analyze_torus_graphs_stencil_rep_v3.chpl

FLAGS += -sTEST_RMAT_ASSOCIATIVE=false
#FLAGS += -sTEST_TORUS_1D=false
FLAGS += -sTEST_TORUS_2D=false
FLAGS += -sTEST_TORUS_3D=false
FLAGS += -sTEST_TORUS_4D=false 

FLAGS += -sVALIDATE_BC=false -sREPRODUCIBLE_PROBLEMS=true -sDEBUG_KERNEL2=true -sDEBUG_KERNEL3=true -sDEBUG_KERNEL4=true

ifeq ($(CHPL_DEVELOPER), true)
  FLAGS += $(F) --devel --no-optimize --gen-ids -g
else
  FLAGS += $(F) --no-devel --fast 
endif

CHPL = chpl
FLAGS += --conditional-dynamic-dispatch-limit 100
FLAGS += --print-commands --print-passes

TARGETS = \
	ssca \
	sscaia \
	sscafeb

ssca: SSCA2_main.chpl SSCA2_execution_config_consts.chpl SSCA2_compilation_config_params.chpl SSCA2_kernels_atomic.chpl analyze_RMAT_graph_associative_array.chpl $(FILES)
	$(CHPL) -o $@ $(FLAGS) --savec c_$@ $^

sscaia: SSCA2_main.chpl SSCA2_execution_config_consts.chpl SSCA2_compilation_config_params.chpl SSCA2_kernels_atomic.chpl analyze_RMAT_graph_associative_array.chpl $(FILES)
	$(CHPL) -o $@ $(FLAGS) --ignore-atomic --savec c_$@ $^

sscafeb: SSCA2_main.chpl SSCA2_execution_config_consts.chpl SSCA2_compilation_config_params.chpl SSCA2_kernels.chpl analyze_RMAT_graph_associative_array.chpl $(FILES)
	$(CHPL) -o $@ $(FLAGS) --savec c_$@ $^

clean:
	rm ssca sscaia
	rm -rf c_ssca c_sscaia
