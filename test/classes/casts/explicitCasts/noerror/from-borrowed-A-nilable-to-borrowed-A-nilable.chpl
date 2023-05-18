// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // casting from borrowed A? to borrowed A?
  var alloc = new owned A();
  var a:borrowed A? = alloc;
  var a_ = a:borrowed A?;
}
proc main() {
  foo();
}
