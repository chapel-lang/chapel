// deprecated in 1.31 by jade
class A {}

{
  var a = new unmanaged A();
  shared.create(a);
}

{
  var a = new A();
  shared.create(a);
}

{
  var a = new shared A();
  shared.create(a);
}

{
  var a = new shared A();
  a.clear();
}

{
  var a = new unmanaged A();
  var s: shared A? = new shared A();
  s.retain(a);
}
