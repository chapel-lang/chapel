// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
// coercing from unmanaged A? to unmanaged A
proc bar(inout x: unmanaged A) {
  x = new unmanaged A();
}
proc foo() {
  var a = new unmanaged A?();
  bar(a);
}
proc main() {
  foo();
}
