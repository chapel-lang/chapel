// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // casting from owned Parent? to borrowed Child?
  var a = new owned Parent?();
  var a_ = a:borrowed Child?;
}
proc main() {
  foo();
}
