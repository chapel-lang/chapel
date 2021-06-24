var a: sync int;
var b: sync int;
var c: sync int;
var d: sync int;

proc jam_records1 () {
  a.readFE();   // wait on thread last statement in cobegin
  b.writeEF(1);
}

proc jam_records2 () {
  b.readFE();   // wait on thread 1
  c.writeEF(1);
}

proc jam_records3 () {
  c.readFE();   // wait on thread 2
  d.writeEF(1);
}

cobegin {
  jam_records3();
  jam_records2();
  jam_records1();
  a.writeEF(1);
}

writeln("d is ", d.readFF());
