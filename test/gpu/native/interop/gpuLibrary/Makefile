include lib/Makefile.udf2
test_udf2_make: test_udf2.cpp lib/libudf2.a
	@$(CHPL_COMPILER) -x c++ -lstdc++ $(CHPL_CFLAGS) -o $@ $< $(CHPL_LDFLAGS)
