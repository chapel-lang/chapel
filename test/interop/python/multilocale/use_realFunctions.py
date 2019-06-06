import realFunctions

realFunctions.chpl_setup(1)
x = realFunctions.getReal()
realFunctions.takesReal(x)
y = 3.2
realFunctions.takesReal(y)
z = realFunctions.takeAndReturn(x)
realFunctions.takesReal(z)
realFunctions.chpl_cleanup()
