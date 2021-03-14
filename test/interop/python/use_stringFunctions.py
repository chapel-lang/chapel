import stringFunctions

stringFunctions.chpl_setup()
x = stringFunctions.getString()
stringFunctions.takesString(x)
y = b"blah blah blah"
stringFunctions.takesString(y)
z = stringFunctions.takeAndReturn(x)
stringFunctions.takesString(z)
stringFunctions.chpl_cleanup()
