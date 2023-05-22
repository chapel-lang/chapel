// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
// coercing from borrowed A to borrowed A?
var globalAlloc = new owned A?();
proc bar(inout x: borrowed A?) {
  x = globalAlloc;
}
proc foo() {
  var alloc = new owned A();
  var a:borrowed A = alloc;
  bar(a);
}
proc main() {
  foo();
}
