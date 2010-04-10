CHPL = chpl

ifeq ($(CHPL_DEVELOPER), true)

FLAGS = $(F) --gen-ids -g --no-checks --print-commands --print-passes

else

FLAGS = $(F) -O --fast --no-checks --print-commands --print-passes

endif

ifeq ($(CHPL_GEN_HTML), true)

FLAGS += -t

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

ran: probSize.chpl ra-randstream-new.chpl ra.chpl
	$(CHPL) -o $@ $(FLAGS) $^

ral: probSize.chpl ra-randstream.chpl ra.chpl
	$(CHPL) -o $@ $(LOCAL_FLAGS) $^

ragtml: probSize.chpl ra-randstream.chpl ra.chpl
	$(CHPL) -o $@ $(GTML_FLAGS) $^

ra-lockl: probSize.chpl ra-randstream.chpl ra-lock.chpl
	$(CHPL) -o $@ $(LOCKL_FLAGS) $^ 

ra-lockg: probSize.chpl ra-randstream.chpl ra-lock.chpl
	$(CHPL) -o $@ $(LOCKG_FLAGS) $^

ra-lplg: probSize.chpl ra-randstream.chpl ra-lpl.chpl
	$(CHPL) -o $@ $(FLAGS) $^

ra-lpeg: probSize.chpl ra-randstream.chpl ra-lpe.chpl
	$(CHPL) -o $@ $(FLAGS) --savec lpeg-ra $^

ra-ladg: probSize.chpl ra-randstream.chpl ra-lad.chpl
	$(CHPL) -o $@ $(FLAGS) --savec ladg-ra $^

rag: probSize.chpl ra-randstream.chpl ra.chpl
	$(CHPL) -o $@ $(FLAGS) --savec g_ra $^

ragia: probSize.chpl ra-randstream.chpl ra.chpl
	$(CHPL) -o $@ $(FLAGS) --savec g_ra-ia --ignore-atomic $^

ragtmg: probSize.chpl ra-randstream.chpl ragtm.chpl
	$(CHPL) -o $@ $(GTMG_FLAGS) $^

ra-stm: probSize.chpl ra-randstream.chpl ChapelSTM.chpl ra-stm.chpl 
	$(CHPL) -o $@ $(FLAGS) --savec stm_ra $^

ra-atomic: probSize.chpl ra-randstream.chpl ChapelSTM.chpl ra-atomic.chpl
	$(CHPL) -o $@ $(FLAGS) --savec atomic_ra $^

ra-mutexg: probSize.chpl ra-randstream.chpl ChapelLock.chpl lock.h ra-mutex.chpl
	$(CHPL) -o $@ $(FLAGS) --savec mutex_ra $^ 

ra-mplg: probSize.chpl ra-randstream.chpl ChapelLock.chpl lock.h ra-mpl.chpl
	$(CHPL) -o $@ $(FLAGS) --savec mpl_ra $^

FORCE:
