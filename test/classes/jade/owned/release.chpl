
class A {
}

{
  var o = new A();
  delete owned.release(o);
}

{
  var o: owned A = new A();
  write(o.type:string, " releases to ");
  var r = owned.release(o);
  writeln(r.type:string);
}
{
  var o: owned A? = new A();
  write(o.type:string, " releases to ");
  var r = owned.release(o);
  writeln(r.type:string);
}

// start_test test/deprecated/classes/owned-clear.chpl \
//   test/deprecated/classes/owned-create.chpl \
//   test/deprecated/classes/owned-release-method.chpl \
//   test/deprecated/classes/owned-retain.chpl \
//   test/classes/jade/owned/*.chpl
