class C {
  type t;
  var x: t;
}

var myC: borrowed C(?);
myC = (new owned C()).borrow();
writeln(myC);
