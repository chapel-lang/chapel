import exportBytes as test

def getFirstNullBytePos(b):
  for i in range(0, len(b)):
    if b[i] == 0x00:
      return i
  return -1

test.chpl_setup()

b1 = test.noArgsRetBytes()

# Use this because we can't capture Python STDOUT.
test.printBytes(b1)

b2 = b"Some long bytes string with \x00 a nasty embedded null!"
test.takeBytesRetVoid(b2)

np1 = getFirstNullBytePos(b2)
np2 = test.getFirstNullBytePos(b2)

assert(np1 == np2)

b3 = b"Cool byte string you got there"
b4 = test.takeBytesRetBytes(b3)

# Use this because we can't capture Python STDOUT.
test.printBytes(b4)

test.chpl_cleanup()

