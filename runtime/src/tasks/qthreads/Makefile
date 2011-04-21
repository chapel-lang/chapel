RUNTIME_ROOT = ../../..
RUNTIME_SUBDIR = src/tasks/$(CHPL_MAKE_TASKS)

include $(RUNTIME_ROOT)/make/Makefile.runtime.head

QTHREAD_FILES=\
	Makefile.main \
	Makefile.include \
	Makefile.share \
	tasks-qthreads.c \
	../../../include/tasks/qthreads/tasks-qthreads.h \

default: copy_qthread_files
	@make -f Makefile.main $@

all: copy_qthread_files
	@make -f Makefile.main $@

depend: copy_qthread_files
	@make -f Makefile.main $@

clean: copy_qthread_files
	@make -f Makefile.main $@
	rm -f $(QTHREAD_FILES)

clobber: copy_qthread_files
	@make -f Makefile.main $@
	rm -f $(QTHREAD_FILES)

copy_qthread_files: $(QTHREAD_FILES)

Makefile.main: $(QTHREAD_SUBDIR)/shims/chapel/src/Makefile
	cp $(QTHREAD_SUBDIR)/shims/chapel/src/Makefile $@

Makefile.include: $(QTHREAD_SUBDIR)/shims/chapel/src/Makefile.include
	cp $< $@

Makefile.share: $(QTHREAD_SUBDIR)/shims/chapel/src/Makefile.share
	cp $< $@

tasks-qthreads.c: $(QTHREAD_SUBDIR)/shims/chapel/src/tasks-qthreads.c
	cp $< $@

../../../include/tasks/qthreads/tasks-qthreads.h: $(QTHREAD_SUBDIR)/shims/chapel/include/tasks-qthreads.h
	cp $< $@

