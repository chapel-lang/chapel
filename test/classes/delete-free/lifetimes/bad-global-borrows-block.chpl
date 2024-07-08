class C { var x: int; }

var globalBorrow: borrowed C? = nil;

{
  var myOwned = new owned C(1);
  globalBorrow = myOwned.borrow();
}
{
  var ownX = new owned C(1);
  var x = ownX.borrow();
  globalBorrow = x;
}

var globalBorrow2: borrowed C?;

{
  var myOwned = new owned C(1);
  globalBorrow2 = myOwned.borrow();
}

var globalBorrow3: borrowed C?;
{
  var ownX = new owned C(1);
  var x = ownX.borrow();
  globalBorrow3 = x;
}
