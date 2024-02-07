proc test1() {
  class X {
  var x : int;
  }

  record Y {
    var y : real;
  }

  var y1 = new Y(), y2 = new Y();

  // Default parSafe
  var a1 : domain(int);           // warn (default parSafe)
  var a2 : domain(string);        // warn (default parSafe)
  var a3 : domain(real);          // warn (default parSafe)
  var a4 : domain(borrowed X);    // warn (default parSafe)
  var a5 : domain(Y);             // warn (default parSafe)
  var a6 = {y1, y2};              // warn (default parSafe)
  var a7 = {1, 2, 4, 5, 100};     // warn (default parSafe)
  writeln(a1.type:string);
  writeln(a6.type:string);
  writeln(a7.type:string);

  const b: domain(int);
  writeln(b.type:string);
  var c:domain(int, parSafe=true); // warn-unstable
  c = a1;

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
  if !parSafeOnByDefault then acceptsExplicitFormal(a);
  acceptsInFormal(b); // warns at formal declaration, because we can't differentiate between default and not
  acceptsConstInFormal(b);
  if !parSafeOnByDefault then acceptsExplicitFormal(b);
  acceptsInFormal(c); // not warn at formal declaration, because it couldn't possible be the default
  acceptsConstInFormal(c);
}
test3();

// function takes type t and makes a variable
proc typeT(type t) {
  var x: t; // warn (can't discriminate default vs not)
}
proc test4() {
  typeT(domain(int, parSafe=true));  // warn-unstable
  typeT(domain(int, parSafe=false)); // warn default at callee (can't discriminate)
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
