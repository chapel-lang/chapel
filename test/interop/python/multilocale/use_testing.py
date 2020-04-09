import testing

testing.chpl_setup(1)
testing.foo()
x = 3
y = testing.takesAndReturns(x)
testing.threadring(1000, 98);
testing.chpl_cleanup()
