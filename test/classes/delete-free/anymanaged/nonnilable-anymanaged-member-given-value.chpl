class Thing1 {
  var thing2: Thing2;
}

class Thing2 {}

var thing1 = new Thing1(new Thing2());

writeln(thing1);
