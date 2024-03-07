use Time;

{
  // check all the comparison operators
  var x = new timeDelta(seconds=30);
  var y = new timeDelta(seconds=40);

  assert(x < y);
  assert(x <= y);
  assert(!(x == y));
  assert(x != y);
  assert(!(x >= y));
  assert(!(x > y));

  assert(!(x < x));
  assert(x <= x);
  assert(x == x);
  assert(!(x != x));
  assert(x >= x);
  assert(!(x > x));

  assert(!(y < x));
  assert(!(y <= x));
  assert(!(y == x));
  assert(y != x);
  assert(y >= x);
  assert(y > x);
}

{
  // also check comparison involving multiple units
  var a = new timeDelta(days=3, seconds=4, microseconds=5);
  var b = new timeDelta(days=2, seconds=20, microseconds=200);
  assert(a > b);
  assert(b < a);
}

{
  // also try cast to string
  var z = new timeDelta(days=400, seconds=8000, microseconds=30000);
  writeln(z:string);
}
