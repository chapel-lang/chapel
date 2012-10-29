class A {
  proc virt() {
    return "A";
  }
}

proc main() {
  var a = new A();
  var str = a.virt();
  __primitive("chpl_error", str);
}

