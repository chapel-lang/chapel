CHPL = chpl

#
# This does not build the torus versions
#
CHPL_FLAGS = --fast -M SSCA2_Modules SSCA2_Modules/SSCA2_kernels.chpl SSCA2_Modules/analyze_torus_graphs_stencil_rep_v1.chpl -sBUILD_TORUS_VERSIONS=false -sREPRODUCIBLE_PROBLEMS=true -sPRINT_TIMING_STATISTICS=false --no-warnings

MODDIR= SSCA2_Modules
SRCS= \
	SSCA2_main.chpl \
	$(MODDIR)/SSCA2_driver.chpl \
	$(MODDIR)/SSCA2_execution_config_consts.chpl \
	$(MODDIR)/SSCA2_compilation_config_params.chpl \
	$(MODDIR)/SSCA2_kernels.chpl \
	$(MODDIR)/SSCA2_RMAT_graph_generator.chpl \
	$(MODDIR)/analyze_RMAT_graph_associative_array.chpl \
	$(MODDIR)/analyze_torus_graphs_stencil_rep_v1.chpl \
	$(MODDIR)/analyze_torus_graphs_array_rep.chpl \
	$(MODDIR)/array_rep_torus_graph_generator.chpl \
	$(MODDIR)/torus_graph_generator_utilities.chpl

TARGET= SSCA2_main

REAL = $(TARGET:%=%_real)

default: all

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CHPL) -o $@ $(CHPL_FLAGS) $(OFLAGS) $<

clean: FORCE
	rm -f $(TARGET) $(REAL)

FORCE:
