
use Reflection;

record R {
  type A;
  type B;
  param C : int;
}

const x = new R(int, real, 42);

// Note: Output can be nondeterministic

const ref q = x.A;
writeln("q = ", q);

for param i in 0..<numFields(x.type) {
  param name = getFieldName(x.type, i);
  const ref val = getField(x, i);

  writeln("x.", name, " = ", val);
}
