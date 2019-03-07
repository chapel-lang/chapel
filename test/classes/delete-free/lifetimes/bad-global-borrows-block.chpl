class C { var x: int; }

var globalBorrow: borrowed C;

{
  var myOwned = new owned C(1);
  globalBorrow = myOwned.borrow();
}

{
  var x = new borrowed C(1);
  globalBorrow = x;
}
