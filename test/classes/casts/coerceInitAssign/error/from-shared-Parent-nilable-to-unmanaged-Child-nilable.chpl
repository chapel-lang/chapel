// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from shared Parent? to unmanaged Child?
  var alloc = new shared Parent();
  var a:shared Parent? = alloc;
  var a_:unmanaged Child? = a;
}
proc main() {
  foo();
}
