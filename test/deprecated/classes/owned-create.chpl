// deprecated for 1.30 by Jade
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
