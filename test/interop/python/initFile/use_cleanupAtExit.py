import cleanupAtExit as test

def exitMessage():
  test.display('Called cleanup routine!')
  return

test.chpl_setup(callback=exitMessage)
test.display('Hello, world!')
