use Reflection;

proc inner() {
  compilerError("I am an error");
}

proc f() {
  inner();
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
