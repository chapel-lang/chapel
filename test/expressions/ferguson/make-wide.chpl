use CPtr;
use CTypes;

config const debug = false;

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
test();

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
test2();

// test from issue 18511
record WidePtr {
  var ptr: c_void_ptr;
  var loc: chpl_localeID_t;
}

class Foo {
  var bar: int;
}

proc create(ref widePtr: WidePtr, bar: int) {
  var foo = new unmanaged Foo(bar);
  widePtr.ptr = __primitive("_wide_get_addr", foo);
  widePtr.loc = __primitive("_wide_get_locale", foo);
  if debug {
    writeln("create foo is ", foo:c_void_ptr, " and ptr is ", widePtr.ptr);
  }
}

proc toFoo(ref widePtr: WidePtr) {
  var ret = __primitive("_wide_make", unmanaged Foo, widePtr.loc, widePtr.ptr);
  if debug {
    writeln("toFoo returning ", ret:c_void_ptr);
  }
  return ret;
}
proc toFooWorkaround(ref widePtr: WidePtr) {
  var loc = widePtr.loc;
  var ptr = widePtr.ptr;
  var ret = __primitive("_wide_make", unmanaged Foo, loc, ptr);
  if debug {
    writeln("toFoo returning ", ret:c_void_ptr);
  }
  return ret;
}

proc destroy(ref widePtr: WidePtr) {
  var foo = toFoo(widePtr);
  if debug {
    writeln("destroy ptr is ", widePtr.ptr, " and foo is ", foo:c_void_ptr);
  }
  delete foo;
}

proc test3() {
  var ptr: WidePtr;
  create(ptr, 1);
  var foo = toFoo(ptr);
  writeln(foo);
  destroy(ptr);
}
test3();

proc test4() {
  var ptr: WidePtr;
  create(ptr, 1);
  var foo = toFooWorkaround(ptr);
  writeln(foo);
  destroy(ptr);
}
test4();
