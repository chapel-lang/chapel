// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // casting from owned A? to shared A?
  var a = new owned A?();
  var a_ = a:shared A?;
}
proc main() {
  foo();
}
