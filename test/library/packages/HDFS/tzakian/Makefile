
CHPL=chpl
PARALLEL?=true
LOCALES?=1

BUILD_DIR=chadoop

# original flags from the student
#CHPL_FLAGS+=-g -scomputeInParallel=$(PARALLEL) -I/usr/lib/jvm/java-openjdk/include -L/usr/lib/jvm/java-openjdk/jre/lib/amd64/server -lhdfs -ljvm

# This was commented already by the student
# /usr/include: /usr/lib64:

# This is the one I modified, so that it would look in the right places for the
# libraries etc. (TZ)
CHPL_FLAGS+=-g -scomputeInParallel=$(PARALLEL) -I$(JAVA_INSTALL)/include -I$(JAVA_INSTALL)/include/linux -L$(JAVA_INSTALL)/jre/lib/amd64/server -I$(HADOOP_INSTALL)/src/c++/libhdfs -L$(HADOOP_INSTALL)/c++/Linux-amd64-64/lib -lhdfs -ljvm


ITER_FLAGS+=-g -I$(JAVA_INSTALL)/include -I$(JAVA_INSTALL)/include/linux -L$(JAVA_INSTALL)/jre/lib/amd64/server -I$(HADOOP_INSTALL)/src/c++/libhdfs -L$(HADOOP_INSTALL)/c++/Linux-amd64-64/lib -lhdfs -ljvm

default: chadoop

deploy: chadoop
	rsync -av --delete chadoop/ ~/chadoop/

#/usr/include/hdfs.h chadoop.h chadoop.c chadoop.chpl

chadoop: hdfs.h chadoop.h chadoop.c chadoop.chpl
	mkdir -p $(BUILD_DIR)
	cp -r multiloc $(BUILD_DIR)
	cp -r conf $(BUILD_DIR)
	$(CHPL) $(CHPL_FLAGS) $^ -o $(BUILD_DIR)/$@

iter: hdfs.h chadoop.h chadoop.c HDFSiterator.chpl
	$(CHPL) $(ITER_FLAGS) $^ -o $@

wc: hdfs.h chadoop.h chadoop.c wc.chpl
	$(CHPL) $(ITER_FLAGS) $^ -o $@

tester: hdfs.h chadoop.h chadoop.c test.chpl
	$(CHPL) $(ITER_FLAGS) $^ -o $@
	
#-o $(BUILD_DIR)/$@ 

test: chadoop
	./chadoop/chadoop $(ARGS) --verbose

clean:
	rm -rf $(BUILD_DIR)
	rm -f ./*.o ./chadoop ./chadoop_real
