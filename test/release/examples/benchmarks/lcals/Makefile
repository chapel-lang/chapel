LCALS_SOURCES = LCALSMain.chpl \
                RunARawLoops.chpl \
                RunBRawLoops.chpl \
                RunCRawLoops.chpl \
                RunParallelRawLoops.chpl \
                RunSPMDRawLoops.chpl \
                RunVectorizeOnlyRawLoops.chpl \
                LCALSConfiguration.chpl \
                LCALSLoops.chpl \
                LCALSDataTypes.chpl \
                LCALSEnums.chpl \
                LCALSParams.chpl \
                LCALSStatic.chpl \
                LCALSChecksums.chpl \
                Timer.chpl

CHPL=chpl

# Set this to "opt" for an optimized build, "debug" for a debug build
OPT_DEBUG=opt

OPT_FLAGS=--fast
DEBUG_FLAGS=--savec LCALS_savec -g

ifeq ($(OPT_DEBUG), opt)
  CHPL_FLAGS = $(OPT_FLAGS)
else
  CHPL_FLAGS = $(DEBUG_FLAGS)
endif

LCALSMain: $(LCALS_SOURCES)
	+$(CHPL) $(CHPL_FLAGS) LCALSMain.chpl -o LCALSMain

clean:
	rm -f LCALSMain LCALSMain_real
	rm -rf LCALS_savec
