proc test1() {
  class X {
  var x : int;
  }

  record Y {
    var y : real;
  }

  var y1 = new Y(), y2 = new Y();

  // Default parSafe
  var a1 : domain(int);
  var a2 : domain(string);
  var a3 : domain(real);
  var a4 : domain(borrowed X);
  var a5 : domain(Y);
  var a6 = {y1, y2};
  var a7 = {1, 2, 4, 5, 100};
  writeln(a1.type:string);
  writeln(a6.type:string);
  writeln(a7.type:string);

  const b: domain(int);
  writeln(b.type:string);
  var c:domain(int, parSafe=true); // warn-unstable
  c = a1;

  var d = {"hi", "there"};

  const e = {"hi", "there"};

  var f: domain(string, parSafe=false) = {"hi", "there"};
  var g: domain(string, parSafe=true) = {"hi", "there"}; // warn-unstable

  var h: domain(string) = {"hi", "there"};
}
test1();

// record fields
record R {
  var aa: domain(string);
  var bb: domain(string, parSafe=false);
}
proc test2() {
  var x = new R();
}
test2();

// in-intent formals
proc acceptsInFormal(in cc) {
}

proc acceptsConstInFormal(const in dd) {
}

proc acceptsExplicitFormal(in ee: domain(int, parSafe=false)) {
}

proc test3() {
  const a: domain(int);
  const b: domain(int, parSafe=false);
  const c: domain(int, parSafe=true); // warn-unstable
  acceptsInFormal(a);
  acceptsConstInFormal(a);

  acceptsInFormal(b);
  acceptsConstInFormal(b);

  acceptsInFormal(c);
  acceptsConstInFormal(c);
}
test3();

// function takes type t and makes a variable
proc typeT(type t) {
  var x: t;
}
proc test4() {
  typeT(domain(int, parSafe=true));  // warn-unstable
  typeT(domain(int, parSafe=false));
}
test4();

proc returnsIt() {
  const a: domain(int);
  return a;
}
proc test5() {
  var y = returnsIt();
}
test5();
