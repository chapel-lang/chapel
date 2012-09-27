class A {
  var x = 7;
}

proc main() {
  var a = new A();
  delete a;
  __primitive("chpl_error", "OK");
}

