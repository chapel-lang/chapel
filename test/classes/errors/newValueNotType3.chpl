class C {
  var x: int;
}

var myC = new owned C(42);

var myC2 = new myC;  // note: Calling `new` on a value, not a type.
writeln(myC2);
