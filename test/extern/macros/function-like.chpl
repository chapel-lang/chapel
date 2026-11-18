use CTypes;

extern proc MACRO_NEXT(): c_int;
extern proc MACRO_SUM(a, b): c_longlong;
extern proc MACRO_TWICE(a: c_int): c_int;
extern proc MACRO_DEREF(p: c_ptr(c_int)): c_int;
extern proc MACRO_DISCARD(a: c_int);

var evaluations = 0;
proc nextArg(): c_int {
  evaluations += 1;
  return 3:c_int;
}

proc nextFromAnotherFunction(): c_int {
  return MACRO_NEXT();
}

proc main() {
  writeln(MACRO_NEXT(), " ", nextFromAnotherFunction());
  writeln(MACRO_SUM(2:c_int, 3:c_int));
  writeln(MACRO_SUM(4:c_longlong, 5:c_longlong));
  writeln(MACRO_TWICE(nextArg()), " ", evaluations);
  var x: c_int = 17;
  writeln(MACRO_DEREF(c_ptrTo(x)));
  MACRO_DISCARD(0:c_int);
  writeln(MACRO_NEXT());
}
