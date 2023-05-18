// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from borrowed A? to owned A?
  var alloc = new owned A?();
  var a:borrowed A? = alloc;
  var a_:owned A? = a;
}
proc main() {
  foo();
}
