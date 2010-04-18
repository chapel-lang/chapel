CHPL = chpl

ifeq ($(CHPL_DEVELOPER), true)

FLAGS = $(F) --devel --gen-ids -g --no-checks --print-commands --print-passes

else

FLAGS = $(F) --no-devel -O --fast --no-checks --print-commands --print-passes

endif

LOCAL_FLAGS = $(FLAGS) --savec l_ra --local
GTML_FLAGS = $(FLAGS) --savec gtml_ra --local
GTMG_FLAGS = $(FLAGS) --savec gtmg_ra --no-local
LOCKG_FLAGS = $(FLAGS) --no-local

TARGETS = \
	stream \
	ral rag ra \
	fft \
	*_real 

default: all

all: $(TARGETS)

clean: FORCE
	rm -f $(TARGETS)

ran: HPCCProblemSize.chpl RARandomStream-new.chpl ra.chpl
	$(CHPL) -o $@ $(FLAGS) $^

ral: HPCCProblemSize.chpl RARandomStream.chpl ra.chpl
	$(CHPL) -o $@ $(LOCAL_FLAGS) $^

ragtml: HPCCProblemSize.chpl RARandomStream.chpl ra.chpl
	$(CHPL) -o $@ $(GTML_FLAGS) $^

ra-lockl: HPCCProblemSize.chpl RARandomStream.chpl ra-lock.chpl
	$(CHPL) -o $@ $(LOCKL_FLAGS) $^ 

ra-lockg: HPCCProblemSize.chpl RARandomStream.chpl ra-lock.chpl
	$(CHPL) -o $@ $(LOCKG_FLAGS) $^

ra-lplg: HPCCProblemSize.chpl RARandomStream.chpl ra-lpl.chpl
	$(CHPL) -o $@ $(FLAGS) $^

ra-lpeg: HPCCProblemSize.chpl RARandomStream.chpl ra-lpe.chpl
	$(CHPL) -o $@ $(FLAGS) --savec lpeg-ra $^

ra-ladg: HPCCProblemSize.chpl RARandomStream.chpl ra-lad.chpl
	$(CHPL) -o $@ $(FLAGS) --savec ladg-ra $^

rag: HPCCProblemSize.chpl RARandomStream.chpl ra.chpl
	$(CHPL) -o $@ $(FLAGS) --savec g_ra $^

ragia: HPCCProblemSize.chpl RARandomStream.chpl ra.chpl
	$(CHPL) -o $@ $(FLAGS) --savec g_ra-ia --ignore-atomic $^

ragtmg: HPCCProblemSize.chpl RARandomStream.chpl ragtm.chpl
	$(CHPL) -o $@ $(GTMG_FLAGS) $^

ra-stm: HPCCProblemSize.chpl RARandomStream.chpl ChapelSTM.chpl ra-stm.chpl 
	$(CHPL) -o $@ $(FLAGS) --savec stm_ra $^

ra-atomic: HPCCProblemSize.chpl RARandomStream.chpl ChapelSTM.chpl ra-atomic.chpl
	$(CHPL) -o $@ $(FLAGS) --savec atomic_ra $^

ra-mutexg: HPCCProblemSize.chpl RARandomStream.chpl ChapelLock.chpl lock.h ra-mutex.chpl
	$(CHPL) -o $@ $(FLAGS) --savec mutex_ra $^ 

ra-mplg: HPCCProblemSize.chpl RARandomStream.chpl ChapelLock.chpl lock.h ra-mpl.chpl
	$(CHPL) -o $@ $(FLAGS) --savec mpl_ra $^

FORCE:
