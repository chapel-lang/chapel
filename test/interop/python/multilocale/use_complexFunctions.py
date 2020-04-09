import complexFunctions

complexFunctions.chpl_setup(1)
x = complexFunctions.getComplex()
complexFunctions.takesComplex(x)
y = 3 + 3j
complexFunctions.takesComplex(y)
z = complexFunctions.takeAndReturn(y)
complexFunctions.takesComplex(z)
complexFunctions.chpl_cleanup()
