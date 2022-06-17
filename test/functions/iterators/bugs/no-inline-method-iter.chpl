iter foo() {
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

writeln("Calling standalone foo()");
for i in foo() {
  writeln(i);
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

