import exportBytes as test

test.chpl_setup()

b1 = test.noArgsRetBytes()
print(b1)

b2 = b"Some long bytes string with \x00 a nasty embedded null!"
test.takeBytesRetVoid(b2)

b3 = b"Sick compiler you got there"
b4 = test.takeBytesRetBytes(b3)

print(b4)

test.chpl_cleanup()

