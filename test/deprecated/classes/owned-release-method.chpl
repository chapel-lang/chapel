// deprecated in 1.31 by jade
class A {
  var x;
}

{
  var a = new A(7);
  delete a.release();
}

