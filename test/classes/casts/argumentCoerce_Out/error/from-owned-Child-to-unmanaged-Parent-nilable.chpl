// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
// coercing from owned Child to unmanaged Parent?
proc bar(out x: unmanaged Parent?) {
  x = new unmanaged Parent?();
}
proc foo() {
  var a = new owned Child();
  bar(a);
}
proc main() {
  foo();
}
