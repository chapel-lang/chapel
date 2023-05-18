// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // casting from shared A? to shared A
  var a = new shared A?();
  var a_ = a:shared A;
}
proc main() {
  foo();
}
