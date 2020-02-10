config const debug = false;

class C {
  var x:int;
}

proc getNum(c:borrowed C?)
{
  if c == nil then return -1;
  else return c!.x;
}

record R {
  var c:unmanaged C?;
  var isowned: bool;
}

proc R.deinit() {
  if debug then writeln("  delete ", getNum(c));
  if isowned then
    delete c;
}

proc R.init() {
  this.c = nil;
  this.isowned = true;
}

proc R.init(aliasing:unmanaged C?) {
  this.c = aliasing;
  this.isowned = false;
}

proc R.init=(other:R) {
  if debug then writeln("  R.init=(R) from ", getNum(other.c));
  this.c = new unmanaged C(other.c!.x);
  this.isowned = true;
}

proc =(ref lhs: R, rhs: R) {
  if debug then writeln("  assign lhs = rhs ", getNum(rhs.c));

  lhs.c!.x = rhs.c!.x;
}

proc addOne(r:R) {
  r.c.x += 1;
}

proc makeAlias(c:unmanaged C?)
{
  if debug then writeln("in makeAlias");
  return new R(aliasing=c);
}

proc returnsR(x:int) {
  if debug then writeln("in returnsR");
  var ret: R;
  ret.c = new unmanaged C(x);
  ret.isowned = true;
  return ret;
}

proc transformR(arg:R) {
  if debug then writeln("in transformR");
  arg.c!.x += 1;
  return arg;
}

var globalR = returnsR(100);

proc test0()
{
  writeln("test0");
  if debug then writeln("R r = returnsR(100)");
  {
    // 2019-03 Neither Chapel, C++11, nor D call copy for this pattern
    var r = returnsR(100);
    writeln("r.x is ", r.c!.x);
  }
  if debug then writeln("R r = transformR(returnsR(100))");
  {
    // 2019-03 Chapel makes 1 copies, D makes 1, C++11 makes 0
    var r = transformR(returnsR(100));
    writeln("r.x is ", r.c!.x);
  }
  if debug then writeln("R r = transformR(transformR(returnsR(100)))");
  {
    // 2019-03 Chapel makes 2 copies, D makes 2, C++11 makes 0
    var r = transformR(transformR(returnsR(100)));
    writeln("r.x is ", r.c!.x);
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
  r.c = new unmanaged C(5);
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
  r.c = new unmanaged C(3);

  if debug then writeln("created r");
  writeln(getNum(r.c));

  if debug then writeln("calling pass");
  // 2019-03 D and C++11 each make a copy here, Chapel makes 0
  // (because of pass by ref, you could do that in C++/D too)
  pass(r);

  if debug then writeln("calling passpass");
  // 2019-03 D and C++11 each make 2 copies here, Chapel makes 0
  // (because of pass by ref, you could do that in C++/D too)
  passpass(r);

  if debug then writeln("calling ret()");
  // 2019-03 Neither Chapel, C++11, nor D call copy here
  var r2 = ret();

  if debug then writeln("calling retret()");
  // 2019-03 Neither Chapel, C++11, nor D make a copy here
  var r3 = retret();

  if debug then writeln("calling pass(ret)");
  // 2019-03 Neither Chapel, C++11, nor D make a copy here
  pass(ret());

  if debug then writeln("calling passret");
  // 2019-03 Chapel makes 1 copy, D makes 2, C++11 makes 1
  var r4 = passret(r);

  if debug then writeln("calling passpassretret");
  // 2019-03 Chapel makes 1 copy, D makes 3, C++11 makes 2
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
  return new R(aliasing=globalR.c);
}

proc test2() {
  globalR.c!.x = 100;
  writeln("test2");
  var local_dom = getAliasGlobalR();
  var curr_dom = local_dom;
  var next_dom = returnsR(3);
  curr_dom = next_dom;
  writeln(globalR.c!.x);
}


proc test3() {
  globalR.c!.x = 100;
  writeln("test3");
  var local_dom:R;
  local_dom.c = globalR.c;
  local_dom.isowned = false;
  var curr_dom = local_dom;
  var next_dom = returnsR(3);
  curr_dom = next_dom;
  writeln(globalR.c!.x); // C++11, D print 100
}
proc test4() {
  globalR.c!.x = 100;
  writeln("test4");
  var local_dom:R;
  local_dom.c = globalR.c;
  local_dom.isowned = false;
  var curr_dom = local_dom;
  var next_dom = returnsR(3);
  curr_dom.c!.x = next_dom.c!.x;
  writeln(globalR.c!.x); // C++11, D print 100
}

proc test5_part2(curr_dom: R)
{
  if debug then writeln("var next_dom = make");
  var next_dom = returnsR(3);
  if debug then writeln("curr_dom = next_dom");
  curr_dom.c!.x = next_dom.c!.x;
  writeln(globalR.c!.x);
}

proc test5()
{
  globalR.c!.x = 100;
  writeln("test5");
  var local_dom:R;
  local_dom.c = globalR.c;
  local_dom.isowned = false;
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
  curr_dom.c!.x = next_dom.c!.x;
  writeln(globalR.c!.x);
}

proc test5ref()
{
  globalR.c!.x = 100;
  writeln("test5ref");
  var local_dom:R;
  local_dom.c = globalR.c;
  local_dom.isowned = false;
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
  curr_dom.c!.x = next_dom.c!.x;
  writeln(globalR.c!.x);
}

proc test5in()
{
  globalR.c!.x = 100;
  writeln("test5in");
  var local_dom:R;
  local_dom.c = globalR.c;
  local_dom.isowned = false;
  if debug then writeln("R curr_dom = local_dom");
  // now local_dom "aliases" globalR
  test5in_part2(local_dom); // does argument passing create a copy?
  // D and C++ both make a copy with the default of pass-by-value
}


proc test6_part2(curr_dom:R)
{
  if debug then writeln("var next_dom = make");
  var next_dom = returnsR(3);
  if debug then writeln("curr_dom = next_dom");
  curr_dom.c!.x = next_dom.c!.x;
  writeln(globalR.c!.x);
}

proc test6()
{
  globalR.c!.x = 100;
  writeln("test6");
  var local_dom:R;
  local_dom.c = globalR.c;
  local_dom.isowned = false;
  if debug then writeln("R curr_dom = local_dom");
  // now local_dom "aliases" globalR
  test6_part2(makeAlias(globalR.c)); // does argument passing create a copy?
  // 2019-03 - Chapel and C++ do not make a copy here
  // (note that the the default argument passing in Chapel corresponds
  // to 'const ref' in C++).
}

proc test6ref_part2(const ref curr_dom:R)
{
  if debug then writeln("var next_dom = make");
  var next_dom = returnsR(3);
  if debug then writeln("curr_dom = next_dom");
  curr_dom.c!.x = next_dom.c!.x;
  writeln(globalR.c!.x);
}

proc test6ref()
{
  globalR.c!.x = 100;
  writeln("test6ref");
  var local_dom:R;
  local_dom.c = globalR.c;
  local_dom.isowned = false;
  if debug then writeln("R curr_dom = local_dom");
  // now local_dom "aliases" globalR
  test6ref_part2(makeAlias(globalR.c)); // does argument passing create a copy?
  // 2019-03 - Chapel and C++ do not create a copy here
  // (when both are using a const ref argument).
}

proc test6in_part2(in curr_dom:R)
{
  if debug then writeln("var next_dom = make");
  var next_dom = returnsR(3);
  if debug then writeln("curr_dom = next_dom");
  curr_dom = next_dom;
  writeln(globalR.c!.x);
}

proc test6in()
{
  globalR.c!.x = 100;
  writeln("test6in");
  var local_dom:R;
  local_dom.c = globalR.c;
  local_dom.isowned = false;
  if debug then writeln("R curr_dom = local_dom");
  // now local_dom "aliases" globalR
  test6in_part2(makeAlias(globalR.c)); // does argument passing create a copy?
  // 2019-03 - Chapel and C++ do not create a copy here.
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

