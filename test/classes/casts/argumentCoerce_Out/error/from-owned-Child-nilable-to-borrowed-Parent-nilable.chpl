// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
// coercing from owned Child? to borrowed Parent?
var globalAlloc = new owned Parent?();
proc bar(out x: borrowed Parent?) {
  x = globalAlloc;
}
proc foo() {
  var a = new owned Child?();
  bar(a);
}
proc main() {
  foo();
}
