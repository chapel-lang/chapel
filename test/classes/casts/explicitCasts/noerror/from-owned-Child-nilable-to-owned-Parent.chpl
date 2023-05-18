// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // casting from owned Child? to owned Parent
  var a = new owned Child?();
  var a_ = a:owned Parent;
}
proc main() {
  foo();
}
