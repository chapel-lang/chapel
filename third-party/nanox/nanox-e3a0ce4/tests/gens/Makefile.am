include $(abs_top_srcdir)/src/common.am

EXTRA_DIST = api-generator.in core-generator.in gpu-generator.in mixed-generator.in nanox-creation-generator.in api-omp-generator.in config.py

noinst_SCRIPTS = api-generator core-generator gpu-generator mixed-generator nanox-creation-generator api-omp-generator

CLEANFILES = api-generator core-generator gpu-generator mixed-generator nanox-creation-generator api-omp-generator

api-generator: api-generator.in
	rm -f api-generator
	tmp_plugins_perfo=`cat $(abs_top_builddir)/src/plugins-list | grep ^performance\= | cut -d'=' -f2` ;\
	tmp_plugins_debug=`cat $(abs_top_builddir)/src/plugins-list | grep ^debug\= | cut -d'=' -f2` ;\
	tmp_plugins_instr=`cat $(abs_top_builddir)/src/plugins-list | grep ^instrumentation\= | cut -d'=' -f2` ;\
	tmp_plugins_instr_debug=`cat $(abs_top_builddir)/src/plugins-list | grep ^instrumentation_debug\= | cut -d'=' -f2` ;\
	tmp_compile_versions=`cat $(abs_top_builddir)/src/plugins-list | cut -d'=' -f1 | sed -e :a -e '$!N; s/\n/ /; ta'` ;\
	tmp_srcdir=$(abs_top_srcdir) ; \
	tmp_builddir=$(abs_top_builddir) ; \
	tmp_perfo_cflags='"$(perfo_cflags)"' ; \
	tmp_debug_cflags='"$(debug_cflags)"' ; \
	tmp_instr_cflags='"$(instr_cflags)"' ; \
	tmp_instrdebug_cflags='"$(instrdebug_cflags)"' ; \
	tmp_perfo_cxxflags='"$(global_perfo_cxxflags)"' ; \
	tmp_debug_cxxflags='"$(global_debug_cxxflags)"' ; \
	tmp_instr_cxxflags='"$(global_instr_cxxflags)"' ; \
	tmp_instrdebug_cxxflags='"$(global_instrdebug_cxxflags)"' ; \
	tmp_cc='"$(CC)"' ; \
	tmp_cxx='"$(CXX)"' ; \
	sed \
		-e's,@SRCDIR\@,'"$$tmp_srcdir"',g' \
		-e's,@BUILDDIR\@,'"$$tmp_builddir"',g' \
		-e's,@PLUGINS_PERFORMANCE\@,'"$$tmp_plugins_perfo"',g' \
		-e's,@PLUGINS_DEBUG\@,'"$$tmp_plugins_debug"',g' \
		-e's,@PLUGINS_INSTRUMENTATION\@,'"$$tmp_plugins_instr"',g' \
		-e's,@PLUGINS_INSTRUMENTATION_DEBUG\@,'"$$tmp_plugins_instr_debug"',g' \
		-e's,@COMPILE_VERSIONS\@,'"$$tmp_compile_versions"',g' \
		-e's,@PERFO_CFLAGS\@,'"$$tmp_perfo_cflags"',g' \
		-e's,@DEBUG_CFLAGS\@,'"$$tmp_debug_cflags"',g' \
		-e's,@INSTR_CFLAGS\@,'"$$tmp_instr_cflags"',g' \
		-e's,@INSTRDEBUG_CFLAGS\@,'"$$tmp_instrdebug_cflags"',g' \
		-e's,@PERFO_CXXFLAGS\@,'"$$tmp_perfo_cxxflags"',g' \
		-e's,@DEBUG_CXXFLAGS\@,'"$$tmp_debug_cxxflags"',g' \
		-e's,@INSTR_CXXFLAGS\@,'"$$tmp_instr_cxxflags"',g' \
		-e's,@INSTRDEBUG_CXXFLAGS\@,'"$$tmp_instrdebug_cxxflags"',g' \
		-e's,@CONFIG_CC\@,'"$$tmp_cc"',g' \
		-e's,@CONFIG_CXX\@,'"$$tmp_cxx"',g' \
		$(srcdir)/api-generator.in > api-generator.tmp
	mv api-generator.tmp api-generator
	chmod 755 api-generator

core-generator: core-generator.in
	rm -f core-generator
	tmp_plugins_perfo=`cat $(abs_top_builddir)/src/plugins-list | grep ^performance\= | cut -d'=' -f2` ;\
	tmp_plugins_debug=`cat $(abs_top_builddir)/src/plugins-list | grep ^debug\= | cut -d'=' -f2` ;\
	tmp_plugins_instr=`cat $(abs_top_builddir)/src/plugins-list | grep ^instrumentation\= | cut -d'=' -f2` ;\
	tmp_plugins_instr_debug=`cat $(abs_top_builddir)/src/plugins-list | grep ^instrumentation_debug\= | cut -d'=' -f2` ;\
	tmp_compile_versions=`cat $(abs_top_builddir)/src/plugins-list | cut -d'=' -f1 | sed -e :a -e '$!N; s/\n/ /; ta'` ;\
	tmp_srcdir=$(abs_top_srcdir) ; \
	tmp_builddir=$(abs_top_builddir) ; \
	tmp_perfo_cflags='"$(perfo_cflags)"' ; \
	tmp_debug_cflags='"$(debug_cflags)"' ; \
	tmp_instr_cflags='"$(instr_cflags)"' ; \
	tmp_instrdebug_cflags='"$(instrdebug_cflags)"' ; \
	tmp_perfo_cxxflags='"$(global_perfo_cxxflags)"' ; \
	tmp_debug_cxxflags='"$(global_debug_cxxflags)"' ; \
	tmp_instr_cxxflags='"$(global_instr_cxxflags)"' ; \
	tmp_instrdebug_cxxflags='"$(global_instrdebug_cxxflags)"' ; \
	tmp_cc='"$(CC)"' ; \
	tmp_cxx='"$(CXX)"' ; \
	sed \
		-e's,@SRCDIR\@,'"$$tmp_srcdir"',g' \
		-e's,@BUILDDIR\@,'"$$tmp_builddir"',g' \
		-e's,@PLUGINS_PERFORMANCE\@,'"$$tmp_plugins_perfo"',g' \
		-e's,@PLUGINS_DEBUG\@,'"$$tmp_plugins_debug"',g' \
		-e's,@PLUGINS_INSTRUMENTATION\@,'"$$tmp_plugins_instr"',g' \
		-e's,@PLUGINS_INSTRUMENTATION_DEBUG\@,'"$$tmp_plugins_instr_debug"',g' \
		-e's,@COMPILE_VERSIONS\@,'"$$tmp_compile_versions"',g' \
		-e's,@PERFO_CFLAGS\@,'"$$tmp_perfo_cflags"',g' \
		-e's,@DEBUG_CFLAGS\@,'"$$tmp_debug_cflags"',g' \
		-e's,@INSTR_CFLAGS\@,'"$$tmp_instr_cflags"',g' \
		-e's,@INSTRDEBUG_CFLAGS\@,'"$$tmp_instrdebug_cflags"',g' \
		-e's,@PERFO_CXXFLAGS\@,'"$$tmp_perfo_cxxflags"',g' \
		-e's,@DEBUG_CXXFLAGS\@,'"$$tmp_debug_cxxflags"',g' \
		-e's,@INSTR_CXXFLAGS\@,'"$$tmp_instr_cxxflags"',g' \
		-e's,@INSTRDEBUG_CXXFLAGS\@,'"$$tmp_instrdebug_cxxflags"',g' \
		-e's,@CONFIG_CC\@,'"$$tmp_cc"',g' \
		-e's,@CONFIG_CXX\@,'"$$tmp_cxx"',g' \
		$(srcdir)/core-generator.in > core-generator.tmp
	mv core-generator.tmp core-generator
	chmod 755 core-generator

gpu-generator: gpu-generator.in
	rm -f gpu-generator
	tmp_plugins_perfo=`cat $(abs_top_builddir)/src/plugins-list | grep ^performance\= | cut -d'=' -f2` ;\
	tmp_plugins_debug=`cat $(abs_top_builddir)/src/plugins-list | grep ^debug\= | cut -d'=' -f2` ;\
	tmp_plugins_instr=`cat $(abs_top_builddir)/src/plugins-list | grep ^instrumentation\= | cut -d'=' -f2` ;\
	tmp_plugins_instr_debug=`cat $(abs_top_builddir)/src/plugins-list | grep ^instrumentation_debug\= | cut -d'=' -f2` ;\
	tmp_compile_versions=`cat $(abs_top_builddir)/src/plugins-list | cut -d'=' -f1 | sed -e :a -e '$!N; s/\n/ /; ta'` ;\
	tmp_srcdir=$(abs_top_srcdir) ; \
	tmp_builddir=$(abs_top_builddir) ; \
	tmp_perfo_cflags='"$(perfo_cflags)"' ; \
	tmp_debug_cflags='"$(debug_cflags)"' ; \
	tmp_instr_cflags='"$(instr_cflags)"' ; \
	tmp_instrdebug_cflags='"$(instrdebug_cflags)"' ; \
	tmp_perfo_cxxflags='"$(global_perfo_cxxflags)"' ; \
	tmp_debug_cxxflags='"$(global_debug_cxxflags)"' ; \
	tmp_instr_cxxflags='"$(global_instr_cxxflags)"' ; \
	tmp_instrdebug_cxxflags='"$(global_instrdebug_cxxflags)"' ; \
	tmp_cc='"$(CC)"' ; \
	tmp_cxx='"$(CXX)"' ; \
	sed \
		-e's,@SRCDIR\@,'"$$tmp_srcdir"',g' \
		-e's,@BUILDDIR\@,'"$$tmp_builddir"',g' \
		-e's,@PLUGINS_PERFORMANCE\@,'"$$tmp_plugins_perfo"',g' \
		-e's,@PLUGINS_DEBUG\@,'"$$tmp_plugins_debug"',g' \
		-e's,@PLUGINS_INSTRUMENTATION\@,'"$$tmp_plugins_instr"',g' \
		-e's,@PLUGINS_INSTRUMENTATION_DEBUG\@,'"$$tmp_plugins_instr_debug"',g' \
		-e's,@COMPILE_VERSIONS\@,'"$$tmp_compile_versions"',g' \
		-e's,@PERFO_CFLAGS\@,'"$$tmp_perfo_cflags"',g' \
		-e's,@DEBUG_CFLAGS\@,'"$$tmp_debug_cflags"',g' \
		-e's,@INSTR_CFLAGS\@,'"$$tmp_instr_cflags"',g' \
		-e's,@INSTRDEBUG_CFLAGS\@,'"$$tmp_instrdebug_cflags"',g' \
		-e's,@PERFO_CXXFLAGS\@,'"$$tmp_perfo_cxxflags"',g' \
		-e's,@DEBUG_CXXFLAGS\@,'"$$tmp_debug_cxxflags"',g' \
		-e's,@INSTR_CXXFLAGS\@,'"$$tmp_instr_cxxflags"',g' \
		-e's,@INSTRDEBUG_CXXFLAGS\@,'"$$tmp_instrdebug_cxxflags"',g' \
		-e's,@CONFIG_CC\@,'"$$tmp_cc"',g' \
		-e's,@CONFIG_CXX\@,'"$$tmp_cxx"',g' \
		$(srcdir)/gpu-generator.in > gpu-generator.tmp
	mv gpu-generator.tmp gpu-generator
	chmod 755 gpu-generator

mixed-generator: mixed-generator.in
	rm -f mixed-generator
	tmp_plugins_perfo=`cat $(abs_top_builddir)/src/plugins-list | grep ^performance\= | cut -d'=' -f2` ;\
	tmp_plugins_debug=`cat $(abs_top_builddir)/src/plugins-list | grep ^debug\= | cut -d'=' -f2` ;\
	tmp_plugins_instr=`cat $(abs_top_builddir)/src/plugins-list | grep ^instrumentation\= | cut -d'=' -f2` ;\
	tmp_plugins_instr_debug=`cat $(abs_top_builddir)/src/plugins-list | grep ^instrumentation_debug\= | cut -d'=' -f2` ;\
	tmp_compile_versions=`cat $(abs_top_builddir)/src/plugins-list | cut -d'=' -f1 | sed -e :a -e '$!N; s/\n/ /; ta'` ;\
	tmp_srcdir=$(abs_top_srcdir) ; \
	tmp_builddir=$(abs_top_builddir) ; \
	tmp_perfo_cflags='"$(perfo_cflags)"' ; \
	tmp_debug_cflags='"$(debug_cflags)"' ; \
	tmp_instr_cflags='"$(instr_cflags)"' ; \
	tmp_instrdebug_cflags='"$(instrdebug_cflags)"' ; \
	tmp_perfo_cxxflags='"$(global_perfo_cxxflags)"' ; \
	tmp_debug_cxxflags='"$(global_debug_cxxflags)"' ; \
	tmp_instr_cxxflags='"$(global_instr_cxxflags)"' ; \
	tmp_instrdebug_cxxflags='"$(global_instrdebug_cxxflags)"' ; \
	tmp_cc='"$(CC)"' ; \
	tmp_cxx='"$(CXX)"' ; \
	sed \
		-e's,@SRCDIR\@,'"$$tmp_srcdir"',g' \
		-e's,@BUILDDIR\@,'"$$tmp_builddir"',g' \
		-e's,@PLUGINS_PERFORMANCE\@,'"$$tmp_plugins_perfo"',g' \
		-e's,@PLUGINS_DEBUG\@,'"$$tmp_plugins_debug"',g' \
		-e's,@PLUGINS_INSTRUMENTATION\@,'"$$tmp_plugins_instr"',g' \
		-e's,@PLUGINS_INSTRUMENTATION_DEBUG\@,'"$$tmp_plugins_instr_debug"',g' \
		-e's,@COMPILE_VERSIONS\@,'"$$tmp_compile_versions"',g' \
		-e's,@PERFO_CFLAGS\@,'"$$tmp_perfo_cflags"',g' \
		-e's,@DEBUG_CFLAGS\@,'"$$tmp_debug_cflags"',g' \
		-e's,@INSTR_CFLAGS\@,'"$$tmp_instr_cflags"',g' \
		-e's,@INSTRDEBUG_CFLAGS\@,'"$$tmp_instrdebug_cflags"',g' \
		-e's,@PERFO_CXXFLAGS\@,'"$$tmp_perfo_cxxflags"',g' \
		-e's,@DEBUG_CXXFLAGS\@,'"$$tmp_debug_cxxflags"',g' \
		-e's,@INSTR_CXXFLAGS\@,'"$$tmp_instr_cxxflags"',g' \
		-e's,@INSTRDEBUG_CXXFLAGS\@,'"$$tmp_instrdebug_cxxflags"',g' \
		-e's,@CONFIG_CC\@,'"$$tmp_cc"',g' \
		-e's,@CONFIG_CXX\@,'"$$tmp_cxx"',g' \
		$(srcdir)/mixed-generator.in > mixed-generator.tmp
	mv mixed-generator.tmp mixed-generator
	chmod 755 mixed-generator

nanox-creation-generator: nanox-creation-generator.in
	rm -f nanox-creation-generator
	tmp_plugins_perfo=`cat $(abs_top_builddir)/src/plugins-list | grep ^performance\= | cut -d'=' -f2` ;\
	tmp_plugins_debug=`cat $(abs_top_builddir)/src/plugins-list | grep ^debug\= | cut -d'=' -f2` ;\
	tmp_plugins_instr=`cat $(abs_top_builddir)/src/plugins-list | grep ^instrumentation\= | cut -d'=' -f2` ;\
	tmp_plugins_instr_debug=`cat $(abs_top_builddir)/src/plugins-list | grep ^instrumentation_debug\= | cut -d'=' -f2` ;\
	tmp_compile_versions=`cat $(abs_top_builddir)/src/plugins-list | cut -d'=' -f1 | sed -e :a -e '$!N; s/\n/ /; ta'` ;\
	tmp_srcdir=$(abs_top_srcdir) ; \
	tmp_builddir=$(abs_top_builddir) ; \
	tmp_perfo_cflags='"$(perfo_cflags)"' ; \
	tmp_debug_cflags='"$(debug_cflags)"' ; \
	tmp_instr_cflags='"$(instr_cflags)"' ; \
	tmp_instrdebug_cflags='"$(instrdebug_cflags)"' ; \
	tmp_perfo_cxxflags='"$(global_perfo_cxxflags)"' ; \
	tmp_debug_cxxflags='"$(global_debug_cxxflags)"' ; \
	tmp_instr_cxxflags='"$(global_instr_cxxflags)"' ; \
	tmp_instrdebug_cxxflags='"$(global_instrdebug_cxxflags)"' ; \
	tmp_cc='"$(CC)"' ; \
	tmp_cxx='"$(CXX)"' ; \
	sed \
		-e's,@SRCDIR\@,'"$$tmp_srcdir"',g' \
		-e's,@BUILDDIR\@,'"$$tmp_builddir"',g' \
		-e's,@PLUGINS_PERFORMANCE\@,'"$$tmp_plugins_perfo"',g' \
		-e's,@PLUGINS_DEBUG\@,'"$$tmp_plugins_debug"',g' \
		-e's,@PLUGINS_INSTRUMENTATION\@,'"$$tmp_plugins_instr"',g' \
		-e's,@PLUGINS_INSTRUMENTATION_DEBUG\@,'"$$tmp_plugins_instr_debug"',g' \
		-e's,@COMPILE_VERSIONS\@,'"$$tmp_compile_versions"',g' \
		-e's,@PERFO_CFLAGS\@,'"$$tmp_perfo_cflags"',g' \
		-e's,@DEBUG_CFLAGS\@,'"$$tmp_debug_cflags"',g' \
		-e's,@INSTR_CFLAGS\@,'"$$tmp_instr_cflags"',g' \
		-e's,@INSTRDEBUG_CFLAGS\@,'"$$tmp_instrdebug_cflags"',g' \
		-e's,@PERFO_CXXFLAGS\@,'"$$tmp_perfo_cxxflags"',g' \
		-e's,@DEBUG_CXXFLAGS\@,'"$$tmp_debug_cxxflags"',g' \
		-e's,@INSTR_CXXFLAGS\@,'"$$tmp_instr_cxxflags"',g' \
		-e's,@INSTRDEBUG_CXXFLAGS\@,'"$$tmp_instrdebug_cxxflags"',g' \
		-e's,@CONFIG_CC\@,'"$$tmp_cc"',g' \
		-e's,@CONFIG_CXX\@,'"$$tmp_cxx"',g' \
		$(srcdir)/nanox-creation-generator.in > nanox-creation-generator.tmp
	mv nanox-creation-generator.tmp nanox-creation-generator
	chmod 755 nanox-creation-generator

api-omp-generator: api-omp-generator.in
	rm -f api-omp-generator
	tmp_plugins_perfo=`cat $(abs_top_builddir)/src/plugins-list | grep ^performance\= | cut -d'=' -f2` ;\
	tmp_plugins_debug=`cat $(abs_top_builddir)/src/plugins-list | grep ^debug\= | cut -d'=' -f2` ;\
	tmp_plugins_instr=`cat $(abs_top_builddir)/src/plugins-list | grep ^instrumentation\= | cut -d'=' -f2` ;\
	tmp_plugins_instr_debug=`cat $(abs_top_builddir)/src/plugins-list | grep ^instrumentation_debug\= | cut -d'=' -f2` ;\
	tmp_compile_versions=`cat $(abs_top_builddir)/src/plugins-list | cut -d'=' -f1 | sed -e :a -e '$!N; s/\n/ /; ta'` ;\
	tmp_srcdir=$(abs_top_srcdir) ; \
	tmp_builddir=$(abs_top_builddir) ; \
	tmp_perfo_cflags='"$(perfo_cflags)"' ; \
	tmp_debug_cflags='"$(debug_cflags)"' ; \
	tmp_instr_cflags='"$(instr_cflags)"' ; \
	tmp_instrdebug_cflags='"$(instrdebug_cflags)"' ; \
	tmp_perfo_cxxflags='"$(global_perfo_cxxflags)"' ; \
	tmp_debug_cxxflags='"$(global_debug_cxxflags)"' ; \
	tmp_instr_cxxflags='"$(global_instr_cxxflags)"' ; \
	tmp_instrdebug_cxxflags='"$(global_instrdebug_cxxflags)"' ; \
	tmp_cc='"$(CC)"' ; \
	tmp_cxx='"$(CXX)"' ; \
	sed \
		-e's,@SRCDIR\@,'"$$tmp_srcdir"',g' \
		-e's,@BUILDDIR\@,'"$$tmp_builddir"',g' \
		-e's,@PLUGINS_PERFORMANCE\@,'"$$tmp_plugins_perfo"',g' \
		-e's,@PLUGINS_DEBUG\@,'"$$tmp_plugins_debug"',g' \
		-e's,@PLUGINS_INSTRUMENTATION\@,'"$$tmp_plugins_instr"',g' \
		-e's,@PLUGINS_INSTRUMENTATION_DEBUG\@,'"$$tmp_plugins_instr_debug"',g' \
		-e's,@COMPILE_VERSIONS\@,'"$$tmp_compile_versions"',g' \
		-e's,@PERFO_CFLAGS\@,'"$$tmp_perfo_cflags"',g' \
		-e's,@DEBUG_CFLAGS\@,'"$$tmp_debug_cflags"',g' \
		-e's,@INSTR_CFLAGS\@,'"$$tmp_instr_cflags"',g' \
		-e's,@INSTRDEBUG_CFLAGS\@,'"$$tmp_instrdebug_cflags"',g' \
		-e's,@PERFO_CXXFLAGS\@,'"$$tmp_perfo_cxxflags"',g' \
		-e's,@DEBUG_CXXFLAGS\@,'"$$tmp_debug_cxxflags"',g' \
		-e's,@INSTR_CXXFLAGS\@,'"$$tmp_instr_cxxflags"',g' \
		-e's,@INSTRDEBUG_CXXFLAGS\@,'"$$tmp_instrdebug_cxxflags"',g' \
		-e's,@CONFIG_CC\@,'"$$tmp_cc"',g' \
		-e's,@CONFIG_CXX\@,'"$$tmp_cxx"',g' \
		$(srcdir)/api-omp-generator.in > api-omp-generator.tmp
	mv api-omp-generator.tmp api-omp-generator
	chmod 755 api-omp-generator

