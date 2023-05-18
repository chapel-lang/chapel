// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from shared Child? to shared Parent
  var alloc = new shared Child();
  var a:shared Child? = alloc;
  var a_:shared Parent = a;
}
proc main() {
  foo();
}
