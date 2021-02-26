use CPtr;

extern proc mytest(x: c_void_ptr):c_void_ptr;

proc mytest2(x: c_void_ptr):c_void_ptr
{
  return x;
}

proc mytest3(x: c_void_ptr):c_void_ptr
{
  return mytest(x);
}

proc mytest4(x: c_void_ptr):c_void_ptr
{
  var y:c_void_ptr;
  return y;
}

extern proc printf(fmt:c_string, ptr:c_void_ptr);
{
  var x:c_void_ptr;
  x = mytest(x);
  printf("%p\n", x);
}

{
  var x:c_void_ptr;
  x = mytest(x);
  x = mytest2(x);
  printf("%p\n", x);
}

{
  var x:c_void_ptr;
  x = mytest(x);
  x = mytest3(x);
  printf("%p\n", x);
}

{
  var x:c_void_ptr;
  x = mytest(x);
  x = mytest4(x);
  x = mytest(x);
  printf("%p\n", x);
}


