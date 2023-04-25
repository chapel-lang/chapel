// deprecated in 1.31 by jade
class A {
  var x;
}

{
  var a = new unmanaged A(7);
  owned.create(a);
}

{
  var a = new A(7);
  owned.create(a);
}

{
  var a = new A(7);
  a.clear();
}

{
  var a = new unmanaged A(7);
  var o: owned A? = new A(0);
  o.retain(a);
}

{
  var a = new A(7);
  delete a.release();
}
