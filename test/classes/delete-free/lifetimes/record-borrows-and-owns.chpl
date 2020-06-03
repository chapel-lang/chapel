pragma "safe"
module zz {

class MyClass {
  var x:int;
}

record R {
  var _borrowed:borrowed MyClass?;

  pragma "owned"
  var myowned:unmanaged MyClass?;

  proc readOwned() {
    return _to_borrowed(myowned);
  }
}

proc R.deinit() {
  delete myowned;
}

proc makeR(borrow:borrowed MyClass) {
  return new R(borrow, new unmanaged MyClass(10*borrow.x));
}

proc makeR2(borrow:borrowed MyClass) {
  var r:R;
  r._borrowed = borrow;
  r.myowned = new unmanaged MyClass(10*borrow.x);
  return r;
}

proc badMyFun(ref lhs:R, const ref arg:R) {
  var tmp:R;
  tmp = arg;
  tmp.myowned = nil;
  lhs = tmp;
}

proc badF2(borrow:borrowed MyClass) {
  var r = makeR(borrow);
  return r.readOwned();
  // r.myowned destroyed here.
}

proc badF3() {
  var own = new owned MyClass(1);
  var c = own.borrow();
  var r = makeR(c);
  {
    var r2 = makeR(r._borrowed!);
    return r2;
  }
}

proc badF4() {
  var a = new R(nil, new unmanaged MyClass(10));
  return makeR(a.myowned!);
  // a's destructor will delete a.myowned
}
const badHelp = new MyClass();
proc badF6() {
  var a = makeR(badHelp);
  return makeR(a.readOwned()!);
}

config const branch = false;

proc badF8() {
  var a = makeR(badHelp);
  if branch then
    return a;
  else
    return makeR(a.readOwned()!);
}


proc test() {
  var myborrow = new borrowed MyClass(1);
  
  var a = makeR(myborrow);
  var b:R;
  badMyFun(b, a);
  
  var c = makeR2(myborrow);
  var v2 = badF2(myborrow);
  var v3 = badF3();
  var v4 = badF4();
  var v6 = badF6();
  var v8 = badF8();

  writeln(a);
  writeln(b);
  writeln(c);
  writeln(v2);
  writeln(v3);
  writeln(v4);
  writeln(v6);
  writeln(v8);
}

test();
}
