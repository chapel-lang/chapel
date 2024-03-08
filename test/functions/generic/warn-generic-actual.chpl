proc f(type t) { }
record genericRecord { type eltType; }

f(genericRecord); // warn
f(genericRecord(?)); // do not warn

type T = genericRecord(?);
f(T); // warn
f(T(?)); // do not warn

// same, within a function
proc test0() {
  f(genericRecord); // warn
  f(genericRecord(?)); // do not warn

  type T = genericRecord(?);
  f(T); // warn
  f(T(?)); // do not warn
}
test0();

// same, within a function, with a type alias
proc test1() {
  type T = genericRecord(?);
  f(T); // warn
  f(T(?)); // do not warn
}
test1();

// within a function, with a type alias, no (?) on type =
proc test2() {
  type T = genericRecord;
  f(T); // warn
  f(T(?)); // do not warn
}
test2();

class C { type t; }
proc test3() {
  f(C); // warn
  f(borrowed C); // warn
  f(unmanaged C); // warn
  f(owned C); // warn
  f(shared C); // warn
}
test3();

pragma "suppress generic actual warning"
proc g(type t) { }

proc test4() {
  g(genericRecord); // do not warn (suppressed by pragma)
}
test4();

proc test5() {
  f(integral); // do not warn (built in generic type)
}
test5();

proc type C.typeMethod() { }
proc test6() {
  C.typeMethod(); // do not warn (method receiver)
}
test6();
