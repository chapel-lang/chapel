class Thing1 {
  var thing2: Thing2?;

  proc th(): int {
    if thing2 then writeln(thing2);
    return 1;
  }
}

class Thing2 { }

var thing1 = new Thing1();

writeln(thing1.th());
