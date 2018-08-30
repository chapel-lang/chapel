import bar

bar.chpl_setup()
bar.foo()
x = 3
y = bar.takesAndReturns(x)
bar.threadring(1000, 98);
bar.chpl_cleanup()
