
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
  delete r;
}
{
  var o: owned A? = new A();
  write(o.type:string, " releases to ");
  var r = owned.release(o);
  writeln(r.type:string);
  delete r;
}
