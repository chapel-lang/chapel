import defaultValues

defaultValues.chpl_setup()
a = 1.3
defaultValues.realDefault(a)
defaultValues.realDefault()
b = False
defaultValues.boolDefault(b)
defaultValues.boolDefault()
defaultValues.uintDefault()
c = 9
defaultValues.intDefault(c)
defaultValues.intDefault()
d = b"not the default"
defaultValues.cstringDefault(d)
defaultValues.cstringDefault()
defaultValues.chpl_cleanup()
