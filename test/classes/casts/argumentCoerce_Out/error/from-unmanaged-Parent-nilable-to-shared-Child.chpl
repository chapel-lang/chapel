// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
// coercing from unmanaged Parent? to shared Child
proc bar(out x: shared Child) {
  x = new shared Child();
}
proc foo() {
  var a = new unmanaged Parent?();
  bar(a);
}
proc main() {
  foo();
}
