class C {
  var x: int;
}

proc test() {

  var c:C;
  
  on Locales[numLocales-1] {
    c = new C(1);
  }

  var loc = __primitive("_wide_get_locale", c);
  var adr = __primitive("_wide_get_addr", c);

  var b = __primitive("_wide_make", C, loc, adr);

  assert( b == c );

  delete c;
}

proc test2() {
  var c:C;
  
  on Locales[numLocales-1] {
    c = new C(1);
  }

  var loc = __primitive("_wide_get_locale", c);
  var adr = __primitive("_wide_get_addr", c);

  var b = __primitive("_wide_make", C, loc, adr);
  var loc2 = __primitive("_wide_get_locale", b);
  var adr2 = __primitive("_wide_get_addr", b);

  assert( b == c );
  assert( loc == loc2 );
  assert( adr == adr2 );

  delete c;
}

test();
test2();
