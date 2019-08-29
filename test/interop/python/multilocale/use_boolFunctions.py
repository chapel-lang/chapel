import boolFunctions

boolFunctions.chpl_setup(1)
x = False
boolFunctions.takesBool(x)
y = boolFunctions.getBool()
boolFunctions.takesBool(y)
z = boolFunctions.takeAndReturn(y)
boolFunctions.takesBool(z)
boolFunctions.chpl_cleanup()
