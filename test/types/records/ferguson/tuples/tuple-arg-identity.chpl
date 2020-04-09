use myrecord;

var global1, global2: R;

proc fooIn(in rec: (R,R)) {
  rec(0).verify();
  global1.increment();
  global1.verify();
  rec(0).verify();
  assert(rec(0).x == 120);
  rec(1).verify();
  global2.increment();
  global2.verify();
  rec(1).verify();
  assert(rec(1).x == 140);
}

proc testIn() {

  global1.setup(x = 120);
  global1.verify();
  assert(global1.x == 120);

  global2.setup(x = 140);
  global2.verify();
  assert(global2.x == 140);

  fooIn( (global1,global2) );

  global1.verify();
  assert(global1.x == 121);
  global2.verify();
  assert(global2.x == 141);
}

testIn();


proc fooConstIn(const in rec: (R,R)) {
  rec(0).verify();
  global1.increment();
  global1.verify();
  rec(0).verify();
  assert(rec(0).x == 220);
  rec(1).verify();
  global2.increment();
  global2.verify();
  rec(1).verify();
  assert(rec(1).x == 240);
}

proc testConstIn() {

  global1.setup(x = 220);
  global1.verify();
  assert(global1.x == 220);

  global2.setup(x = 240);
  global2.verify();
  assert(global2.x == 240);

  fooConstIn( (global1,global2) );

  global1.verify();
  assert(global1.x == 221);
  global2.verify();
  assert(global2.x == 241);
}

testConstIn();

proc fooConst(const rec: (R,R)) {
  rec(0).verify();
  global1.increment();
  global1.verify();
  rec(0).verify();
  assert(rec(0).x == 321);
  rec(1).verify();
  global2.increment();
  global2.verify();
  rec(1).verify();
  assert(rec(1).x == 341);
}

proc testConst() {

  global1.setup(x = 320);
  global1.verify();
  assert(global1.x == 320);

  global2.setup(x = 340);
  global2.verify();
  assert(global2.x == 340);

  fooConst( (global1,global2) );

  global1.verify();
  assert(global1.x == 321);
  global2.verify();
  assert(global2.x == 341);
}

testConst();

proc fooBlank(rec: (R,R)) {
  rec(0).verify();
  global1.increment();
  global1.verify();
  rec(0).verify();
  assert(rec(0).x == 421);
  rec(1).verify();
  global2.increment();
  global2.verify();
  rec(1).verify();
  assert(rec(1).x == 441);
}

proc testBlank() {

  global1.setup(x = 420);
  global1.verify();
  assert(global1.x == 420);

  global2.setup(x = 440);
  global2.verify();
  assert(global2.x == 440);

  fooBlank( (global1,global2) );

  global1.verify();
  assert(global1.x == 421);
  global2.verify();
  assert(global2.x == 441);
}

testBlank();

global1.destroy();
global2.destroy();

verify();
