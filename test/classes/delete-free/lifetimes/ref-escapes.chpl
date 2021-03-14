pragma "safe"
module refescapes {

proc ref_identity(ref x:int) ref return x;

proc bad() ref {
  var x:int;
  return ref_identity(x);
}

proc badder(inout x:int) ref {
  return ref_identity(x);
}

record R {
  var x:int;
  proc get() ref { return x; }
}

proc baddest() ref {
  var r:R;
  return r.get();
}

proc nested_bad() ref {
  var x: int;
  proc inner() ref {
    return x;
  }
  return inner();
}

record A {
  var x:int;
  var y:real;
}

proc const_ref_identity(const ref x) const ref {
  return x;
}

proc bad_two_args(x:int, y:real) const ref {
  return const_ref_identity(x);
}

proc bad_record_arg(in arg:A) const ref {
  return arg.x;
}

proc return_record_arg(arg:A) const ref {
  return arg.x;
}

proc bad_return_return_record_arg() const ref {
  var aa = new A(1, 2.0);
  return return_record_arg(aa);
}

proc identity2(ref x:int, ref y:int) ref {
  return x;
}

proc badBar1(ref x: int) ref {
  var y:int;
  return identity2(x,y);
}

proc badBar2(ref x: int) ref {
  var y:int;
  return identity2(y,x);
}


proc R.badBorrow1(const ref other) const ref {
  return other;
}

proc R.badBorrow2(const ref other) const ref {
  const ref tmp = other;
  return tmp;
}


var global = 1;
var globalR:R;

proc ok1() ref {
  return global;
}

proc ok2() {
  var x:int; 
  return x;
}

proc value_identity(x) { return x; }

proc ok3() {
  var x:int; 
  return value_identity(x);
}

proc ok4() {
  var x:int; 
  return ref_identity(x);
}

proc test() {
  ref a = bad();
  var tmpInt = 1;
  ref b = badder(tmpInt);
  ref c = baddest();
  ref d = nested_bad();

  const ref aa = bad_two_args(1, 2.0);
  var aaa = new A(1, 2.0);
  const ref bb = bad_record_arg(aaa);
  const ref cc = bad_return_return_record_arg();

  var arg:int;
  ref r1 = badBar1(arg);
  ref r2 = badBar2(arg);
  writeln(r1);
  writeln(r2);

  var anR:R;
  const ref r3 = anR.badBorrow1(globalR);
  const ref r4 = anR.badBorrow2(globalR);
  writeln(r3);
  writeln(r4);

  ref e = ok1();
  var f = ok2();
  var g = ok3();
  var h = ok4();
}

test();

}
