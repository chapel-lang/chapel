use myrecord;

proc makeRecord(i:int) {
  var r:R;
  r.setup(x = i);
  r.verify();
  assert(r.x == i);
  return r;
}

proc makeFiveTupleOfRecords() {
  return (makeRecord(1),
          makeRecord(2),
          makeRecord(3),
          makeRecord(4),
          makeRecord(5));
}

proc myfunction() {

  const (a, b, c, d, e) = makeFiveTupleOfRecords();

  a.verify();
  assert(a.x == 1);
  b.verify();
  assert(b.x == 2);
  c.verify();
  assert(c.x == 3);
  d.verify();
  assert(d.x == 4);
  e.verify();
  assert(e.x == 5);
}

myfunction();

verify();
