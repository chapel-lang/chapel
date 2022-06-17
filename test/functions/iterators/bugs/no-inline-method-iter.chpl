// This works
iter foo() {
  var i: int;
  const start = 0;
  const end = 1;
  const stride = 1;
  while i <= end {
    yield i;
    i += stride;
  }
}

// Once it's a method, it doesn't
iter range.bar() {
  var i: int;
  const start = 0;
  const end = 1;
  const stride = 1;
  i = start;
  while i <= end {
    yield i;
    i += stride;
  }
}

// Even a for-loop based method doesn't
iter range.baz() {
  var i: int;
  const start = 0;
  const end = 1;
  const stride = 1;
  for i in start..end by stride do
    yield i;
}

// This is similar to the unbounded range iterator for bools/enums
iter range.foo() {
  var i: int;
  const start = 0;
  const end = 1;
  const stride = 1;
  while __primitive("C for loop",
                    __primitive( "=", i, start),
                    __primitive("<=", i, end),
                    __primitive("+=", i, stride)) {
    yield i;
  }
}

writeln("Calling standalone foo()");
for i in foo() {
  writeln(i);
}

writeln("Calling range.baz()");
for i in (false..).baz() {
  writeln(i);
  if i > 1 {
    writeln("This is not working...");
    break;
  }
}

writeln("Calling range.bar()");
for i in (false..).bar() {
  writeln(i);
  if i > 1 {
    writeln("This is not working...");
    break;
  }
}

writeln("Calling range.foo()");
for i in (false..).foo() {
  writeln(i);
  if i > 1 {
    writeln("This is not working...");
    break;
  }
}

var count = 0;
writeln("Calling range.iterator()");
for i in (false..) {
  count += 1;
  writeln(i);
  if count > 2 {
    writeln("This is not working...");
    break;
  }
}

writeln("Calling range.foo() on a bounded range");
for i in (false..true).foo() {
  writeln(i);
}
