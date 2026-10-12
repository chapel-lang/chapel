
proc other(arg) {
  return arg("test");
}

// The AST of 'arg' should eventually end up with a 'ref' intent during
// 'resolveIntents'. This means that function types of the same type need to
// undergo the same transformation.
proc helper(in arg: string) {
  return "got: " + arg;
}

proc main() {
  var result = other(helper);
  writeln(result);
}
