proc test1() {
  var a: domain(int);   // warn (default parSafe)
  const b: domain(int);

  var c:domain(int, parSafe=true); // warn-unstable
  c = a;

  var d = {"hi", "there"};  // warn (default parSafe)

  const e = {"hi", "there"};

  var f: domain(string, parSafe=false) = {"hi", "there"};
  var g: domain(string, parSafe=true) = {"hi", "there"}; // warn-unstable

  var h: domain(string) = {"hi", "there"};  // warn (default parSafe)
}
test1();

// record fields
record R {
  var aa: domain(string); // warn (default parSafe)
  var bb: domain(string, parSafe=false);
}
proc test2() {
  var x = new R();
}
test2();

// in-intent formals
proc acceptsInFormal(in cc) { // warn
}

proc acceptsConstInFormal(const in dd) {
}

proc acceptsExplicitFormal(in ee: domain(int, parSafe=false)) {
}

proc test3() {
  const a: domain(int);
  const b: domain(int, parSafe=false);
  const c: domain(int, parSafe=true); // warn-unstable
  acceptsInFormal(a); // warns at formal declaration
  acceptsConstInFormal(a);
  acceptsExplicitFormal(a);
  acceptsInFormal(b); // warns at formal declaration, because we can't differentiate between default and not
  acceptsConstInFormal(b);
  acceptsExplicitFormal(b);
  acceptsInFormal(c); // not warn at formal declaration, because it couldn't possible be the default
  acceptsConstInFormal(c);
}
test3();

// function takes type t and makes a variable
proc typeT(type t) {
  var x: t; // warn (can't discriminate default vs not)
}
proc test4() {
  typeT(domain(int, parSafe=true));
}
test4();

proc returnsIt() {
  const a: domain(int);
  return a;
}
proc test5() {
  var y = returnsIt(); // warn
}
test5();
