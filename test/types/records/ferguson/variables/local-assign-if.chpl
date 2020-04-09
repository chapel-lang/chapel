use myrecord;

proc myfunction(choice:bool) {
  var a: R;
  a.setup(x = 20);
  a.verify();
  assert(a.x == 20);

  var b: R;
  b.setup(x = 40);
  b.verify();
  assert(b.x == 40);

  if choice {
    b = a;
  }

  b.verify();
  if choice {
    assert(b.x == 20);
  } else {
    assert(b.x == 40);
  }
}

myfunction(true);
myfunction(false);

verify();

