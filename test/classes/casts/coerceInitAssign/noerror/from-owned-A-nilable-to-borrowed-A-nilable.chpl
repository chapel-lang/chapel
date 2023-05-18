// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from owned A? to borrowed A?
  var alloc = new owned A();
  var a:owned A? = alloc;
  var a_:borrowed A? = a;
}
proc main() {
  foo();
}
