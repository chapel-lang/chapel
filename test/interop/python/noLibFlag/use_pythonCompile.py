import pythonCompile

pythonCompile.chpl_setup()
pythonCompile.foo()
x = 3
y = pythonCompile.takesAndReturns(x)
pythonCompile.threadring(1000, 98);
pythonCompile.chpl_cleanup()
