import exportStringArray as test

test.chpl_setup()

a = ['foo', 'bar', 'baz']
b = test.makeStringArray()

for (x, y) in zip(a, b):
  assert(x == y)

print(type(b))
print(b)

test.chpl_cleanup()

