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
