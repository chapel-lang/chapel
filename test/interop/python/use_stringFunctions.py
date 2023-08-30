import stringFunctions

stringFunctions.chpl_setup()
x = stringFunctions.getString()
stringFunctions.takesString(x)
y = b"blah blah blah"
stringFunctions.takesString(y)
z = stringFunctions.takeAndReturn(x)
stringFunctions.takesString(z)
# preserve c_string tests while deprecating
# TODO: remove once c_string fully deprecated
cx = stringFunctions.getStringC()
stringFunctions.takesStringC(cx)
cy = b"blah blah blah"
stringFunctions.takesStringC(cy)
cz = stringFunctions.takeAndReturnC(cx)
stringFunctions.takesStringC(cz)
stringFunctions.chpl_cleanup()
