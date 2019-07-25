class C {
  var jump : int = 0;
  var result : int;
  var i : int;
}

/*
proc next_foo(c : C) : C {
  if c.jump == 0 then
    goto _0;
  else if c.jump == 1 then
    goto _1;
label _0
  c.i = 1;
  while c.i < 5 {
    c.result = c.i;
    c.jump = 1;
    return c;
label _1
    c.i += 1;
  }
  return nil;
}
*/

proc next_foo(c : unmanaged C) : unmanaged C? {
  if c.jump == 0 {
    c.i = 1;
    if c.i < 5 {
      c.result = c.i;
      c.jump = 1;
      return c;
    }
  } else if c.jump == 1 {
    c.i += 1;
    if c.i < 5 {
      c.result = c.i;
      c.jump = 1;
      return c;
    }
  }
  return nil;
}

iter bar() : int {
  var i = 1;
  while i < 5 {
    yield i;
    i += 1;
  }
}

