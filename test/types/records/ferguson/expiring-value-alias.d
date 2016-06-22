/* compile with ldc2 expiring.d */

module test;

import std.stdio;


class C {
  int x;
  this(int x_) { x = x_; }
};

int getNum(const C c)
{
  if( c is null ) return -1;
  else return c.x;
}

struct R {
  C c;
  this(int x) {
    c = new C(x);
  }
  this(this) {
    writeln("  copy from ", getNum(c));
    int num = getNum(c);
    c = new C(num);
  }
  ~this() {
    writeln("  delete ", getNum(c));
    //delete c;
  }
  ref R opAssign(ref const R arg)
  {
    writeln("  assign lhs = rhs ", getNum(arg.c));
    c.x = arg.c.x;
    return this;
  }
};

R makeAlias(C c)
{
  writeln("in makeAlias");
  R ret;
  ret.c = c;
  return ret;
}

R returnsR(int x)
{
  //writeln("in returnsR");
  R ret;
  ret.c = new C(x);
  return ret;
}

R transformR(R arg) {
  writeln("in transformR");
  arg.c.x += 1;
  return arg;
}


R globalR;

void test0()
{
  writeln("test0");
  writeln("R r = returnsR(100)");
  {
    R r = returnsR(100);
    writeln("r.x is ", r.c.x);
  }
  writeln("R r = transformR(returnsR(100))");
  {
    R r = transformR(returnsR(100));
    writeln("r.x is ", r.c.x);
  }
  writeln("R r = transformR(transformR(returnsR(100)))");
  {
    R r = transformR(transformR(returnsR(100)));
    writeln("r.x is ", r.c.x);
  }
}

void pass(R r)
{
  writeln("in pass");
}

void passpass(R r)
{
  pass(r);
}


R ret()
{
  R r;
  r.c = new C(5);
  return r;
}

R retret()
{
  return ret();
}

R passret(R r)
{
  return r;
}

R passpassretret(R r)
{
  return passret(r);
}

void test1()
{
  writeln("test1");

  R r;
  r.c = new C(3);

  writeln("created r");
  writeln(getNum(r.c));

  writeln("calling pass");
  pass(r);

  writeln("calling passpass");
  passpass(r);

  writeln("calling ret");
  R r2 = ret();

  writeln("calling retret");
  R r3 = retret();

  writeln("calling pass(ret)");
  pass(ret());

  writeln("calling passret");
  R r4 = passret(r);

  writeln("calling passpassretret");
  R r5 = passpassretret(r);

  writeln("r.x=", getNum(r.c));
  writeln("r2.x=", getNum(r2.c));
  writeln("r3.x=", getNum(r3.c));
  writeln("r4.x=", getNum(r4.c));
  writeln("r5.x=", getNum(r5.c));
}

void test4()
{
  globalR.c.x = 100;
  writeln("test4");
  R local_dom;
  local_dom.c = globalR.c;
  writeln("R curr_dom = local_dom");
  // now local_dom "aliases" globalR
  R curr_dom = local_dom;
  writeln("R next_dom = make");
  R next_dom = returnsR(3);
  //curr_dom.c.x = next_dom.c.x; // could be = overload
  writeln("curr_dom = next_dom");
  curr_dom = next_dom;
  writeln(globalR.c.x);
}


void test5_part2(R curr_dom)
{
  writeln("R next_dom = make");
  R next_dom = returnsR(3);
  writeln("curr_dom = next_dom");
  curr_dom = next_dom;
  writeln(globalR.c.x);
}

void test5()
{
  globalR.c.x = 100;
  writeln("test5");
  R local_dom;
  local_dom.c = globalR.c;
  writeln("R curr_dom = local_dom");
  // now local_dom "aliases" globalR
  test5_part2(local_dom); // does argument passing create a copy?
}

void test6_part2(R curr_dom)
{
  writeln("R next_dom = make");
  R next_dom = returnsR(3);
  writeln("curr_dom = next_dom");
  curr_dom = next_dom;
  writeln(globalR.c.x);
}

void test6()
{
  globalR.c.x = 100;
  writeln("test6");
  R local_dom;
  local_dom.c = globalR.c;
  writeln("R curr_dom = local_dom");
  // now local_dom "aliases" globalR
  test6_part2(makeAlias(globalR.c)); // does argument passing create a copy?
}

/* compilation error b/c can't pass fn return to ref arg 
void test6ref_part2(ref R curr_dom)
{
  writeln("R next_dom = make");
  R next_dom = returnsR(3);
  writeln("curr_dom = next_dom");
  curr_dom.c.x = next_dom.c.x;
  writeln(globalR.c.x);
}

void test6ref()
{
  globalR.c.x = 100;
  writeln("test6ref");
  R local_dom;
  local_dom.c = globalR.c;
  writeln("R curr_dom = local_dom");
  // now local_dom "aliases" globalR
  test6ref_part2(makeAlias(globalR.c)); // does argument passing create a copy?
}
*/

/* compilation error b/c immutable const
void test6in_part2(in R curr_dom)
{
  writeln("R next_dom = make");
  R next_dom = returnsR(3);
  writeln("curr_dom = next_dom");
  curr_dom.c.x = next_dom.c.x;
  writeln(globalR.c.x);
}

void test6in()
{
  globalR.c.x = 100;
  writeln("test6in");
  R local_dom;
  local_dom.c = globalR.c;
  writeln("R curr_dom = local_dom");
  // now local_dom "aliases" globalR
  test6in_part2(makeAlias(globalR.c)); // does argument passing create a copy?
}
*/



int main()
{
  globalR.c = new C(100);

  test0();
  test1();
  test4();
  test5();
  test6();

  return 0;
}

