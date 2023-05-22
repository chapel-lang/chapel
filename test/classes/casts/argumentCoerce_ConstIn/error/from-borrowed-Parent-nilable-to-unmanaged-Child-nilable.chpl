// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
// coercing from borrowed Parent? to unmanaged Child?
proc bar(const in x: unmanaged Child?) {}
proc foo() {
  var alloc = new owned Parent?();
  var a:borrowed Parent? = alloc;
  bar(a);
}
proc main() {
  foo();
}
