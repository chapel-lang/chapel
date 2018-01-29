pragma "safe"
module zz {

class MyClass {
  var x:int;
}

record R {
  var borrowed:MyClass;

  pragma "owned"
  var owned:MyClass;

  proc readOwned() {
    return owned;
  }
}

proc R.deinit() {
  delete owned;
}

proc makeR(borrow:MyClass) {
  return new R(borrow, new MyClass(10*borrow.x));
}

proc makeR2(borrow:MyClass) {
  var r:R;
  r.borrowed = borrow;
  r.owned = new MyClass(10*borrow.x);
  return r;
}

proc myfun(ref lhs:R, const ref arg:R) {
  var tmp:R;
  tmp = arg;
  tmp.owned = nil;
  lhs = tmp;
}

proc badF1(borrow:MyClass) {
  var r = makeR(borrow);
  return r.owned;
  // r.owned destroyed here.
}

proc badF2(borrow:MyClass) {
  var r = makeR(borrow);
  return r.readOwned();
  // r.owned destroyed here.
}

config const branch = false;
proc g() {
  var a = makeR(nil);
  if branch then
    return a;
  else
    return makeR(a.owned);
}

proc h() {
  var a = makeR(nil);
  if branch then
    return a;
  else
    return makeR(a.readOwned());
}


proc test() {
  var c = new MyClass(1);
  
  var v1 = makeR(c);
  var v2:R;
  myfun(v2, v1);
  
  var v3 = badF1(c);
  var v4 = makeR2(c);
  var v5 = g();
  var v6 = h();
  var v7 = badF2(c);
 
  writeln(v1);
  writeln(v2);
  writeln(v3);
  writeln(v4);
  writeln(v5);
  writeln(v6);
  writeln(v7);
}

test();
}
