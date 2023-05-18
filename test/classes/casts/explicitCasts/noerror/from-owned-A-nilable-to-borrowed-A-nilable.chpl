// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // casting from owned A? to borrowed A?
  var a = new owned A?();
  var a_ = a:borrowed A?;
}
proc main() {
  foo();
}
