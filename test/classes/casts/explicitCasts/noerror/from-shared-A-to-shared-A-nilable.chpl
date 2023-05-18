// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // casting from shared A to shared A?
  var alloc = new shared A();
  var a:shared A = alloc;
  var a_ = a:shared A?;
}
proc main() {
  foo();
}
