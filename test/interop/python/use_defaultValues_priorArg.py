import defaultValues_priorArg

defaultValues_priorArg.chpl_setup()
xVal = 2
defaultValues_priorArg.foo(xVal)
yVal = 4
defaultValues_priorArg.foo(xVal, yVal)
defaultValues_priorArg.chpl_cleanup()
