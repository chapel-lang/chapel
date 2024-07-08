// test uses of `domain(?)` in var, arg, and return types,
// on its own and in tuples

writeln("return types");

// reference: a concrete declared return type
proc f11(): domain(1) {
  return LocaleSpace;
}
writeln(f11());

// domain(?) as the declared return type
proc f12(): domain(?) {
  var d1: domain(1);
  return d1;
}
writeln(f12());

// domain(?) as the declared yield type
iter f13(): domain(?) {
  yield LocaleSpace;
}
for loc in f13() do writeln(loc);

// domain(?) as the declared type of the returned var; inferred return type
proc f14() {
  var d: domain(?) = LocaleSpace;
  writeln(d);
}
f14();

/////////////////////////////////

writeln("argument types");

// domain(?) as the declared formal type
proc f3(arg3: domain(?)) {
  writeln("f3 ", arg3);
}
f3(LocaleSpace);

type t4 = (domain(?),int);

// domain(?) as a tuple component of the declared formal type
//todo: allow proc f4(arg: (domain, int))
// t4(?): the (?) is required because t4 is generic
proc f4(arg4: t4(?)) {
  writeln("f4 ", arg4);
}
f4((LocaleSpace,0));

/////////////////////////////////

writeln("tuple types");

// refernce: generic declared type of a variable
proc f81() {
  var tup: _tuple = (LocaleSpace, 0);
  return tup;
}
writeln(f81());

// domain(?) as a component of declared tuple type of a variable
// using split init
proc f82() {
  var tup: (domain(?), int);
  tup = (LocaleSpace, 0);
  return tup;
}
writeln(f82());

// domain(?) as a component of declared tuple return type
proc f83(): (domain(?), int) {
  return (LocaleSpace, 0);
}
writeln(f83());

// ditto; returning a split-initialized variable with same declared type
proc f84(): (domain(?), int) {
  var tup: (domain(?), int);
  tup = (LocaleSpace, 0);
  return tup;
}
writeln(f84());

/////////////////////////////////

// double-checking argument passing
writeln("function calls");

f3(f12());
f3(f13());

f4(f81());
f4(f82());
f4(f83());
f4(f84());
