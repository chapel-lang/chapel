// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
// coercing from borrowed Parent to borrowed Child?
var globalAlloc = new owned Child?();
proc bar(inout x: borrowed Child?) {
  x = globalAlloc;
}
proc foo() {
  var alloc = new owned Parent();
  var a:borrowed Parent = alloc;
  bar(a);
}
proc main() {
  foo();
}
