import intArrays

intArrays.chpl_setup()
x = [5, 4, 3, 2, 1]
intArrays.takesArray(x)
y = intArrays.returnsArray()
intArrays.takesArray(y)
intArrays.chpl_cleanup()
