class A {
  proc virt() {
    return "A";
  }
}

class B : A {
  proc virt() {
    return "B";
  }
}

proc main() {
  var b = new B();
  var str = b.virt();
  __primitive("chpl_error", str);
}

