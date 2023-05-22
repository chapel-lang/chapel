// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
// coercing from owned Parent? to borrowed Child?
var globalAlloc = new owned Child?();
proc bar(out x: borrowed Child?) {
  x = globalAlloc;
}
proc foo() {
  var a = new owned Parent?();
  bar(a);
}
proc main() {
  foo();
}
