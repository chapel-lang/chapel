include lib/Makefile.udf2
test_udf2_make_cpu: test_udf2.cpp lib/libudf2.a
	@$(CHPL_COMPILER) -x c++ -lstdc++ $(CXXFLAGS) $(CHPL_CFLAGS) -o test_udf2_make $< $(CHPL_LDFLAGS)

test_udf2_make_nvidia: test_udf2.cpp lib/libudf2.a
	@$(CHPL_COMPILER) -x cuda -lstdc++ $(CXXFLAGS) $(CHPL_CFLAGS) -o test_udf2_make $< $(CHPL_LDFLAGS)

test_udf2_make_amd: test_udf2.cpp lib/libudf2.a
	@$(CHPL_COMPILER) -x hip -lstdc++ $(CXXFLAGS) $(CHPL_CFLAGS) -o test_udf2_make $< $(CHPL_LDFLAGS)

