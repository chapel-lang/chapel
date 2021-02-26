import retBytesArray as test

test.chpl_setup()

a = [b'f\x00oo', b'bar', b'baz']
b = test.makeBytesArray()

for (x, y) in zip(a, b):
  assert(x == y)

test.display(str(type(b)))
test.display(str(type(b[0])))
test.display(str(b))

test.chpl_cleanup()

