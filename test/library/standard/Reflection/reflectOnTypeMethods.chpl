proc type int.foo() {
  writeln("In int foo");
}

proc type int.bar(x: int) {
  writeln("In int bar(", x, ")");
}

proc type int.baz(x: int, y: int, z: int) {
  writeln("In int baz", (x, y, z));
}

record R {
  proc type foo() {
    writeln("In R foo");
  }

  proc type bar(x: int) {
    writeln("In R bar(", x, ")");
  }

  proc type baz(x: int, y: int, z: int) {
    writeln("In R bar", (x, y, z));
  }
}

checkType(int);
checkType(R);

proc checkType(type t) {
  use Reflection;
  
  assert(canResolveTypeMethod(t, "foo"));
  assert(!canResolveTypeMethod(t, "fog"));
  
  assert(canResolveTypeMethod(t, "bar", 1));
  assert(!canResolveTypeMethod(t, "bar", 1.2));
  assert(!canResolveTypeMethod(t, "bar", 1, 2));
  assert(!canResolveTypeMethod(t, "bar"));
  assert(!canResolveTypeMethod(t, "bag", 1));

  assert(canResolveTypeMethod(t, "baz", 1, 2, 3));
  assert(!canResolveTypeMethod(t, "baz", 1.2, 3, 4));
  assert(!canResolveTypeMethod(t, "baz", 1, 2));
  assert(!canResolveTypeMethod(t, "baz", 1, 2, 3, 4));
  assert(!canResolveTypeMethod(t, "bag", 1, 2, 3));

  writeln("passed all assertions for ", t:string);

  t.foo();
  t.bar(1);
  t.baz(2,3,4);
}

