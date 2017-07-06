How to test ANNOTATIONS.yaml changes locally
============================================

You're doing performance triage and you want to add an annotation to
the graphs, but want to test it before committing (good choice!).
Here's what you should do:


1) Make sure you have set up a Python environment that supports PyYAML:

   a) Does 'pip list' list pyYAML?  If not, do (b)-(d) the first time;
      (c)-only thereafter.

   b) Once, use: 'virtualenv <dirname>' to set up a virtual python env
      via a directory in the current working directory.  This will
      create a little bin/ include/ lib/ directory structure in the
      dirname you give.

   c) Every time: 'source <dirname>/bin/activate'.  Your environment
      should show up in your prompt once it's activated.

   d) Once: 'easy_install pyYAML'


2) Make sure you have a performance directory that contains sufficient
   performance data to span the time period of interest and are
   pointing CHPL_TEST_PERF_DIR to it.  My technique is:

   a) update my local copy of the chapel-perf repository

       'cd ~/chapel-perf && git pull'

   b) symbolically link one of the subdirectories from that repository
      into my $CHPL_HOME/test/perfdat directory.

       'cd $CHPL_HOME/test/perfdat && ln -s ~/chapel-perf/chap04 ./chap04-repo'

   c) make sure CHPL_TEST_PERF_DIR is set to point to that link:

       'export CHPL_TEST_PERF_DIR=$CHPL_HOME/test/perfdat/chap04-repo'


3) Re-generate the graphs:

       'cd $CHPL_HOME && start_test --gen-graphs'


4) Point a web browser to $CHPL_TEST_PERF_DIR/html/index.html

       'firefox $CHPL_TEST_PERF_DIR/html/index.html'
