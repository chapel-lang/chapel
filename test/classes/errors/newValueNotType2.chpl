class C {
  var x: int;
}

var myC = new owned C(42);

var myC2 = new borrowed myC;  // note: Calling `new` on a value, not a type.
writeln(myC2);

var myC3 = new unmanaged myC;
writeln(myC3);
