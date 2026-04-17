class A {
  type t = A?;
  var a: t;
  proc init(doit=false) {
    if doit then
      a = new unmanaged A();
  }
}

proc main() {
  var a = new A(true);
}
