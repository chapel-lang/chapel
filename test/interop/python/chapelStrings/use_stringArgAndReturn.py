import stringArgAndReturn

stringArgAndReturn.chpl_setup()

x = stringArgAndReturn.noArgsRetString()
stringArgAndReturn.stringArgsRetVoid(x)
y = b"eek!"
stringArgAndReturn.stringArgsRetVoid(y)
a = b"*slams "
b = b"door*"
c = stringArgAndReturn.stringArgsRetString(a, b)
stringArgAndReturn.stringArgsRetVoid(c)

stringArgAndReturn.chpl_cleanup()
