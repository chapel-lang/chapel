// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
// coercing from borrowed Child? to borrowed Parent
var globalAlloc = new owned Parent();
proc bar(out x: borrowed Parent) {
  x = globalAlloc;
}
proc foo() {
  var alloc = new owned Child?();
  var a:borrowed Child? = alloc;
  bar(a);
}
proc main() {
  foo();
}
