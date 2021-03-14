import stringTypeInPython as test

test.chpl_setup()

msg1 = 'This is a string.'
msg1Type = str(type(msg1))

test.doPrint(msg1Type)

msg2 = test.doReturnArg(msg1)
msg2Type = str(type(msg2))

test.doPrint(msg2Type)

if type(msg1) != type(msg2):
  test.doPrint('false')

