// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from shared A? to shared A
  var alloc = new shared A();
  var a:shared A? = alloc;
  var a_:shared A = a;
}
proc main() {
  foo();
}
