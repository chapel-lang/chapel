// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from unmanaged A to unmanaged A
  var a = new unmanaged A();
  var a_:unmanaged A = a;
}
proc main() {
  foo();
}
