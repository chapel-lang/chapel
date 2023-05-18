// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from shared Child? to owned Parent?
  var alloc = new shared Child();
  var a:shared Child? = alloc;
  var a_:owned Parent? = a;
}
proc main() {
  foo();
}
