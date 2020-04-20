import cleanupAtExit as test

def exitMessage():
  test.display('Called cleanup routine!')
  return

# Set hidden debug callback.
test.chpl_set_cleanup_callback(exitMessage)

test.chpl_setup()

test.display('Hello, world!')
