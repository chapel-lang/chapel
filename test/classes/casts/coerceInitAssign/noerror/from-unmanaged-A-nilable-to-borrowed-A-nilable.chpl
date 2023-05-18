// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from unmanaged A? to borrowed A?
  var a = new unmanaged A?();
  var a_:borrowed A? = a;
}
proc main() {
  foo();
}
