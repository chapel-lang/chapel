// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
// coercing from shared Child? to borrowed Parent?
var globalAlloc = new owned Parent?();
proc bar(inout x: borrowed Parent?) {
  x = globalAlloc;
}
proc foo() {
  var a = new shared Child?();
  bar(a);
}
proc main() {
  foo();
}
