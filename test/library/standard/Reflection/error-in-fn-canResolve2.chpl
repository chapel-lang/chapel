use Reflection;

proc f() {
  compilerError("I am an error");
}

proc t() {
  if canResolve("f") {
    writeln("in t, f resolved");
  } else {
    writeln("in t, f did not resolve");
  }

  f();
}
t();
