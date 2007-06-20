class C {
  var jump = 0;
  var result = 0;
  var i = 0;
}

def next_foo(c : C) : C {
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

def foo() {
  var c = next_foo(C());
  var s : list of c.result;
  while c != nil {
    s.append(c.result);
    c = next_foo(c);
  }
  return s;
}

writeln(foo());

def bar() : int {
  var i = 1;
  while i < 5 {
    yield i;
    i += 1;
  }
}

writeln( bar());
