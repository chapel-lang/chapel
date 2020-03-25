class C { var x: int; }

var globalBorrow: borrowed C? = nil;

{
  var myOwned = new owned C(1);
  globalBorrow = myOwned.borrow();
}

{
  var x = new borrowed C(1);
  globalBorrow = x;
}

var globalBorrow2: borrowed C?;

{
  var myOwned = new owned C(1);
  globalBorrow2 = myOwned.borrow();
}


var globalBorrow3: borrowed C?;
{
  var x = new borrowed C(1);
  globalBorrow3 = x;
}
