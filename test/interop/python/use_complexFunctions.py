import complexFunctions

complexFunctions.chpl_setup()
x = complexFunctions.getComplex()
complexFunctions.takesComplex(x)
y = 3 + 3j
complexFunctions.takesComplex(y)
z = complexFunctions.takeAndReturn(y)
complexFunctions.takesComplex(z)
complexFunctions.chpl_cleanup()
