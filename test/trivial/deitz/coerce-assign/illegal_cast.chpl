class C {
  var x: int;
}

var c = (new owned C(2)).borrow();

writeln(c:int);
