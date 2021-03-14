import uintFunctions

uintFunctions.chpl_setup(1)
x = uintFunctions.getUint()
uintFunctions.takesUint(x)
y = uintFunctions.takeAndReturn(x)
uintFunctions.takesUint(y)
uintFunctions.chpl_cleanup()
