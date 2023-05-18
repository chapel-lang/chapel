// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from shared Child? to unmanaged Parent
  var alloc = new shared Child();
  var a:shared Child? = alloc;
  var a_:unmanaged Parent = a;
}
proc main() {
  foo();
}
