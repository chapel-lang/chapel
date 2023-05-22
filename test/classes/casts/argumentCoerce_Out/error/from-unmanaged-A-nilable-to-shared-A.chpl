// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
// coercing from unmanaged A? to shared A
proc bar(out x: shared A) {
  x = new shared A();
}
proc foo() {
  var a = new unmanaged A?();
  bar(a);
}
proc main() {
  foo();
}
