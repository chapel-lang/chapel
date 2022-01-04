import defaultValues

defaultValues.chpl_setup(1)
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
defaultValues.chpl_cleanup()
