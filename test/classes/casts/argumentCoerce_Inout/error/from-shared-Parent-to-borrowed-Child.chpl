// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
// coercing from shared Parent to borrowed Child
var globalAlloc = new owned Child();
proc bar(inout x: borrowed Child) {
  x = globalAlloc;
}
proc foo() {
  var a = new shared Parent();
  bar(a);
}
proc main() {
  foo();
}
