use CTypes;

extern proc mytest(x: c_ptr(void)):c_ptr(void);

proc mytest2(x: c_ptr(void)):c_ptr(void)
{
  return x;
}

proc mytest3(x: c_ptr(void)):c_ptr(void)
{
  return mytest(x);
}

proc mytest4(x: c_ptr(void)):c_ptr(void)
{
  var y:c_ptr(void);
  return y;
}

extern proc printf(fmt:c_ptrConst(c_char), ptr:c_ptr(void));
{
  var x:c_ptr(void);
  x = mytest(x);
  printf("%p\n", x);
}

{
  var x:c_ptr(void);
  x = mytest(x);
  x = mytest2(x);
  printf("%p\n", x);
}

{
  var x:c_ptr(void);
  x = mytest(x);
  x = mytest3(x);
  printf("%p\n", x);
}

{
  var x:c_ptr(void);
  x = mytest(x);
  x = mytest4(x);
  x = mytest(x);
  printf("%p\n", x);
}


