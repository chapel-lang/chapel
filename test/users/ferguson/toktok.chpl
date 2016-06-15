

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

  return tok;
}
proc c() {
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
  var f = foo();
  
  assert(aa == bb);
  assert(bb == cc);

  assert(aa != f);
}

bar();
