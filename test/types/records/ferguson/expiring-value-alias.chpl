config const debug = false;

class C {
  var x:int;
}

record R {
  var c:C;
}


proc addOne(r:R) {
  r.c.x += 1;
}

proc makeR(x:int) {
  var ret: R;
  ret.c = new C(x);
  return ret;
}

proc identity(arg:R) {
  return arg;
}

proc test1() {
  writeln("test1");
  var r1 = makeR(1);
  var r2 = makeR(2);
  r2 = r1;
  addOne(r2);
  writeln(r1);
  writeln(r2);
}

test1();

var globalR = makeR(100);
proc getGlobalR() ref {
  return globalR;
}
proc getAliasGlobalR() {
  return new R(c=globalR.c);
}

proc test2() {
  writeln("test2");
  var local_dom = getAliasGlobalR();
  var curr_dom = local_dom;
  var next_dom = makeR(3);
  curr_dom = next_dom;
  writeln(globalR);
}

test2();


proc test3() {
  writeln("test3");
  var local_dom:R;
  local_dom.c = globalR.c;
  var curr_dom = local_dom;
  var next_dom = makeR(3);
  curr_dom = next_dom;
  writeln(globalR);
}

test3();

proc test4() {
  writeln("test4");
  var local_dom:R;
  local_dom.c = globalR.c;
  var curr_dom = local_dom;
  var next_dom = makeR(3);
  curr_dom.c.x = next_dom.c.x;
  writeln(globalR);
}

test4();


proc R.~R() {
  if debug then writeln("In record destructor");
}

// We'd like this to be by ref, but doing so leads to an internal
// compiler error.  See
// $CHPL_HOME/test/types/records/sungeun/recordWithRefCopyFns.future
pragma "donor fn"
pragma "auto copy fn"
proc chpl__autoCopy(arg: R) {
  // TODO - is no auto destroy necessary here?
  pragma "no auto destroy"
  var ret: R;

  ret.c = arg.c;

  if debug {
    writeln("leaving auto copy");
  }

  return ret;
}

// I'd like this to be ref, but that breaks
//    var outerX: R; begin { var x = outerX; }
pragma "init copy fn"
proc chpl__initCopy(arg: R) {
  var ret: R;

  ret.c = new C(arg.c.x);

  if debug {
    writeln("leaving init copy");
  }

  return ret;
}

proc =(ref lhs: R, rhs: R) {
  lhs.c.x = rhs.c.x;

  if debug {
    writeln("leaving assign");
  }
}


