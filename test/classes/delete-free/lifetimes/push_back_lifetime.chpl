class C { var x: int; }
 
proc bad1() {
  var A:[1..0] borrowed C;

  {
    var own = new owned C();
    var bb = own.borrow();
    A.push_back(bb);
  }
} 
bad1();

proc bad2() {
  var A:[1..0] borrowed C;

  {
    var own = new owned C();
    A.push_back(own);
  }
} 
bad2();


proc ok() {
  var A:[1..0] owned C;

  {
    var own = new owned C();
    A.push_back(own);
  }
}
ok();
