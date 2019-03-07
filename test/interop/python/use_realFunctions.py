import realFunctions

realFunctions.chpl_setup()
x = realFunctions.getReal()
realFunctions.takesReal(x)
y = 3.2
realFunctions.takesReal(y)
z = realFunctions.takeAndReturn(x)
realFunctions.takesReal(z)
realFunctions.chpl_cleanup()
