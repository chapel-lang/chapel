class C {
  var x: int;
}

var myC = new owned C(42);

// note: Calling `new` on a value, not a type.

var myC1 = new myC();
writeln(myC1);

var myC2 = new borrowed myC();
writeln(myC2);

var myC3 = new unmanaged myC();
writeln(myC3);

var myC4 = new owned myC();
writeln(myC4);

var myC5 = new shared myC();
writeln(myC5);
