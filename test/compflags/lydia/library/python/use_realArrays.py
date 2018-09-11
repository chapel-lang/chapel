import realArrays

realArrays.chpl_setup()
x = [1.0, 2.0, 3.0, 4.0, 5.0, 6.0]
realArrays.takesArray(x)
y = realArrays.returnsArray()
realArrays.takesArray(y)
realArrays.chpl_cleanup()
