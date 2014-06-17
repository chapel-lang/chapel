class A {
  var name = "A";
}

proc main() {
  var a = new A();
  var str = a.name;
  __primitive("chpl_error", str);
}

