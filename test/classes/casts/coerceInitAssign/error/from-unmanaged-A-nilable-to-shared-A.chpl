// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from unmanaged A? to shared A
  var alloc = new unmanaged A();
  var a:unmanaged A? = alloc;
  var a_:shared A = a;
}
proc main() {
  foo();
}
