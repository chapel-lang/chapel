use Reflection;

proc f() {
  compilerError("I am an error");
}

proc g() {
  if canResolve("f") {
    f();
    writeln("in g, f resolved");
  } else {
    writeln("in g, f did not resolve");
  }
}

proc t() {
  g();

  if canResolve("f") {
    writeln("in t, f resolved");
  } else {
    writeln("in t, f did not resolve");
  }

  f();
}
t();
