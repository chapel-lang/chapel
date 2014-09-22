var a: sync int;
var b: sync int;
var c: sync int;
var d: sync int;

proc jam_records1 () {
  a;   // wait on thread last statement in cobegin
  b = 1;
}

proc jam_records2 () {
  b;   // wait on thread 1
  c = 1;
}

proc jam_records3 () {
  c;   // wait on thread 2
  d = 1;
}

cobegin {
  jam_records3();
  jam_records2();
  jam_records1();
  a = 1;
}

writeln("d is ", d.readFF());
