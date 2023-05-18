// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from owned A to shared A
  var a = new owned A();
  var a_ = new shared A();
  a_ = a;
}
proc main() {
  foo();
}
