import checkExplicitLibname

checkExplicitLibname.chpl_setup()
checkExplicitLibname.foo()
x = 3
y = checkExplicitLibname.takesAndReturns(x)
checkExplicitLibname.threadring(1000, 98);
checkExplicitLibname.chpl_cleanup()
