CHPL = chpl

#
# By default this
# - does not build the torus versions
# - does yield reproducible results
# - does not print timing statistics
# - adds no Chapel compiler flags to those shown below.
#
# If you want otherwise you have to say so.
#
DO_BUILD_TORUS_VERSIONS=
DO_REPRODUCIBLE_PROBLEMS=yes
DO_PRINT_TIMING_STATISTICS=
MORE_CHPL_FLAGS=

OPT = --fast
CHPL_FLAGS = $(OPT) -M SSCA2_Modules \
             SSCA2_Modules/SSCA2_kernels.chpl \
             SSCA2_Modules/analyze_torus_graphs_stencil_rep_v1.chpl

ifeq ($(DO_BUILD_TORUS_VERSIONS),)
CHPL_FLAGS += -sBUILD_TORUS_VERSIONS=false
else
CHPL_FLAGS += -sBUILD_TORUS_VERSIONS=true
endif

ifeq ($(DO_REPRODUCIBLE_PROBLEMS),)
CHPL_FLAGS += -sREPRODUCIBLE_PROBLEMS=false
else
CHPL_FLAGS += -sREPRODUCIBLE_PROBLEMS=true
endif

ifeq ($(DO_PRINT_TIMING_STATISTICS),)
CHPL_FLAGS += -sPRINT_TIMING_STATISTICS=false
else
CHPL_FLAGS += -sPRINT_TIMING_STATISTICS=true
endif

CHPL_FLAGS += $(MORE_CHPL_FLAGS)

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
	$(MODDIR)/io_RMAT_graph.chpl \
	$(MODDIR)/torus_graph_generator_utilities.chpl

TARGET= SSCA2_main

REAL = $(TARGET:%=%_real)

default: all

all: $(TARGET)

$(TARGET): $(SRCS)
	+$(CHPL) -o $@ $(CHPL_FLAGS) $(OFLAGS) $<

clean: FORCE
	rm -f $(TARGET) $(REAL)

FORCE:
