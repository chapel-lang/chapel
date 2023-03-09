class A {
  var x;
}

{
  var a = new unmanaged A(7);
  var o: owned A? = new A(0);
  o.retain(a);
}
