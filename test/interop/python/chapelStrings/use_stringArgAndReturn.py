import stringArgAndReturn

stringArgAndReturn.chpl_setup()

x = stringArgAndReturn.noArgsRetString()
stringArgAndReturn.stringArgsRetVoid(x)
y = "eek!"
stringArgAndReturn.stringArgsRetVoid(y)
a = "*slams "
b = "door*"
c = stringArgAndReturn.stringArgsRetString(a, b)
stringArgAndReturn.stringArgsRetVoid(c)

stringArgAndReturn.chpl_cleanup()
