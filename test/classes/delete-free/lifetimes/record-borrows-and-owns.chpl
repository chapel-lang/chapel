pragma "safe"
module zz {

pragma "use default init"
class MyClass {
  var x:int;
}

pragma "use default init"
record R {
  var _borrowed:MyClass;

  pragma "owned"
  var myowned:MyClass;

  proc readOwned() {
    return myowned;
  }
}

proc R.deinit() {
  delete myowned;
}

proc makeR(borrow:MyClass) {
  return new R(borrow, new MyClass(10*borrow.x));
}

proc makeR2(borrow:MyClass) {
  var r:R;
  r._borrowed = borrow;
  r.myowned = new MyClass(10*borrow.x);
  return r;
}

proc badMyFun(ref lhs:R, const ref arg:R) {
  var tmp:R;
  tmp = arg;
  tmp.myowned = nil;
  lhs = tmp;
}

proc badF1(borrow:MyClass) {
  var r = makeR(borrow);
  return r.myowned;
  // r.myowned destroyed here.
}

proc badF2(borrow:MyClass) {
  var r = makeR(borrow);
  return r.readOwned();
  // r.myowned destroyed here.
}

proc badF3() {
  var own = new owned MyClass(1);
  var c = own.borrow();
  var r = makeR(c);
  {
    var r2 = makeR(r._borrowed);
    return r2;
  }
}

proc badF4() {
  var a = new R(nil, new MyClass(10));
  return makeR(a.myowned);
  // a's destructor will delete a.myowned
}

proc badF5() {
  var a = makeR(nil);
  return makeR(a.myowned);
}

proc badF6() {
  var a = makeR(nil);
  return makeR(a.readOwned());
}

config const branch = false;
proc badF7() {
  var a = makeR(nil);
  if branch then
    return a;
  else
    return makeR(a.myowned);
}

proc badF8() {
  var a = makeR(nil);
  if branch then
    return a;
  else
    return makeR(a.readOwned());
}


proc test() {
  var myborrow = new MyClass(1);
  
  var a = makeR(myborrow);
  var b:R;
  badMyFun(b, a);
  
  var v1 = badF1(myborrow);
  var c = makeR2(myborrow);
  var v2 = badF2(myborrow);
  var v3 = badF3();
  var v4 = badF4();
  var v5 = badF5();
  var v6 = badF6();
  var v7 = badF7();
  var v8 = badF8();

  writeln(a);
  writeln(b);
  writeln(c);
  writeln(v1);
  writeln(v2);
  writeln(v3);
  writeln(v4);
  writeln(v5);
  writeln(v6);
  writeln(v7);
  writeln(v8);
}

test();
}
