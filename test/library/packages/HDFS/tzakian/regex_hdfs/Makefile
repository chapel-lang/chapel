all: regex 

CHPL = chpl

regex: regex_hdfs.chpl
	$(CHPL) -g $^ -o $@
