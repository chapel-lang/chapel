// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // casting from borrowed A? to shared A?
  var alloc = new owned A?();
  var a:borrowed A? = alloc;
  var a_ = a:shared A?;
}
proc main() {
  foo();
}
