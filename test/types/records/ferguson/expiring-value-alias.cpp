// compile with e.g. g++ --std=c++11

#include <stdio.h>

struct C {
  int x;
  C(int x_) : x(x_) { }  
};

int getNum(C* c)
{
  if( c == NULL ) return -1;
  else return c->x;
}

struct R {
  C* c;
  R() : c(NULL) { }
  R(int x) {
    this->c = new C(x);
  }
  R(const R& o) {
    printf("  copy from %i\n", getNum(o.c));
    this->c = new C(getNum(o.c));
  }
  R(R&& o) noexcept {
    printf("  move from %i\n", getNum(o.c));
    this->c = o.c;
    o.c = NULL;
  }
  ~R() {
    printf("  delete %i\n", getNum(this->c));
    //delete c;
  }
  R& operator=(const R& arg)
  {
    printf("  assign lhs = rhs %i\n", getNum(arg.c));
    this->c->x = arg.c->x;
    return *this;
  }
};

R makeAlias(C* c)
{
  printf("in makeAlias\n");
  R ret;
  ret.c = c;
  return ret;
}


R returnsR(int x)
{
  printf("in returnsR\n");
  R ret;
  ret.c = new C(x);
  return ret;
}

R transformR(R arg) {
  printf("in transformR\n");
  arg.c->x += 1;
  return arg;
}


R globalR;

void test0()
{
  printf("test0\n");
  printf("R r = returnsR(100)\n");
  {
    R r = returnsR(100);
    printf("r.x is %i\n", r.c->x);
  }
  printf("R r = transformR(returnsR(100))\n");
  {
    R r = transformR(returnsR(100));
    printf("r.x is %i\n", r.c->x);
  }
  printf("R r = transformR(transformR(returnsR(100)))\n");
  {
    R r = transformR(transformR(returnsR(100)));
    printf("r.x is %i\n", r.c->x);
  }
}

void pass(R r)
{
  printf("in pass\n");
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
  printf("test1\n");
  R r;
  r.c = new C(3);

  printf("created r\n");
  printf("%i\n", getNum(r.c));

  printf("calling pass\n");
  pass(r);

  printf("calling passpass\n");
  passpass(r);

  printf("calling ret\n");
  R r2 = ret();

  printf("calling retret\n");
  R r3 = retret();

  printf("calling pass(ret)\n");
  pass(ret());

  printf("calling passret\n");
  R r4 = passret(r);

  printf("calling passpassretret\n");
  R r5 = passpassretret(r);

  printf("r.x=%i\n", getNum(r.c));
  printf("r2.x=%i\n", getNum(r2.c));
  printf("r3.x=%i\n", getNum(r3.c));
  printf("r4.x=%i\n", getNum(r4.c));
  printf("r5.x=%i\n", getNum(r5.c));

}


void test4()
{
  globalR.c->x = 100;
  printf("test4\n");
  R local_dom;
  local_dom.c = globalR.c;
  printf("R curr_dom = local_dom\n");
  // now local_dom "aliases" globalR
  R curr_dom = local_dom;
  printf("R next_dom = make\n");
  R next_dom = returnsR(3);
  //curr_dom.c->x = next_dom.c->x; // could be = overload
  printf("curr_dom = next_dom\n");
  curr_dom = next_dom;
  printf("%i\n", globalR.c->x);
}

void test5_part2(R curr_dom)
{
  printf("R next_dom = make\n");
  R next_dom = returnsR(3);
  printf("curr_dom = next_dom\n");
  curr_dom = next_dom;
  printf("%i\n", globalR.c->x);
}

void test5()
{
  globalR.c->x = 100;
  printf("test5\n");
  R local_dom;
  local_dom.c = globalR.c;
  printf("R curr_dom = local_dom\n");
  // now local_dom "aliases" globalR
  test5_part2(local_dom); // does argument passing create a copy?
}

void test5ref_part2(const R& curr_dom)
{
  printf("R next_dom = make\n");
  R next_dom = returnsR(3);
  printf("curr_dom = next_dom\n");
  curr_dom.c->x = next_dom.c->x;
  printf("%i\n", globalR.c->x);
}

void test5ref()
{
  globalR.c->x = 100;
  printf("test5ref\n");
  R local_dom;
  local_dom.c = globalR.c;
  printf("R curr_dom = local_dom\n");
  // now local_dom "aliases" globalR
  test5ref_part2(local_dom); // does argument passing create a copy?
}


void test6_part2(R curr_dom)
{
  printf("R next_dom = make\n");
  R next_dom = returnsR(3);
  printf("curr_dom = next_dom\n");
  curr_dom = next_dom;
  printf("%i\n", globalR.c->x);
}

void test6()
{
  globalR.c->x = 100;
  printf("test6\n");
  R local_dom;
  local_dom.c = globalR.c;
  printf("R curr_dom = local_dom\n");
  // now local_dom "aliases" globalR
  test6_part2(makeAlias(globalR.c)); // does argument passing create a copy?
}

void test6ref_part2(const R& curr_dom)
{
  printf("R next_dom = make\n");
  R next_dom = returnsR(3);
  printf("curr_dom = next_dom\n");
  curr_dom.c->x = next_dom.c->x;
  printf("%i\n", globalR.c->x);
}

void test6ref()
{
  globalR.c->x = 100;
  printf("test6ref\n");
  R local_dom;
  local_dom.c = globalR.c;
  printf("R curr_dom = local_dom\n");
  // now local_dom "aliases" globalR
  test6ref_part2(makeAlias(globalR.c)); // does argument passing create a copy?
}



int main(int argc, char** argv)
{
  globalR.c = new C(100);

  test0();
  test1();
  test4();
  test5();
  test5ref();
  test6();
  test6ref();

  return 0;
}


