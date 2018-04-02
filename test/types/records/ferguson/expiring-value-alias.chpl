config const debug = false;

class C {
  var x:int;
}

proc getNum(c:C)
{
  if c == nil then return -1;
  else return c.x;
}

record R {
  var c:C;
}


proc addOne(r:R) {
  r.c.x += 1;
}

proc makeAlias(c:C)
{
  if debug then writeln("in makeAlias");
  return new R(c = c);
}

proc returnsR(x:int) {
  if debug then writeln("in returnsR");
  var ret: R;
  ret.c = new C(x);
  return ret;
}

proc transformR(arg:R) {
  if debug then writeln("in transformR");
  arg.c.x += 1;
  return arg;
}

var globalR = returnsR(100);

proc test0()
{
  writeln("test0");
  if debug then writeln("R r = returnsR(100)");
  {
    // 2016-01 Chapel calls a copy, but neither C++11 nor D do
    var r = returnsR(100);
    writeln("r.x is ", r.c.x);
  }
  if debug then writeln("R r = transformR(returnsR(100))");
  {
    // 2016-01 Chapel makes 2 copies, D makes 1, C++11 makes 0
    var r = transformR(returnsR(100));
    writeln("r.x is ", r.c.x);
  }
  if debug then writeln("R r = transformR(transformR(returnsR(100)))");
  {
    // 2016-01 Chapel makes 3 copies, D makes 2, C++11 makes 0
    var r = transformR(transformR(returnsR(100)));
    writeln("r.x is ", r.c.x);
  }
}

proc pass(r:R) {
  if debug then writeln("in pass");
}

proc passpass(r:R) {
  pass(r);
}

proc ret() {
  var r:R;
  r.c = new C(5);
  return r;
}

proc retret() {
  return ret();
}

proc passret(r:R)
{
  return r;
}

proc passpassretret(r:R)
{
  return passret(r);
}


proc test1() {
  writeln("test1");
  var r:R;
  r.c = new C(3);

  if debug then writeln("created r");
  writeln(getNum(r.c));

  if debug then writeln("calling pass");
  // 2016-01 D and C++11 each make a copy here, Chapel makes 0
  // (because of pass by ref, you could do that in C++/D too)
  pass(r);

  if debug then writeln("calling passpass");
  // 2016-01 D and C++11 each make 2 copies here, Chapel makes 0
  // (because of pass by ref, you could do that in C++/D too)
  passpass(r);

  if debug then writeln("calling ret()");
  // 2016-01 Chapel calls a copy, but neither C++11 nor D do
  var r2 = ret();

  if debug then writeln("calling retret()");
  // 2016-01 Chapel makes 2 copies, but D and C++11 make 0
  var r3 = retret();

  if debug then writeln("calling pass(ret)");
  // 2016-01 Chapel makes 1 copies, but D and C++11 make 0
  pass(ret());

  if debug then writeln("calling passret");
  // 2016-01 Chapel makes 1 copy, D makes 2, C++11 makes 1
  var r4 = passret(r);

  if debug then writeln("calling passpassretret");
  // 2016-01 Chapel makes 2 copies, D makes 3, C++11 makes 2
  var r5 = passpassretret(r);

  writeln("r.x=", getNum(r.c));
  writeln("r2.x=", getNum(r2.c));
  writeln("r3.x=", getNum(r3.c));
  writeln("r4.x=", getNum(r4.c));
  writeln("r5.x=", getNum(r5.c));
}


proc getGlobalR() ref {
  return globalR;
}
proc getAliasGlobalR() {
  return new R(c=globalR.c);
}

proc test2() {
  globalR.c.x = 100;
  writeln("test2");
  var local_dom = getAliasGlobalR();
  var curr_dom = local_dom;
  var next_dom = returnsR(3);
  curr_dom = next_dom;
  writeln(globalR);
}


proc test3() {
  globalR.c.x = 100;
  writeln("test3");
  var local_dom:R;
  local_dom.c = globalR.c;
  var curr_dom = local_dom;
  var next_dom = returnsR(3);
  curr_dom = next_dom;
  writeln(globalR); // Chapel, C++11, D print 100
}
proc test4() {
  globalR.c.x = 100;
  writeln("test4");
  var local_dom:R;
  local_dom.c = globalR.c;
  var curr_dom = local_dom;
  var next_dom = returnsR(3);
  curr_dom.c.x = next_dom.c.x;
  writeln(globalR); // Chapel, C++11, D print 100
}

proc test5_part2(curr_dom: R)
{
  if debug then writeln("var next_dom = make");
  var next_dom = returnsR(3);
  if debug then writeln("curr_dom = next_dom");
  curr_dom.c.x = next_dom.c.x;
  writeln(globalR.c.x);
}

proc test5()
{
  globalR.c.x = 100;
  writeln("test5");
  var local_dom:R;
  local_dom.c = globalR.c;
  if debug then writeln("R curr_dom = local_dom");
  // now local_dom "aliases" globalR
  test5_part2(local_dom); // does argument passing create a copy?
  // Because records pass by const ref (default intent),
  // the answer is no (prints 3)
  // It would be yes for D and C++11, since these are pass-by-value
  // (prints 100) - but a better comparison is test5in
}

proc test5ref_part2(ref curr_dom: R)
{
  if debug then writeln("var next_dom = make");
  var next_dom = returnsR(3);
  if debug then writeln("curr_dom = next_dom");
  curr_dom.c.x = next_dom.c.x;
  writeln(globalR.c.x);
}

proc test5ref()
{
  globalR.c.x = 100;
  writeln("test5ref");
  var local_dom:R;
  local_dom.c = globalR.c;
  if debug then writeln("R curr_dom = local_dom");
  // now local_dom "aliases" globalR
  test5ref_part2(local_dom); // does argument passing create a copy?
  // C++11 and Chapel both do not make a copy (prints 3)
  // This pattern is not possible to write in D
}

proc test5in_part2(in curr_dom: R)
{
  if debug then writeln("var next_dom = make");
  var next_dom = returnsR(3);
  if debug then writeln("curr_dom = next_dom");
  curr_dom.c.x = next_dom.c.x;
  writeln(globalR.c.x);
}

proc test5in()
{
  globalR.c.x = 100;
  writeln("test5in");
  var local_dom:R;
  local_dom.c = globalR.c;
  if debug then writeln("R curr_dom = local_dom");
  // now local_dom "aliases" globalR
  test5in_part2(local_dom); // does argument passing create a copy?
  // The answer for Chapel is yes with in intent (prints 100).
  // It is also yes for D and C++11 with the default of pass-by-value.
}


proc test6_part2(curr_dom:R)
{
  if debug then writeln("var next_dom = make");
  var next_dom = returnsR(3);
  if debug then writeln("curr_dom = next_dom");
  curr_dom.c.x = next_dom.c.x;
  writeln(globalR.c.x);
}

proc test6()
{
  globalR.c.x = 100;
  writeln("test6");
  var local_dom:R;
  local_dom.c = globalR.c;
  if debug then writeln("R curr_dom = local_dom");
  // now local_dom "aliases" globalR
  test6_part2(makeAlias(globalR.c)); // does argument passing create a copy?
  // 2016-01 - Chapel creates a copy here, but C++11 does not,
  // although the default argument passing in Chapel corresponds
  // to 'const ref' in C++.
}

proc test6ref_part2(const ref curr_dom:R)
{
  if debug then writeln("var next_dom = make");
  var next_dom = returnsR(3);
  if debug then writeln("curr_dom = next_dom");
  curr_dom.c.x = next_dom.c.x;
  writeln(globalR.c.x);
}

proc test6ref()
{
  globalR.c.x = 100;
  writeln("test6ref");
  var local_dom:R;
  local_dom.c = globalR.c;
  if debug then writeln("R curr_dom = local_dom");
  // now local_dom "aliases" globalR
  test6ref_part2(makeAlias(globalR.c)); // does argument passing create a copy?
  // 2016-01 - Chapel creates a copy here, but C++11 does not
  // (when both are using a const ref argument).
}

proc test6in_part2(in curr_dom:R)
{
  if debug then writeln("var next_dom = make");
  var next_dom = returnsR(3);
  if debug then writeln("curr_dom = next_dom");
  curr_dom = next_dom;
  writeln(globalR.c.x);
}

proc test6in()
{
  globalR.c.x = 100;
  writeln("test6in");
  var local_dom:R;
  local_dom.c = globalR.c;
  if debug then writeln("R curr_dom = local_dom");
  // now local_dom "aliases" globalR
  test6in_part2(makeAlias(globalR.c)); // does argument passing create a copy?
  // 2016-01 - Chapel creates a copy here, but D and C++11 do not.
  // The default argument passing for D and C++11 corresponds to in.
}

test0();
test1();
test2();
test3();
test4();
test5();
test5ref();
test5in();
test6();
test6ref();
test6in();


proc R.deinit() {
  if debug then writeln("  delete ", getNum(c));
}

pragma "donor fn"
pragma "auto copy fn"
proc chpl__autoCopy(arg: R) {
  if debug then writeln("  auto copy from ", getNum(arg.c));
  // Note -- this auto copy could be the same as
  // init copy and it wouldn't affect the test.

  // TODO - is no auto destroy necessary here?
  pragma "no auto destroy"
  var ret: R;

  //ret.c = arg.c;
  ret.c = new C(arg.c.x);

  return ret;
}

// I'd like this to be ref, but that breaks
//    var outerX: R; begin { var x = outerX; }
pragma "init copy fn"
proc chpl__initCopy(arg: R) {
  if debug then writeln("  init copy from ", getNum(arg.c));

  var ret: R;

  ret.c = new C(arg.c.x);

  return ret;
}

proc =(ref lhs: R, rhs: R) {
  if debug then writeln("  assign lhs = rhs ", getNum(rhs.c));

  lhs.c.x = rhs.c.x;
}


