// It's quite easy for _write_one_internal to blow past the instantiation limit
// so we've exempted it. This test locks that down.

config param instantiationLimit : int;

record R {
  param i: int;
}

for param i in 1..(instantiationLimit + 1) {
  var myR: R(i);
  write(myR);
}
writeln();

