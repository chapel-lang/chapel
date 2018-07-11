var a: int = 1;                      // module scope
var n: int = 14;

proc b(c: int): int {        // proc param scope
  var d: int = 4;                    // proc scope

  writeln("c is: ", c);
  writeln("d is: ", d);

  return 2;
}

class e {
  var f: int = 6;                    // class scope

  proc g(h: int): int {
    var i: int = 9;

    writeln("h is: ", h);
    writeln("i is: ", i);

    return 7;
  }
}

proc main() {
  writeln("a is: ", a);

  var myB: int;

  myB = b(3);

  writeln("b(3) is: ", myB);

  var myE: borrowed e = new borrowed e();
  var myG: int;

  myG = myE.g(8);

  writeln("e.g(8) is: ", myG);

  {                                      // block stmt scope
    var m: int = 13;
    {
      var o: int = 15;

      writeln("m is: ", m);
      writeln("o is: ", o);
    }
  }
}
