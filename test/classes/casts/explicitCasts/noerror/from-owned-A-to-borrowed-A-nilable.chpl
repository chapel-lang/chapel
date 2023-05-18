// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // casting from owned A to borrowed A?
  var alloc = new owned A();
  var a:owned A = alloc;
  var a_ = a:borrowed A?;
}
proc main() {
  foo();
}
