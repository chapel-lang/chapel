import uintFunctions

uintFunctions.chpl_setup()
x = uintFunctions.getUint()
uintFunctions.takesUint(x)
y = uintFunctions.takeAndReturn(x)
uintFunctions.takesUint(y)
uintFunctions.chpl_cleanup()
