proc test1() {
  var a: domain(int);   // warn
  const b: domain(int);

  var c:domain(int, parSafe=true);
  c = a;

  var d = {"hi", "there"}; // warn

  const e = {"hi", "there"};
  
  var f: domain(string, parSafe=true) = {"hi", "there"};
  
  var g: domain(string) = {"hi", "there"}; // warn
}
test1();

// record fields
record R {
  var aa: domain(string); // warn
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

proc acceptsExplicitFormal(in ee: domain(int, parSafe=true)) {
}

proc test3() {
  const a: domain(int);
  acceptsInFormal(a); // warns at formal declaration
  acceptsConstInFormal(a);
  acceptsExplicitFormal(a);
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
