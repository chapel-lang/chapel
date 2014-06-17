record A {
  var name = "A";
}

proc main() {
  var a:A;
  var str = a.name;
  __primitive("chpl_error", str);
}

