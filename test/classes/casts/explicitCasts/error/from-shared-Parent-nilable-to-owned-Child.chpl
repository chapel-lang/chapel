// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // casting from shared Parent? to owned Child
  var alloc = new shared Parent();
  var a:shared Parent? = alloc;
  var a_ = a:owned Child;
}
proc main() {
  foo();
}
