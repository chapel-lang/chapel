GENC   = gen_c

CHPL    = chpl
CHPL_FLAGS  = --print-passes
#CHPL_FLAGS += -g --savec=$(GENC) --short-names
#CHPL_FLAGS += --ccflags "-Wall"
#CHPL_FLAGS += --no-cg-cpp-lines

SOURCES= AnalyticFcn.chpl  \
         FTree.chpl        \
         MRA.chpl          \
         Quadrature.chpl   \
         Tensor.chpl       \
         TwoScale.chpl     \

TARGETS= test_diff       \
         test_gaxpy      \
         test_mul        \
         test_likepy     \
         test_showboxes

all: $(TARGETS)

FORCE:

test_diff: $(SOURCES) test_diff.chpl MadAnalytics.chpl
	$(CHPL) $(CHPL_FLAGS) --main-module $@ -o $@ $+


test_gaxpy: $(SOURCES) test_gaxpy.chpl MadAnalytics.chpl
	$(CHPL) $(CHPL_FLAGS) --main-module $@ -o $@ $+


test_mul: $(SOURCES) test_mul.chpl MadAnalytics.chpl
	$(CHPL) $(CHPL_FLAGS) --main-module $@ -o $@ $+


test_likepy: $(SOURCES) test_likepy.chpl MadAnalytics.chpl
	$(CHPL) $(CHPL_FLAGS) --main-module $@ -o $@ $+


test_showboxes: $(SOURCES) test_showboxes.chpl MadAnalytics.chpl
	$(CHPL) $(CHPL_FLAGS) --main-module $@ -o $@ $+

.PHONY: ps
PSDIR = ps
ps:
	@mkdir $(PSDIR)
	@for file in *.chpl; do out=$$(echo $$file|cut -d. -f1);    \
	    vim -e -c "set printoptions=paper:letter"               \
	    -c ":hardcopy > $(PSDIR)/$$out.ps" -c ":q" $$file;      \
	done
	@echo Postscript output is in $(PSDIR)
	@echo To 2-up any of the .ps files, do: "psnup -2 -pletter in.ps out.ps"


.PHONY: distrib
DISTDIR = mad_chapel
distrib: clean
	mkdir $(DISTDIR)
	tar -X DIST_EXCLUDE --exclude=$(DISTDIR) -c * | tar -C $(DISTDIR) -xf -
	@echo "++ Distribution has been built in $(DISTDIR)"


.PHONY: clean
clean:
	rm -f $(TARGETS)
	rm -rf $(GENC)
	rm -rf $(PSDIR)
