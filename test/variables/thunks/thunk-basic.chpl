proc doSomething() {
  writeln("doing something!");
  return 42;
}

var theThunk = __primitive("create thunk", doSomething());
writeln("Not yet, Ferb!");
writeln(__primitive("force thunk", theThunk));
