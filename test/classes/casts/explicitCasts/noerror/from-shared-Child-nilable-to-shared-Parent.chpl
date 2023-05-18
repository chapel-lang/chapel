// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // casting from shared Child? to shared Parent
  var alloc = new shared Child();
  var a:shared Child? = alloc;
  var a_ = a:shared Parent;
}
proc main() {
  foo();
}
