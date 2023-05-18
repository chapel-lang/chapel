// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // casting from shared A? to borrowed A
  var alloc = new shared A();
  var a:shared A? = alloc;
  var a_ = a:borrowed A;
}
proc main() {
  foo();
}
