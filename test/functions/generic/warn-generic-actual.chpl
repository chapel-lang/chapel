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
