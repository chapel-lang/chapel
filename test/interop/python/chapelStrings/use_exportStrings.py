import exportStrings as test

def getFirstNullCodepointPos(s):
  for i in range(0, len(s)):
    cp = ord(s[i])
    if cp == 0x00:
      return i
  return -1

test.chpl_setup()

s1 = test.noArgsRetString()

# Use this because we can't capture Python STDOUT.
test.printString(s1)

s2 = "Some long UTF-8 string with \x00 a nasty embedded null!"
test.takeStringRetVoid(s2)

np1 = getFirstNullCodepointPos(s2)
np2 = test.getFirstNullCodepointPos(s2)

assert(np1 == np2)

s3 = "Cool UTF-8 string you got there"
s4 = test.takeStringRetString(s3)

# Use this because we can't capture Python STDOUT.
test.printString(s4)

test.chpl_cleanup()

