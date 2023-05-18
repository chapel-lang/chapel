// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // casting from borrowed Child to shared Parent?
  var alloc = new owned Child();
  var a:borrowed Child = alloc;
  var a_ = a:shared Parent?;
}
proc main() {
  foo();
}
