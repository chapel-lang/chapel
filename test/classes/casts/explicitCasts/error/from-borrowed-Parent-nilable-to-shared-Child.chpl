// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // casting from borrowed Parent? to shared Child
  var alloc = new owned Parent();
  var a:borrowed Parent? = alloc;
  var a_ = a:shared Child;
}
proc main() {
  foo();
}
