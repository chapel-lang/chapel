// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // casting from owned A? to owned A
  var alloc = new owned A();
  var a:owned A? = alloc;
  var a_ = a:owned A;
}
proc main() {
  foo();
}
