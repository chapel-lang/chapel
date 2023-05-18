// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // casting from owned Child? to owned Parent?
  var alloc = new owned Child();
  var a:owned Child? = alloc;
  var a_ = a:owned Parent?;
}
proc main() {
  foo();
}
