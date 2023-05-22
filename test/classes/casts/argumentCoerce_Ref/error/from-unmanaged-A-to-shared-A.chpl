// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
// coercing from unmanaged A to shared A
proc bar(ref x: shared A) {}
proc foo() {
  var a = new unmanaged A();
  bar(a);
}
proc main() {
  foo();
}
