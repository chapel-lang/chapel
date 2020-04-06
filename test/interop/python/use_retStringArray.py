import retStringArray as test

test.chpl_setup()

a = ['foo', 'bar', 'baz']
b = test.makeStringArray()

for (x, y) in zip(a, b):
  assert(x == y)

test.display(str(type(b)))
test.display(str(type(b[0])))
test.display(str(b))

test.chpl_cleanup()

