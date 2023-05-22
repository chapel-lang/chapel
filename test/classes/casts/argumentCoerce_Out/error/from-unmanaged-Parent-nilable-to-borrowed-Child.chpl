// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
// coercing from unmanaged Parent? to borrowed Child
var globalAlloc = new owned Child();
proc bar(out x: borrowed Child) {
  x = globalAlloc;
}
proc foo() {
  var a = new unmanaged Parent?();
  bar(a);
}
proc main() {
  foo();
}
