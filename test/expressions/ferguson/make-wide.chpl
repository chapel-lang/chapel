class C {
  var x: int;
}

proc test() {

  var c:unmanaged C?;
  
  on Locales[numLocales-1] {
    c = new unmanaged C(1);
  }

  var cc:unmanaged C? = c;

  var loc = __primitive("_wide_get_locale", cc);
  var adr = __primitive("_wide_get_addr", cc);

  var b = __primitive("_wide_make", unmanaged C, loc, adr);

  assert( b == c );

  delete c;
}

proc test2() {
  var own:owned C?;
  var c:borrowed C?;
  
  on Locales[numLocales-1] {
    own = new owned C(1);
    c = own:borrowed C;
  }

  var cc:borrowed C? = c;

  var loc = __primitive("_wide_get_locale", cc);
  var adr = __primitive("_wide_get_addr", cc);

  var b = __primitive("_wide_make", borrowed C, loc, adr);
  var loc2 = __primitive("_wide_get_locale", b);
  var adr2 = __primitive("_wide_get_addr", b);

  assert( b == c );
  assert( loc == loc2 );
  assert( adr == adr2 );
}

test();
test2();
