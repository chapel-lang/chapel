// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
// coercing from borrowed A? to owned A
proc bar(out x: owned A) {
  x = new owned A();
}
proc foo() {
  var alloc = new owned A?();
  var a:borrowed A? = alloc;
  bar(a);
}
proc main() {
  foo();
}
