// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
// coercing from unmanaged A to borrowed A
var globalAlloc = new owned A();
proc bar(inout x: borrowed A) {
  x = globalAlloc;
}
proc foo() {
  var a = new unmanaged A();
  bar(a);
}
proc main() {
  foo();
}
