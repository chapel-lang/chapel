import cptrConstCharArrays

cptrConstCharArrays.chpl_setup()
x = cptrConstCharArrays.returnsArray()
cptrConstCharArrays.takesArray(x)
y = [b"maybe", b"this", b"also", b"works", b"too?"]
cptrConstCharArrays.takesArray(y)
cptrConstCharArrays.chpl_cleanup()
