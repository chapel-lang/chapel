# import via the current working directory, since we have an `__init__.py` there
import myLoc.testing as testing

testing.chpl_setup()
testing.foo()
x = 3
y = testing.takesAndReturns(x)
testing.threadring(1000, 98)
testing.chpl_cleanup()
