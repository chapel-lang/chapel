// note that x is *not* a type argument
proc f(x) type {
  return int;
}

class MyClass {
  const primary;
  var secondary: f(primary);
}

proc test() {
  var c:borrowed MyClass(int)?;
}

test();
