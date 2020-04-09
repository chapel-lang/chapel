import testing

testing.chpl_setup()
testing.foo()
x = 3
y = testing.takesAndReturns(x)
testing.threadring(1000, 98);
testing.chpl_cleanup()
