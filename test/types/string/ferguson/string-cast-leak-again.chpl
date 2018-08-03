config const s = "hello";

proc test1() {
  var mystring:string = s*8;
  var y:string;
  ref z = y;

  // Case 1: "useless cast"
  z = mystring:string;
  writeln(y);
}

test1();

record R {
  var s:string;
}

proc _cast(type t, x:R) where t == string {
  return x.s;
}

proc test2() {
  var mystring:string = s*8;
  var myr = new R(mystring);
  var y:string;
  ref z = y;

  // Case 2: not a useless cast
  z = myr:string;
  writeln(y);
}

test2();
