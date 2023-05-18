// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from owned A? to unmanaged A
  var alloc = new owned A();
  var a:owned A? = alloc;
  var a_:unmanaged A = a;
}
proc main() {
  foo();
}
