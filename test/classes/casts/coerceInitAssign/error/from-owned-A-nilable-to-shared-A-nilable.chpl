// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from owned A? to shared A?
  var alloc = new owned A();
  var a:owned A? = alloc;
  var a_:shared A? = a;
}
proc main() {
  foo();
}
