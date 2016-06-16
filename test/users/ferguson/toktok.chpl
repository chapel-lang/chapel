
// Works around "self-comparison always evaluates to true" C comp error
proc is_same(a: c_void_ptr, b: c_void_ptr)
{
  if a == b then return true;
  else return false;
}

proc a() {
  var tok = __primitive("get caller stack token");
/*
  extern proc printf(fmt:c_string, arg);
  printf("tok: %p\n", tok);
  */
  return tok;
}
proc b() {
  var tok = __primitive("get caller stack token");
  var tok2 = __primitive("get caller stack token");

  assert(is_same(tok, tok2));

  return tok;
}
proc c() {
  var tok = __primitive("get caller stack token");

  return tok;
}
inline
proc d() {
  var tok = __primitive("get caller stack token");

  return tok;
}


proc foo() {
  return a();
}

proc bar() {
  var aa = a();
  var bb = b();
  var cc = c();
  var dd = d();
  var f = foo();
  
  assert(is_same(aa, bb));
  assert(is_same(bb, cc));
  assert(is_same(cc, dd));

  assert(!is_same(aa, f));
}

bar();
