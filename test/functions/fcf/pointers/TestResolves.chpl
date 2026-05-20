
use Reflection;

proc helper(arg: int) {
  return arg*2;
}

proc other(arg: int) {
  return arg*3;
}

proc caller(in fn, arg) {
  writeln("caller: ", __primitive("resolves", fn(arg)));
}

proc main() {
  writeln("helper: ", __primitive("resolves", helper(5)));
  var x = if numLocales > 100 then other else helper;
  writeln("stored: ", __primitive("resolves", x(5)));
  caller(x, 5);
  caller(helper, 5);
  var y = proc(arg: int) {
    return arg*4;
  };
  writeln("anon: ", __primitive("resolves", y(5)));

  writeln("\n--- failures ---");
  // These should all fail to resolve, but not crash the compiler.
  writeln("helper: ", __primitive("resolves", helper("not an int")));
  writeln("stored: ", __primitive("resolves", x("not an int")));
  caller(x, "not an int");
  caller(helper, "not an int");
  writeln("anon: ", __primitive("resolves", y("not an int")));
}
